#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Utility.h"


class Texture;

class Sprite2D
{
private:
	struct CONSTANT_BUFFER
	{
		DirectX::XMMATRIX matWVP;
		DirectX::XMMATRIX matUV;
	};

	struct Vertex
	{
		DirectX::XMVECTOR pos;  // �ʒu
		DirectX::XMVECTOR uv;  // UV���W
	};

public:
	Sprite2D();
	virtual ~Sprite2D();

	virtual HRESULT Initialize(const char* _fileName);
	virtual void Draw(const RectanInt& _dest);
	virtual void Draw(const RectanInt& _dest, const float _angle);
	virtual void Draw(const RectanInt& _dest, const float _angle, const RectanInt& _src);
	void Release();
private:
	Texture* pTexture_;

	ID3D11Buffer* pVertexBuffer_;    // ���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;     // �C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;  // �R���X�^���g�o�b�t�@
};
