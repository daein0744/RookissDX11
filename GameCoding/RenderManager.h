#pragma once
#include "RenderHelper.h"

class RasterizerState;
class SamplerState;
class BlendState;
class GameObject;
class Pipeline;
class Graphics;

template <typename T> class ConstantBuffer;

class RenderManager
{
public:
	RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~RenderManager();

	void Init();
	void Update(shared_ptr<Graphics> graphics);

private:
	void PushCameraData();
	void PushTransformData();
	void PushAnimationData();

	void GatherRenderableObjects();
	void RenderObjects();

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	shared_ptr<Pipeline> _pipeline;

private:
	// Camera
	CameraData	_cameraData = {};
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;

	// SRT
	TransformData	_transformData = {};
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;

	// Animation
	AnimationData _animationData = {};
	shared_ptr<ConstantBuffer<AnimationData>> _animationBuffer;

private:
	shared_ptr<SamplerState> _samplerState;
	shared_ptr <RasterizerState> _rasterizerState;
	shared_ptr<BlendState> _blendState;

	vector<shared_ptr<GameObject>> _renderObjects;
};