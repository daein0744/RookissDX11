#pragma once

class SceneManager;

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

private:
	HWND _hwnd = nullptr;

private:
	// DX
	shared_ptr<class Graphics> _graphics;
	shared_ptr<class Pipeline> _pipeline;

private:
	shared_ptr<SceneManager> _scene;
};

extern unique_ptr<Game> GGame;