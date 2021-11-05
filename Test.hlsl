Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t3);
TextureCube cubedifuseMap : register(t4);
SamplerState tMapSampler : register(s0);

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



float4 main(PSIn psin) : SV_Target
{
    float4 materialDiffuse = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float3 ambientLight = float4(0.1f, 0.1f, 0.1f, 0.1f);
    float3 lightColor = float3(0.8f, 0.8f, 0.8f);
    float3 lightDirection = float3( 0.0f, 0.5f,-0.5f );
    
    float4 sampleDiffuse = diffuseMap.Sample(tMapSampler, psin.texcord.xy);
    
    //ambient
    float3 ambient = ambientLight * sampleDiffuse.rgb + ambientLight * materialDiffuse.xyz;
    
    //diffuse
    float light = saturate(dot(normalize(psin.normal), normalize(lightDirection)));
    float3 diffuse = light * lightColor * sampleDiffuse.rgb + light * lightColor * materialDiffuse.rgb;
    //specular
    float3 specular = (0.0f, 0.0f, 0.0f);
    
    return CombineRGBWithAlpha(ambient + diffuse + specular,materialDiffuse.a*sampleDiffuse.a);
}