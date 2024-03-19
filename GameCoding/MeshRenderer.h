#pragma once
#include "Component.h"

class VertexBuffer;
class IndexBuffer;
class InputLayout;
class VertexShader;
class PixelShader;
class Texture;

template <typename T> class Geometry;

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer();

	virtual void Update() override;
	void Render(shared_ptr<class Pipeline> pipeline);
private:
	friend class RenderManager;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	// IA
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	// VS
	shared_ptr<VertexShader> _vertexShader;

	// PS
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture1;
};

