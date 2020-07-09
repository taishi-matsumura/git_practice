#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;
DWORD g_StartTime = 0;
DWORD g_ElapsedTime = 0;
int g_MashCount = 0;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (Engine::Initialize(640, 480, "Sample") == false)
	{
		return 0;
	}

	g_StartTime = timeGetTime();

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 連打ゲームを作る
			// ・最初にカウントダウンを表示
			// ・カウントダウンが終わったらキー入力の受付を開始
			// ・現在何発連打されているかを表示
			// ・現在何発連打されているかを表示
			// ・一定時間経過したら終了（終了した旨を表示、連打した回数はそのまま残す）

			g_ElapsedTime = timeGetTime() - g_StartTime;

			// 入力に対する処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	Engine::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;
}
void GameProcessing()
{
	// 入力データの更新
	Engine::Update();


	//========================================================
	// キーボードの入力取得
	//========================================================

	if (3000 <= g_ElapsedTime && g_ElapsedTime < 13000) {
		if (Engine::IsKeyboardKeyPushed(DIK_Z) == true) {
			// キーが押された瞬間の処理
			g_MashCount++;

		}
	}

	//if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	//{
	//	// キーが押されている間の処理
	//}

	//if (Engine::IsKeyboardKeyReleased(DIK_A))
	//{
	//	// キーが離された瞬間の処理
	//}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	if (g_ElapsedTime < 3000) {
		int countDownTime = 3 - (g_ElapsedTime / 1000);

		char countDownTimeString[2];
		sprintf_s(countDownTimeString, 2, "%d", countDownTime);
		Engine::DrawFont(300.0f, 200.0f, countDownTimeString, FontSize::Large, FontColor::White);
	} else if (g_ElapsedTime < 4000) {
		Engine::DrawFont(250.0f, 200.0f, "Start!!", FontSize::Large, FontColor::White);
	} else if (g_ElapsedTime < 11000) {
		Engine::DrawFont(250.0f, 200.0f, "がんばれ～", FontSize::Large, FontColor::White);
	} else if (g_ElapsedTime < 13000) {
		Engine::DrawFont(250.0f, 200.0f, "あと少し！！", FontSize::Large, FontColor::White);
	} else {
		Engine::DrawFont(250.0f, 200.0f, "しゅーりょー", FontSize::Large, FontColor::White);
	}

	char mashCountString[32];
	sprintf_s(mashCountString, 32, "連打回数：%d回", g_MashCount);
	Engine::DrawFont(200.0f, 260.0f, mashCountString, FontSize::Large, FontColor::White);


	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}
