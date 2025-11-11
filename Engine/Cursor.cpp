#include "Cursor.h"

namespace
{
	HWND hWnd_{ nullptr };
	RECT clientRect_{};
	RECT screenRect_{};
	POINT size_{};
	bool isLocking_{};
}

void Cursor::Initialize(const HWND _hWnd, const RECT& _clientRect, const POINT _size)
{
	hWnd_ = _hWnd;
	clientRect_ = _clientRect;
	size_ = _size;
}

void Cursor::SetLock(const bool _onLock)
{
	POINT topLeft{ clientRect_.left, clientRect_.top };
	POINT bottomRight{ clientRect_.right, clientRect_.bottom };
	ClientToScreen(hWnd_, &topLeft);
	ClientToScreen(hWnd_, &bottomRight);

	screenRect_ =
	{
		topLeft.x,
		topLeft.y + (clientRect_.bottom - clientRect_.top) - size_.y,
		bottomRight.x,
		bottomRight.y
	};

	if (_onLock)
	{
		// マウスカーソルをウィンドウの範囲内に収める
		ClipCursor(&screenRect_);
	}
	else
	{
		ClipCursor(nullptr);
	}
	isLocking_ = _onLock;
}

bool Cursor::IsLock()
{
	return isLocking_;
}

void Cursor::SetPositionCenter()
{
	POINT topLeft{ clientRect_.left, clientRect_.top };
	POINT bottomRight{ clientRect_.right, clientRect_.bottom };
	POINT center
	{
		(clientRect_.right - clientRect_.left) / 2,
		(clientRect_.bottom - clientRect_.top) / 2
	};
	ClientToScreen(hWnd_, &topLeft);
	ClientToScreen(hWnd_, &bottomRight);
	ClientToScreen(hWnd_, &center);

	SetCursorPos(center.x, center.y);
}
