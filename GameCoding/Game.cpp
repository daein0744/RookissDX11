#include "pch.h"
#include "Game.h"
#include "MyMatrix.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_graphics = make_shared<Graphics>(hwnd);
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());

	CreateGeometry();
	CreateVS(_vsBlob);
	CreateInputLayout();
	CreatePS(_psBlob);
	CreateSRV();
	CreateConstantBuffer();
	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();
}

void Game::Update()
{
	auto _deviceContext = _graphics->GetDeviceContext();

	 Matrix matScale = MyMatrix::CreateScale(_localScale);
	 Matrix matRotX = MyMatrix::CreateRotationX(XMConvertToRadians(_localRotation.x));
	 Matrix matRotY = MyMatrix::CreateRotationY(XMConvertToRadians(_localRotation.y));
	 Matrix matRotZ = MyMatrix::CreateRotationZ(XMConvertToRadians(_localRotation.z));
	 Matrix matTranslation = MyMatrix::CreateTranslation(_localPosition);
	 _transformData.matWorld = matTranslation * matRotX * matRotY * matRotZ * matScale;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));
	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &_transformData, sizeof(_transformData));
	_deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	auto _deviceContext = _graphics->GetDeviceContext();
	// RS, OM
	_graphics->RenderBegin();

	// IA - VS - RS - PS - OM
	// IA
	uint32 strides = { sizeof(Vertex) };
	uint32 offset = { 0 };
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &strides, &offset);
	_deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// VS
	_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	
	// RS
	_deviceContext->RSSetState(_rasterizerState.Get());

	// PS
	_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
	_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
	_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

	// OM
	_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xffffffff);

	_deviceContext->DrawIndexed((uint32)_indices.size(), 0, 0);
	_graphics->RenderEnd();
}

void Game::CreateGeometry()
{
	auto _device = _graphics->GetDevice();
	_vertices.resize(4);
	_vertices[0].position = Vec3(-0.5f, 0.5f, 0.f); // 왼쪽위
	_vertices[0].uv = Vec2(0.f, 0.f);
	_vertices[1].position = Vec3(0.5f, 0.5f, 0.f); // 오른쪽위
	_vertices[1].uv = Vec2(1.f, 0.f);
	_vertices[2].position = Vec3(0.5f, -0.5f, 0.f); // 오른쪽아래
	_vertices[2].uv = Vec2(1.f, 1.f);
	_vertices[3].position = Vec3(-0.5f, -0.5f, 0.f); // 왼쪽아래
	_vertices[3].uv = Vec2(0.f, 1.f);
	_vertexBuffer->Create(_vertices);

	_indices = { 0, 1, 2, 0, 2, 3 };
	_indexBuffer->Create(_indices);
	
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
	auto _device = _graphics->GetDevice();
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
	auto _device = _graphics->GetDevice();
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
	auto _device = _graphics->GetDevice();
	vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	_inputLayout->Create(inputLayout, _vsBlob);

}

void Game::CreateSRV()
{
	auto _device = _graphics->GetDevice();
	HRESULT hr;
	TexMetadata imageMetadata;
	ScratchImage image;
	hr = LoadFromWICFile(L"Skeleton.png", WIC_FLAGS_NONE, &imageMetadata, image);
	CHECK(hr);

	hr = CreateShaderResourceView(_device.Get(),
		image.GetImages(),
		image.GetImageCount(),
		imageMetadata,
		_shaderResourceView.GetAddressOf());
	CHECK(hr);
}

void Game::CreateConstantBuffer()
{
	auto _device = _graphics->GetDevice();
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = (uint32)sizeof(TransformData);

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = &_transformData;
	HRESULT hr = _device->CreateBuffer(
		&bufferDesc
		, &subData
		, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::CreateRasterizerState()
{
	auto _device = _graphics->GetDevice();
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSamplerState()
{
	auto _device = _graphics->GetDevice();
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.BorderColor[0] = 1.f;
	desc.BorderColor[1] = 0.f;
	desc.BorderColor[2] = 0.f;
	desc.BorderColor[3] = 1.f;
	desc.MipLODBias = 0;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	desc.MaxAnisotropy = 2;

	HRESULT hr = _device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
	CHECK(hr);
}	

void Game::CreateBlendState()
{
	auto _device = _graphics->GetDevice();
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}	
