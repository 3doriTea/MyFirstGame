#pragma once
#include <Windows.h>

/// <summary>
/// マウスカーソルを制御するクラス
/// </summary>
namespace Cursor
{
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="_hWnd">ウィンドウハンドル</param>
	/// <param name="_clientRect">クライアント領域</param>
	/// <param name="_size">ウィンドウサイズ</param>
	void Initialize(const HWND _hWnd, const RECT& _clientRect, const POINT _size);

	/// <summary>
	/// マウスカーソルをウィンドウの範囲内に制限する
	/// </summary>
	/// <param name="_onLock">制限する true /false</param>
	void SetLock(const bool _onLock);

	/// <summary>
	/// 制限しているかを取得する
	/// </summary>
	/// <returns>制限している true / false</returns>
	bool IsLock();

	/// <summary>
	/// カーソルの移動量を加算する
	/// </summary>
	/// <param name="_moveX">移動量 x</param>
	/// <param name="_moveY">移動量 x</param>
	void AddMove(const int _moveX, const int _moveY);

	void ClearMove()

	/// <summary>
	/// マウスカーソルを画面中心に移動する
	/// </summary>
	void SetPositionCenter();
};
