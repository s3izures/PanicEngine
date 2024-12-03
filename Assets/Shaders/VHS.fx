//Description: An effect that mimics a retro, VHS-like aesthetic

//PARAMETERS
cbuffer SettingsBuffer : register(b0)
{
    float abberationIntensity;
    float scanlineIntensity;
    float scanlineDensity;
    float noiseIntensity;
    float noiseSpeed;
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
    
    //Chromatic Abberation
    float2 distortion = float2(abberationIntensity, -abberationIntensity);
    float4 redChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
    float4 greenChannel = textureMap0.Sample(textureSampler, input.texCoord);
    float4 blueChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
    finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    
    //Scanlines
    float scanline = sin(input.texCoord.y * scanlineDensity) * 0.5 + 0.5;
    finalColor.rgb *= lerp(1.0, scanline, scanlineIntensity);
    //HOW DO I MAKE SCANLINES MOVE
    
    //Noise
    //float time = 0.02; //NEED A TIME FUNCTION
    //Make noise according to speed
    //Make Function to generate pseudo-random value
    //chromaticRes.rgb *= lerp(chromaticRes.rgb, noise, float3(noiseIntensity));
    
    //Noise v2
    float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
    float4 color1 = staticMap.Sample(textureSampler, input.texCoord);
    if(color1.r > 0.0)
    {
        finalColor = (color0 + color1) * 0.5f;
    }
    else
    {
        finalColor = color0;
    }

    return finalColor;
}