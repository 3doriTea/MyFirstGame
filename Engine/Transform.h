#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//�ʒu�A�����A�g�嗦�Ȃǂ��Ǘ�����N���X
class Transform
{
public:
	XMFLOAT3 position_;	//�ʒu
	XMFLOAT3 rotate_;	//����
	XMFLOAT3 scale_;	//�g�嗦

	//�R���X�g���N�^
	Transform();

	//�f�X�g���N�^
	~Transform();

	//�e�s��̌v�Z
	void Calculation();

	//���[���h�s����擾
	XMMATRIX GetWorldMatrix() const;

	/// <summary>
	/// �@���̃��[���h�s����擾����
	/// </summary>
	/// <returns>�@���̍s��</returns>
	XMMATRIX GetNormalMatrix() const;

	void SetParent(Transform* _pParent) { pParent_ = _pParent; }

private:
	XMMATRIX matTranslate_;  // �ړ��s��
	XMMATRIX matRotate_;     // ��]�s��
	XMMATRIX matScale_;      // �g��s��

	Transform* pParent_;  // �e�̍��W�n
};
