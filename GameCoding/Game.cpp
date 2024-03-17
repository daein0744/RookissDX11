#include "pch.h"
#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;
	
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	CreateVS(_vsBlob);
	CreateInputLayout();
	CreatePS(_psBlob);
}

void Game::Update()
{

}

void Game::Render()
{
	// RS, OM
	RenderBegin();

	// IA - VS - RS - PS - OM
	
	// IA
	uint32 strides = { sizeof(Vertex) };
	uint32 offset = { 0 };
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &strides, &offset);
	_deviceContext->IASetInputLayout(_inputLayout.Get());
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VS
	_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

	// PS
	_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	_deviceContext->Draw((uint32)_vertices.size(), 0);
	RenderEnd();
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferDesc.Width = _width;
	desc.BufferDesc.Height = _height;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = _hwnd;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);
	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
		, (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	hr = _device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		_renderTargetView.GetAddressOf()
	);
	CHECK(hr);
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

void Game::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateGeometry()
{
	_vertices.resize(3);
	_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
	_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
	_vertices[1].position = Vec3(0.f, 0.5f, 0.f);
	_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
	_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
	_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size();

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = _vertices.data();
	HRESULT hr = _device->CreateBuffer(
		&bufferDesc
		, &subData
		, _vertexBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str()
		, version.c_str()
		, compileFlag
		, 0
		, blob.GetAddressOf()
		, nullptr);
	CHECK(hr);
}

void Game::CreateVS(ComPtr<ID3DBlob>& blob)
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);
	HRESULT hr = _device->CreateVertexShader(
		blob->GetBufferPointer()
		, blob->GetBufferSize()
		, nullptr
		, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS(ComPtr<ID3DBlob>& blob)
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(
		blob->GetBufferPointer()
		, blob->GetBufferSize()
		, nullptr
		, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = _device->CreateInputLayout(inputLayout
		, sizeof(inputLayout) / sizeof(D3D11_INPUT_ELEMENT_DESC)
		, _vsBlob->GetBufferPointer()
		, _vsBlob->GetBufferSize()
		, _inputLayout.GetAddressOf());
	CHECK(hr);
}
