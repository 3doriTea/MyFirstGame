// MyFirstGame.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "MyFirstGame.h"
#include <thread>
#include <cassert>
#include <chrono>

#include "Direct3D.h"

#include "Camera.h"
#include "Quad.h"
#include "Cube.h"
#include "Sprite2D.h"

HWND hWnd{ nullptr };

#define MAX_LOADSTRING 100

const wchar_t* WIN_CLASS_NAME{ L"△プルGameうぃん同" };
// ウィンドウ幅
const int SCREEN_WIDTH{ 800 };
const int SCREEN_HEIGHT{ 600 };

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名


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

	Camera::Initialize();

	MSG msg{};

	/*
		アプリケーションのメインループで、`GetMessage`や`PeekMessage`を呼ぶことで、メッセージを取得できる。
		`TranslateMessage`でキーボードの生入力を、キー文字として変換、
		`DispatchMessage`でWindowプロシージャを実行し、メッセージを送りつける。
	*/

	Cube* quad{ new Cube{} };
	hResult = quad->Initialize();

	if (FAILED(hResult))
	{
		SAFE_DELETE(quad);
		Direct3D::Release();
		return FALSE;
	}

	Sprite2D* pSprite{ new Sprite2D{} };
	hResult = pSprite->Initialize("Sushi512x512.png");

	if (FAILED(hResult))
	{
		SAFE_DELETE(pSprite);
		Direct3D::Release();
		return FALSE;
	}

	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))

		{

			TranslateMessage(&msg);

			DispatchMessage(&msg);

		}
		//メッセージなし
		else
		{
			Camera::Update();  // カメラ更新のタイミングはここが望ましい!!

			Direct3D::Instance().BeginDraw();
			//ゲームの処理

			//描画処理

			static float angle{};
			angle += XMConvertToRadians(0.1f);
			if (angle > XM_2PI * 2.0f)
				angle -= XM_2PI * 2.0f;

			XMMATRIX mat{};
			mat = XMMatrixIdentity();
			mat *= XMMatrixRotationY(angle) * XMMatrixRotationX(angle / 2.0f);
			float scalingAngle{ angle };
			scalingAngle = 0.1f;
			mat *= DirectX::XMMatrixScaling(
				std::fabsf(std::cosf(scalingAngle) * 4),
				std::fabsf(std::cosf(scalingAngle) * 3),
				std::fabsf(std::cosf(scalingAngle) * 2));
			quad->Draw(mat);

			mat = XMMatrixIdentity();
			mat *= XMMatrixRotationY(angle / 2.0f) * XMMatrixRotationX(angle / 1.0f);
			mat *= DirectX::XMMatrixScaling(
				std::fabsf(std::cosf(scalingAngle) * 2),
				std::fabsf(std::cosf(scalingAngle) * 1),
				std::fabsf(std::cosf(scalingAngle) * 3));
			quad->Draw(mat);

			const Vector2Int imageSize{ pSprite->GetSize() };
			pSprite->Draw(
				{ 0, 0, (Direct3D::ScreenSize() / 2) },
				angle,
				{
					static_cast<int>(std::fabsf(std::sinf(angle)) * (imageSize.x / 2)),
					static_cast<int>(std::fabsf(std::sinf(angle)) * (imageSize.y / 2)),
					static_cast<int>(std::fabsf(std::cosf(angle)) * imageSize.x),
					static_cast<int>(std::fabsf(std::cosf(angle)) * imageSize.y)
				});

			Direct3D::Instance().EndDraw();
			//描画処理

		}

	}

	SAFE_RELEASE(pSprite);
	SAFE_DELETE(pSprite);

	SAFE_RELEASE(quad);
	SAFE_DELETE(quad);

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

	RECT windowRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
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
