#include "pch.h"
#include "Texture.h"

Texture::Texture(ComPtr<ID3D11Device> device) : _device(device)
{
}

Texture::~Texture()
{
}

ComPtr<ID3D11ShaderResourceView> Texture::GetComPtr()
{
	return _shaderResourceView;
}

void Texture::Create(const wstring& path)
{
	TexMetadata imageMetadata;
	ScratchImage image;
	HRESULT hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &imageMetadata, image);
	CHECK(hr);

	hr = CreateShaderResourceView(_device.Get(),
		image.GetImages(),
		image.GetImageCount(),
		imageMetadata,
		_shaderResourceView.GetAddressOf());
	CHECK(hr);
}
