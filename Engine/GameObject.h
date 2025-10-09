#pragma once

#include <list>
#include <string>
#include "Transform.h"
#include "Input.h"

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

public:
	void DrawSub();  // このオブジェクトを描画する処理

protected:
	std::list<GameObject*> childList_;  // 子のリスト
	Transform transform_;  // 座標系
	GameObject* pParent_;  // 親のゲームオブジェクトポインタ
	std::string name_;  // オブジェクトの名前
};
