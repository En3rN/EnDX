struct VSOut
{
    float3 ndcpos : Position;
    float4 pos : SV_Position;
};
cbuffer CBuf
{
    matrix transform;
};

VSOut main(float3 pos : Position)
{
    VSOut vso;
    vso.ndcpos = pos;
    vso.pos = mul(float4(pos, 1.0f), transform);
    return vso;
}