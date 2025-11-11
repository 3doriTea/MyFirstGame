#include "Camera.h"

namespace
{
	XMFLOAT3 upperDirection_{ 0, 1, 0 };  // カメラ上方向
	//変数
	XMFLOAT3 position_;	//カメラの位置（視点）
	XMFLOAT3 target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
}

//初期化
void Camera::Initialize()
{
	position_ = { 0, 3, -10 };	//カメラの位置
	target_ = { 0, 0, 0 };	//カメラの焦点

	// プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
}

//更新
void Camera::Update()
{
	// ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(
		XMLoadFloat3(&position_),
		XMLoadFloat3(&target_),
		XMLoadFloat3(&upperDirection_));
}

//位置を設定
void Camera::SetPosition(const XMFLOAT3& position)
{
	position_ = position;
}

//焦点を設定
void Camera::SetTarget(const XMFLOAT3& target)
{
	target_ = target;
}

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}
