// 等离子爆破效果 Shader
// 实现紫色等离子球体和电弧闪光效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;

texture PlasmaTexture;
sampler PlasmaSampler = sampler_state
{
    Texture = <PlasmaTexture>;
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

VS_OUTPUT PlasmaVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

float4 PlasmaPS(VS_OUTPUT input) : COLOR0
{
    float2 uv = input.TexCoord;
    float2 center = float2(0.5, 0.5);
    
    // 基础等离子纹理
    float4 baseColor = tex2D(PlasmaSampler, uv);
    
    // 距离中心的距离
    float distance = length(uv - center);
    
    // 等离子球体效果
    float sphere = 1.0 - smoothstep(0.2, 0.5, distance);
    float plasmaCore = exp(-distance * 8.0) * sin(Time * 10.0) * 0.5 + 0.5;
    
    // 紫色等离子调色
    float3 plasmaColor = float3(0.8, 0.3, 1.0) * plasmaCore + float3(1.0, 0.5, 0.8) * sphere;
    
    // 电弧闪光效果
    float arc = 0.0;
    for (int i = 0; i < 6; i++)
    {
        float angle = float(i) * 1.047 + Time * 2.0; // 60度间隔
        float2 arcDir = float2(cos(angle), sin(angle));
        float arcDistance = abs(dot(uv - center, float2(-arcDir.y, arcDir.x)));
        arc += exp(-arcDistance * 50.0) * sin(Time * 8.0 + float(i)) * 0.5 + 0.5;
    }
    
    // 等离子扩散效果
    float expansion = sin(Time * 3.0) * 0.5 + 0.5;
    float expandedSphere = 1.0 - smoothstep(0.1, 0.5 + expansion * 0.3, distance);
    
    // 能量脉冲
    float pulse = sin(distance * 20.0 - Time * 15.0) * exp(-distance * 3.0);
    
    // 电离效果
    float ionization = frac(sin(dot(uv * 100.0, float2(12.9898, 78.233)) + Time * 5.0) * 43758.5453);
    ionization = step(0.95, ionization) * sin(Time * 20.0);
    
    // 最终颜色混合
    float4 finalColor = baseColor;
    finalColor.rgb *= plasmaColor;
    finalColor.rgb += arc * float3(1.0, 0.8, 1.0) * 0.7;
    finalColor.rgb += pulse * 0.5;
    finalColor.rgb += ionization * float3(0.9, 0.9, 1.0);
    finalColor.a *= input.Color.a * expandedSphere * Intensity;
    
    return finalColor;
}

technique PlasmaExplosion
{
    pass P0
    {
        VertexShader = compile vs_2_0 PlasmaVS();
        PixelShader = compile ps_2_0 PlasmaPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}