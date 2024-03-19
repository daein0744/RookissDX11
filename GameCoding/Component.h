#pragma once

class GameObject;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	virtual void Init() abstract;
	virtual void Update() abstract;

	shared_ptr<GameObject> GetGameObject() { return _owner.lock(); }

private:
	weak_ptr<GameObject> _owner;
};

