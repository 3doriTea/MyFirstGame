#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

#include "Snake.h"
#include "SceneCube.h"

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

	Camera::SetPosition({ 0, 0, 0 });
	Camera::SetTarget({ 0, 0, -1.0f });

	Instantiate<SceneCube>(this, XMFLOAT3{ 0, 0, 0 });
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
