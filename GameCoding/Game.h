#pragma once

class Graphics;
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	HWND _hwnd = nullptr;

	void CreateGeometry();
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);
	void CreateVS(ComPtr<ID3DBlob>& blob);
	void CreatePS(ComPtr<ID3DBlob>& blob);
	void CreateInputLayout();
	void CreateSRV();
	void CreateConstantBuffer();
	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();
private:

private:
	shared_ptr<Graphics> _graphics;

	//Geometry
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	vector<Vertex> _vertices;
	vector<uint32> _indices;
	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	// SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;

	// Misc
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;
	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	ComPtr<ID3D11BlendState> _blendState = nullptr;

private:
	// SRT
	TransformData	_transformData = {};
	ComPtr<ID3D11Buffer> _constantBuffer = nullptr;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 10.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
};

