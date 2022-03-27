Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
TextureCube cubeDiffuseMap : register(t3);
SamplerState tMapSampler : register(s0);

#include "Structs.hlsli"
#define numLights 3

cbuffer MaterialCB : register(b1)
{
    Material material;
}


cbuffer LightCB : register(b0)
{
    float4 LightAmbient;
    Lights lights[numLights];
}

float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}

float3 NormalTangentToTargetSpace(float2 texCord, float3 tan, float3 bitan, float3 normal)
{
    const float3 sampleNormal = normalMap.Sample(tMapSampler, texCord);
    float3 reMappedNormalized = normalize(sampleNormal * 2 - 1);
#ifdef FLIPYNORMALMAP
    reMappedNormalized.y *= -1;
#endif

    tan = normalize(tan);
    bitan = normalize(bitan);
    normal = normalize(normal);
    const float3x3 tbnMatrix = float3x3(tan, bitan, normal);
    normal = normalize(mul(reMappedNormalized, tbnMatrix));
    
    return normal;
}

LightParameters CalculateLightParameters(Lights light, float3 position)
{
    LightParameters result;
    if(light.isDirectional)
    {
        result.dirToL = normalize(-light.direction);
        result.color = light.color;
        return result;
    }
    result.dirToL = light.position - position;
    result.disToL = length(result.dirToL);
    result.dirToL = normalize(result.dirToL);

    float dotL = dot(light.direction, result.dirToL);
    float conAtt = dotL > light.coneInner;
    float isPointLight = light.coneOuter >= 1;
    conAtt = max(conAtt, isPointLight);
    result.color = light.color.rgb * conAtt * Attenuate(light.attenuation.x, light.attenuation.y, light.attenuation.z,result.disToL);
    return result;
}

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
    float specularIntensity,
    float3 lightColor
    )
{
    // compute specular contribution
    float3 vHalf = normalize(lightNormal + toEye);
    float specularAmount = saturate(dot(surfaceNormal, vHalf));
    specularAmount = pow(specularAmount, max(materialSpecularPower, 0.0001f)) * specularIntensity;
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

Light CalculateLight(float3 position, float3 normal, float3 specularColor, float specularPower, float3 toEye)
{
    Light result;
    float3x3 diffuse;
    float3x3 specular;
    LightParameters params[numLights];

    [unroll]
    for (int i = 0; i < numLights;i++)
    {
        params[i] = CalculateLightParameters(lights[i], position);
        //diffuse
        diffuse[i]  = LambertLighting(params[i].dirToL, normal, params[i].color.rgb);
        //specular
        specular[i] = SpecularContribution(toEye, params[i].dirToL, normal, specularColor, specularPower, material.specularIntensity, params[i].color.rgb) * diffuse[i];
    }
    result.diffuse = max(max(diffuse[0], diffuse[1]), diffuse[2]);
    result.specular = max(max(specular[0], specular[1]), specular[2]);
    return result;
}

float4 Diffuse(PSIn psin) : SV_Target
{
    const float4 sampleDiffuse = diffuseMap.Sample(tMapSampler, psin.texcord.xy);
   
#ifdef ALPHATEST
    clip(sampleDiffuse.w < 0.01f ? -1:1);
#endif
    float3 materialColor = saturate(sampleDiffuse.rgb);
    float3 specularColor = material.specular;
    float specularPower = pow(2.0f, material.specularPower * 13.0f);
    
    float3 normal = normalize(psin.normal);
    float3 toEye = normalize(psin.toEye);
    
    //ambient
    float3 ambient = LightAmbient.rgb * sampleDiffuse.w;
    
    Light light = CalculateLight(psin.pos, normal, specularColor, specularPower, toEye);

    return float4((ambient + light.diffuse + .1f) * (materialColor) - .1f + light.specular + material.emissive, 1);
}

float4 DiffuseSpecular(PSIn psin) : SV_Target
{
    
    const float4 sampleDiffuse = diffuseMap.Sample(tMapSampler, psin.texcord.xy);
    const float4 sampleSpecular = specularMap.Sample(tMapSampler, psin.texcord.xy);
    
#ifdef ALPHATEST
    clip(sampleDiffuse.w < 0.01f ? -1:1);
#endif
    float3 materialColor = saturate(sampleDiffuse.rgb);
    float3 specularColor =sampleSpecular.rgb;
    float specularPower = pow(2.0f, sampleSpecular.a * 13.0f);    
    
    float3 normal = normalize(psin.normal);
    float3 toEye = normalize(psin.toEye);
    
    //ambient
    float3 ambient = LightAmbient.rgb * sampleDiffuse.w;
    
    Light light = CalculateLight(psin.pos, normal, specularColor, specularPower, toEye);

    return float4((ambient + light.diffuse + .1f) * (materialColor) - .1f + light.specular + material.emissive, 1);
}

float4 DiffuseSpecularNormal(PSIn psin) : SV_Target
{
    
    const float4 sampleDiffuse = diffuseMap.Sample(tMapSampler, psin.texcord.xy);
    const float4 sampleSpecular = specularMap.Sample(tMapSampler, psin.texcord.xy);    
#ifdef ALPHATEST
    clip(sampleDiffuse.w < 0.01f ? -1:1);
#endif
    float3 materialColor = saturate(sampleDiffuse.rgb);
    float3 specularColor = sampleSpecular.rgb;
    
    float specularPower = pow(2.0f, sampleSpecular.a * 13.0f);
    float3 normal = NormalTangentToTargetSpace(psin.texcord.xy, psin.tangent, psin.bitangent, psin.normal);
    float3 toEye = normalize(psin.toEye);
    
    //ambient
    float3 ambient = LightAmbient.rgb * sampleDiffuse.w;
    Light light = CalculateLight(psin.pos, normal, specularColor, specularPower, toEye);

    return float4((ambient + light.diffuse + .1f) * (materialColor) - .1f + light.specular + material.emissive, 1);
}
float4 DiffuseNormal(PSIn psin) : SV_Target
{
    const float4 sampleDiffuse = diffuseMap.Sample(tMapSampler, psin.texcord.xy);   
#ifdef ALPHATEST
    clip(sampleDiffuse.w < 0.01f ? -1:1);
#endif
    float3 materialColor = saturate(sampleDiffuse.rgb);
    float3 specularColor = material.specular;
    float specularPower = pow(2.0f, material.specularPower * 13.0f);

    float3 normal = NormalTangentToTargetSpace(psin.texcord.xy, psin.tangent, psin.bitangent, psin.normal);
    float3 toEye = normalize(psin.toEye);    
    
    //ambient
    float3 ambient = LightAmbient.rgb * sampleDiffuse.w;
    
    Light light = CalculateLight(psin.pos, normal, specularColor, specularPower, toEye);

    return float4((ambient + light.diffuse + .1f) * (materialColor) - .1f + light.specular + material.emissive, 1);
}

float4 main(PSIn psin) : SV_Target
{
#ifdef ALPHATEST
    clip(sampleDiffuse.w < 0.01f ? -1:1);
#endif
    float3 materialColor = material.diffuse.rgb;
    float3 specularColor = material.specular;
    float specularPower = material.specularPower;
    float3 normal = normalize(psin.normal);
    float3 toEye = normalize(psin.toEye);
    
    //ambient
    float3 ambient = LightAmbient.rgb;
    
    Light light = CalculateLight(psin.pos,normal,specularColor,specularPower,toEye);

    return float4((ambient + light.diffuse + .1f) * (materialColor) - .1f + light.specular + material.emissive, 1);
}

float4 debug(PSIn psin) : SV_Target
{
    return float4(NormalTangentToTargetSpace(psin.texcord.xy, psin.tangent, psin.bitangent, psin.normal), 1);
    //return float4(psin.tangent, 1);
}



