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
	shared_ptr<Geometry<VertexColorData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	// VS
	shared_ptr<VertexShader> _vertexShader;
	TransformData	_transformData = {};

	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;
	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 10.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	// RS
	shared_ptr <RasterizerState> _rasterizerState;

	// PS
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture1;
	shared_ptr<SamplerState> _samplerState;
			   		 
	// OM	   		 
	shared_ptr<BlendState> _blendState;
};

