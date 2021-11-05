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
    vso.pos = mul(vsin.pos, (float3x4) model);
    vso.posW = mul(float4(vsin.pos, 1.0f), mvp);
    vso.texcord = float4(vsin.texcord, 0);
    #ifdef FLIPTC
    vso.texcord.z *= -1;
    #endif
    vso.normal = mul(vsin.normal, (float3x4) model);
    return vso;
}