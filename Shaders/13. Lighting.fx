#include "00. Global.fx"
#include "00. Light.fx"

float4 MaterialEmissive;

MeshOutput VS(VertexTextureNormal input)
{
    MeshOutput output;
	output.position = mul(input.position, W);
    output.worldPosition = mul(input.position, W);
	output.position = mul(output.position, VP);
	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	return output;
}

// Diffuse(분산광)
// 물체의 표면에 분산되어 눈으로 바로 들어오는 빛
// 각도에 따라 밝기가 다르다(Lambert 공식)

// Emissive
// 외곽선 구할 때 사용
// (림라이트)
float4 PS(MeshOutput input) : SV_TARGET
{
    float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
    
    return color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
};
