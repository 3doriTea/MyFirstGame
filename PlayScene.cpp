#include "PlayScene.h"
#include "Player.h"

PlayScene::PlayScene(GameObject* pParent)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	Player* pPlayer_{ new Player{ this } };
	childList_.push_back(pPlayer_);
	pPlayer_->Initialize();
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
