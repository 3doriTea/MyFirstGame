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
	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得

	vertexCount_ = mesh->GetControlPointsCount();	//頂点の数

	polygonCount_ = mesh->GetPolygonCount();	//ポリゴンの数


	//マネージャ解放
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
