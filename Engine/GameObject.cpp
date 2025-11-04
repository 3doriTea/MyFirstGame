#include "GameObject.h"
#include "RootJob.h"
#include "SceneManager.h"
#include "SphereCollider.h"


GameObject::GameObject() :
	GameObject{ nullptr, "GameObject" }
{
}

GameObject::GameObject(GameObject* _pParent, const std::string& _name) :
	pParent_{ _pParent },
	name_{ _name },
	pCollider_{ nullptr }
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

	RoundRobin(GetRootJob());

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

void GameObject::AddCollider(SphereCollider* _pCollider)
{
	pCollider_ = _pCollider;
}

void GameObject::Collision(GameObject* _pTarget)
{
	if (!_pTarget)
	{
		return;  // ターゲットがいないならダメ
	}
	if (!pCollider_ || !_pTarget->pCollider_)
	{
		return;  // 両方ともにコライダーがなければダメ
	}

	// 閾値
	float threshold
	{
		(pCollider_->GetRadius() + _pTarget->pCollider_->GetRadius())
		* (pCollider_->GetRadius() + _pTarget->pCollider_->GetRadius())
	};

	XMVECTOR diff  // 距離
	{
		(XMLoadFloat3(&this->transform_.position_) + pCollider_->GetCenter())
		- (XMLoadFloat3(&_pTarget->transform_.position_) + _pTarget->pCollider_->GetCenter())
	};

	float distance{ XMVector3LengthSq(diff).m128_f32[0] };
	/*for (int i = 0; i < 3; i++)
	{
		distance += diff.m128_f32[i] * diff.m128_f32[i];
	}*/

	if (distance <= threshold)
	{
		OnCollision();

		MessageBox(nullptr, L"あたったお", L"あたった", MB_OK);
	}
}

void GameObject::RoundRobin(GameObject* _pTarget)
{
	if (!pCollider_)
	{
		return;
	}
	if (_pTarget->pCollider_)
	{
		return;
	}

	_pTarget->Collision(this);

	for (auto& child : _pTarget->childList_)
	{
		RoundRobin(child);
	}
}
