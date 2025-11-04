#include "PlayScene.h"
#include "Engine/Input.h"
#include "Player.h"
#include "ChildOden.h"
#include "Engine/SceneManager.h"
#include "Enemy.h"

PlayScene::PlayScene(GameObject* pParent) :
	GameObject{ pParent, "PlayScene" }
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	/*
	Player* pPlayer_{ new Player{ this } };
	childList_.push_back(pPlayer_);
	pPlayer_->Initialize();]
	*/

	Player* pPlayer = Instantiate<Player>(this);
	Instantiate<ChildOden>(pPlayer, DirectX::XMFLOAT3{ 0, 3, 3.5 });
	Instantiate<ChildOden>(pPlayer, DirectX::XMFLOAT3{ 0, 3, -3.5 });
	
	Enemy* pEnemy = Instantiate<Enemy>(this);
}

void PlayScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
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
