#pragma once


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
	uint32 _width = 0;
	uint32 _height = 0;
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();
	void RenderBegin();
	void RenderEnd();

	void CreateGeometry();
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);
	void CreateVS(ComPtr<ID3DBlob>& blob);
	void CreatePS(ComPtr<ID3DBlob>& blob);
	void CreateInputLayout();

private:
	// DX
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

private:
	//Geometry
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	vector<Vertex> _vertices;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;
};

