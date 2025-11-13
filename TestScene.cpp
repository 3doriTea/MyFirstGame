#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"

#include "Snake.h"

TestScene::TestScene(GameObject* _pParent) :
	GameObject{ _pParent, "TestScene" }
{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	Instantiate<Snake>(this);
}

void TestScene::Update()
{
	if (Input::IsKeyDown(DIK_F))
	{
		GetSceneManager()->Move(SceneManager::Scene::PLay);
	}
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
