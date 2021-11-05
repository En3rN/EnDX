Texture2D textureMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t3);
TextureCube cube : register(t0);
SamplerState smpler : register(s0);

//cbuffer Material : register(b0)
//{
//    float4 MaterialDiffuse;
//    float4 MaterialSpecular;
//    float4 MaterialEmissive;
//    float MaterialSpecularPower;
//}

//cbuffer Light : register(b1)
//{
//    float4 AmbientLight;
//    float4 LightColor;
//    float4 LightAttenuation;
//    float3 LightDirection;
//    float  LightSpecularIntensity;
    
//}


struct Vs2Ps
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

float3 ambientLight = float3(0.5, 0.5, 0.5);
float4 MaterialDiffuse = float4(0.5, 0.5, 0.5,1);
float3 lightDirection = float3(0.5, 0.5, 0);
float3 lightColor = float3(1, 1, 1);



float4 Mat(Vs2Ps psin) : SV_Target
{    
    float3 normal = normalize(psin.normal.xyz);
    float3 diffuse = ambientLight * MaterialDiffuse.xyz;
    diffuse += LambertLighting(lightDirection, normal, lightColor, diffuse);
    diffuse = saturate(diffuse);
    return CombineRGBWithAlpha(diffuse, MaterialDiffuse.w);
}

float4 t2d(Vs2Ps psin) : SV_Target
{
    float3 normal = normalize(psin.normal.xyz);
    float4 sampleDiffuse = textureMap.Sample(smpler, psin.texcord.xy);
    float3 diffuse = ambientLight * MaterialDiffuse.xyz * sampleDiffuse.xyz;
    diffuse += LambertLighting(lightDirection, normal, lightColor, diffuse);
    diffuse = saturate(diffuse);
    return CombineRGBWithAlpha(diffuse, MaterialDiffuse.w* sampleDiffuse.w);
}

float4 tcube(Vs2Ps psin) : SV_Target
{
    float3 normal = normalize(psin.normal.xyz);
    float4 sampleDiffuse = cube.Sample(smpler, psin.texcord.xyz);
    float3 diffuse = ambientLight * MaterialDiffuse.xyz * sampleDiffuse.xyz;
    diffuse += LambertLighting(lightDirection, normal, lightColor, diffuse);
    diffuse = saturate(diffuse);
    return CombineRGBWithAlpha(diffuse, MaterialDiffuse.w*sampleDiffuse.w);
}

float4 debug(Vs2Ps psin) : SV_Target
{
    return psin.debug;
}