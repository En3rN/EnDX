
#include "Structs.hlsli"

 PSIn main( float3 pos : Position, float3 tc : TexCoord )
{
	PSIn vso = (PSIn) 0;
    vso.texcord = tc;
    vso.posW = float4(pos, 1);
	return vso;
}