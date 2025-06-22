// 全息干扰效果 Shader
// 实现红色全息故障条纹和绿色数据码流效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;

texture HologramTexture;
sampler HologramSampler = sampler_state
{
    Texture = <HologramTexture>;
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
    float2 uv = input.TexCoord;
    
    // 基础全息纹理
    float4 baseColor = tex2D(HologramSampler, uv);
    
    // 全息故障条纹效果
    float glitchIntensity = sin(Time * 15.0) * 0.5 + 0.5;
    float scanline = sin(uv.y * 800.0 + Time * 10.0);
    float glitchOffset = sin(Time * 7.0 + uv.y * 50.0) * 0.02 * glitchIntensity;
    
    // 扭曲的UV坐标
    float2 distortedUV = uv + float2(glitchOffset, 0);
    float4 glitchedColor = tex2D(HologramSampler, distortedUV);
    
    // 红色故障条纹
    float redGlitch = step(0.98, frac(sin(dot(uv, float2(12.9898, 78.233)) + Time) * 43758.5453));
    float3 redStripe = float3(1.0, 0.2, 0.2) * redGlitch * glitchIntensity;
    
    // 绿色数据码流
    float dataStream = frac(sin(dot(floor(uv * 20.0), float2(12.9898, 78.233))) * 43758.5453);
    dataStream = step(0.7, dataStream) * sin(Time * 20.0 - uv.y * 30.0);
    float3 greenData = float3(0.2, 1.0, 0.3) * dataStream * 0.8;
    
    // 全息扫描线
    float hologramScan = abs(sin(uv.y * 100.0 + Time * 5.0)) * 0.3;
    
    // RGB通道分离效果
    float4 rgbShift;
    rgbShift.r = tex2D(HologramSampler, uv + float2(0.002 * Intensity, 0)).r;
    rgbShift.g = glitchedColor.g;
    rgbShift.b = tex2D(HologramSampler, uv - float2(0.002 * Intensity, 0)).b;
    rgbShift.a = baseColor.a;
    
    // 最终颜色混合
    float4 finalColor = lerp(baseColor, rgbShift, glitchIntensity);
    finalColor.rgb += redStripe + greenData;
    finalColor.rgb += hologramScan;
    finalColor.a *= input.Color.a * (0.7 + 0.3 * sin(Time * 3.0));
    
    return finalColor;
}

technique HologramGlitch
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