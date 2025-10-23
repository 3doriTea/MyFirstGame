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
	/// 初期化処理
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;
	/// <summary>
	/// 解放処理
	/// </summary>
	void Release() override;

	/// <summary>
	/// シーンを遷移する
	/// </summary>
	/// <param name="_next">次のシーン</param>
	void Move(const Scene _next);
private:
	Scene current_;  // 現在のシーン
	Scene next_;     // 次のシーン
};
