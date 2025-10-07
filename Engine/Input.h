#pragma once

#include <DirectXMath.h>
#include <dInput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyUp(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsButton(int button);
	bool IsButtonUp(int button);
	bool IsButtonDown(int button);
	bool IsMouseButton(int button);
	bool IsMouseButtonUp(int button);
	bool IsMouseButtonDown(int button);

	DirectX::XMVECTOR GetMousePosition();
	void SetMousePosition(const int x, const int y);
	void Release();

};
