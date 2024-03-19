#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	HWND _hwnd = nullptr;

private:
	// DX
	shared_ptr<class Graphics> _graphics;
	shared_ptr<class Pipeline> _pipeline;
	shared_ptr<class GameObject> _monster;
	shared_ptr<class GameObject> _camera;
};