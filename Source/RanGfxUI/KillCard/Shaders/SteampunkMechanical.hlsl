// 蒸汽朋克機械效果 Shader
// 實現齒輪轉動、蒸汽煙霧和維多利亞裝飾風格

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;
float SteamFlow : STEAM_FLOW;
float GearRotation : GEAR_ROTATION;

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

texture GearTexture;
sampler GearSampler = sampler_state
{
    Texture = <GearTexture>;
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
    float2 GearCoord : TEXCOORD1;
};

VS_OUTPUT SteampunkVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    // 齒輪旋轉座標
    float2 center = float2(0.5, 0.5);
    float2 rotatedCoord = input.TexCoord - center;
    float angle = Time * GearRotation;
    float cosA = cos(angle);
    float sinA = sin(angle);
    
    output.GearCoord.x = rotatedCoord.x * cosA - rotatedCoord.y * sinA + center.x;
    output.GearCoord.y = rotatedCoord.x * sinA + rotatedCoord.y * cosA + center.y;
    
    return output;
}

float4 SteampunkPS(VS_OUTPUT input) : COLOR0
{
    // 基礎紋理
    float4 baseColor = tex2D(BackgroundSampler, input.TexCoord);
    
    // 復古銅色 #B87333
    float3 vintageBronze = float3(0.722, 0.451, 0.2);
    
    // 蒸汽白 #F5F5DC
    float3 steamWhite = float3(0.961, 0.961, 0.863);
    
    // 齒輪紋理
    float4 gearTexture = tex2D(GearSampler, input.GearCoord);
    
    // 蒸汽管道效果
    float pipePattern = step(0.45, abs(sin(input.TexCoord.x * 10.0 + Time * 2.0)));
    float steamPipe = pipePattern * smoothstep(0.1, 0.3, abs(input.TexCoord.y - 0.5));
    
    // 蒸汽煙霧效果
    float2 steamCoord = input.TexCoord + float2(sin(Time + input.TexCoord.y * 5.0) * 0.02,
                                                -Time * 0.1);
    float steam = sin(steamCoord.x * 15.0) * sin(steamCoord.y * 8.0);
    steam = smoothstep(0.3, 0.8, (steam + 1.0) * 0.5);
    
    // 維多利亞風格裝飾邊框
    float2 center = float2(0.5, 0.5);
    float dist = distance(input.TexCoord, center);
    
    // 裝飾性邊框
    float ornateFrame = 0.0;
    if (dist > 0.4 && dist < 0.5)
    {
        float framePattern = sin(atan2(input.TexCoord.y - center.y, input.TexCoord.x - center.x) * 8.0);
        ornateFrame = smoothstep(0.4, 0.5, dist) * smoothstep(0.5, 0.4, dist) * 
                      (framePattern * 0.3 + 0.7);
    }
    
    // 機械結構線條
    float mechanicalLines = 0.0;
    float lineThickness = 0.005;
    
    // 水平線條
    if (abs(input.TexCoord.y - 0.3) < lineThickness || abs(input.TexCoord.y - 0.7) < lineThickness)
    {
        mechanicalLines = 1.0;
    }
    
    // 垂直線條
    if (abs(input.TexCoord.x - 0.2) < lineThickness || abs(input.TexCoord.x - 0.8) < lineThickness)
    {
        mechanicalLines = 1.0;
    }
    
    // 齒輪碎片效果
    float gearFragments = step(0.7, gearTexture.r) * sin(Time * 15.0 + input.TexCoord.x * 20.0) * 0.3;
    
    // 蒸汽噴發效果
    float steamBurst = 0.0;
    if (input.TexCoord.y > 0.8)
    {
        steamBurst = step(0.6, sin(Time * 10.0 + input.TexCoord.x * 30.0)) * 
                     smoothstep(0.8, 1.0, input.TexCoord.y) * 0.5;
    }
    
    // 機械打字效果（文字區域）
    float typewriter = 0.0;
    if (input.TexCoord.y > 0.1 && input.TexCoord.y < 0.25)
    {
        float typeProgress = fmod(Time * 2.0, 1.0);
        if (input.TexCoord.x < typeProgress)
        {
            typewriter = step(0.8, sin(input.TexCoord.x * 50.0)) * 0.4;
        }
    }
    
    // 混合顏色
    float3 finalColor = baseColor.rgb;
    
    // 銅色基調
    finalColor = lerp(finalColor, vintageBronze, 0.4);
    
    // 齒輪效果
    finalColor = lerp(finalColor, vintageBronze * 1.2, gearTexture.r * 0.6);
    
    // 蒸汽管道
    finalColor += vintageBronze * steamPipe * 0.3;
    
    // 蒸汽煙霧
    finalColor += steamWhite * steam * 0.4;
    
    // 裝飾邊框
    finalColor += vintageBronze * ornateFrame * 0.8;
    
    // 機械線條
    finalColor += vintageBronze * mechanicalLines * 0.6;
    
    // 齒輪碎片
    finalColor += vintageBronze * gearFragments;
    
    // 蒸汽噴發
    finalColor += steamWhite * steamBurst;
    
    // 打字效果
    finalColor += vintageBronze * typewriter;
    
    float4 result = float4(finalColor, baseColor.a * input.Color.a);
    return result;
}

technique SteampunkMechanical
{
    pass P0
    {
        VertexShader = compile vs_2_0 SteampunkVS();
        PixelShader = compile ps_2_0 SteampunkPS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}