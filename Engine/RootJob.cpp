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
	RoundRobin(this);  // TODO: “–‚½‚è”»’è‚ÌŒÄ‚Ñ•û‚ð‰ü—Ç‚·‚é
}

void RootJob::Draw()
{
}

void RootJob::Release()
{
}
