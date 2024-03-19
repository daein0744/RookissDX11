
struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer CameraData : register(b0)
{
    row_major float4x4 matView;
    row_major float4x4 matProjection;
}

cbuffer TransformData : register(b1)
{
    row_major float4x4 matWorld;
}

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);
    
    output.uv = input.uv;
	return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, input.uv);
    
    return color;
}