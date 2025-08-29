#include "Fbx.h"

Fbx::Fbx() :
	vertexCount_{ 0 },
	polygonCount_{ 0 },
	pVertexBuffer_{ nullptr },
	pIndexBuffer_{ nullptr },
	pConstantBuffer_{ nullptr }
{
}

HRESULT Fbx::Load(std::string fileName)
{
	//�}�l�[�W���𐶐�
	FbxManager* pFbxManager = FbxManager::Create();

	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//�V�[���I�u�W�F�N�g��FBX�t�@�C���̏��𗬂�����
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//���b�V�������擾
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//�e���̌����擾

	vertexCount_ = mesh->GetControlPointsCount();	//���_�̐�

	polygonCount_ = mesh->GetPolygonCount();	//�|���S���̐�


	//�}�l�[�W�����
	pFbxManager->Destroy();
	return S_OK;
}

void Fbx::Draw(Transform& transform)
{
}

void Fbx::Release()
{
}

void Fbx::InitVertex(FbxMesh* mesh)
{
	VERTEX* vertexes{ new VERTEX[vertexCount_]{} };

	for (int64_t poly = 0; poly < polygonCount_; poly++)
	{
		for (int vertex = 0; vertex < 3; vertex++)
		{
			int index{ mesh->GetPolygonVertex(poly, vertex) };

			FbxVector4 pos{ mesh->GetControlPointAt(index) };
			vertexes[index].position =
				XMVectorSet(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]), 0.0f);
		}
	}
}

void Fbx::InitIndex(FbxMesh* mesh)
{
	int64_t* index{ new int64_t[polygonCount_ * 3] };


}
