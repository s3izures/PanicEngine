// Description: terrain fx file that blends textures

cbuffer TransformBuffer : register(b0)
{
    matrix wvp; // World View Projection
    matrix world; // World position
    float3 viewPosition; // Camera position
};

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialEmissive;
    float materialPower;
}

cbuffer SettingsBuffer : register(b3)
{
    bool useBlend;
    float blendHeight;
    float blendThickness;
}

Texture2D lowMap : register(t0);
Texture2D highMap : register(t1);

SamplerState textureSampler : register(s0); // Whenever there is a texture, there is a sampler

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 worldPosition : TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float3 localPosition = input.position;
    
    VS_OUTPUT output;
    output.position = mul(float4(localPosition, 1.0f), wvp);
    output.worldPosition = mul(float4(localPosition, 1.0f), world);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - (mul(float4(localPosition, 1.0f), world).xyz));
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal); // normal
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    // ambient color
    float4 ambient = lightAmbient * materialAmbient;
    
    // diffuse color
    float d = saturate(dot(light, n)); // dot value   // saturate means 0 to 1
    float4 diffuse = d * lightDiffuse * materialDiffuse;
    
    // specular color
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialPower);
    float4 specular = s * lightSpecular * materialSpecular;
    
    // emissive
    float4 emissive = materialEmissive;
    
    // texture color
    float4 colorToUse = lowMap.Sample(textureSampler, input.texCoord);
    if (useBlend)
    {
        float blendStart = blendHeight - (blendThickness * 0.5f);
        float blendEnd = blendHeight + (blendThickness * 0.5f);
        float4 highMapColor = highMap.Sample(textureSampler, input.texCoord);
        if (input.worldPosition.y > blendEnd)
        {
            colorToUse = highMapColor;
        }
        else if (input.worldPosition.y > blendStart)
        {
            float t = saturate((input.worldPosition.y - blendStart) / blendThickness); // saturate means value will be between 0 and 1
            colorToUse = lerp(colorToUse, highMapColor, t);
        }
    }
    
    float4 finalColor = (emissive + ambient + diffuse) * colorToUse + (specular);
    return finalColor;
}