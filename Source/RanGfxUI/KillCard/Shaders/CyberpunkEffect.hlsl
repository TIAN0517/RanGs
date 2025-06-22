// 賽博朋克風格效果 Shader
// 實現故障閃爍、霓虹邊框和數位雨滴效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;
float GlitchIntensity : GLITCH_INTENSITY;

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

texture NoiseTexture;
sampler NoiseSampler = sampler_state
{
    Texture = <NoiseTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
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
    float2 NoiseCoord : TEXCOORD1;
};

VS_OUTPUT CyberpunkVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    // 故障偏移
    float glitchOffset = sin(Time * 50.0 + input.Position.y * 0.1) * GlitchIntensity * 0.01;
    output.TexCoord.x += glitchOffset;
    
    // 噪聲座標
    output.NoiseCoord = input.TexCoord + float2(Time * 0.1, Time * 0.05);
    
    return output;
}

float4 CyberpunkPS(VS_OUTPUT input) : COLOR0
{
    // 基礎紋理
    float4 baseColor = tex2D(BackgroundSampler, input.TexCoord);
    
    // 噪聲用於故障效果
    float4 noise = tex2D(NoiseSampler, input.NoiseCoord);
    
    // 霓虹粉紫色 #FF007F
    float3 neonPink = float3(1.0, 0.0, 0.498);
    
    // 電藍色 #00FFFF
    float3 electricBlue = float3(0.0, 1.0, 1.0);
    
    // 邊框發光效果
    float2 center = float2(0.5, 0.5);
    float dist = distance(input.TexCoord, center);
    float border = smoothstep(0.45, 0.5, dist);
    
    // 故障條紋
    float glitch = step(0.7, noise.r) * sin(Time * 30.0 + input.TexCoord.y * 50.0);
    
    // 霓虹脈衝
    float pulse = 0.8 + 0.2 * sin(Time * 4.0);
    
    // 混合顏色
    float3 finalColor = baseColor.rgb;
    finalColor = lerp(finalColor, neonPink * pulse, border * 0.6);
    finalColor = lerp(finalColor, electricBlue, glitch * GlitchIntensity * 0.3);
    
    // 數位雨滴效果（簡化版）
    float rain = step(0.95, noise.g) * sin(Time * 10.0 + input.TexCoord.x * 20.0);
    finalColor += electricBlue * rain * 0.4;
    
    float4 result = float4(finalColor, baseColor.a * input.Color.a);
    return result;
}

technique CyberpunkEffect
{
    pass P0
    {
        VertexShader = compile vs_2_0 CyberpunkVS();
        PixelShader = compile ps_2_0 CyberpunkPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}