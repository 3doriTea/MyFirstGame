#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	// 初期化（プロジェクション行列作成）
	void Initialize();

	// 更新（ビュー行列作成）
	void Update();

	// 視点（カメラの位置）を設定
	void SetPosition(const XMFLOAT3& position);

	// 焦点（見る位置）を設定
	void SetTarget(const XMFLOAT3& target);

	// カメラの座標を取得する
	XMFLOAT3 GetPosition();

	// カメラの焦点の座標を取得する
	XMFLOAT3 GetTarget();

	// ビュー行列を取得
	XMMATRIX GetViewMatrix();

	// プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();
};
