#include "RootJob.h"
#include "SceneManager.h"

RootJob::RootJob(GameObject* _pParent) :
	GameObject{ _pParent, "RootJob" }
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{
	Instantiate<SceneManager>(this);
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
