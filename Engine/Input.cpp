#include "Input.h"
#include "Xinput.h"

#include <format>

#pragma comment(lib, "Xinput.lib")

using DirectX::XMVECTOR;

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256]{};
	BYTE keyStatePrev[256]{};
	XMVECTOR mousePosition{};
	LPDIRECTINPUTDEVICE8 mouseDevice = nullptr;
	DIMOUSESTATE mouseState{};
	DIMOUSESTATE mouseStatePrev{};
	XINPUT_STATE controllerState_;
	XINPUT_STATE controllerStatePrev_;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(
			GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		pDInput->CreateDevice(GUID_SysMouse, &mouseDevice, nullptr);
		mouseDevice->SetDataFormat(&c_dfDIMouse);
		mouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		memcpy(keyStatePrev, keyState, sizeof(keyState));
		memcpy(&controllerStatePrev_, &controllerState_, sizeof(controllerState_));
		memcpy(&mouseStatePrev, &mouseState, sizeof(mouseState));

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		mouseDevice->Acquire();
		mouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

		XInputGetState(0, &controllerState_);

		OutputDebugString(std::format(
			L"{}, {}, {}, {}\n",
			mouseState.rgbButtons[0],
			mouseState.rgbButtons[1],
			mouseState.rgbButtons[2],
			mouseState.rgbButtons[3]
			).c_str());
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode])
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if (!keyState[keyCode] && keyStatePrev[keyCode])
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		if (keyState[keyCode] && !keyStatePrev[keyCode])
		{
			return true;
		}
		return false;
	}

	bool IsButton(int button)
	{
		if (controllerState_.Gamepad.wButtons & button)
		{
			return true;
		}
		return false;
	}

	bool IsButtonUp(int button)
	{
		if ((controllerState_.Gamepad.wButtons & button)
			&& !(controllerStatePrev_.Gamepad.wButtons & button))
		{
			return true;
		}
		return false;
	}

	bool IsButtonDown(int button)
	{
		if (!(controllerState_.Gamepad.wButtons & button)
			&& (controllerStatePrev_.Gamepad.wButtons & button))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButton(int button)
	{
		if (mouseState.rgbButtons[button])
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonUp(int button)
	{
		if (mouseState.rgbButtons[button] && !mouseStatePrev.rgbButtons[button])
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int button)
	{
		if (!mouseState.rgbButtons[button] && mouseStatePrev.rgbButtons[button])
		{
			return true;
		}
		return false;
	}

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(const int x, const int y)
	{
		mousePosition = DirectX::XMVectorSet((float)x, (float)y, 0, 0);
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
	}
}