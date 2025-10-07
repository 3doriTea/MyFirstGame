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
		DirectX::XMMATRIX matWVP;  // �X�N���[����Ԃ̂��
		DirectX::XMMATRIX textureMatrix;  // �e�N�X�`���̃}�g���N�X
		DirectX::XMMATRIX matW;  // ���[���h�s��
		DirectX::XMMATRIX matRotateW;  // �@���p�̍s��
		DirectX::XMVECTOR lightDir;  // ���s�����̌����x�N�g��
		DirectX::XMVECTOR lightColor;  // ���s�����̐F
		float ambientValue;  // �����̖��邳
	};
	struct Vertex
	{
		DirectX::XMVECTOR pos;  // �ʒu
		DirectX::XMVECTOR uv;  // UV���W
		DirectX::XMVECTOR normal;  // �@��
	};

public:
	Quad();
	virtual ~Quad();
	virtual HRESULT Initialize();
	virtual HRESULT Initialize(const char* _fileName);
	virtual void Draw(const DirectX::XMMATRIX& _worldMatrix);
	virtual void Draw(const DirectX::XMMATRIX& _worldMatrix, const DirectX::XMMATRIX& _uvMatrix);
	void Release();

private:
	Texture* pTexture_;

	ID3D11Buffer* pVertexBuffer_;    // ���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;     // �C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;  // �R���X�^���g�o�b�t�@
};
