#include "Model.h"
#include <set>

#include "Direct3D.h"

namespace
{
	std::vector<Model::ModelData> models_;
}

int Model::Load(const std::string& _fileName)
{
	// ハンドルは固有で、transformも固有だが、fbxだけ共有する
	const int HANDLE{ static_cast<int>(models_.size()) };

	models_.push_back(
		ModelData
		{
			.pFbx = nullptr,
			.transform = {},
			.fileName = _fileName,
		});

	// ひたすら同じファイル名がないかチェックする
	for (auto itr = models_.begin(); itr != (--models_.end()); itr++)
	{
		if (itr->fileName == _fileName)
		{
			models_[HANDLE].pFbx = itr->pFbx;
			return HANDLE;
		}
	}

	models_[HANDLE].pFbx = new Fbx{};
	models_[HANDLE].pFbx->Load(_fileName);

	return HANDLE;
}

void Model::SetTransform(const int _hModel, const Transform _transform)
{
	models_[_hModel].transform = _transform;
}

void Model::Draw(const int _hModel)
{
	models_[_hModel].pFbx->Draw(
		models_[_hModel].transform);
}

void Model::Release()
{
	/*bool isReffered = false;
	for (int i = 0; i < models_.size(); i++)
	{
		for (int j = i + 1; j < models_.size(); j++)
		{
			if (models_[i].pFbx == models_[j].pFbx)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(models_[i].pFbx);
		}
	}

	return;*/

	std::set<Fbx*> uniqueFbxPtrs{};

	for (auto& model : models_)
	{
		uniqueFbxPtrs.insert(model.pFbx);
	}

	for (auto& pFbx : uniqueFbxPtrs)
	{
		pFbx->Release();
		delete pFbx;
	}
}
