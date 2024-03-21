#include "00. Global.fx"

float3 LightDir;
float4 LightSpecular;
float4 MeterialSpecular;
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

// Diffuse(�л걤)
// ��ü�� ǥ�鿡 �л�Ǿ� ������ �ٷ� ������ ��
// ������ ���� ��Ⱑ �ٸ���(Lambert ����)

float4 PS(MeshOutput input) : SV_TARGET
{
    //float3 R = reflect(LightDir, input.normal);
    float3 R = LightDir - (2 * input.normal * dot(LightDir, input.normal));
    R = normalize(R);
    
    float3 cameraPosition = -V._41_42_43;
    float3 E = normalize(cameraPosition - input.worldPosition);
    
    float value = saturate(dot(R, E));
    float specular = pow(value, 10);
    
    float4 color = LightSpecular * MeterialSpecular * specular;
    
    return DiffuseMap.Sample(LinearSampler, input.uv) + color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
};
