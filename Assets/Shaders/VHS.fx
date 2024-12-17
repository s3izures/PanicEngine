//Description: An effect that mimics a retro, VHS-like aesthetic

//PARAMETERS
cbuffer SettingsBuffer : register(b0)
{
    float abberationIntensity;
    float scanlineIntensity;
    float scanlineDensity;
    float noiseIntensity;
    float distortionIntensity;
}

Texture2D textureMap0 : register(t0);
Texture2D staticMap : register(t1);

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

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor;
    
    //Curve
    float2 center = float2(0.5f, 0.5f);
    float2 uvDistort = input.texCoord - center;
    uvDistort *= 1.0 + distortionIntensity * (uvDistort.x * uvDistort.x + uvDistort.y * uvDistort.y);
    input.texCoord = uvDistort + center;
    
    bool isEdge = (input.texCoord.x < 0.0f || input.texCoord.x > 1.0f || input.texCoord.y < 0.0f || input.texCoord.y > 1.0f);

    //Chromatic Abberation
    float2 distortion = float2(abberationIntensity, -abberationIntensity);
    float4 redChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
    float4 greenChannel = textureMap0.Sample(textureSampler, input.texCoord);
    float4 blueChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
    finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    
    //Scanlines
    float scanline = sin(input.texCoord.y * scanlineDensity) * 0.5 + 0.5;
    finalColor.rgb *= lerp(1.0, scanline, scanlineIntensity);
    
    //Noise v2
    float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
    float4 color1 = staticMap.Sample(textureSampler, input.texCoord);
    if(color1.r > 0.0)
    {
        finalColor.rgb *= (color0 + color1) / noiseIntensity; //Divide for the bright burn look
    }
    else
    {
        finalColor.rgb *= color0;
    }
    
        
    if (isEdge)
    {
        finalColor *= float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    return finalColor;
}