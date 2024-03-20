
struct VertexInput
{
	float4 position : POSITION;
    float4 color : COLOR;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
    float4 color : COLOR;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;
    output.color = input.color;
	return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    return input.color;
}

float4 PS2(VertexOutput input) : SV_TARGET
{
	return float4(0, 1, 0, 1);
}

float4 PS3(VertexOutput input) : SV_TARGET
{
	return float4(0, 0, 1, 1);
}

RasterizerState FillModeWireFrame
{
    FillMode = Wireframe;
};


technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
        SetRasterizerState(FillModeWireFrame);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};