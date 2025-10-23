#pragma once
#include "GameObject.h"

class GameObject;

class SceneManager : public GameObject
{
public:
	enum struct Scene : int
	{
		Test,
		Title,
		PLay,
		Max,
	};

public:
	SceneManager(GameObject* _pParent);
	~SceneManager();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() override;
	/// <summary>
	/// �������
	/// </summary>
	void Release() override;

	/// <summary>
	/// �V�[����J�ڂ���
	/// </summary>
	/// <param name="_next">���̃V�[��</param>
	void Move(const Scene _next);
private:
	Scene current_;  // ���݂̃V�[��
	Scene next_;     // ���̃V�[��
};
