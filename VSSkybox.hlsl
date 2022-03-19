struct VSOut
{
    float3 ndcpos : Position;
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


VSOut main(float3 pos : Position)
{
    matrix mvp;
    VSOut vso;
    vso.ndcpos = pos;
    mvp = mul(model, view);
    mvp = mul(mvp, proj);    
    vso.pos = mul(float4(pos, 0.0f), mvp);
    // make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
    vso.pos.z = vso.pos.w;
    return vso;
}