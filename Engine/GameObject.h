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
	void DrawSub();  // ���̃I�u�W�F�N�g��`�悷�鏈��
	void UpdateSub();  // ���̃I�u�W�F�N�g���X�V���鏈��

protected:
	std::list<GameObject*> childList_{};  // �q�̃��X�g
	Transform transform_;  // ���W�n
	GameObject* pParent_;  // �e�̃Q�[���I�u�W�F�N�g�|�C���^
	std::string name_;  // �I�u�W�F�N�g�̖��O
};

template<GameObjectT T>
inline GameObject* GameObject::Instantiate(GameObject* _pParent)
{
	GameObject* pGameObject{ new T{ _pParent } };
	_pParent->childList_.push_back(pGameObject);
	pGameObject->Initialize();

	return pGameObject;
}
