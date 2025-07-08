#pragma once
#include <Windows.h>
#include <d3d11.h>


// �����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#define SAFE_DELETE(p) \
if (p != nullptr)\
{\
	delete p;\
	p = nullptr;\
}

#define SAFE_RELEASE(p) \
if (p != nullptr)\
{\
	p->Release();\
	p = nullptr;\
}


//class Quad;
//class Texture;

class Direct3D final
{
	friend class Quad;
	friend class Texture;
public:
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="winW">�E�B���h�E����</param>
	/// <param name="winH">�E�B���h�E����</param>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	HRESULT Initialize(const int winW, const int winH, HWND hWnd);

	HRESULT InitializeShader();

	/// <summary>
	/// �`��J�n
	/// </summary>
	void BeginDraw();

	/// <summary>
	/// �`��I��
	/// </summary>
	void EndDraw();

	/// <summary>
	/// �������
	/// </summary>
	static void Release();

	static Direct3D& Instance();

private:
	Direct3D();
	~Direct3D();

	static Direct3D* pInstance_;

private:
	// �|�C���^�����̂�nullptr�����Ȃ����R
	// Microsoft�̂�ŏ��������邩��ʂɂ����� �炵��?
	ID3D11Device* pDevice{ nullptr };                      // �f�o�C�X
	ID3D11DeviceContext* pContext{ nullptr };              // �f�o�C�X�R���e�L�X�g
	IDXGISwapChain* pSwapChain{ nullptr };                 // �X���b�v�`�F�C��
	ID3D11RenderTargetView* pRenderTargetView{ nullptr };  // �����_�[�^�[�Q�b�g�r���[
	ID3D11VertexShader* pVertexShader{ nullptr };          // ���_�V�F�[�_�[
	ID3D11PixelShader* pPixelShader{ nullptr };            // �s�N�Z���V�F�[�_�[
	ID3D11InputLayout* pVertexLayout{ nullptr };           // ���_�C���v�b�g���C�A�E�g
	ID3D11RasterizerState* pRasterizerState{ nullptr };    // ���X�^���C�U�[
};
