#include "SceneManager.h"
#include "../PlayScene.h"
#include "../TestScene.h"

SceneManager::SceneManager(GameObject* _pParent) :
	GameObject{ _pParent, "SceneManager" }
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	current_ = Scene::Test;
	next_ = current_;
	Instantiate<TestScene>(this);
}

void SceneManager::Update()
{
	if (current_ != next_)
	{
 		childList_.front()->ReleaseSub();
		childList_.clear();
		switch (next_)
		{
		case SceneManager::Scene::Test:
			Instantiate<TestScene>(this);
			break;
		case SceneManager::Scene::Title:
			// Instantiate<Title>();
			break;
		case SceneManager::Scene::PLay:
			Instantiate<PlayScene>(this);
			break;
		case SceneManager::Scene::Max:
		default:
			break;
		}
		current_ = next_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::Move(const Scene _next)
{
	next_ = _next;
}
