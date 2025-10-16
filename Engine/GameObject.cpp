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

void GameObject::DrawSub()
{
	this->Draw();

	for (auto pChild : childList_)
	{
		pChild->DrawSub();
	}
}

void GameObject::UpdateSub()
{
	this->Update();
	for (auto pChild : childList_)
	{
		pChild->UpdateSub();
	}
}

void GameObject::ReleaseSub()
{
	for (auto pChild : childList_)
	{
		pChild->ReleaseSub();
	}
	this->Release();
}
