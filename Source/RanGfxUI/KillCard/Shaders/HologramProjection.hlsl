// 全息投影風格 Shader
// 實現全息掃描線、透明度變化和光線衍射效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;
float HologramAlpha : HOLOGRAM_ALPHA;

texture BackgroundTexture;
sampler BackgroundSampler = sampler_state
{
    Texture = <BackgroundTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_INPUT
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Color : COLOR0;
};

struct VS_OUTPUT
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Color : COLOR0;
};

VS_OUTPUT HologramVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

float4 HologramPS(VS_OUTPUT input) : COLOR0
{
    // 基礎紋理
    float4 baseColor = tex2D(BackgroundSampler, input.TexCoord);
    
    // 全息藍 #40E0D0
    float3 hologramBlue = float3(0.251, 0.878, 0.816);
    
    // 透明白光 #FFFFFF80
    float3 transparentWhite = float3(1.0, 1.0, 1.0);
    
    // 掃描線效果
    float scanLine = sin((input.TexCoord.y + Time * 0.5) * 40.0) * 0.5 + 0.5;
    scanLine = pow(scanLine, 3.0) * 0.3;
    
    // 全息投影失真
    float distortion = sin(input.TexCoord.x * 10.0 + Time) * 0.01;
    float2 distortedCoord = input.TexCoord + float2(distortion, 0.0);
    
    // 透明度波動
    float alphaWave = 0.7 + 0.3 * sin(Time * 3.0 + input.TexCoord.x * 5.0);
    
    // 光線衍射效果
    float2 center = float2(0.5, 0.5);
    float dist = distance(input.TexCoord, center);
    float diffraction = sin(dist * 30.0 - Time * 5.0) * 0.1 + 0.9;
    
    // 邊緣發光（純光線勾勒）
    float edge = smoothstep(0.47, 0.5, dist) * smoothstep(0.5, 0.47, dist);
    
    // 3D投影效果
    float projection = sin(input.TexCoord.x * 3.14159) * sin(input.TexCoord.y * 3.14159);
    
    // 混合顏色
    float3 finalColor = baseColor.rgb;
    finalColor = lerp(finalColor, hologramBlue, 0.4);
    finalColor += hologramBlue * scanLine;
    finalColor += transparentWhite * edge * 0.8;
    finalColor *= diffraction;
    finalColor += hologramBlue * projection * 0.2;
    
    // 最終透明度
    float finalAlpha = baseColor.a * input.Color.a * alphaWave * HologramAlpha;
    
    float4 result = float4(finalColor, finalAlpha);
    return result;
}

technique HologramProjection
{
    pass P0
    {
        VertexShader = compile vs_2_0 HologramVS();
        PixelShader = compile ps_2_0 HologramPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}