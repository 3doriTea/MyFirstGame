#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"

TestScene::TestScene(GameObject* _pParent) :
	GameObject{ _pParent, "TestScene" }
{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
}

void TestScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
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
