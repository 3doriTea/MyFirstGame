// TheMain.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "TheMain.h"
#include <thread>
#include <format>
#include <cassert>
#include <chrono>

#include "Engine/Direct3D.h"

#include "Engine/Transform.h"

#include "Engine/Camera.h"
#include "Engine/Quad.h"
//#include "Engine/Cube.h"
#include "Engine/Sprite2D.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Cursor.h"

#include "Engine/RootJob.h"

#pragma comment(lib, "winmm.lib")

HWND hWnd{ nullptr };

#define MAX_LOADSTRING 100

const wchar_t* WIN_CLASS_NAME{ L"△プルGameうぃん同" };
// ウィンドウ幅
const int SCREEN_WIDTH{ 800 };
const int SCREEN_HEIGHT{ 600 };

// ウィンドウ範囲
RECT windowRect;

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

RootJob* pRootJob{ nullptr };

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/*
_In_     HINSTANCE hInstance : アプリケーションをメモリに展開するときの識別、リソースの読み込みで使用
_In_opt_ HINSTANCE hPrevInstance : 常にNULL、過去の16bit Windowsで使われていた名残
_In_     LPWSTR    lpCmdLine : コマンドライン引数（ANSIまたはUnicode文字列） exeを実行したときの引数が格納される
_In_     int       nCmdShow : ウィンドウの最初の表示状態 (最小化・最大化・通常表示など)
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MYFIRSTGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化の実行:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYFIRSTGAME));

	HRESULT hResult{};
	hResult = Direct3D::Instance().Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, hWnd);

	if (FAILED(hResult))
	{
		Direct3D::Instance().Release();
		return FALSE;
	}

	Input::Initialize(hWnd);
	Camera::Initialize();

	MSG msg{};

	/*
		アプリケーションのメインループで、`GetMessage`や`PeekMessage`を呼ぶことで、メッセージを取得できる。
		`TranslateMessage`でキーボードの生入力を、キー文字として変換、
		`DispatchMessage`でWindowプロシージャを実行し、メッセージを送りつける。
	*/


#pragma region 過去の遺物おでん
	/*Cube* quad{ new Cube{} };
	hResult = quad->Initialize();


	if (FAILED(hResult))
	{
		SAFE_DELETE(quad);
		Direct3D::Release();
		return FALSE;
	}*/

	/*Fbx odenModel{};
	hResult = odenModel.Load("Oden.fbx");
	
	if (FAILED(hResult))
	{
		Direct3D::Release();
		return FALSE;
	}


	Sprite2D* pSprite{ new Sprite2D{} };
	hResult = pSprite->Initialize("Assets/Sushi512x512.png");

	if (FAILED(hResult))
	{
		SAFE_DELETE(pSprite);
		Direct3D::Release();
		return FALSE;
	}*/
