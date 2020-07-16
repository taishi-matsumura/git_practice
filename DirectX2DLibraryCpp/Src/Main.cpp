#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

int g_playerHand = -1;
int g_enemyHand;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

const char* GetTextureName(int hand);

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

	srand((unsigned)time(NULL));
	g_enemyHand = rand() % 3;

	Engine::LoadTexture("rock",		"Res/janken_gu.png");
	Engine::LoadTexture("scisors",	"Res/janken_choki.png");
	Engine::LoadTexture("paper",	"Res/janken_pa.png");

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
void GameProcessing() {
	// 入力データの更新
	Engine::Update();


	//========================================================
	// キーボードの入力取得
	//========================================================

	if (Engine::IsKeyboardKeyPushed(DIK_0) == true) {
		g_playerHand = 0;
	} else if (Engine::IsKeyboardKeyPushed(DIK_1) == true) {
		g_playerHand = 1;
	} else if (Engine::IsKeyboardKeyPushed(DIK_2) == true) {
		g_playerHand = 2;
	}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// テクスチャ描画
	if (g_playerHand != -1) {
		//switch (g_playerHand) {
		//case 0:
		//	Engine::DrawTexture(200 - 208, 240 - 208, "rock", 255, 0.0F, 0.3F, 0.3F);
		//	break;

		//case 1:
		//	Engine::DrawTexture(200 - 211, 240 - 230, "scisors", 255, 0.0F, 0.3F, 0.3F);
		//	break;

		//case 2:
		//	Engine::DrawTexture(200 - 237, 240 - 218, "paper", 255, 0.0F, 0.3F, 0.3F);
		//	break;
		//}

		const char* textureName = GetTextureName(g_playerHand);
		Texture* textureInfo = Engine::GetTexture(textureName);
		int playerX = 200 - textureInfo->Width / 2;
		int playerY = 240 - textureInfo->Height / 2;
		Engine::DrawTexture(playerX, playerY, textureName, 255, 0.0F, 0.3F, 0.3F);


		//switch (g_enemyHand) {
		//case 0:
		//	Engine::DrawTexture(440 - 208, 240 - 208, "rock", 255, 0.0F, 0.3F, 0.3F);
		//	break;

		//case 1:
		//	Engine::DrawTexture(440 - 211, 240 - 230, "scisors", 255, 0.0F, 0.3F, 0.3F);
		//	break;

		//case 2:
		//	Engine::DrawTexture(440 - 237, 240 - 218, "paper", 255, 0.0F, 0.3F, 0.3F);
		//	break;
		//}

		textureName = GetTextureName(g_enemyHand);
		textureInfo = Engine::GetTexture(textureName);
		int enemyX = 440 - textureInfo->Width / 2;
		int enemyY = 240 - textureInfo->Height / 2;
		Engine::DrawTexture(enemyX, enemyY, textureName, 255, 0.0F, 0.3F, 0.3F);
	}

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

const char* GetTextureName(int hand) {
	switch (hand) {
	case 0:
		return "rock";
	case 1:
		return "scisors";
	case 2:
		return "paper";
	default:
		return NULL;
	}
}
