#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

class Texture;  // �O���錾

class Quad
{
private:
	// �R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		DirectX::XMMATRIX matWVP;
		DirectX::XMMATRIX textureMatrix;
	};
	struct Vertex
	{
		DirectX::XMFLOAT4 pos;  // �ʒu
		DirectX::XMVECTOR uv;  // UV���W
	};

public:
	Quad();
	virtual ~Quad();
	virtual HRESULT Initialize();
	virtual HRESULT Initialize(const char* _fileName);
	virtual void Draw(DirectX::XMMATRIX& _worldMatrix);
	virtual void Draw(const DirectX::XMMATRIX& _worldMatrix, const DirectX::XMMATRIX& _uvMatrix);
	void Release();

private:
	Texture* pTexture_;

	ID3D11Buffer* pVertexBuffer_;    // ���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;     // �C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;  // �R���X�^���g�o�b�t�@
};
