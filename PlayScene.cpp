#include "PlayScene.h"
#include "Player.h"
#include "ChildOden.h"

PlayScene::PlayScene(GameObject* pParent)
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
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
