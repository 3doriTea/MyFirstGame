#pragma once
#include <string>
#include <vector>
#include "Fbx.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pFbx;
		Transform transform;
		std::string fileName;
	};

	/// <summary>
	/// モデルをロードする
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	int Load(const std::string& _fileName);

	/// <summary>
	/// 座標系をセットする
	/// </summary>
	/// <param name="_hModel"></param>
	/// <param name="_transform"></param>
	void SetTransform(const int _hModel, const Transform& _transform);
	
	/// <summary>
	/// モデルを描画する
	/// </summary>
	/// <param name="_hModel"></param>
	void Draw(const int _hModel);
	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();
}
