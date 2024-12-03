//Description: An effect that mimics a retro, VHS-like aesthetic

//PARAMETERS
cbuffer SettingsBuffer : register(b0)
{
    float abberationIntensity;
    float scanlineIntensity;
    float scanlineThickness;
    float noiseIntensity;
    float noiseSpeed;
}

Texture2D textureMap0 : register(t0);

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
    //Chromatic Abberation
    float4 chromaticRes;
    float2 distortion = float2(abberationIntensity, -abberationIntensity);
    float4 redChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
    float4 greenChannel = textureMap0.Sample(textureSampler, input.texCoord);
    float4 blueChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
    chromaticRes = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    
    //Scanlines
    float scanline = sin(input.texCoord.y * scanlineThickness) * 0.5 + 0.5;
    chromaticRes.rgb *= lerp(1.0, scanline, scanlineIntensity);
    //HOW DO I MAKE SCANLINES MOVE
    
    //Noise
    //float time = 0.02; //NEED A TIME FUNCTION
    //float2 noiseUV = input.texCoord + float2(time * 0.1 * noiseSpeed, time * 0.2 * noiseSpeed);
    //// Make Function to generate pseudo-random value
    //float noise = (sin(dot(noiseUV.xy, float2(12.9898, 78.233))) * 43758.5453);
    //chromaticRes.rgb *= lerp(chromaticRes.rgb, float3(noise), noiseIntensity);
    
    //Combine everything
    float4 finalColor = chromaticRes;
    return finalColor;
}