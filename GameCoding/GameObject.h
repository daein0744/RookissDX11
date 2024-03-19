#pragma once

class Graphics;
class VertexBuffer;
class IndexBuffer;
class InputLayout;
class VertexShader;
class PixelShader;
class Texture;
class RasterizerState;
class SamplerState;
class BlendState;
class Pipeline;
class Transform;
template <typename T> class Geometry;
template <typename T> class ConstantBuffer;

class GameObject
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Update();
	void Render(shared_ptr<class Pipeline> pipeline);

private:
	ComPtr<ID3D11Device> _device;

	// IA
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	// VS
	shared_ptr<VertexShader> _vertexShader;

	// RS
	shared_ptr <RasterizerState> _rasterizerState;

	// PS
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture1;
	shared_ptr<SamplerState> _samplerState;

	// OM	   		 
	shared_ptr<BlendState> _blendState;

private:
	// SRT
	TransformData	_transformData = {};
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;
	shared_ptr<Transform> _transform;
	shared_ptr<Transform> _parent;
};

