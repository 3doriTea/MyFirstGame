#include "GameObject.h"

GameObject::GameObject() :
	GameObject{ nullptr, "GameObject" }
{
}

GameObject::GameObject(GameObject* _pParent, const std::string& _name) :
	pParent_{ _pParent },
	name_{ _name }
{
}

GameObject::~GameObject()
{
}
