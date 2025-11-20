#pragma once
#include "Engine/GameObject.h"

class PlayScene : public GameObject
{
public:
	PlayScene(GameObject* pParent);
	~PlayScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;

	/// <summary>
	/// 敵が破壊されたときに呼び出す
	/// </summary>
	void OnBoomEnemy();

private:
	int enemyCount_;
	bool toOver_;  // オーバーシーンに移行中か
	float timeLeft_;  // オーバーシーンへ移行するカウントダウンタイマ
};
