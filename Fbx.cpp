#include "Fbx.h"
#include "Direct3D.h"
#include "Camera.h"
#include <cstdint>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

Fbx::Fbx() :
	vertexCount_{ 0 },
	polygonCount_{ 0 },
	pVertexBuffer_{ nullptr },
	pIndexBuffer_{ nullptr },
	pConstantBuffer_{ nullptr }
{
}

Fbx::~Fbx()
{
	Release();
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

	// �m�[�h��fbx�ł̃��b�V���������Ă�����

	// ���b�V�������擾
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	// �e���̌����擾

	vertexCount_ = mesh->GetControlPointsCount();  // ���_�̐�
	polygonCount_ = mesh->GetPolygonCount();       // �|���S���̐�
	materialCount_ = pNode->GetMaterialCount();    // �}�e���A����

	wchar_t currDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, currDir);
	SetCurrentDirectory(L"./Assets");

	InitVertex(mesh);
	InitIndex(mesh);
	InitConstant();
	InitMaterial(pNode);

	//�}�l�[�W�����
	pFbxManager->Destroy();

	SetCurrentDirectory(currDir);
	
	return S_OK;
}

void Fbx::Draw(Transform& transform)
{
#if 1
	//Quad���A�����W
	Direct3D::SetShader(Direct3D::SHADER_FBX);
	transform.Calculation();

	CONSTANT_BUFFER cb{};
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = XMMatrixIdentity();

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::Instance().pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	Direct3D::Instance().pContext_->Unmap(pConstantBuffer_, 0);	//�ĊJ

	//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�R���X�^���g�o�b�t�@���p�C�v���C���ɃZ�b�g
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::Instance().pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	for (int i = 0; i < materialCount_; i++)
	{

		// �C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		Direct3D::Instance().pContext_->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//�R���X�^���g�o�b�t�@
		Direct3D::Instance().pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
		Direct3D::Instance().pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p


		if (materials_[i].pTexture)
		{
			ID3D11SamplerState* pSampler = materials_[i].pTexture->GetSampler();
			Direct3D::Instance().pContext_->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = materials_[i].pTexture->GetSRV();
			Direct3D::Instance().pContext_->PSSetShaderResources(0, 1, &pSRV);
		}

		//�`��
		Direct3D::Instance().pContext_->DrawIndexed(polygonCount_ * 3, 0, 0);
	}
#else
	//Direct3D::SetShader(Direct3D::SHADER_FBX);
	Direct3D::SetShader(Direct3D::SHADER_FBX);

#pragma region �R���X�^���g�o�b�t�@�ɓn�����
	CONSTANT_BUFFER cb{};
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matRotateW = XMMatrixIdentity();
#pragma endregion

#pragma region �R���X�^���g�o�b�t�@�̑��M
	D3D11_MAPPED_SUBRESOURCE pdata{};
	Direct3D::Instance().pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	Direct3D::Instance().pContext_->Unmap(pConstantBuffer_, 0);	//�ĊJ
#pragma endregion

#pragma region ���X�Z�b�g
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::Instance().pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	for (int i = 0; i < materialCount_; i++)
	{
		stride = sizeof(int);
		offset = 0;
		Direct3D::Instance().pContext_->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		// �R���X�^���g�o�b�t�@
		Direct3D::Instance().pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
		Direct3D::Instance().pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

		if (materials_[i].pTexture)
		{
			ID3D11SamplerState* pSampler = materials_[i].pTexture->GetSampler();
			Direct3D::Instance().pContext_->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = materials_[i].pTexture->GetSRV();
			Direct3D::Instance().pContext_->PSSetShaderResources(0, 1, &pSRV);
		}


#pragma endregion

		// ���ۂɕ`��
		Direct3D::Instance().pContext_->DrawIndexed(polygonCount_ * 3, 0, 0);
	}
#endif
}

void Fbx::Release()
{
	for (auto& material : materials_)
	{
		SAFE_DELETE(material.pTexture);
	}
}

