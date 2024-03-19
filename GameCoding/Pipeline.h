#pragma once
#include "ShaderBase.h"

struct PipelineInfo
{
	shared_ptr<class InputLayout> inputLayout;
	shared_ptr<class VertexShader> vertexShader;
	shared_ptr<class PixelShader> pixelShader;
	shared_ptr<class RasterizerState> rasterizerState;
	shared_ptr<class BlendState> blendState;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

template<typename T> class ConstantBuffer;

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> deviceContext);
	~Pipeline();

	void UpdatePipeline(PipelineInfo info);
	void SetVertexBuffer(shared_ptr<class VertexBuffer> buffer);
	void SetIndexBuffer(shared_ptr<class IndexBuffer> buffer);

	template <typename T>
	void SetConstantBuffer(int slot, uint32 scope, shared_ptr<ConstantBuffer<T>> buffer);

	void SetTexture(uint32 slot, uint32 scope, shared_ptr<class Texture> texture);
	void SetSamplerState(uint32 slot, uint32 scope, shared_ptr<class SamplerState> samplerState);

	void Draw(uint32 vertexCount, uint32 startVertexLocation);
	void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation);
private:
	ComPtr<ID3D11DeviceContext> _deviceContext;
};

template<typename T>
inline void Pipeline::SetConstantBuffer(int slot, uint32 scope, shared_ptr<ConstantBuffer<T>> buffer)
{
	if(scope & SS_VertexShader)
		_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
	if(scope & SS_PixelShader)
		_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
}