#pragma endregion

	// 一番親のゲームオブジェクト
	pRootJob = new RootJob{ nullptr };
	pRootJob->Initialize();

	Cursor::Initialize(hWnd, windowRect, { SCREEN_WIDTH, SCREEN_HEIGHT });

	timeBeginPeriod(1);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))  //メッセージあり
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else  //メッセージなし
		{
			static DWORD secPrevTime{ timeGetTime() };
			static DWORD prevTime{ timeGetTime() };

			DWORD currTime{ timeGetTime() };
			static DWORD countFps{ 0 };

			if ((currTime - prevTime) * 60 < 1000)
			{
				continue;
			}
			prevTime = currTime;

			countFps++;
			static DWORD fps{ 0u };
			if (currTime - secPrevTime >= 1000)
			{
				secPrevTime = currTime;
				fps = countFps;
				countFps = 1;
			}
			SetWindowTextA(hWnd, std::format("FPS:{}, {}", fps, countFps, 1000.0f / (currTime - prevTime)).c_str());

			Input::Update();

			Camera::Update();  // カメラ更新のタイミングはここが望ましい!!

			pRootJob->Update();

			/*if (Input::IsKeyUp(DIK_ESCAPE) || Input::IsMouseButtonDown(0x00))
			{
				static int count{ 0 };
				count++;
				if (count >= 3)
				{
					PostQuitMessage(0);
				}
			}*/

			//DirectX::XMVECTOR pos{ Input::GetMousePosition() };
			//OutputDebugString(std::format(L"x:{}, y:{}\n", pos.m128_f32[0], pos.m128_f32[1]).c_str());

			pRootJob->UpdateSub();

			Cursor::ClearMove();

			Direct3D::Instance().BeginDraw();
			//ゲームの処理

			pRootJob->DrawSub();
#pragma region 過去の異物おでん
			////描画処理
			//static float angle{};
			//const Vector2Int imageSize{ pSprite->GetSize() };
			//static Vector2Int pickPos{ (imageSize.x / 2), (imageSize.y / 2) };
			//
			//angle += XMConvertToRadians(0.1f);
			//if (angle > XM_2PI * 2.0f)
			//{
			//	angle -= XM_2PI * 2.0f;
			//	auto getRand
			//	{
			//		[]() -> float { return std::rand() / static_cast<float>(RAND_MAX); }
			//	};

			//	pickPos = { static_cast<int>(getRand() * imageSize.x), static_cast<int>(getRand() * imageSize.y) };
			//}

			//Transform transform{};

			////XMMATRIX mat{};
			//transform.rotate_ = { angle, angle / 2.0f, 0.0f };
			////mat *= XMMatrixRotationY(angle) * XMMatrixRotationX(angle / 2.0f);
			//float scalingAngle{ angle };
			//scalingAngle = 0.1f;
			//transform.scale_ =
			//{
			//	std::fabsf(std::cosf(scalingAngle) * 4),
			//	std::fabsf(std::cosf(scalingAngle) * 3),
			//	std::fabsf(std::cosf(scalingAngle) * 2),
			//};

			////quad->Draw(mat);
			//transform.Calculation();
			//
			////quad->Draw(transform.GetWorldMatrix());

			///*mat = XMMatrixIdentity();
			//mat *= XMMatrixRotationY(angle / 2.0f) * XMMatrixRotationX(angle / 1.0f);
			//mat *= DirectX::XMMatrixScaling(
			//	std::fabsf(std::cosf(scalingAngle) * 2),
			//	std::fabsf(std::cosf(scalingAngle) * 1),
			//	std::fabsf(std::cosf(scalingAngle) * 3));
			//quad->Draw(mat);*/

			//transform = {};

			//transform.rotate_ = { 0.0f, angle / 2.0f, 0.0f };
			///*transform.scale_ =
			//{
			//	std::fabsf(std::cosf(scalingAngle) * 2),
			//	std::fabsf(std::cosf(scalingAngle) * 1),
			//	std::fabsf(std::cosf(scalingAngle) * 3),
			//};*/
			////transform.Calculation();

			////transform = {};
			//transform.position_ = { 0, -10, 20 };
			//// transform.rotate_ = { 0, DirectX::XMConvertToRadians(45), 0 };
			//// transform.Calculation();

			//odenModel.Draw(transform);
			////quad->Draw(transform.GetWorldMatrix());

			////pSprite->Draw(
			////	RectanInt{ 0, 0, (Direct3D::ScreenSize() / 2) },
			////	angle,
			////	RectanInt
			////	{
			////		static_cast<int>(pickPos.x),  // std::fabsf(std::sinf(angle)) * 
			////		static_cast<int>(pickPos.y),  // std::fabsf(std::sinf(angle)) * 
			////		static_cast<int>(std::fabsf(std::cosf(angle)) * imageSize.x),
			////		static_cast<int>(std::fabsf(std::cosf(angle)) * imageSize.y)
			////	});
			#pragma endregion

			Direct3D::Instance().EndDraw();
			//描画処理
		}
	}
	timeEndPeriod(1);

	if (pRootJob)
	{
		pRootJob->ReleaseSub();
	}
	Model::Release();
	//SAFE_RELEASE(pRootJob);

	#pragma region 過去の遺物おでん
	/*SAFE_RELEASE(pSprite);
	SAFE_DELETE(pSprite);*/

	/*SAFE_RELEASE(quad);
	SAFE_DELETE(quad);*/
	#pragma endregion

	Direct3D::Release();

	return (int) msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	/*
		Windowsアプリケーションでウィンドウの属性や動作を定義するための構造体。
		この構造体にウィンドウのスタイル、アイコン、カーソル、背景色、メニュー、ウィンドウプロシージャ（メッセージ処理関数）などの情報を設定し、`
		RegisterClassEx`関数でWindowsに登録できる。
		これを作ることで、同じ属性を持つウィンドウを複数作成できるようになります。
	*/
	WNDCLASSEXW wcex;

	/*
		- `cbSize` : 構造体のサイズ
		- `style` : ウィンドウクラスのスタイル
		- `lpfnWndProc` : ウィンドウプロシージャのアドレス（コールバック = WndProc）
		- `hInstance` : インスタンスハンドル（アプリケーションやDLLの識別子）
		- `hIcon`/`hIconSm` : ウィンドウのアイコン
		- `hCursor` : マウスカーソル
		- `hbrBackground` : 背景ブラシ
		- `lpszMenuName` : メニュー名
		- `lpszClassName` : クラス名（ウィンドウ識別用）
	*/

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYFIRSTGAME));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;//MAKEINTRESOURCEW(IDC_MYFIRSTGAME);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	/*
	- `DWORD dwExStyle` : 拡張ウィンドウスタイル
	- `LPCTSTR lpClassName` : ウィンドウクラス名（`WNDCLASSEX`で登録した名前）
	- `LPCTSTR lpWindowName` : ウィンドウのタイトルバーに表示される文字列
	- `DWORD dwStyle` : ウィンドウスタイル（例 : WS_OVERLAPPEDWINDOW）
	- `int x, int y` : ウィンドウの初期位置
	- `int nWidth, int nHeight` : ウィンドウの幅・高さ
	- `HWND hWndParent` : 親ウィンドウのハンドル（トップレベルの場合はNULL）
	- `HMENU hMenu` : メニューハンドル（メニューが不要ならNULL）
	- `HINSTANCE hInstance` : インスタンスハンドル（`WinMain`の引数を渡す）
	- `LPVOID lpParam` : ウィンドウ作成時に渡す追加パラメータ（通常はNULL）
	*/

	windowRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	assert(
		AdjustWindowRectEx(
			&windowRect,
			WS_OVERLAPPEDWINDOW,
			FALSE,
			WS_EX_OVERLAPPEDWINDOW) != FALSE  // スクリーンボーダを考慮したウィンドウサイズの取得に成功
		&& "スクリーンボーダを考慮したウィンドウサイズの取得に失敗");

	LONG winW{ windowRect.right - windowRect.left };  // ウィンドウ幅
	LONG winH{ windowRect.bottom - windowRect.top };  // ウィンドウ高さ


	hWnd = CreateWindowW(
		szWindowClass,
		WIN_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		winW,
		winH,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	//{  // windowの作成
	//    DWORD     exWindowStyle{ WS_EX_OVERLAPPEDWINDOW };
	//    LPCWSTR    className{ L"Title" };
	//    LPCWSTR    windowName{ L"Windowname"};
	//    DWORD     windowStyle{ WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME };
	//    int       windowPositionX{ CW_USEDEFAULT };
	//    int       windowPositionY{ CW_USEDEFAULT };
	//    int       windowWidth{ windowRect.right - windowRect.left };
	//    int       windowHeight{ windowRect.bottom - windowRect.top };
	//    HWND      hWndParent{ nullptr };
	//    HMENU     hMenu{ nullptr };
	//    HINSTANCE hInstance{ hInst };
	//    LPVOID    param{ nullptr };
	//    hWnd = CreateWindowEx(
	//        exWindowStyle,
	//        className,
	//        windowName,
	//        windowStyle,
	//        windowPositionX,
	//        windowPositionY,
	//        windowWidth,
	//        windowHeight,
	//        hWndParent,
	//        hMenu,
	//        hInstance,
	//        param);
	//}
		assert(hWnd && "Window作成に失敗");

	/*if (!hWnd)
	{
	   return FALSE;
	}*/

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	return TRUE;
}

/*
関数の引数に関数を入れることで、関数を実行したときの内部で、引数に渡した関数を実行できる。
`WNDCLASSEX`でこれを設定することで、メッセージ受信時にこの関数が呼び出される。
ここで、各メッセージを処理していく。
ちなみに、
`return DefWindowProc(hWnd, message, wParam, lParam);`
を書かないとエラー出ないし、Window表示されないし、で大変なことになる。
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 選択されたメニューの解析:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: HDC を使用する描画コードをここに追加してください...
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		/*
		* ひたすらエラーがでます。
		*/
		//while (true)
		//{
		//    // BLOCK
		//    std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//    std::thread
		//    {
		//        []()
		//        {
		//            MessageBox(NULL, L"エラー", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL | MB_RIGHT);
		//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//        }
		//    }.detach();
		//}
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		if (Cursor::IsLock())
		{
			Cursor::AddMove(LOWORD(lParam), HIWORD(lParam));
		}
		else
		{
			Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
