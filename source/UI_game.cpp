//==================================================================================================================
//
// UI_game処理[UI_game.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_game.h"
#include "renderer.h"
#include "player.h"
#include "scene2D.h"
#include "kananlibrary.h"
#include "sound.h"
#include "game.h"
#include "inputKeyboard.h"
#include "debugProc.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define STONE_POS_Y 620								// 石位置縦
#define STONE_SIZE_X 60								// 石大きさX
#define STONE_SIZE_Y 50								// 石大きさY

//==================================================================================================================
// 静的メンバー変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_game::m_pTexture[LOGOTYPE_MAX] = {};		// テクスチャ情報
char *CUI_game::m_apFileName[LOGOTYPE_MAX] =					// 読み込むモデルのソース先
{
	{ "data/TEXTURE/gameBG.png" },		// ゲーム背景
	{ "data/TEXTURE/jewelryBG.png" },	// 宝石背景1P
	{ "data/TEXTURE/jewelryBG.png" },	// 宝石背景2P
	{ "data/TEXTURE/jewelryRed.png" },	// 宝石赤
	{ "data/TEXTURE/jewelryBule.png" },	// 宝石青
	{ "data/TEXTURE/jewelryGreen.png" },// 宝石緑
	{ "data/TEXTURE/FULLchara.png" },	// キャラ1P
	{ "data/TEXTURE/FULLchara.png" },	// キャラ2P
	{ "data/TEXTURE/charaName0.png" },	// 1Pのキャラクターネーム
	{ "data/TEXTURE/charaName1.png" },	// 2Pのキャラクターネーム
};

//==================================================================================================================
//
// コンストラクタ
//
//==================================================================================================================
CUI_game::CUI_game(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// デストラクタ
//
//==================================================================================================================
CUI_game::~CUI_game()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI_game::Init(void)
{
	m_bDisplay = true;		// UIを表示するかどうか

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_nCharaNum[nCnt] = 0;	// キャラクター番号
		m_bTransform[nCnt] = false;// 変身したかどうか

		for (int nCntType = 0; nCntType < CStone::STONE_ID_MAX; nCntType++)
		{
			m_bStoneID[nCnt][nCntType] = false;// ストーンを取得したかどうか
		}
	}

	// ロゴの最大枚数カウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// 生成処理
		m_pScene2D[nCnt] = CScene2D::Create();

		// テクスチャを貼る
		m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
	}
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUI_game::Uninit(void)
{

}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI_game::Update(void)
{
	// キーボード取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// UIゲーム更新処理
	GameUpdate();

#ifdef _DEBUG
	// 表示しているとき
	if (m_bDisplay)
	{
		// キーボードのF3を押したとき
		if (pKeyboard->GetKeyboardTrigger(DIK_F3))
		{
			// 表示させないようにする
			m_bDisplay = false;
		}
		CDebugProc::Print("UI非表示[F3]\n");

	}
	else
	{// 表示していないとき
		// キーボードのF3を押したとき
		if (pKeyboard->GetKeyboardTrigger(DIK_F3))
		{
			// 表示させるようにする
			m_bDisplay = true;
		}
		CDebugProc::Print("UI表示[F3]\n");
	}

#endif // DEBUG

}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI_game::Draw(void)
{

}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CUI_game *CUI_game::Create(void)
{
	// シーン動的に確保
	CUI_game *pUI = new CUI_game(CScene::PRIORITY_UI);

	// シーン初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャロード
//==================================================================================================================
HRESULT CUI_game::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得

	//==============================テクスチャの読み込み==============================//
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
	}

	// 値を返す
	return S_OK;
}

//==================================================================================================================
// 破棄
//==================================================================================================================
void CUI_game::Unload(void)
{
	// ロゴの最大種類までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// 開放
		m_pTexture[nCnt] = NULL;			// NULLにする
	}
}

