// 虚拟重构效果 Shader
// 实现线框模型和彩色光线扫描效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;

texture VirtualTexture;
sampler VirtualSampler = sampler_state
{
    Texture = <VirtualTexture>;
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

VS_OUTPUT VirtualVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

float4 VirtualPS(VS_OUTPUT input) : COLOR0
{
    float2 uv = input.TexCoord;
    
    // 基础虚拟纹理
    float4 baseColor = tex2D(VirtualSampler, uv);
    
    // 重构进度
    float reconstructProgress = sin(Time * 2.0) * 0.5 + 0.5;
    
    // 线框模型效果
    float wireframe = 0.0;
    float2 grid = frac(uv * 20.0);
    
    // 水平和垂直线框
    if (grid.x < 0.05 || grid.x > 0.95 || grid.y < 0.05 || grid.y > 0.95)
    {
        wireframe = 1.0;
    }
    
    // 对角线框
    if (abs(grid.x - grid.y) < 0.05 || abs(grid.x + grid.y - 1.0) < 0.05)
    {
        wireframe = max(wireframe, 0.7);
    }
    
    // 扫描线效果
    float scanProgress = frac(Time * 0.8);
    float scanLine = smoothstep(0.0, 0.02, abs(uv.y - scanProgress)) * 
                    smoothstep(0.02, 0.0, abs(uv.y - scanProgress));
    scanLine = 1.0 - scanLine;
    
    // 彩色光线扫描
    float3 scanColor = float3(
        sin(Time * 3.0) * 0.5 + 0.5,
        sin(Time * 3.0 + 2.0) * 0.5 + 0.5,
        sin(Time * 3.0 + 4.0) * 0.5 + 0.5
    );
    
    // 数据重构粒子
    float dataPixels = 0.0;
    float2 pixelCoord = floor(uv * 50.0) / 50.0;
    float pixelNoise = frac(sin(dot(pixelCoord, float2(12.9898, 78.233))) * 43758.5453);
    
    if (pixelNoise > 0.7 && uv.y < scanProgress + 0.1)
    {
        dataPixels = sin(Time * 10.0 + pixelNoise * 20.0) * 0.5 + 0.5;
    }
    
    // 全息投影效果
    float hologram = sin(uv.y * 100.0 + Time * 5.0) * 0.1 + 0.9;
    
    // 重构完成效果
    float reconstructComplete = smoothstep(0.0, 1.0, uv.y - scanProgress + 0.1);
    
    // 能量场效果
    float energyField = exp(-length(uv - float2(0.5, scanProgress)) * 5.0);
    
    // 最终颜色混合
    float4 finalColor = baseColor;
    
    // 应用线框效果
    finalColor.rgb = lerp(finalColor.rgb, float3(0.0, 1.0, 1.0), wireframe * 0.8);
    
    // 应用扫描效果
    finalColor.rgb += scanColor * scanLine * 0.6;
    
    // 应用数据粒子
    finalColor.rgb += dataPixels * float3(1.0, 1.0, 0.0) * 0.7;
    
    // 应用全息效果
    finalColor.rgb *= hologram;
    
    // 应用能量场
    finalColor.rgb += energyField * scanColor * 0.5;
    
    // 重构后的正常显示
    finalColor.rgb = lerp(wireframe * float3(0.0, 1.0, 1.0), finalColor.rgb, reconstructComplete);
    
    // 透明度处理
    finalColor.a *= input.Color.a * Intensity;
    
    return finalColor;
}

technique VirtualReconstruction
{
    pass P0
    {
        VertexShader = compile vs_2_0 VirtualVS();
        PixelShader = compile ps_2_0 VirtualPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}