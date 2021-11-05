Texture2D textureMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t3);
TextureCube cube : register(t0);
SamplerState smpler : register(s0);

cbuffer Material : register(b0)
{
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialEmissive;
    float MaterialSpecularPower;
}

cbuffer Light : register(b1)
{
    float4 AmbientLight;
    float4 LightColor;
    float4 LightAttenuation;
    float3 LightDirection;
    float LightSpecularIntensity;
    
}


struct PSIn
{
    float3 pos : Position;
    float4 posW : SV_Position;
    float4 color : Color;
    float4 texcord : TexCoord;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 bitangent : BiTangent;
    float3 toEye : ToEye;
    float4 debug : Debug;
};

struct PSOut
{
    float4 target : SV_Target;
};

float3 LambertLighting(
    float3 lightNormal,
    float3 surfaceNormal,
    float3 lightColor,
    float3 pixelColor
    )
{
    // compute amount of contribution per light
    float diffuseAmount = saturate(dot(lightNormal, surfaceNormal));
    float3 diffuse = diffuseAmount * lightColor * pixelColor;
    return diffuse;
};

//
// combines a float3 RGB value with an alpha value into a float4
//
float4 CombineRGBWithAlpha(float3 rgb, float a)
{
    return float4(rgb.r, rgb.g, rgb.b, a);
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
    specularAmount = pow(specularAmount, max(materialSpecularPower, 0.0001f)) * lightSpecularIntensity;
    float3 specular = materialSpecularColor * lightColor * specularAmount;
    
    return specular;
}


PSOut main(PSIn psin)
{
    
    float3 normal = normalize(psin.normal.xyz);
    float3 toEye = normalize(psin.toEye.xyz);
    float3 lightDirection = normalize(lightDirection.xyz);
    
    float3 diffuse = AmbientLight.xyz * MaterialDiffuse.xyz;
    diffuse += LambertLighting(LightDirection.xyz, normal, LightColor.xyz, diffuse);
    diffuse += SpecularContribution(toEye, LightDirection.xyz, normal, MaterialSpecular.xyz, MaterialSpecularPower, LightSpecularIntensity, LightColor.xyz);
    diffuse = saturate(diffuse);
    PSOut psout;
    psout.target = CombineRGBWithAlpha(diffuse, MaterialDiffuse.w);
    return psout;
}

PSOut t2d(PSIn psin)
{
    float3 normal = normalize(psin.normal.xyz);
    float4 sampleDiffuse = textureMap.Sample(smpler, psin.texcord.xy);
    float3 diffuse = AmbientLight.xyz * MaterialDiffuse.xyz * sampleDiffuse.xyz;
    diffuse += LambertLighting(LightDirection.xyz, normal, LightColor.xyz, diffuse);
    diffuse = saturate(diffuse);
    PSOut psout;
    psout.target = CombineRGBWithAlpha(diffuse, MaterialDiffuse.w);
    return psout;
}

PSOut tcube(PSIn psin)
{
    float3 normal = normalize(psin.normal.xyz);
    float4 sampleDiffuse = cube.Sample(smpler, psin.texcord.xyz);
    float3 diffuse = AmbientLight.xyz * MaterialDiffuse.xyz * sampleDiffuse.xyz;
    diffuse += LambertLighting(LightDirection.xyz, normal, LightColor.xyz, diffuse);
    diffuse = saturate(diffuse);
    PSOut psout;
    psout.target = CombineRGBWithAlpha(diffuse, MaterialDiffuse.w);
    return psout;
}

PSOut debug(PSIn psin)
{
    PSOut psout;
    psout.target = psin.debug;
    return psout;
}