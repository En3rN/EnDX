Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
TextureCube cubeDifuseMap : register(t3);
SamplerState tMapSampler : register(s0);

#include "Structs.hlsli"


cbuffer MaterialCB : register(b1)
{
    Material material;
}

cbuffer LightCB : register(b0)
{
    float4 LightAmbient;
    Lights light[3];
}

struct PSOut
{
    float4 target : SV_Target;
};

float3 LambertLighting(
    float3 lightNormal,
    float3 surfaceNormal,
    float3 lightColor
    )
{
    // compute amount of contribution per light
    float diffuseAmount = saturate(dot(lightNormal, surfaceNormal));
    float3 diffuse = diffuseAmount * lightColor;
    return diffuse;
};

float3 SpecularContribution(
    float3 toEye,
    float3 lightNormal,
    float3 surfaceNormal,
    float3 materialSpecularColor,
    float materialSpecularPower,
    float lightSpecularIntensity,
    float3 lightColor
    )
{
    // compute specular contribution
    float3 vHalf = normalize(lightNormal + toEye);
    float specularAmount = saturate(dot(surfaceNormal, vHalf));
    specularAmount = pow(specularAmount, materialSpecularPower) * lightSpecularIntensity;
    float3 specular = materialSpecularColor * lightColor * specularAmount;
    
    return specular;
}

//
// combines a float3 RGB value with an alpha value into a float4
//
float4 CombineRGBWithAlpha(float3 rgb, float a)
{
    return float4(rgb.r, rgb.g, rgb.b, a);
};

//#define ALPHATEST;


float4 main2(PSIn psin) : SV_Target
{
    
    const float4 sampleDiffuse = diffuseMap.Sample(tMapSampler, psin.texcord.xy);
    const float4 sampleSpecular = specularMap.Sample(tMapSampler, psin.texcord.xy);
    const float3 sampleNormal = normalMap.Sample(tMapSampler, psin.texcord.xy);
#ifdef ALPHATEST
    clip(sampleDiffuse.w < 0.01f ? -1:1);
#endif
    float3 materialColor = max((sampleDiffuse.rgb), material.diffuse.
    rgb);
    float3 specularColor = sampleSpecular.rgb; // * step(sampleSpecular.rgb, 0));
    float specularPower = max(pow(2.0f, sampleSpecular.a * 13.0f), material.specularPower * step(sampleSpecular.a, 0));
    //float specularPower = pow(2.0f, sampleSpecular.a * 13.0f);
    float3 NLightDirection = normalize(light[0].direction);
    float3 NSurfaceNormal = normalize(psin.normal);
    float3 NtoEye = normalize(psin.toEye);
    
    //ambient
    float3 ambient = LightAmbient.rgb;
    //diffuse
    float3 diffuse = LambertLighting(NLightDirection, NSurfaceNormal, light[0].color.rgb);
    //specular
    float3 specular = SpecularContribution(NtoEye, NLightDirection, NSurfaceNormal, specularColor, specularPower, material.specularIntensity, light[0].color.rgb);
    
    return float4((ambient + diffuse + .1f) * (materialColor) - .1f+specular + material.emissive, 1);
}

float4 main(PSIn psin) : SV_TARGET
{
    const float4 sampleDiffuse = diffuseMap.Sample(tMapSampler, psin.texcord.xy);
    const float3 sampleNormal = normalMap.Sample(tMapSampler, psin.texcord.xy);
    const float3 normalObj = float3(sampleNormal.x * 2 - 1, sampleNormal.y * 2 - 1, sampleNormal.z * 2 - 1);
#ifdef ALPHATEST
    clip(sampleDiffuse.w < 0.01f ? -1:1);
#endif
    float3 materialColor = saturate(sampleDiffuse.rgb);
    float3 specularColor = material.specular;
    
    float specularPower = material.specularPower;
    float3 NLightDirection = normalize(light[0].direction);
    float3 NSurfaceNormal = normalize(psin.normal);
    float3 NtoEye = normalize(psin.toEye);
    const float3x3 tbnMatrix = float3x3(psin.tangent, psin.bitangent, psin.normal);
    NSurfaceNormal = normalize(mul(sampleNormal, tbnMatrix));
    
    //ambient
    float3 ambient = LightAmbient.rgb * sampleDiffuse.w;
    //diffuse
    float3 diffuse = LambertLighting(NLightDirection, NSurfaceNormal, light[0].color.rgb);
    //specular
    float3 specular = SpecularContribution(NtoEye, NLightDirection, NSurfaceNormal, specularColor, specularPower, material.specularIntensity, light[0].color.rgb);
    
    return float4((ambient + diffuse + .1f) * (materialColor) - .1f + specular + material.emissive, 1);
}