//==================================================================================================================
// ゲーム更新処理
//==================================================================================================================
void CUI_game::GameUpdate(void)
{
	CPlayer *pPlayer0, *pPlayer1;

	// プレイヤー情報取得
	pPlayer0 = CGame::GetPlayer(0);
	pPlayer1 = CGame::GetPlayer(1);

	// プレイヤーが2体いるとき
	if (pPlayer0 != NULL && pPlayer1 != NULL)
	{
		// プレイヤー番号取得
		m_nCharaNum[0] = pPlayer0->GetCharaType();
		m_nCharaNum[1] = pPlayer1->GetCharaType();

		// 変身状況取得
		m_bTransform[0] = pPlayer0->GetTrans();
		m_bTransform[1] = pPlayer1->GetTrans();

		// 最大石までカウント
		for (int nCntStone = 0; nCntStone < CStone::STONE_ID_MAX; nCntStone++)
		{
			// 各プレイヤーがどの種類の石を獲得したか
			m_bStoneID[0][nCntStone] = pPlayer0->GetStoneType(nCntStone);
			m_bStoneID[1][nCntStone] = pPlayer1->GetStoneType(nCntStone);
		}
	}

	// ゲーム背景
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_GAMEBG, WhiteColor, m_bDisplay);

	if (!m_bTransform[0])
	{
		// 宝石背景1P
		SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG1P, WhiteColor, m_bDisplay);

		// 1Pが赤石を取ったとき
		if (m_bStoneID[0][0])
		{
			// 宝石赤
			SetUI(D3DXVECTOR3(125, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, WhiteColor, m_bDisplay);
		}

		// 1Pが緑石を取ったとき
		if (m_bStoneID[0][1])
		{
			// 宝石青
			SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, WhiteColor, m_bDisplay);
		}

		// 1Pが青石を取ったとき
		if (m_bStoneID[0][2])
		{
			// 宝石緑
			SetUI(D3DXVECTOR3(275, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, WhiteColor, m_bDisplay);
		}
	}
	else
	{
		// 宝石背景1P
		SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG1P, AlphaColor, m_bDisplay);
		// 宝石赤
		SetUI(D3DXVECTOR3(125, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, AlphaColor, m_bDisplay);
		// 宝石青
		SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, AlphaColor, m_bDisplay);
		// 宝石緑
		SetUI(D3DXVECTOR3(275, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, AlphaColor, m_bDisplay);
	}

	if (!m_bTransform[1])
	{
		// 宝石背景2P
		SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG2P, WhiteColor, m_bDisplay);

		// 2Pが赤石を取ったとき
		if (m_bStoneID[1][0])
		{
			// 宝石赤
			SetUI(D3DXVECTOR3(1005, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, WhiteColor, m_bDisplay);
		}

		// 2Pが緑石を取ったとき
		if (m_bStoneID[1][1])
		{
			// 宝石青
			SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, WhiteColor, m_bDisplay);
		}

		// 2Pが青石を取ったとき
		if (m_bStoneID[1][2])
		{
			// 宝石緑
			SetUI(D3DXVECTOR3(1155, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, WhiteColor, m_bDisplay);
		}
	}
	else
	{
		// 宝石背景2P
		SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG2P, AlphaColor, m_bDisplay);
		// 宝石赤
		SetUI(D3DXVECTOR3(1005, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, AlphaColor, m_bDisplay);
		// 宝石青
		SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, AlphaColor, m_bDisplay);
		// 宝石緑
		SetUI(D3DXVECTOR3(1155, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, AlphaColor, m_bDisplay);
	}

	// 1PキャラクターUI
	SetUI(D3DXVECTOR3(55, 70, 0), 110, 130, LOGOTYPE_PLAYER1, WhiteColor, m_bDisplay);
	// テクスチャ設定
	m_pScene2D[LOGOTYPE_PLAYER1]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0] / 2);
	// 2PキャラクターUI
	SetUI(D3DXVECTOR3(1225, 70, 0), 110, 130, LOGOTYPE_PLAYER2, WhiteColor, m_bDisplay);
	// テクスチャ設定
	m_pScene2D[LOGOTYPE_PLAYER2]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1] / 2);

	// 1PキャラクターネームUI
	SetUI(D3DXVECTOR3(205, 155, 0), 400, 50, LOGOTYPE_CHARANAME0, WhiteColor, m_bDisplay);
	// テクスチャ設定
	m_pScene2D[LOGOTYPE_CHARANAME0]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[0] / 2) * 0.25f, 0.0f + (m_nCharaNum[0] / 2) * 0.25f, 0);
	// 2PキャラクターネームUI
	SetUI(D3DXVECTOR3(1075, 155, 0), 400, 50, LOGOTYPE_CHARANAME1, WhiteColor, m_bDisplay);
	// テクスチャ設定
	m_pScene2D[LOGOTYPE_CHARANAME1]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[1] / 2) * 0.25f, 0.0f + (m_nCharaNum[1] / 2) * 0.25f, 0);
}

//==================================================================================================================
// ロゴ作成
//==================================================================================================================
void CUI_game::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col, bool bDisplay)
{
	m_pScene2D[nCnt]->SetPos(pos);									// 位置設定
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// 大きさ設定

	// 表示していいとき
	if (m_bDisplay)
	{
		m_pScene2D[nCnt]->SetCol(col);								// 色設定
	}
	else
	{// 表示させないとき
		m_pScene2D[nCnt]->SetCol(ZeroColor);						// 色設定
	}
}
