#pragma once

#include <list>
#include <string>
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* _pParent, const std::string& _name);
	virtual ~GameObject();

	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Release() {}
	virtual void Draw() const {}

private:
	std::list<GameObject*> childList_;  // �q�̃��X�g
	Transform transform_;  // ���W�n
	GameObject* pParent_;  // �e�̃Q�[���I�u�W�F�N�g�|�C���^
	std::string name_;  // �I�u�W�F�N�g�̖��O
};
