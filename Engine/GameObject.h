#pragma once

#include <list>
#include <functional>
#include <string>
#include <cassert>
#include "Transform.h"
#include "Input.h"

class SceneManager;
class GameObject;
class RootJob;

template<typename T>
concept GameObjectT = std::is_base_of_v<GameObject, T>;

class GameObject
{
	using ChildList = std::list<GameObject*>;
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

	RootJob* GetRootJob();

	GameObject* FindGameObject(const std::string& _name);
	GameObject* FindChild(const std::function<bool(GameObject*)>& _checkCallback);

	SceneManager* GetSceneManager();
	
	template<GameObjectT T>
	T* FindGameObject();

	ChildList::iterator begin() { return childList_.begin(); }
	ChildList::iterator end() { return childList_.end(); }
	ChildList::const_iterator begin() const { return childList_.begin(); }
	ChildList::const_iterator end() const { return childList_.end(); }

public:
	void DrawSub();  // ���̃I�u�W�F�N�g��`�悷�鏈��
	void UpdateSub();  // ���̃I�u�W�F�N�g���X�V���鏈��
	void ReleaseSub(); // ���̃I�u�W�F�N�g����������
	bool ToDestroy() const { return status.toDestroy_; };  // ���̃I�u�W�F�N�g�͔j�󂳂��\�肩
	void DestroyMe() { status.toDestroy_ = TRUE; }

	GameObject* GetParent() { return pParent_; }

	Transform* GetTransform() { return &transform_; };

private:
	struct Status
	{
		uint8_t toDestroy_ = 0;
	} status;

protected:
	ChildList childList_{};  // �q�̃��X�g
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

template<GameObjectT T>
inline T* GameObject::FindGameObject()
{
	GameObject* pRootJob{ reinterpret_cast<GameObject*>(GetRootJob()) };

	assert(pRootJob && "pRootJob��������Ȃ�����");

	return dynamic_cast<T*>(
		pRootJob->FindChild([](GameObject* _pGameObject) { return dynamic_cast<T*>(_pGameObject); }));
}
