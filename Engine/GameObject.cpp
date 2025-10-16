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

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->ToDestroy())
		{
			(*itr)->ReleaseSub();
			delete (*itr);
			itr = childList_.erase(itr);
		}
		else
		{
			itr++;
		}
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
