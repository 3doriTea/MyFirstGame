#pragma once
#include <d3d11.h>
#include "string"
#include "Utility.h"

class Texture
{
public:
	Texture();
	~Texture();
	HRESULT Load(std::string fileName);
	void Release();

	inline ID3D11SamplerState* GetSampler()   { return pSampler_; }
	inline ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
	inline Vector2Int GetSize() const         { return size_; }

private:
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
	Vector2Int size_;
};
