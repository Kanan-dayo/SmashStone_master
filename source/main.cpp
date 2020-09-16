﻿//==================================================================================================================
// 
// メイン[main.cpp]
// Author : Seiya Takahashi
// 
//==================================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"

#define _CRTDBG_MAP_ALLOC
#include <time.h>
#include <stdlib.h>
#include <crtdbg.h>

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define CLASS_NAME	"WindowClass"				// ウィンドウクラスの名前
#define WINDOW_NAME	"SmashStone"				// ウィンドウの名前
#define ICON_NAME	"data/TEXTURE/Icon.ico"		// アイコンのファイル名

//==================================================================================================================
// プロトタイプ宣言
//==================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//==================================================================================================================
// グローバル変数
//==================================================================================================================
int g_nFpsCnt;

//==================================================================================================================
// 
// メイン関数
// 
//==================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	srand((unsigned int)time(NULL));
	_CrtDumpMemoryLeaks();											// アプリ終了時にメモリリークポイントを表示
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// リーク検出時に必要
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);				// リダイレクトやウィンドウ行き来を可能にする

	//_CrtSetBreakAlloc(1105);										// メモリリーク検出用（）内に番号を入力してブレイクポイントを発生させる

	CManager *pManager;
	pManager = new CManager;				// 動的確保

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,							// 表示するウィンドウのスタイルを設定
		WindowProc,							// ウィンドウプロシージャのアドレス（関数名）を指定
		0,									// 通常使用しないので0指定
		0,									// 通常使用しないので0指定
		hInstance,							// Windowsの引数のインスタンスハンドル指定
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),		// タスクバーに使用するアイコン指定
		LoadCursor(NULL, IDC_ARROW),		// 使用するマウスカーソル指定
		(HBRUSH)(COLOR_WINDOW + 1),			// ウィンドウクライアント領域の色収差
		NULL,								// メニューを指定
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))		// 小さいアイコンの指定
	};

	HWND hWnd;					// ウィンドウハンドル
	MSG msg;					// メッセージを格納する変数
	RECT rect =					// ウィンドウサイズ
	{ 0,	0, 
		SCREEN_WIDTH, SCREEN_HEIGHT };

	DWORD dwCurrentTime;		// 現在時刻
	DWORD dwExecLastTime;		// 最後に処理した時刻	
	DWORD dwFrameCount;			// 描画数をカウント
	DWORD dwFPSLastTime;		// FPSを最後に計測した時刻

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウインドウを作成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),	// キャプションサイズを差し引いたウィンドウサイズ
		(rect.bottom - rect.top),	// キャプションサイズを差し引いたウィンドウサイズ
		NULL,
		NULL,
		hInstance,
		NULL);
	
#ifdef _DEBUG
	// コンソールウィンドウの表示
	DispConsol();
#endif

	pManager->Init(hInstance,hWnd, TRUE);

	// ウィンドウの表示
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);
	timeBeginPeriod(1);					// 分解能の設定
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// 現在時間を取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();
	// メッセージループ（メッセージキューからメッセージを取得）
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				// ウィンドウプロシージャにメッセージを送る
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
		 // システム時間取得
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
#ifdef _DEBUG
				// FPSを測定
				g_nFpsCnt = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
#endif // _DEBUG
				// FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;
				// カウントをクリア
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 処理した時間で保存
				dwExecLastTime = dwCurrentTime;
				// 更新処理
				pManager->Update();
				// 描画処理
				pManager->Draw();
				dwFrameCount++;
			}
		}
	}

#ifdef _DEBUG
	// デバイスのクリーンアップ
	pManager->GetRenderer()->CleanupDeviceD3D();
#endif

	// マネージャがあるとき
	if (pManager != NULL)
	{
		// マネージャの終了処理
		pManager->Uninit();
		
		delete pManager;		// マネージャを消去
		pManager = NULL;		// NULLにする
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 値を返す
	return(int)msg.wParam;
}

//==================================================================================================================
// ウィンドウプロシージャ関数
//==================================================================================================================
// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
		// ウィンドウ破棄メッセージ
	case WM_DESTROY:
		// WM_qUITメッセージを返す
		PostQuitMessage(0);
		break;
		// ✖を押されたとき
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			// [ESC]キーが押された
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	default:
		break;
	}

	// 既定の処理を提供
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==================================================================================================================
// FPSの取得処理
//==================================================================================================================
int GetFPS(void)
{
	return g_nFpsCnt;
}

#ifdef _DEBUG
//==================================================================================================================
// コンソールウィンドウ出力
//==================================================================================================================
void DispConsol()
{
	// これでコンソールが表示される
	AllocConsole();

	// 変数宣言
	FILE *fp = NULL;

	// ストリーム先の変更 これがないと文字が表示されない
	freopen_s(&fp, "CONOUT$", "w", stdout);
}
#endif