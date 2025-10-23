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
	/// ���f�������[�h����
	/// </summary>
	/// <param name="_fileName">�t�@�C����</param>
	int Load(const std::string& _fileName);

	/// <summary>
	/// ���W�n���Z�b�g����
	/// </summary>
	/// <param name="_hModel"></param>
	/// <param name="_transform"></param>
	void SetTransform(const int _hModel, const Transform& _transform);
	
	/// <summary>
	/// ���f����`�悷��
	/// </summary>
	/// <param name="_hModel"></param>
	void Draw(const int _hModel);
	/// <summary>
	/// �������
	/// </summary>
	void Release();
}
