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

	template<GameObjectT T, typename ...Arg>
	T* Instantiate(GameObject* _pParent, Arg... _arg);

public:
	void DrawSub();  // このオブジェクトを描画する処理
	void UpdateSub();  // このオブジェクトを更新する処理
	void ReleaseSub(); // このオブジェクトを消す処理
	bool ToDestroy() const { return status.toDestroy_; };  // このオブジェクトは破壊される予定か
	void DestroyMe() { status.toDestroy_ = TRUE; }

	Transform* GetTransform() { return &transform_; };

private:
	struct Status
	{
		uint8_t toDestroy_ = 0;
	} status;

protected:
	std::list<GameObject*> childList_{};  // 子のリスト
	Transform transform_;  // 座標系
	GameObject* pParent_;  // 親のゲームオブジェクトポインタ
	std::string name_;  // オブジェクトの名前
};

template<GameObjectT T, typename ...Arg>
inline T* GameObject::Instantiate(GameObject* _pParent, Arg... _arg)
{
	T* pGameObject{ new T{ _pParent, _arg... } };
	_pParent->childList_.push_back(pGameObject);
	pGameObject->Initialize();

	return pGameObject;
}
