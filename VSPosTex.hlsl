


struct VSOut
{
    float2 texcoord : TexCoord;
    float4 pos : SV_Position;
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

VSOut main(float3 pos : Position, float2 tcoord : TexCoord)
{
    VSOut vso;
    matrix mvp;
    mvp = mul(model, view);
    mvp = mul(mvp, proj);
    vso.pos = mul(float4(pos, 1.0f), mvp);
    vso.texcoord = tcoord;
    return vso;
}