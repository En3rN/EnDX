struct PSIn
{
    float3 pos : Position;
    float4 posW : SV_Position;
    float4 color : Color;
    float3 texcord : TexCoord;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 bitangent : BiTangent;
    float3 toEye : ToEye;
    float4 debug : Debug;
};
struct Lights
{
    float4 color;
    float3 attenuation;
    float coneOuter;
    float3 position;
    bool isDirectional;
    float3 direction;
    float coneInner; // spotlight;
};

struct LightParameters
{
    float3 dirToL;
    float3 disToL;
    float3 color;
};

struct Light
{
    float3 diffuse;
    float3 specular;
};

struct Material
{
    float4 diffuse;
    float3 specular;
    float specularPower;
    float3 emissive;
    float specularIntensity;
};