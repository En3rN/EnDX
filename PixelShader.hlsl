
Texture2D tex : register(t0);
SamplerState smpler : register(s0);

float4 main(float2 tc : TexCoord ) : SV_Target
{
    return tex.Sample(smpler, tc);
}

