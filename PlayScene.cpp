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

PlayScene::PlayScene(GameObject* pParent) :
	GameObject{ pParent, "PlayScene" }
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
	
	Enemy* pEnemy = Instantiate<Enemy>(this, pPlayer);

	CameraController* pCameraController{ Instantiate<CameraController>(this) };
	Ground* pGround{ Instantiate<Ground>(this) };

	Cursor::SetLock(true);
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

	if (Input::IsKeyDown(DIK_F))
	{
		GetSceneManager()->Move(SceneManager::Scene::Test);
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
