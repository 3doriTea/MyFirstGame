#include "GameOverScene.h"
#include "Engine/Camera.h"
#include "Engine/Cursor.h"
#include "Engine/SceneManager.h"

#include "Snake.h"
#include "SceneCube.h"

GameOverScene::GameOverScene(GameObject* _pParent) :
	GameObject{ _pParent, "GameOverScene" }
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	Cursor::SetLock(false);

	Instantiate<Snake>(this);

	Camera::SetPosition({ 0, 0, 0 });
	Camera::SetTarget({ 10, 0, 0 });

	Instantiate<SceneCube>(this, XMFLOAT3{ 0, 0, 0 });
}

void GameOverScene::Update()
{
	if (Input::IsKeyDown(DIK_F))
	{
		GetSceneManager()->Move(SceneManager::Scene::Test);
	}
}

void GameOverScene::Draw()
{
}

void GameOverScene::Release()
{
}
