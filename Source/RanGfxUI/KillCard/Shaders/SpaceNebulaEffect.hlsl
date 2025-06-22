// 太空科幻星雲效果 Shader
// 實現星空背景、星雲流動和引力波動畫

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float Time : TIME;
float Intensity : INTENSITY;
float NebulaFlow : NEBULA_FLOW;

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

texture StarfieldTexture;
sampler StarfieldSampler = sampler_state
{
    Texture = <StarfieldTexture>;
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
    float2 StarCoord : TEXCOORD1;
};

VS_OUTPUT SpaceVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(input.Position, WorldViewProj);
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    // 星空背景滾動
    output.StarCoord = input.TexCoord + float2(Time * 0.02, Time * 0.01);
    
    return output;
}

float4 SpacePS(VS_OUTPUT input) : COLOR0
{
    // 基礎紋理
    float4 baseColor = tex2D(BackgroundSampler, input.TexCoord);
    
    // 深空藍 #191970
    float3 deepSpaceBlue = float3(0.098, 0.098, 0.439);
    
    // 星光銀 #C0C0C0
    float3 starSilver = float3(0.753, 0.753, 0.753);
    
    // 星空背景
    float4 starfield = tex2D(StarfieldSampler, input.StarCoord);
    
    // 星雲流動效果
    float2 nebulaCoord = input.TexCoord + float2(sin(Time * 0.3 + input.TexCoord.y * 2.0) * 0.05,
                                                 cos(Time * 0.2 + input.TexCoord.x * 1.5) * 0.03);
    
    // 星雲密度變化
    float nebulaDensity = sin(nebulaCoord.x * 3.14159 * 2.0) * sin(nebulaCoord.y * 3.14159 * 1.5);
    nebulaDensity = (nebulaDensity + 1.0) * 0.5; // 標準化到 0-1
    
    // 引力波效果
    float2 center = float2(0.5, 0.5);
    float dist = distance(input.TexCoord, center);
    float gravityWave = sin(dist * 15.0 - Time * 4.0) * 0.1 + 0.9;
    
    // 星際傳送門效果（中心區域）
    float portal = 0.0;
    if (dist < 0.2)
    {
        portal = smoothstep(0.2, 0.1, dist) * sin(Time * 8.0) * 0.3;
    }
    
    // 太空艙窗口邊框
    float windowFrame = 0.0;
    float frameThickness = 0.05;
    if (dist > 0.45 - frameThickness && dist < 0.45 + frameThickness)
    {
        windowFrame = smoothstep(frameThickness, 0.0, abs(dist - 0.45)) * 0.8;
    }
    
    // 星塵軌跡
    float stardust = step(0.8, starfield.r) * sin(Time * 20.0 + input.TexCoord.x * 50.0) * 0.4;
    
    // 混合顏色
    float3 finalColor = baseColor.rgb;
    
    // 深空背景
    finalColor = lerp(finalColor, deepSpaceBlue, 0.6);
    
    // 星雲效果
    finalColor = lerp(finalColor, deepSpaceBlue * 1.5, nebulaDensity * 0.3);
    
    // 星光點綴
    finalColor += starSilver * starfield.rgb * 0.8;
    
    // 引力波動畫
    finalColor *= gravityWave;
    
    // 星際傳送門
    finalColor += starSilver * portal;
    
    // 窗口邊框
    finalColor += starSilver * windowFrame;
    
    // 星塵軌跡
    finalColor += starSilver * stardust;
    
    float4 result = float4(finalColor, baseColor.a * input.Color.a);
    return result;
}

technique SpaceNebulaEffect
{
    pass P0
    {
        VertexShader = compile vs_2_0 SpaceVS();
        PixelShader = compile ps_2_0 SpacePS();
        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZWriteEnable = false;
    }
}