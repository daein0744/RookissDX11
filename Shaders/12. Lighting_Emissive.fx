#include "00. Global.fx"

float4 MaterialEmissive;
Texture2D DiffuseMap;

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
    float3 cameraPosition = -V._41_42_43;
    float3 E = normalize(cameraPosition - input.worldPosition);
    float value = saturate(dot(E, input.normal));
    float emissive = 1.0f - value;
    emissive = pow(emissive, 3.141592);
    
    emissive = smoothstep(0.f, 1.f, emissive);
    float4 color = MaterialEmissive * emissive;
    
    return color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
};
