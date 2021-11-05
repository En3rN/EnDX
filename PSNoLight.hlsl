Texture2D tex : register(t0);
TextureCube cube : register(t0);
SamplerState smpler : register(s0);

cbuffer Material : register(b0)
{
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialEmissive;
    float MaterialSpecularPower;
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

PSOut main(PSIn psin)
{
    PSOut psout;
    psout.target = MaterialDiffuse;
    return psout;
}

PSOut t2d(PSIn psin)
{
    PSOut psout;
    psout.target = tex.Sample(smpler, psin.texcord.xy);
    return psout;
}

PSOut tcube(PSIn psin)
{
    PSOut psout;
    psout.target = cube.Sample(smpler, psin.texcord.xyz);
    return psout;
}

PSOut debug(PSIn psin)
{
    PSOut psout;
    psout.target = psin.debug;
    return psout;
}