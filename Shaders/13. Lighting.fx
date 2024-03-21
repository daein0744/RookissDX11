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

// Diffuse(�л걤)
// ��ü�� ǥ�鿡 �л�Ǿ� ������ �ٷ� ������ ��
// ������ ���� ��Ⱑ �ٸ���(Lambert ����)

// Emissive
// �ܰ��� ���� �� ���
// (������Ʈ)
float4 PS(MeshOutput input) : SV_TARGET
{
    float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
    
    return color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
};
