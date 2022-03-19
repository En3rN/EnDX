Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
TextureCube cubeDiffuseMap : register(t3);
SamplerState tMapSampler : register(s0);

#include "Structs.hlsli"

cbuffer Material : register(b1)
{
    Material material;
}

float4 main(PSIn psin) : SV_Target
{
    return material.diffuse;
}

float4 Diffuse(PSIn psin) : SV_Target
{
    return diffuseMap.Sample(tMapSampler, psin.texcord.xy);
}

float4 tcube(PSIn psin) : SV_Target
{
    return cubeDiffuseMap.Sample(tMapSampler, psin.texcord.xyz);
}

float4 debug(PSIn psin) : SV_Target
{
    return psin.debug;
}