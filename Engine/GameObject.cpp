#include "GameObject.h"
#include "RootJob.h"
#include "SceneManager.h"


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

RootJob* GameObject::GetRootJob()
{
	GameObject* pGameObject{ this };
	while (pGameObject->GetParent() != nullptr)
	{
		pGameObject = pGameObject->GetParent();
	}
	return dynamic_cast<RootJob*>(pGameObject);
}

GameObject* GameObject::FindGameObject(const std::string& _name)
{
	return GetRootJob()->FindChild([&_name](GameObject* _pGameObject){ return _pGameObject->name_ == _name; });
}

GameObject* GameObject::FindChild(const std::function<bool(GameObject*)>& _checkCallback)
{
	if (_checkCallback(this))
	{
		return this;
	}
	else
	{

	}

	for (auto child : *this)
	{
		if (_checkCallback(child))
		{
			return child;
		}
	}

	for (auto child : *this)
	{
		GameObject* foundGameObject = child->FindChild(_checkCallback);
		if (foundGameObject)
		{
			return foundGameObject;
		}
	}

	return nullptr;
}

SceneManager* GameObject::GetSceneManager()
{
	return FindGameObject<SceneManager>();
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
	transform_.Calculation();
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
	childList_.clear();
	this->Release();
}
