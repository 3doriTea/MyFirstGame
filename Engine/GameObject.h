#pragma once

#include <list>
#include <string>
#include "Transform.h"
#include "Input.h"

class GameObject;

template<typename T>
concept GameObjectT = std::is_base_of_v<GameObject, T>;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* _pParent, const std::string& _name);
	virtual ~GameObject();

	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Release() {}
	virtual void Draw() {}

	template<GameObjectT T>
	GameObject* Instantiate(GameObject* _pParent);

public:
	void DrawSub();  // このオブジェクトを描画する処理
	void UpdateSub();  // このオブジェクトを更新する処理

protected:
	std::list<GameObject*> childList_{};  // 子のリスト
	Transform transform_;  // 座標系
	GameObject* pParent_;  // 親のゲームオブジェクトポインタ
	std::string name_;  // オブジェクトの名前
};

template<GameObjectT T>
inline GameObject* GameObject::Instantiate(GameObject* _pParent)
{
	GameObject* pGameObject{ new T{ _pParent } };
	_pParent->childList_.push_back(pGameObject);
	pGameObject->Initialize();

	return pGameObject;
}
