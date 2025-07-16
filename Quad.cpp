#include "Quad.h"
#include <cassert>
#include "Camera.h"
#include "Texture.h"

using namespace DirectX;

Quad::Quad() :
	pVertexBuffer_{ nullptr },
	pIndexBuffer_{ nullptr },
	pConstantBuffer_{ nullptr },
	pTexture_{ nullptr }
{
}

Quad::~Quad()
{
}

HRESULT Quad::Initialize()
{
	return Initialize("Sushi512x512.png");
}

HRESULT Quad::Initialize(const char* _fileName)
{
	HRESULT hResult{};//512x256

#pragma region 頂点バッファを作成
	// 頂点情報
	Vertex vertices[]
	{
		// { { POSITION }, { UV }, { NORMAL } }
		{ { -1.0f,  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0, 0 }, { 0, 0, -1, 0 } },  // 四角形の頂点（左上）
		{ {  1.0f,  1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0, 0 }, { 0, 0, -1, 0 } },  // 四角形の頂点（右上）
		{ {  1.0f, -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0, 0 }, { 0, 0, -1, 0 } },  // 四角形の頂点（右下）
		{ { -1.0f, -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0, 0 }, { 0, 0, -1, 0 } },  // 四角形の頂点（左下）
	};

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex{};
	//bd_vertex.ByteWidth = sizeof(vertices);  // 型の大きさ
	bd_vertex.ByteWidth = sizeof(vertices);  // 型の大きさ
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex{};
	data_vertex.pSysMem = vertices;
	hResult = Direct3D::Instance().pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	//assert(SUCCEEDED(hResult) && "頂点バッファの作成に失敗 @Quad::Initialize");

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点バッファの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region インデックスバッファを作成
	//インデックス情報
	int index[] = { 0,2,3, 0,1,2 };  // CW

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = index;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;
	hResult = Direct3D::Instance().pDevice->CreateBuffer(&bd, &initData, &pIndexBuffer_);

	//assert(SUCCEEDED(hResult) && "インデックスバッファの作成に失敗 @Quad::Initialize");

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

#pragma region コンスタントバッファの作成
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb{};
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	hResult = Direct3D::Instance().pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);

	//assert(SUCCEEDED(hResult) && "コンスタントバッファの作成に失敗 @Quad::Initialize");

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"コンスタントバッファの作成に失敗しました", L"エラー", MB_OK);
		return hResult;
	}
#pragma endregion

	pTexture_ = new Texture{};
	hResult = pTexture_->Load(_fileName);

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"テクスチャのロードに失敗しました", L"エラー", MB_OK);
		return hResult;
	}

	return S_OK;
}

void Quad::Draw(XMMATRIX& _worldMatrix)
{
	Draw(_worldMatrix, XMMatrixIdentity());
}

void Quad::Draw(const DirectX::XMMATRIX& _worldMatrix, const DirectX::XMMATRIX& _uvMatrix)
{
#pragma region コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb{};
	cb.matWVP = XMMatrixTranspose(_worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.textureMatrix = XMMatrixTranspose(_uvMatrix);
	cb.matW = XMMatrixTranspose(_worldMatrix);
	cb.matRotateW = XMMatrixTranspose(_worldMatrix);
	cb.matRotateW.r[0].m128_f32[0] = 0.0;
	cb.matRotateW.r[0].m128_f32[1] = 0.0;
	cb.matRotateW.r[0].m128_f32[2] = 0.0;
#pragma endregion

#pragma region コンスタントバッファの送信
	D3D11_MAPPED_SUBRESOURCE pdata{};
	Direct3D::Instance().pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();

	Direct3D::Instance().pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();

	Direct3D::Instance().pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::Instance().pContext->Unmap(pConstantBuffer_, 0);	//再開
#pragma endregion

#pragma region 諸々セット
	//頂点バッファ
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Direct3D::Instance().pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::Instance().pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// コンスタントバッファ
	Direct3D::Instance().pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::Instance().pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用
#pragma endregion

	// 実際に描画
	Direct3D::Instance().pContext->DrawIndexed(6, 0, 0);
}

void Quad::Release()
{
	pTexture_->Release();
	SAFE_DELETE(pTexture_);

	// 逆順でrelease
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

//ID3D11Buffer* Quad::pVertexBuffer_{ nullptr };
//ID3D11Buffer* Quad::pIndexBuffer_{ nullptr };
//ID3D11Buffer* Quad::pConstantBuffer_{ nullptr };
