#include "PlayScene.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"
#include "ChildOden.h"
#include "Engine/SceneManager.h"
#include "Enemy.h"
#include "CameraController.h"
#include "Ground.h"
#include "Engine/Cursor.h"
#include "SceneCube.h"
#include "GameOverScene.h"

namespace
{
	const float DELTA_TIME{ 1.0f / 60.0f };
	const int ENEMY_COUNT{ 10 };
	const int RAND_SEED[] { 99581, 99607, 99611, 99623, 99643 };
	const int RAND_SEED_COUNT{ sizeof(RAND_SEED) / sizeof(int) };
	const float WORLD_RADIUS{ 300.0f };
}

PlayScene::PlayScene(GameObject* pParent) :
	GameObject{ pParent, "PlayScene" },
	enemyCount_{ 0 },
	toOver_{ false },
	timeLeft_{}
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	Camera::SetPosition({ 0, 30.0f, -30.0f });
	Camera::SetTarget({ 0, 0.0f, 50.0f });

	/*
	Player* pPlayer_{ new Player{ this } };
	childList_.push_back(pPlayer_);
	pPlayer_->Initialize();]
	*/

	Player* pPlayer = Instantiate<Player>(this);
	//Instantiate<ChildOden>(pPlayer, DirectX::XMFLOAT3{ 0, 3, 3.5 });
	//Instantiate<ChildOden>(pPlayer, DirectX::XMFLOAT3{ 0, 3, -3.5 });

	
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float dist{ WORLD_RADIUS * static_cast<float>(i) / ENEMY_COUNT };
		float rand{ static_cast<float>(i * RAND_SEED[i % RAND_SEED_COUNT]) };

		XMVECTOR v{ std::cosf(rand), 0.0f, std::sinf(rand) };
		v *= dist;

		XMFLOAT3 position{};
		XMStoreFloat3(&position, v);
		Enemy* pEnemy = Instantiate<Enemy>(this, pPlayer, position);

		enemyCount_++;
	}


	CameraController* pCameraController{ Instantiate<CameraController>(this) };
	Ground* pGround{ Instantiate<Ground>(this) };

	Cursor::SetLock(true);

	Instantiate<SceneCube>(this, XMFLOAT3{ 0, 0, 0 });
}

void PlayScene::Update()
{
	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		Cursor::SetLock(false);
	}
	if (Input::IsMouseButton(0))
	{
		Cursor::SetLock(true);
	}

	if (toOver_)
	{
		timeLeft_ -= DELTA_TIME;
		if (timeLeft_ <= 0.0f)
		{
			GetSceneManager()->Move(SceneManager::Scene::Over);
		}
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}

void PlayScene::OnBoomEnemy()
{
	enemyCount_--;
	if (enemyCount_ <= 0)
	{
		SceneCube* pSceneCube{ Instantiate<SceneCube>(this, XMFLOAT3{ 0, 0, 0 }, true) };
		pSceneCube->Fire();
		timeLeft_ = SceneCube::GetDestroyTime();
		toOver_ = true;
	}
}
