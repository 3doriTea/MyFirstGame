#include "Direct3D.h"
#include <cassert>
#include <d3d11shader.h>
#include <d3dcompiler.h>

#include <sstream>

#include "Quad.h"
#include "Sprite2D.h"
#include "Texture.h"
#include "Fbx.h"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")


void Direct3D::SetShader(ShaderType _shaderType)
{
	Direct3D& instance{ *pInstance_ };
	instance.pContext_->VSSetShader(instance.shaders_[_shaderType].pVertexShader, NULL, 0);  // ���_�V�F�[�_�[
	instance.pContext_->PSSetShader(instance.shaders_[_shaderType].pPixelShader, NULL, 0);   // �s�N�Z���V�F�[�_�[
	instance.pContext_->IASetInputLayout(instance.shaders_[_shaderType].pVertexLayout);      // ���_�C���v�b�g���C�A�E�g
	instance.pContext_->RSSetState(instance.shaders_[_shaderType].pRasterizerState);         // ���X�^���C�U�[
}

HRESULT Direct3D::Initialize(const int winW, const int winH, HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scDesc{};

	screenSize_ = { winW, winH };

	//�`���̃t�H�[�}�b�g
	scDesc.BufferDesc.Width = winW;		//��ʕ�
	scDesc.BufferDesc.Height = winH;	//��ʍ���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���F�g���邩

	//FPS�i1/60�b��1��j
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//���̑�
	scDesc.Windowed = TRUE;			//�E�B���h�E���[�h���t���X�N���[����
	scDesc.OutputWindow = hWnd;		//�E�B���h�E�n���h��
	scDesc.BufferCount = 1;			//�o�b�N�o�b�t�@�̖���
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
	scDesc.SampleDesc.Count = 1;		//MSAA�i�A���`�G�C���A�X�j�̐ݒ�
	scDesc.SampleDesc.Quality = 0;		//�@�V

	////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
	// �����ł͖{���͂����ƍׂ����ݒ�����āA���[�U�̊��ɍ������f�o�C�X�����K�v������
	D3D_FEATURE_LEVEL level{};
	D3D11CreateDeviceAndSwapChain(
		nullptr,				// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr��
		D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���B�ӂ���HARDWARE
		nullptr,				// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
		D3D11_CREATE_DEVICE_DEBUG,					// ���炩�̃t���O���w�肷��B�i�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG�H�j
		nullptr,				// �f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Bnullptr�ɂ��Ƃ���OK
		0,					// ��̈����Ń��x�������w�肵����
		D3D11_SDK_VERSION,			// SDK�̃o�[�W�����B�K�����̒l
		&scDesc,				// ��ł��낢��ݒ肵���\����
		&pSwapChain_,				// ������������SwapChain�̃A�h���X���Ԃ��Ă���
		&pDevice_,				// ������������Device�A�h���X���Ԃ��Ă���
		&level,					// ������������Device�AContext�̃��x�����Ԃ��Ă���
		&pContext_);				// ������������Context�̃A�h���X���Ԃ��Ă���

	///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
	//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾�i�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g�j
	ID3D11Texture2D* pBackBuffer{ nullptr };
	HRESULT hResult = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hResult))
	{
		//assert(false && "�o�b�t�@�̎擾�Ɏ��s");
		MessageBox(nullptr, L"�o�b�t�@�̎擾�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	//�����_�[�^�[�Q�b�g�r���[���쐬
	pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);

	//�ꎞ�I�Ƀo�b�N�o�b�t�@���擾���������Ȃ̂ŉ��
	pBackBuffer->Release();

	///////////////////////////�[�x�X�e���V���r���[�̍쐬///////////////////////////

	const D3D11_TEXTURE2D_DESC DEPTH_DESC
	{
		.Width  = static_cast<UINT>(winW),
		.Height = static_cast<UINT>(winH),
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_D32_FLOAT,
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0,
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
	};

	hResult = pDevice_->CreateTexture2D(&DEPTH_DESC, nullptr, &pDepthStencil_);
	assert(SUCCEEDED(hResult) && "�[�x�X�e���V���̍쐬�Ɏ��s");

	if (pDepthStencil_ == nullptr)
	{
		MessageBox(nullptr, L"�[�x�o�b�t�@�̎擾�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	pDevice_->CreateDepthStencilView(pDepthStencil_, nullptr, &pDepthStencilView_);

	///////////////////////////�r���[�|�[�g�i�`��͈́j�ݒ�///////////////////////////////
	//�����_�����O���ʂ�\������͈�
	D3D11_VIEWPORT vp{};
	vp.Width = (float)winW;	//��
	vp.Height = (float)winH;//����
	vp.MinDepth = 0.0f;	//��O
	vp.MaxDepth = 1.0f;	//��
	vp.TopLeftX = 0;	//��
	vp.TopLeftY = 0;	//��

	//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�i�p�C�v���C���j
	// MEMO: ���ꂩ��f�[�^�Ƃ��ĕ`���Ă����O�p�`�A�|���S���̌q����������
	//     : �ǂ���O�p�`�ƌ��Ȃ����A���̂̌`�̂Ȃ���̊w�₪����
	//     : �q������͂��낢�날��A�g�|���W�[
	//     : �g���C�A���O���̃g�|���W�[
	pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // �f�[�^�̓��͎�ނ��w��
	// MEMO: �����_�[�^�[�Q�b�g�r���[��`��p�Ɏg���Ă�������
	//     : �����ō��̂��Ďg�����Ƃ�����
	pContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView_);            // �`����ݒ�
	// ��Ō��߂��r���[�|�[�g��ݒ肷��
	pContext_->RSSetViewports(1, &vp);

	hResult = InitializeShader3D();
	if (FAILED(hResult))
	{
		return hResult;
	}

	hResult = InitializeShader2D();
	if (FAILED(hResult))
	{
		return hResult;
	}

	hResult = InitializeShaderFbx();
	if (FAILED(hResult))
	{
		return hResult;
	}

	SetShader(SHADER_3D);

	return S_OK;
}

HRESULT Direct3D::InitializeShader3D()
{
	HRESULT hResult{};
#pragma region ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS{ nullptr };  // View�Ō��Ȃ��Ƃ킩��Ȃ��������̉�
	hResult = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	
	assert(pCompileVS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "���_�V�F�[�_�R���p�C���Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_�V�F�[�_�R���p�C���Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	//{  // debug blob print
	//	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
	//	HRESULT hr = D3DReflect(
	//		pCompileVS->GetBufferPointer(),
	//		pCompileVS->GetBufferSize(),
	//		IID_ID3D11ShaderReflection,
	//		(void**)&pVertexShaderReflection
	//	);
	//	if (FAILED(hr)) {}
	//	D3D11_SHADER_DESC shaderDesc;
	//	pVertexShaderReflection->GetDesc(&shaderDesc);
	//	std::stringstream ss{};
	//	for (UINT i = 0; i < shaderDesc.InputParameters; i++) {
	//		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
	//		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);
	//		ss << "SemanticName: " << paramDesc.SemanticName
	//			<< ", SemanticIndex: " << paramDesc.SemanticIndex
	//			<< ", Register: " << paramDesc.Register
	//			<< ", SystemValueType: " << paramDesc.SystemValueType
	//			<< ", Stream:" << paramDesc.Stream
	//			<< std::endl;
	//	}
	//	std::string str = ss.str();
	//	pVertexShaderReflection->Release();
	//}

	hResult = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaders_[SHADER_3D].pVertexShader);
	
	/*assert(SUCCEEDED(hResult)
		&& "���_�V�F�[�_�̍쐬�Ɏ��s Direct3D::InitializeShader");*/
	
	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region ���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // UV���W
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32 , D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �@��
	};

	hResult = pDevice_->CreateInputLayout(
		layout,
		sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),  // �v�f��
		pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaders_[SHADER_3D].pVertexLayout);

	/*assert(SUCCEEDED(hResult)
		&& "���_���C�A�E�g�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_���C�A�E�g�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	pCompileVS->Release();
#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	hResult = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	
	assert(pCompilePS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "�s�N�Z���V�F�[�_�R���p�C���Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�R���p�C���Ɏ��s", L"�G���[", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreatePixelShader(
		pCompilePS->GetBufferPointer(),
		pCompilePS->GetBufferSize(),
		NULL,
		&shaders_[SHADER_3D].pPixelShader);
	
	/*assert(SUCCEEDED(hResult)
		&& "�s�N�Z���V�F�[�_�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	pCompilePS->Release();
#pragma endregion

#pragma region ���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc{};
	rdc.CullMode = D3D11_CULL_BACK;  // ��ʏ���
	rdc.FillMode = D3D11_FILL_SOLID;  // �s�N�Z���V�F�[�_�ɍ��킹�ēh��Ԃ�
	rdc.FrontCounterClockwise = FALSE;  // front �O�� counter �t clockwise ���v���
	// FALSE: ���v��肪�\
	// TRUE: ���v��肪��
	hResult = pDevice_->CreateRasterizerState(&rdc, &shaders_[SHADER_3D].pRasterizerState);

	/*assert(SUCCEEDED(hResult)
		&& "���X�^���C�U�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}
#pragma endregion

	return S_OK;  // ���Ȃ�
}

HRESULT Direct3D::InitializeShader2D()
{
	HRESULT hResult{};
#pragma region ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS{ nullptr };  // View�Ō��Ȃ��Ƃ킩��Ȃ��������̉�
	hResult = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);

	assert(pCompileVS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "���_�V�F�[�_�R���p�C���Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_�V�F�[�_�R���p�C���Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaders_[SHADER_2D].pVertexShader);

	/*assert(SUCCEEDED(hResult)
		&& "���_�V�F�[�_�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region ���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },  // �ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // UV���W
	};

	hResult = pDevice_->CreateInputLayout(
		layout,
		sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),  // �v�f��
		pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaders_[SHADER_2D].pVertexLayout);

	/*assert(SUCCEEDED(hResult)
		&& "���_���C�A�E�g�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_���C�A�E�g�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	pCompileVS->Release();
#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	hResult = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);

	assert(pCompilePS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "�s�N�Z���V�F�[�_�R���p�C���Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�R���p�C���Ɏ��s", L"�G���[", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaders_[SHADER_2D].pPixelShader);

	/*assert(SUCCEEDED(hResult)
		&& "�s�N�Z���V�F�[�_�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	pCompilePS->Release();
#pragma endregion

#pragma region ���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc{};
	rdc.CullMode = D3D11_CULL_NONE;  // ��ʏ���
	rdc.FillMode = D3D11_FILL_SOLID;  // �s�N�Z���V�F�[�_�ɍ��킹�ēh��Ԃ�
	rdc.FrontCounterClockwise = FALSE;  // front �O�� counter �t clockwise ���v���
	// FALSE: ���v��肪�\
	// TRUE: ���v��肪��
	hResult = pDevice_->CreateRasterizerState(&rdc, &shaders_[SHADER_2D].pRasterizerState);

	/*assert(SUCCEEDED(hResult)
		&& "���X�^���C�U�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region ���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g
	//pContext_->VSSetShader(pVertexShader, NULL, 0);  // ���_�V�F�[�_�[
	//pContext_->PSSetShader(pPixelShader, NULL, 0);   // �s�N�Z���V�F�[�_�[
	//pContext_->IASetInputLayout(pVertexLayout);      // ���_�C���v�b�g���C�A�E�g
	//pContext_->RSSetState(pRasterizerState);         // ���X�^���C�U�[
#pragma endregion

	return S_OK;  // ���Ȃ�
}

HRESULT Direct3D::InitializeShaderFbx()
{
#if 1

	HRESULT hr;


	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);


	hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(),
		pCompileVS->GetBufferSize(), NULL, &(shaders_[SHADER_FBX].pVertexShader));

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}



	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(),
		pCompilePS->GetBufferSize(), NULL, &(shaders_[SHADER_FBX].pPixelShader));

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV���W
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }, //�@���x�N�g��
	};

	hr = pDevice_->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
		pCompileVS->GetBufferSize(), &(shaders_[SHADER_FBX].pVertexLayout));

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���_�C���v�b�g���C�A�E�g�̍쐬�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}


	pCompileVS->Release();
	pCompilePS->Release();
	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	pDevice_->CreateRasterizerState(&rdc, &(shaders_[SHADER_FBX].pRasterizerState));

	//���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g
	//pContext->VSSetShader(pVertexShader, NULL, 0);	//���_�V�F�[�_�[
	//pContext->PSSetShader(pPixelShader, NULL, 0);	//�s�N�Z���V�F�[�_�[
	//pContext->IASetInputLayout(pVertexLayout);	//���_�C���v�b�g���C�A�E�g
	//pContext->RSSetState(pRasterizerState);		//���X�^���C�U�[


	return S_OK;

#else
	HRESULT hResult{};
#pragma region ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS{ nullptr };  // View�Ō��Ȃ��Ƃ킩��Ȃ��������̉�
	hResult = D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "���_�V�F�[�_�R���p�C���Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_�V�F�[�_�R���p�C���Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaders_[SHADER_FBX].pVertexShader);

	/*assert(SUCCEEDED(hResult)
		&& "���_�V�F�[�_�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region ���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,  sizeof(DirectX::XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV���W
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 } //�@���x�N�g��
	};

	hResult = pDevice_->CreateInputLayout(
		layout,
		sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),  // �v�f��
		pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaders_[SHADER_FBX].pVertexLayout);

	/*assert(SUCCEEDED(hResult)
		&& "���_���C�A�E�g�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���_���C�A�E�g�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	pCompileVS->Release();
#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	hResult = D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);

	assert(pCompilePS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "�s�N�Z���V�F�[�_�R���p�C���Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�R���p�C���Ɏ��s", L"�G���[", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreatePixelShader(
		pCompilePS->GetBufferPointer(),
		pCompilePS->GetBufferSize(),
		NULL,
		&shaders_[SHADER_FBX].pPixelShader);

	/*assert(SUCCEEDED(hResult)
		&& "�s�N�Z���V�F�[�_�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}

	pCompilePS->Release();
#pragma endregion

#pragma region ���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc{};
	rdc.CullMode = D3D11_CULL_NONE;  // ��ʏ���
	rdc.FillMode = D3D11_FILL_SOLID;  // �s�N�Z���V�F�[�_�ɍ��킹�ēh��Ԃ�
	rdc.FrontCounterClockwise = FALSE;  // front �O�� counter �t clockwise ���v���
	// FALSE: ���v��肪�\
	// TRUE: ���v��肪��
	hResult = pDevice_->CreateRasterizerState(&rdc, &shaders_[SHADER_FBX].pRasterizerState);

	/*assert(SUCCEEDED(hResult)
		&& "���X�^���C�U�̍쐬�Ɏ��s Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hResult;
	}
#pragma endregion

	return S_OK;  // ���Ȃ�
#endif
}

void Direct3D::BeginDraw()
{
	// �w�i�̐F
	float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	// ��ʂ��N���A
	pContext_->ClearRenderTargetView(pRenderTargetView_, clearColor);

	pContext_->ClearDepthStencilView(pDepthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndDraw()
{
	// �X���b�v�i�o�b�N�o�b�t�@��\�ɕ\������j
	pSwapChain_->Present(0, 0);
}

void Direct3D::Release()
{
	SAFE_DELETE(pInstance_);
}

Direct3D& Direct3D::Instance()
{
	if (pInstance_ == nullptr)
	{
		pInstance_ = new Direct3D{};
	}

	return *pInstance_;
}

Direct3D::Direct3D()
{
}

Direct3D::~Direct3D()
{
	for (int shaderType = 0; shaderType < SHADER_MAX; shaderType++)
	{
		SAFE_RELEASE(shaders_[shaderType].pRasterizerState);
		SAFE_RELEASE(shaders_[shaderType].pVertexLayout);
		SAFE_RELEASE(shaders_[shaderType].pPixelShader);
		SAFE_RELEASE(shaders_[shaderType].pVertexShader);
	}

	SAFE_RELEASE(pRenderTargetView_);
	SAFE_RELEASE(pSwapChain_);  // �W���b�L�[�`�F���A�e���Ƃ������炵�� by Yz
	SAFE_RELEASE(pDevice_);
	SAFE_RELEASE(pContext_);
}

Direct3D* Direct3D::pInstance_{ nullptr };
