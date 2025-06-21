// 时空扭曲效果 Shader
// 实现空间漣漪扭曲和慢动作效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;

texture SpaceTexture;
sampler SpaceSampler = sampler_state
{
    Texture = <SpaceTexture>;
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

VS_OUTPUT SpaceTimeVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // 时空扭曲的顶点位置
    float4 pos = input.Position;
    float warpStrength = sin(Time * 2.0) * 0.01 * Intensity;
    float distance = length(input.TexCoord - 0.5);
    pos.xy += sin(distance * 10.0 - Time * 5.0) * warpStrength;
    
    output.Position = mul(pos, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

float4 SpaceTimePS(VS_OUTPUT input) : COLOR0
{
    float2 uv = input.TexCoord;
    float2 center = float2(0.5, 0.5);
    
    // 距离中心的距离
    float distance = length(uv - center);
    
    // 时空漣漪效果
    float ripple = sin(distance * 15.0 - Time * 8.0) * exp(-distance * 2.0);
    float warpFactor = ripple * 0.05 * Intensity;
    
    // 扭曲UV坐标
    float2 warpedUV = uv + (uv - center) * warpFactor;
    
    // 采样扭曲后的纹理
    float4 warpedColor = tex2D(SpaceSampler, warpedUV);
    
    // 时间膨胀效果（颜色渐变）
    float timeDilation = sin(Time * 1.5) * 0.5 + 0.5;
    float3 timeColor = lerp(float3(0.5, 0.8, 1.0), float3(1.0, 0.8, 0.5), timeDilation);
    
    // 空间格栅效果
    float grid = 0.0;
    float2 gridUV = uv * 20.0;
    if (frac(gridUV.x) < 0.1 || frac(gridUV.y) < 0.1)
    {
        grid = 0.3 * sin(Time * 3.0);
    }
    
    // 能量环效果
    float ring = abs(sin((distance - Time * 0.3) * 20.0)) * exp(-abs(distance - 0.3) * 10.0);
    
    // 慢动作残影效果
    float4 trail1 = tex2D(SpaceSampler, uv + float2(sin(Time) * 0.005, 0));
    float4 trail2 = tex2D(SpaceSampler, uv - float2(sin(Time) * 0.005, 0));
    float4 motionBlur = (warpedColor + trail1 * 0.5 + trail2 * 0.5) / 2.0;
    
    // 最终颜色混合
    float4 finalColor = motionBlur;
    finalColor.rgb *= timeColor;
    finalColor.rgb += grid + ring * 0.5;
    finalColor.a *= input.Color.a * (0.8 + 0.2 * sin(Time * 2.0));
    
    return finalColor;
}

technique SpaceTimeWarp
{
    pass P0
    {
        VertexShader = compile vs_2_0 SpaceTimeVS();
        PixelShader = compile ps_2_0 SpaceTimePS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}