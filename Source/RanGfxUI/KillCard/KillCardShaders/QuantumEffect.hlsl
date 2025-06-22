// Quantum Fission Effect Shader - 量子裂变特效着色器
// 实现粒子分解和蓝白色能量波纹效果

float4x4 g_mWorldViewProjection : WorldViewProjection;
float4x4 g_mWorld : World;
float4x4 g_mView : View;
float4x4 g_mProjection : Projection;

// 时间和动画参数
float g_fTime : Time;
float g_fDeltaTime : DeltaTime;
float g_fAnimationPhase : AnimationPhase = 0.0f;
float g_fEffectIntensity : EffectIntensity = 1.0f;

texture ParticleTexture;
sampler ParticleSampler = sampler_state
{
    Texture = <ParticleTexture>;
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

VS_OUTPUT QuantumVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    // 添加时间偏移用于噪声动画
    output.NoiseCoord = input.TexCoord + float2(Time * 0.1, Time * 0.15);
    
    return output;
}

float4 QuantumPS(VS_OUTPUT input) : COLOR0
{
    // 基础纹理
    float4 baseColor = tex2D(ParticleSampler, input.TexCoord);
    
    // 噪声扰动
    float4 noise = tex2D(NoiseSampler, input.NoiseCoord);
    
    // 量子分解效果 - 像素化分解
    float2 pixelatedCoord = floor(input.TexCoord * 32.0) / 32.0;
    float4 pixelatedColor = tex2D(ParticleSampler, pixelatedCoord);
    
    // 能量波纹效果
    float distance = length(input.TexCoord - 0.5);
    float wave = sin(distance * 20.0 - Time * 8.0) * 0.5 + 0.5;
    
    // 蓝白色能量调色
    float3 quantumColor = float3(0.3, 0.7, 1.0) * wave + float3(1.0, 1.0, 1.0) * wave * 0.5;
    
    // 混合效果
    float dissolve = noise.r + sin(Time * 2.0) * 0.2;
    float4 finalColor = lerp(baseColor, pixelatedColor, dissolve * Intensity);
    finalColor.rgb *= quantumColor;
    finalColor.a *= input.Color.a * (1.0 - dissolve * 0.7);
    
    return finalColor;
}

technique QuantumFission
{
    pass P0
    {
        VertexShader = compile vs_2_0 QuantumVS();
        PixelShader = compile ps_2_0 QuantumPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}