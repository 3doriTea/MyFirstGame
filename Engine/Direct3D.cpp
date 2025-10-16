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
	instance.pContext_->VSSetShader(instance.shaders_[_shaderType].pVertexShader, NULL, 0);  // 頂点シェーダー
	instance.pContext_->PSSetShader(instance.shaders_[_shaderType].pPixelShader, NULL, 0);   // ピクセルシェーダー
	instance.pContext_->IASetInputLayout(instance.shaders_[_shaderType].pVertexLayout);      // 頂点インプットレイアウト
	instance.pContext_->RSSetState(instance.shaders_[_shaderType].pRasterizerState);         // ラスタライザー
}

HRESULT Direct3D::Initialize(const int winW, const int winH, HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scDesc{};

	screenSize_ = { winW, winH };

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃

	////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	// ここでは本当はもっと細かい設定をして、ユーザの環境に合ったデバイスを作る必要がある
	D3D_FEATURE_LEVEL level{};
	D3D11CreateDeviceAndSwapChain(
		nullptr,				// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,				// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		D3D11_CREATE_DEVICE_DEBUG,					// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,				// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,					// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
		&scDesc,				// 上でいろいろ設定した構造体
		&pSwapChain_,				// 無事完成したSwapChainのアドレスが返ってくる
		&pDevice_,				// 無事完成したDeviceアドレスが返ってくる
		&level,					// 無事完成したDevice、Contextのレベルが返ってくる
		&pContext_);				// 無事完成したContextのアドレスが返ってくる

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ID3D11Texture2D* pBackBuffer{ nullptr };
	HRESULT hResult = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hResult))
	{
		//assert(false && "バッファの取得に失敗");
		MessageBox(nullptr, L"バッファの取得に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	//レンダーターゲットビューを作成
	pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);

	//一時的にバックバッファを取得しただけなので解放
	pBackBuffer->Release();

	///////////////////////////深度ステンシルビューの作成///////////////////////////

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
	assert(SUCCEEDED(hResult) && "深度ステンシルの作成に失敗");

	if (pDepthStencil_ == nullptr)
	{
		MessageBox(nullptr, L"深度バッファの取得に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	pDevice_->CreateDepthStencilView(pDepthStencil_, nullptr, &pDepthStencilView_);

	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
	D3D11_VIEWPORT vp{};
	vp.Width = (float)winW;	//幅
	vp.Height = (float)winH;//高さ
	vp.MinDepth = 0.0f;	//手前
	vp.MaxDepth = 1.0f;	//奥
	vp.TopLeftX = 0;	//左
	vp.TopLeftY = 0;	//上

	//データを画面に描画するための一通りの設定（パイプライン）
	// MEMO: これからデータとして描いていく三角形、ポリゴンの繋ぎ方がある
	//     : どれを三角形と見なすか、ものの形のつながりの学問がある
	//     : 繋がり方はいろいろある、トポロジー
	//     : トライアングルのトポロジー
	pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
	// MEMO: レンダーターゲットビューを描画用に使ってください
	//     : 何個かで合体して使うことがある
	pContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView_);            // 描画先を設定
	// 上で決めたビューポートを設定する
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
#pragma region 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS{ nullptr };  // Viewで見ないとわからないメモリの塊
	hResult = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	
	assert(pCompileVS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "頂点シェーダコンパイルに失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点シェーダコンパイルに失敗しました", L"エラー", MB_OK);
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
		&& "頂点シェーダの作成に失敗 Direct3D::InitializeShader");*/
	
	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点シェーダの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region 頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },  // 位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // UV座標
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32 , D3D11_INPUT_PER_VERTEX_DATA, 0 },  // 法線
	};

	hResult = pDevice_->CreateInputLayout(
		layout,
		sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),  // 要素数
		pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaders_[SHADER_3D].pVertexLayout);

	/*assert(SUCCEEDED(hResult)
		&& "頂点レイアウトの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点レイアウトの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	pCompileVS->Release();
#pragma endregion

#pragma region ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	hResult = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	
	assert(pCompilePS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "ピクセルシェーダコンパイルに失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ピクセルシェーダコンパイルに失敗", L"エラー", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreatePixelShader(
		pCompilePS->GetBufferPointer(),
		pCompilePS->GetBufferSize(),
		NULL,
		&shaders_[SHADER_3D].pPixelShader);
	
	/*assert(SUCCEEDED(hResult)
		&& "ピクセルシェーダの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	pCompilePS->Release();
#pragma endregion

#pragma region ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc{};
	rdc.CullMode = D3D11_CULL_BACK;  // 印面消去
	rdc.FillMode = D3D11_FILL_SOLID;  // ピクセルシェーダに合わせて塗りつぶす
	rdc.FrontCounterClockwise = FALSE;  // front 前は counter 逆 clockwise 時計回り
	// FALSE: 時計回りが表
	// TRUE: 時計回りが裏
	hResult = pDevice_->CreateRasterizerState(&rdc, &shaders_[SHADER_3D].pRasterizerState);

	/*assert(SUCCEEDED(hResult)
		&& "ラスタライザの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

	return S_OK;  // 問題なし
}

HRESULT Direct3D::InitializeShader2D()
{
	HRESULT hResult{};
#pragma region 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS{ nullptr };  // Viewで見ないとわからないメモリの塊
	hResult = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);

	assert(pCompileVS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "頂点シェーダコンパイルに失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点シェーダコンパイルに失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaders_[SHADER_2D].pVertexShader);

	/*assert(SUCCEEDED(hResult)
		&& "頂点シェーダの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点シェーダの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region 頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },  // 位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // UV座標
	};

	hResult = pDevice_->CreateInputLayout(
		layout,
		sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),  // 要素数
		pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaders_[SHADER_2D].pVertexLayout);

	/*assert(SUCCEEDED(hResult)
		&& "頂点レイアウトの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点レイアウトの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	pCompileVS->Release();
#pragma endregion

#pragma region ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	hResult = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);

	assert(pCompilePS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "ピクセルシェーダコンパイルに失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ピクセルシェーダコンパイルに失敗", L"エラー", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaders_[SHADER_2D].pPixelShader);

	/*assert(SUCCEEDED(hResult)
		&& "ピクセルシェーダの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	pCompilePS->Release();
#pragma endregion

#pragma region ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc{};
	rdc.CullMode = D3D11_CULL_NONE;  // 印面消去
	rdc.FillMode = D3D11_FILL_SOLID;  // ピクセルシェーダに合わせて塗りつぶす
	rdc.FrontCounterClockwise = FALSE;  // front 前は counter 逆 clockwise 時計回り
	// FALSE: 時計回りが表
	// TRUE: 時計回りが裏
	hResult = pDevice_->CreateRasterizerState(&rdc, &shaders_[SHADER_2D].pRasterizerState);

	/*assert(SUCCEEDED(hResult)
		&& "ラスタライザの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region それぞれをデバイスコンテキストにセット
	//pContext_->VSSetShader(pVertexShader, NULL, 0);  // 頂点シェーダー
	//pContext_->PSSetShader(pPixelShader, NULL, 0);   // ピクセルシェーダー
	//pContext_->IASetInputLayout(pVertexLayout);      // 頂点インプットレイアウト
	//pContext_->RSSetState(pRasterizerState);         // ラスタライザー
#pragma endregion

	return S_OK;  // 問題なし
}

HRESULT Direct3D::InitializeShaderFbx()
{
#if 1

	HRESULT hr;


	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);


	hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(),
		pCompileVS->GetBufferSize(), NULL, &(shaders_[SHADER_FBX].pVertexShader));

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"頂点シェーダの作成の作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}



	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(),
		pCompilePS->GetBufferSize(), NULL, &(shaders_[SHADER_FBX].pPixelShader));

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成の作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }, //法線ベクトル
	};

	hr = pDevice_->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
		pCompileVS->GetBufferSize(), &(shaders_[SHADER_FBX].pVertexLayout));

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"頂点インプットレイアウトの作成の作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}


	pCompileVS->Release();
	pCompilePS->Release();
	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	pDevice_->CreateRasterizerState(&rdc, &(shaders_[SHADER_FBX].pRasterizerState));

	//それぞれをデバイスコンテキストにセット
	//pContext->VSSetShader(pVertexShader, NULL, 0);	//頂点シェーダー
	//pContext->PSSetShader(pPixelShader, NULL, 0);	//ピクセルシェーダー
	//pContext->IASetInputLayout(pVertexLayout);	//頂点インプットレイアウト
	//pContext->RSSetState(pRasterizerState);		//ラスタライザー


	return S_OK;

#else
	HRESULT hResult{};
#pragma region 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS{ nullptr };  // Viewで見ないとわからないメモリの塊
	hResult = D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "頂点シェーダコンパイルに失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点シェーダコンパイルに失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaders_[SHADER_FBX].pVertexShader);

	/*assert(SUCCEEDED(hResult)
		&& "頂点シェーダの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点シェーダの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region 頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,  sizeof(DirectX::XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 } //法線ベクトル
	};

	hResult = pDevice_->CreateInputLayout(
		layout,
		sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),  // 要素数
		pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaders_[SHADER_FBX].pVertexLayout);

	/*assert(SUCCEEDED(hResult)
		&& "頂点レイアウトの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点レイアウトの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	pCompileVS->Release();
#pragma endregion

#pragma region ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	hResult = D3DCompileFromFile(L"FbxModel.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);

	assert(pCompilePS != nullptr);
	/*assert(SUCCEEDED(hResult)
		&& "ピクセルシェーダコンパイルに失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ピクセルシェーダコンパイルに失敗", L"エラー", MB_OK);
		return hResult;
	}

	hResult = pDevice_->CreatePixelShader(
		pCompilePS->GetBufferPointer(),
		pCompilePS->GetBufferSize(),
		NULL,
		&shaders_[SHADER_FBX].pPixelShader);

	/*assert(SUCCEEDED(hResult)
		&& "ピクセルシェーダの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	pCompilePS->Release();
#pragma endregion

#pragma region ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc{};
	rdc.CullMode = D3D11_CULL_NONE;  // 印面消去
	rdc.FillMode = D3D11_FILL_SOLID;  // ピクセルシェーダに合わせて塗りつぶす
	rdc.FrontCounterClockwise = FALSE;  // front 前は counter 逆 clockwise 時計回り
	// FALSE: 時計回りが表
	// TRUE: 時計回りが裏
	hResult = pDevice_->CreateRasterizerState(&rdc, &shaders_[SHADER_FBX].pRasterizerState);

	/*assert(SUCCEEDED(hResult)
		&& "ラスタライザの作成に失敗 Direct3D::InitializeShader");*/

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

	return S_OK;  // 問題なし
#endif
}

void Direct3D::BeginDraw()
{
	// 背景の色
	float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	// 画面をクリア
	pContext_->ClearRenderTargetView(pRenderTargetView_, clearColor);

	pContext_->ClearDepthStencilView(pDepthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndDraw()
{
	// スワップ（バックバッファを表に表示する）
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
	SAFE_RELEASE(pSwapChain_);  // ジャッキーチェン、親日家だったらしい by Yz
	SAFE_RELEASE(pDevice_);
	SAFE_RELEASE(pContext_);
}

Direct3D* Direct3D::pInstance_{ nullptr };
