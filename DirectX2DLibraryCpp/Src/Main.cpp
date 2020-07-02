#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

int g_frameCount = 0;
int g_mashCount = 0;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

// カウントダウン
void CountDown();
// 開始メッセージを描画
void DrawStartMessage();
// 終了メッセージを描画
void DrawEndMessage();
// 連打回数を描画
void DrawMashCount();

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
			// 現在何フレーム目かをインクリメント
			g_frameCount++;

			// ゲーム処理
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
	if (g_frameCount < 300 || 630 <= g_frameCount) {
		return;
	}

	if (Engine::IsKeyboardKeyPushed(DIK_X) == true)
	{
		// キーが押された瞬間の処理
		g_mashCount++;
	}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// フォント描画
	if (g_frameCount < 180) {
		CountDown();
	} else if (g_frameCount < 300) {
		DrawStartMessage();
	} else if(630 <= g_frameCount) {
		DrawEndMessage();
	}

	DrawMashCount();

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

void CountDown()
{
	int second = g_frameCount / 60;
	int count = 3 - second;
	const int bufferSize = 1 + 1;
	char countDownString[bufferSize] = {};
	sprintf_s(countDownString, bufferSize, "%d", count);
	Engine::DrawFont(300.0f, 100.0f, countDownString, FontSize::Large, FontColor::White);
}

void DrawStartMessage() {
	Engine::DrawFont(200.0f, 100.0f, "連打スタート！！", FontSize::Large, FontColor::White);
}

void DrawEndMessage() {
	Engine::DrawFont(200.0f, 100.0f, "しゅーりょー", FontSize::Large, FontColor::White);
}

void DrawMashCount() {
	const int bufferSize = 32;
	char mashCountString[bufferSize] = {};
	sprintf_s(mashCountString, bufferSize, "連打回数： %d回", g_mashCount);
	Engine::DrawFont(250.0f, 180.0f, mashCountString, FontSize::Large, FontColor::White);
}

