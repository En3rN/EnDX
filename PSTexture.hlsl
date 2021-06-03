Texture2D tex : register(t0);
TextureCube cube : register(t0);

SamplerState smpler : register(s0);


float4 main(float2 texcord : TexCoord) : SV_Target
{
    return tex.Sample(smpler, texcord);
}

float4 t3d(float3 texcord : TexCoord) : SV_Target
{
    return cube.Sample(smpler, texcord);
}