#pragma once

class SceneManager;
class InputManager;
class TimeManager;
class ResourceManager;
class RenderManager;
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

	shared_ptr<SceneManager> GetSceneManager() { return _scene; }
	shared_ptr<InputManager> GetInputManager() { return _input; }
	shared_ptr<TimeManager> GetTimeManager() { return _time; }
	shared_ptr<ResourceManager> GetResourceManager() { return _resource; }
	shared_ptr<RenderManager> GetRenderManager() { return _render; }
	shared_ptr<class Pipeline> GetPipeline();

private:
	HWND _hwnd = nullptr;

private:
	// DX
	shared_ptr<class Graphics> _graphics;
	shared_ptr<class Pipeline> _pipeline;

private:
	shared_ptr<SceneManager> _scene;
	shared_ptr<InputManager> _input;
	shared_ptr<TimeManager> _time;
	shared_ptr<ResourceManager> _resource;
	shared_ptr<RenderManager> _render;
};

extern unique_ptr<Game> GGame;