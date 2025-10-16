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
	void DrawSub();  // ���̃I�u�W�F�N�g��`�悷�鏈��
	void UpdateSub();  // ���̃I�u�W�F�N�g���X�V���鏈��
	void ReleaseSub(); // ���̃I�u�W�F�N�g����������
	bool ToDestroy() const { return status.toDestroy_; };  // ���̃I�u�W�F�N�g�͔j�󂳂��\�肩
	void DestroyMe() { status.toDestroy_ = TRUE; }

	Transform* GetTransform() { return &transform_; };

private:
	struct Status
	{
		uint8_t toDestroy_ = 0;
	} status;

protected:
	std::list<GameObject*> childList_{};  // �q�̃��X�g
	Transform transform_;  // ���W�n
	GameObject* pParent_;  // �e�̃Q�[���I�u�W�F�N�g�|�C���^
	std::string name_;  // �I�u�W�F�N�g�̖��O
};

template<GameObjectT T, typename ...Arg>
inline T* GameObject::Instantiate(GameObject* _pParent, Arg... _arg)
{
	T* pGameObject{ new T{ _pParent, _arg... } };
	_pParent->childList_.push_back(pGameObject);
	pGameObject->Initialize();

	return pGameObject;
}
