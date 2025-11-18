#include "SceneCube.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"

SceneCube::SceneCube(GameObject* _pParent, const XMFLOAT3& _beginRotation) :
	GameObject{ _pParent, "SceneCube" },
	hModel_{ -1 }
{
	transform_.rotate_ = _beginRotation;
	transform_.scale_ = { 1.0f, 1.0f, 1.0f };
}

void SceneCube::Initialize()
{
	hModel_ = Model::Load("SceneCube/SceneCube.fbx");
}

void SceneCube::Update()
{
	transform_.position_ = Camera::GetPosition();
	XMVECTOR dirV{ XMLoadFloat3(&Camera::GetTarget()) - XMLoadFloat3(&Camera::GetPosition());
	dirV = XMVector3Normalize(dirV);
	XMVECTOR posV{ dirV + XMLoadFloat3(&Camera::GetPosition()) };
	//transform_.position_
	//XMVector
	//transform_.rotate_( dirV * 10.0;
}

void SceneCube::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Direct3D::SetWriteDepthBuffer(false);
	Direct3D::UseOnceShader(Direct3D::SHADER_BULLET);
	Model::Draw(hModel_);
	Direct3D::SetWriteDepthBuffer(true);
}
