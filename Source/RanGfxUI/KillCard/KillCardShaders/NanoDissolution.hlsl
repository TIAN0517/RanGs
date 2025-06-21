// 纳米溶解效果 Shader
// 实现金属质感溶解和银色纳米粒子效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;

texture MetalTexture;
sampler MetalSampler = sampler_state
{
    Texture = <MetalTexture>;
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

VS_OUTPUT NanoVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    output.NoiseCoord = input.TexCoord * 2.0 + Time * 0.1;
    
    return output;
}

float4 NanoPS(VS_OUTPUT input) : COLOR0
{
    float2 uv = input.TexCoord;
    
    // 基础金属纹理
    float4 metalColor = tex2D(MetalSampler, uv);
    
    // 噪声纹理用于溶解效果
    float4 noise = tex2D(NoiseSampler, input.NoiseCoord);
    float4 noise2 = tex2D(NoiseSampler, input.NoiseCoord * 0.5 + Time * 0.05);
    
    // 溶解进度
    float dissolveProgress = sin(Time * 1.5) * 0.5 + 0.5;
    
    // 边缘溶解效果
    float edgeDistance = min(min(uv.x, 1.0 - uv.x), min(uv.y, 1.0 - uv.y));
    float edgeDissolve = smoothstep(0.0, 0.3, edgeDistance - dissolveProgress * 0.4);
    
    // 纳米粒子噪声
    float nanoParticles = (noise.r + noise2.g) * 0.5;
    nanoParticles = smoothstep(0.6, 0.9, nanoParticles + dissolveProgress * 0.3);
    
    // 金属光泽效果
    float metallic = metalColor.r * 0.3 + metalColor.g * 0.59 + metalColor.b * 0.11;
    float3 metallicShine = float3(0.8, 0.85, 0.9) * metallic;
    
    // 银色纳米粒子
    float3 nanoColor = float3(0.9, 0.95, 1.0) * nanoParticles;
    
    // 溶解边缘发光
    float edgeGlow = exp(-abs(edgeDistance - dissolveProgress * 0.4) * 20.0);
    float3 glowColor = float3(0.7, 0.8, 1.0) * edgeGlow;
    
    // 金属腐蚀效果
    float corrosion = noise.b * dissolveProgress;
    float3 corrosionColor = float3(0.4, 0.5, 0.6) * corrosion;
    
    // 粒子飘散动画
    float particleFloat = sin(uv.y * 10.0 + Time * 3.0) * cos(uv.x * 8.0 + Time * 2.0);
    particleFloat = smoothstep(0.3, 0.7, particleFloat + dissolveProgress);
    
    // 最终颜色混合
    float4 finalColor = metalColor;
    finalColor.rgb = lerp(finalColor.rgb, metallicShine, 0.5);
    finalColor.rgb += nanoColor * 0.8;
    finalColor.rgb += glowColor * 0.6;
    finalColor.rgb = lerp(finalColor.rgb, corrosionColor, corrosion * 0.7);
    finalColor.rgb += particleFloat * float3(0.8, 0.9, 1.0) * 0.3;
    
    // 透明度基于溶解效果
    finalColor.a *= input.Color.a * edgeDissolve * (1.0 - dissolveProgress * 0.8);
    
    return finalColor;
}

technique NanoDissolution
{
    pass P0
    {
        VertexShader = compile vs_2_0 NanoVS();
        PixelShader = compile ps_2_0 NanoPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}