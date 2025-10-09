#include "RootJob.h"
#include "../PlayScene.h"

RootJob::RootJob(GameObject* _pParent) :
	GameObject{ _pParent, "RootJob" }
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{
	PlayScene* pPlayScene{ new PlayScene{ this } };
	pPlayScene->Initialize();
	childList_.push_back(pPlayScene);
}

void RootJob::Update()
{
}

void RootJob::Draw()
{
}

void RootJob::Release()
{
}
