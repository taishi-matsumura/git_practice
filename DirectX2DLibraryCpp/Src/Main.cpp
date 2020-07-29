#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

// =====================================================================
// 自作型
// =====================================================================
enum Phase {
	PHASE_START,
	PHASE_INPUT,
	PHASE_RESULT
};


// =====================================================================
// グローバル変数
// =====================================================================
static Phase g_Phase = PHASE_START;
static Vec2 g_Position = Vec2(0.0f, 0.0f);
static Vec2 g_Scale = Vec2(1.0f, 1.0f);
static float g_Angle = 0.0f;
static int g_playerHand = -1;
static int g_enemyHand;
static int g_FrameCount = 0;


// =====================================================================
// プロトタイプ宣言
// =====================================================================
// ゲーム処理
void GameProcessing();

// 描画処理
void DrawProcessing();

// テクスチャ名取得
const char* GetTextureName(int hand);

// タイトル描画
void DrawTitle();

// 入力受付画面描画
void DrawInput();

// 結果画面描画
void DrawResult();


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
			g_FrameCount++;

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
	if (g_Phase != PHASE_INPUT) {
		// 入力受付フェイズでなければ、キー入力無視
		return;
	}

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

	switch (g_Phase) {
	case PHASE_START:
		DrawTitle();
		break;

	case PHASE_INPUT:
		DrawInput();
		break;

	case PHASE_RESULT:
		DrawResult();
		break;
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

void DrawTitle() {
	Engine::DrawFont(0, 0, "ここでゲームの説明を表示する", FontSize::Large, FontColor::Red);
	if (g_FrameCount == 60 * 2) {
		g_Phase = PHASE_INPUT;
	}
}

void DrawInput() {
	if (g_playerHand != -1) {
		// プレイヤーの手を描画
		const char* textureName = GetTextureName(g_playerHand);
		Texture* textureInfo = Engine::GetTexture(textureName);
		float playerX = 200 - textureInfo->Width / 2;
		float playerY = 240 - textureInfo->Height / 2;
		Engine::DrawTexture(playerX, playerY, textureName, 255, 0.0F, 0.3F, 0.3F);

		// CPUの手を描画
		textureName = GetTextureName(g_enemyHand);
		textureInfo = Engine::GetTexture(textureName);
		int enemyX = 440 - textureInfo->Width / 2;
		int enemyY = 240 - textureInfo->Height / 2;
		Engine::DrawTexture(enemyX, enemyY, textureName, 255, 0.0F, 0.3F, 0.3F);
	}
}

void DrawResult() {
}
