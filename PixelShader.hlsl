
TextureCube cube : register(t0);
SamplerState smpler : register(s0);

float4 main(float3 tc : Position ) : SV_Target
{
    return cube.Sample(smpler, tc);
}

