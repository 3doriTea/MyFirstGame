#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "Utility.h"


// リンカ
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
	friend class Sprite2D;
	friend class Texture;
	friend class Fbx;

public:
	enum ShaderType
	{
		SHADER_3D,  // 3Dシェーダ
		SHADER_2D,  // 2Dシェーダ
		SHADER_FBX,  // Fbx
		SHADER_MAX,  // シェーダの種類数
	};

private:
	struct ShaderBundle
	{
		ID3D11VertexShader* pVertexShader{ nullptr };          // 頂点シェーダー
		ID3D11PixelShader* pPixelShader{ nullptr };            // ピクセルシェーダー
		ID3D11InputLayout* pVertexLayout{ nullptr };           // 頂点インプットレイアウト
		ID3D11RasterizerState* pRasterizerState{ nullptr };    // ラスタライザー
	};

public:
	static void SetShader(ShaderType _shaderType);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="winW">ウィンドウ横幅</param>
	/// <param name="winH">ウィンドウ高さ</param>
	/// <param name="hWnd">ウィンドウハンドル</param>
	HRESULT Initialize(const int winW, const int winH, HWND hWnd);

	HRESULT InitializeShader3D();
	HRESULT InitializeShader2D();
	HRESULT InitializeShaderFbx();

	/// <summary>
	/// 描画開始
	/// </summary>
	void BeginDraw();

	/// <summary>
	/// 描画終了
	/// </summary>
	void EndDraw();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Release();

	static Direct3D& Instance();
	
	static inline Vector2Int ScreenSize() { return pInstance_->screenSize_; }

private:
	Direct3D();
	~Direct3D();

	static Direct3D* pInstance_;

private:
	Vector2Int screenSize_;

	// ポインタ扱うのにnullptrを入れない理由
	// Microsoftのやつで初期化するから別にいいや らしい?
	ID3D11Device* pDevice_{ nullptr };                      // デバイス
	ID3D11DeviceContext* pContext_{ nullptr };              // デバイスコンテキスト
	IDXGISwapChain* pSwapChain_{ nullptr };                 // スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView_{ nullptr };  // レンダーターゲットビュー
	ShaderBundle shaders_[SHADER_MAX]{};
};
