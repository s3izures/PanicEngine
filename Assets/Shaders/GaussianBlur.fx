cbuffer SettingsBuffer : register(b0)
{
    float screenX;
    float screenY;
    float multiplier;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

static const float gaussianWeights[5] = { 0.227027f, 0.1959564f, 0.1216216f, 0.054054f, 0.016216f };

float4 HorizontalBlurPS(VS_OUTPUT input) : SV_Target
{
    float2 offset = float2(2.0f / screenX, 0.0f);
    float4 finalColor = textureMap.Sample(textureSampler, input.texCoord) * gaussianWeights[0];
    for (int i = 1; i < 5; ++i)
    {
        finalColor += textureMap.Sample(textureSampler, input.texCoord + (offset * i)) * gaussianWeights[i];
        finalColor += textureMap.Sample(textureSampler, input.texCoord - (offset * i)) * gaussianWeights[i];
    }
    
    return finalColor * multiplier;
}

float4 VerticalBlurPS(VS_OUTPUT input) : SV_Target
{
    float2 offset = float2(0.0f, 2.0f / screenY);
    float4 finalColor = textureMap.Sample(textureSampler, input.texCoord) * gaussianWeights[0];
    for (int i = 1; i < 5; ++i)
    {
        finalColor += textureMap.Sample(textureSampler, input.texCoord + (offset * i)) * gaussianWeights[i];
        finalColor += textureMap.Sample(textureSampler, input.texCoord - (offset * i)) * gaussianWeights[i];
    }
    
    return finalColor * multiplier;
}