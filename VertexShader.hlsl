struct VSOut
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
cbuffer camera : register(b0)
{
    matrix view;
    matrix worldCam;
    matrix proj;
};
cbuffer model : register(b1)
{
    matrix model;
};
struct VSIn
{
    float3 pos : Position;
    float3 texcord : TexCoord;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 bitangent : BiTangent;
};

//float3 pos : Position;
//float4 posW : SV_Position;
//float4 color : Color;
//float4 texcord : TexCoord;
//float3 normal : Normal;
//float3 tangent : Tangent;
//float3 bitangent : BiTangent;
//float3 toEye : ToEye;
//float4 debug : Debug;

VSOut main(VSIn vsin) 
{ 
    VSOut vso = (VSOut) 0;
    matrix mvp;
    matrix mv;
    mv = mul(model, view);
    mvp = mul(mv, proj);
    vso.pos = mul(float4(vsin.pos, 1.0f), model);

    vso.posW = mul(float4(vsin.pos, 1.0f), mvp);
    vso.texcord = float4(vsin.texcord, 0);
    #ifdef FLIPTC
    vso.texcord.z *= -1;
    #endif

    float3 camPos = worldCam._41_42_43;
    float3 camDir = worldCam._31_32_33;

    vso.toEye = camPos - vso.pos;
    vso.normal = normalize(mul(vsin.normal.xyz, (float3x3) model));
    vso.tangent = normalize(mul(vsin.tangent, (float3x3) model));
    vso.bitangent = normalize(mul(vsin.bitangent, (float3x3) model));
    //vso.normal = vsin.normal;
    //vso.tangent = vsin.tangent;
    //vso.bitangent = vsin.bitangent;

    vso.debug = float4(vso.bitangent, 1);
    return vso;
}