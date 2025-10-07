#include "Texture.h"
//#include <wincodec.h>
#include "Direct3D.h"
#include <DirectXTex.h>

// DirectXTexのライブラリをリンク

#pragma comment(lib, "DirectXTex.lib")

using namespace DirectX;

Texture::Texture() :
	pSampler_{ nullptr },
	pSRV_{ nullptr }
{
}

Texture::~Texture()
{
	Release();
}

HRESULT Texture::Load(std::string _fileName)
{
	TexMetadata metadata; //画像の付属情報

	ScratchImage image;   //画像本体

	HRESULT hr;

	std::wstring fileName{ _fileName.begin(), _fileName.end() };

	hr = LoadFromWICFile(
		fileName.c_str(),
		WIC_FLAGS::WIC_FLAGS_NONE,
		&metadata, image);

	if (FAILED(hr))
	{
		return hr;
	}

	size_.x = metadata.width;
	size_.y = metadata.height;

	D3D11_SAMPLER_DESC SamDesc{};
	// MEMO: 線形補間もハードウェアでやってくれるらしい
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// MEMO: 0~1の範囲を出た場合どうするか
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = Direct3D::Instance().pDevice_->CreateSamplerState(&SamDesc, &pSampler_);

	if (FAILED(hr))
	{
		return hr;
	}

	// シェーダーリソースビュー
	D3D11_SHADER_RESOURCE_VIEW_DESC srv{};

	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// MEMO: ここで3Dのテクスチャも指定できる
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;  // LODのミップマップレベル

	hr = CreateShaderResourceView(
		Direct3D::Instance().pDevice_,
		image.GetImages(),
		image.GetImageCount(),
		metadata,
		&pSRV_);

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void Texture::Release()
{
	SAFE_RELEASE(pSRV_);
	SAFE_RELEASE(pSampler_);
}