void Fbx::InitVertex(FbxMesh* mesh)
{
	VERTEX* vertices{ new VERTEX[vertexCount_]{} };

	//���_��UV
	FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();

	FbxLayerElement::EMappingMode mappingMode{ pUV->GetMappingMode() };
	FbxLayerElement::EReferenceMode referenceMode{ pUV->GetReferenceMode() };

	for (int64_t poly = 0; poly < polygonCount_; poly++)
	{
		for (int vertex = 0; vertex < 3; vertex++)
		{
			enum { X, Y, Z };
			enum { U, V };
			
			int index{ mesh->GetPolygonVertex(poly, vertex) };
			
			FbxVector4 pos{ mesh->GetControlPointAt(index) };
			vertices[index].position =
				XMVectorSet(static_cast<float>(pos[X]), static_cast<float>(pos[Y]), static_cast<float>(pos[Z]), 0.0f);

			int uvIndex{};
			FbxVector2 uv = pUV->GetDirectArray().GetAt(uvIndex);

			// MappingMode�ŕ���
			if (mappingMode == FbxLayerElement::EMappingMode::eByPolygonVertex)
			{
				uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			}
			else if (mappingMode == FbxLayerElement::eByControlPoint)
			{
				uvIndex = index;
			}
			else
			{
				uv = { 0, 0 };
			}

			// ReferenceMode�ŕ���
			if (referenceMode == FbxLayerElement::eDirect)
			{
				uv = pUV->GetDirectArray().GetAt(uvIndex);
			}
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
			{
				int realIndex{ pUV->GetIndexArray().GetAt(uvIndex) };
				uv = pUV->GetDirectArray().GetAt(realIndex);
			}
			
			// UV�̏c�����̊���t�ɂȂ邩��UV���t
			vertices[index].uv = XMVectorSet((float)uv.mData[U], (float)(1.0f - uv.mData[V]), 0.0f, 0.0f);
		}
	}

#pragma region ���_�o�b�t�@���쐬

	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_vertex{};
	//bd_vertex.ByteWidth = sizeof(vertices);  // �^�̑傫��
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;  // �^�̑傫��
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex{};
	data_vertex.pSysMem = vertices;
	HRESULT hResult = Direct3D::Instance().pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	//assert(SUCCEEDED(hResult) && "���_�o�b�t�@�̍쐬�Ɏ��s @Quad::Initialize");

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		//return hResult;
	}
#pragma endregion
}

void Fbx::InitIndex(FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer*[materialCount_];  // �}�e���A���̕��������

	int* index{ new int[polygonCount_ * 3] };

	for (int i = 0; i < materialCount_; i++)
	{
		int count{ 0 };

		for (int poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl{ mesh->GetLayer(0)->GetMaterials() };
			int mtlId{ mtl->GetIndexArray().GetAt(poly) };

			if (mtlId == i)
			{
				for (int vertex = 0; vertex < 3; vertex++)
				{
					index[count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
		}

#pragma region �C���f�b�N�X�o�b�t�@���쐬
		// �C���f�b�N�X�o�b�t�@�𐶐�����
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = index;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		HRESULT hResult = Direct3D::Instance().pDevice_->CreateBuffer(&bd, &initData, &pIndexBuffer_[i]);

		//assert(SUCCEEDED(hResult) && "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s @Quad::Initialize");

		if (FAILED(hResult))
		{
			MessageBox(nullptr, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
			//return hResult;
		}
#pragma endregion
	}
}

void Fbx::InitConstant()
{
#pragma region �R���X�^���g�o�b�t�@�̍쐬
	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb{};
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hResult = Direct3D::Instance().pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);

	//assert(SUCCEEDED(hResult) && "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s @Quad::Initialize");

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
	}
#pragma endregion
}

void Fbx::InitMaterial(FbxNode* _pNode)
{
	materials_.resize(materialCount_);

	for (int i = 0; i < materialCount_; i++)
	{
		FbxSurfaceMaterial* pMaterial{ _pNode->GetMaterial(i) };

		FbxProperty fbxProperty{ pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse) };
		int fileTextureCount{ fbxProperty.GetSrcObjectCount<FbxFileTexture>() };

		if (fileTextureCount > 0)
		{
			FbxFileTexture* pTextureInfo{ fbxProperty.GetSrcObject<FbxFileTexture>(0) };
			const char* textureFileName{ pTextureInfo->GetRelativeFileName() };

			fs::path tPath{ textureFileName };

			// fs::exists ��� fs::is_regular_file �̂ق�������
			if (fs::is_regular_file(tPath))
			//if (fs::exists(tPath))
			{
				// �e�N�X�`���̓ǂݍ���
				materials_[i].pTexture = new Texture{};
				HRESULT hResult = materials_[i].pTexture->Load(textureFileName);

				assert(SUCCEEDED(hResult) && "�ǂݍ��ݎ��s");
			}
			else
			{
				// �e�N�X�`���̓ǂݍ��݂ł��Ȃ�
				assert(false && "�t�@�C�����Ȃ�");
			}
		}
		else
		{
			materials_[i].pTexture = nullptr;
		}
	}
}
