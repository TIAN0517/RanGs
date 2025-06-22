// 軍事戰術風格 HUD Shader
// 實現雷達掃描、戰術網格和目標鎖定效果

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;
float ScanProgress : SCAN_PROGRESS;

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

VS_OUTPUT MilitaryVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

float4 MilitaryPS(VS_OUTPUT input) : COLOR0
{
    // 基礎紋理
    float4 baseColor = tex2D(BackgroundSampler, input.TexCoord);
    
    // 軍綠色 #4B7C59
    float3 militaryGreen = float3(0.294, 0.486, 0.349);
    
    // 橙色警告 #FF8C00
    float3 warningOrange = float3(1.0, 0.549, 0.0);
    
    // 戰術網格
    float2 gridCoord = input.TexCoord * 20.0;
    float2 grid = abs(frac(gridCoord - 0.5) - 0.5) / fwidth(gridCoord);
    float gridLine = min(grid.x, grid.y);
    float tacticalGrid = 1.0 - smoothstep(0.0, 1.0, gridLine);
    
    // 雷達掃描線
    float2 center = float2(0.5, 0.5);
    float angle = atan2(input.TexCoord.y - center.y, input.TexCoord.x - center.x);
    float scanAngle = Time * 2.0;
    float scanLine = step(abs(angle - scanAngle), 0.1) * 
                     smoothstep(0.1, 0.4, distance(input.TexCoord, center));
    
    // 十字準星
    float crosshair = 0.0;
    float2 crossCenter = input.TexCoord - center;
    if (abs(crossCenter.x) < 0.002 || abs(crossCenter.y) < 0.002)
    {
        crosshair = smoothstep(0.1, 0.3, distance(input.TexCoord, center));
    }
    
    // 邊框（硬朗直角）
    float border = 0.0;
    if (input.TexCoord.x < 0.02 || input.TexCoord.x > 0.98 || 
        input.TexCoord.y < 0.02 || input.TexCoord.y > 0.98)
    {
        border = 1.0;
    }
    
    // 數據刷新效果
    float dataRefresh = step(0.8, sin(Time * 15.0 + input.TexCoord.x * 10.0));
    
    // 混合顏色
    float3 finalColor = baseColor.rgb;
    finalColor = lerp(finalColor, militaryGreen, tacticalGrid * 0.3);
    finalColor += warningOrange * scanLine * 0.8;
    finalColor += warningOrange * crosshair * 0.6;
    finalColor += militaryGreen * border * 0.5;
    finalColor += warningOrange * dataRefresh * 0.2;
    
    float4 result = float4(finalColor, baseColor.a * input.Color.a);
    return result;
}

technique MilitaryHUD
{
    pass P0
    {
        VertexShader = compile vs_2_0 MilitaryVS();
        PixelShader = compile ps_2_0 MilitaryPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}