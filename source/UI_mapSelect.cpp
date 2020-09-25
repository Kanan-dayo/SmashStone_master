//==================================================================================================================
//
// UI_mapSelect処理[UI_mapSelect.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_mapSelect.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "scene2D.h"
#include "kananlibrary.h"
#include "inputGamepad.h"
#include "sound.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define MAPUI_SIZE_X 600							// マップUI大きさX
#define MAPUI_SIZE_Y 400							// マップUI大きさY
#define MAPUI_POS_X0 310							// マップUI縦の位置0列目
#define MAPUI_POS_X1 950							// マップUI縦の位置1列目
#define MAPUI_POS_Y0 360							// マップUI横の位置0列目
#define MAPUI_POS_Y1 500							// マップUI横の位置1列目
#define MAPFRAMEUI_SIZE_X 640						// マップ枠線大きさX
#define MAPFRAMEUI_SIZE_Y 440						// マップ枠線大きさY

//==================================================================================================================
// 静的メンバー変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_mapSelect::m_pTexture[LOGOTYPE_MAX] = {};		// テクスチャ情報
char *CUI_mapSelect::m_apFileName[LOGOTYPE_MAX] =						// 読み込むモデルのソース先
{
	{ "data/TEXTURE/worldMap.jpg" },	// 世界地図
	{ "data/TEXTURE/MapFrame.jpg" },	// マップ選択枠
	{ "data/TEXTURE/map1.jpg" },		// マップ1
	{ "data/TEXTURE/map2.jpg" },		// マップ2
};

int CUI_mapSelect::m_nMapID = 0;	// マップID

//==================================================================================================================
//
// コンストラクタ
//
//==================================================================================================================
CUI_mapSelect::CUI_mapSelect(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// デストラクタ
//
//==================================================================================================================
CUI_mapSelect::~CUI_mapSelect()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI_mapSelect::Init(void)
{
	m_nMapID = 0;				// マップ番号
	m_bMapSelect = false;		// マップを選択したかどうか

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_nCharaNum[nCnt] = 0;	// キャラクター番号
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
void CUI_mapSelect::Uninit(void)
{

}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI_mapSelect::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド変数
	CInputGamepad *pGamepad[MAX_PLAYER];

	// 最大人数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// ゲームパッド取得
		pGamepad[nCnt] = CManager::GetInputGamepad(nCnt);
	}

	// UIマップ選択画面更新処理
	MapSelectUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI_mapSelect::Draw(void)
{

}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CUI_mapSelect *CUI_mapSelect::Create(void)
{
	// シーン動的に確保
	CUI_mapSelect *pUI = new CUI_mapSelect(CScene::PRIORITY_UI);

	// シーン初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャロード
//==================================================================================================================
HRESULT CUI_mapSelect::Load(void)
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
void CUI_mapSelect::Unload(void)
{
	// ロゴの最大種類までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// 開放
		m_pTexture[nCnt] = NULL;			// NULLにする
	}
}

//==================================================================================================================
// マップ選択画面の更新処理
//==================================================================================================================
void CUI_mapSelect::MapSelectUpdate(CInputKeyboard * pKeyboard, CInputGamepad * pGamepad0, CInputGamepad * pGamepad1)
{
	// 世界地図
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_WORLDMAP, WhiteColor);

	// マップ1
	SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP1, WhiteColor);

	// マップ2
	SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP2, WhiteColor);

	// ゲームパッド有効時
	if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
		// ゲームパッド操作
		ControlGamepad(pGamepad0, pGamepad1);
	// ゲームパッド無効時
	else
		// キーボード操作
		ControlKeyboard(pKeyboard);
}

//==================================================================================================================
// ゲームパッド操作
//==================================================================================================================
void CUI_mapSelect::ControlGamepad(CInputGamepad * pGamepad0, CInputGamepad *pGamepad1)
{
	float fValueX0, fValueY0 = 0;	// ゲームパッドのスティック情報の取得用
	float fValueX1, fValueY1 = 0;	// ゲームパッドのスティック情報の取得用

	// 左スティック取得
	pGamepad0->GetStickLeft(&fValueX0, &fValueY0);
	pGamepad1->GetStickLeft(&fValueX1, &fValueY1);

	//// 何も入力されていなければ、処理しない
	//if (FAILED(CKananLibrary::GetMoveByGamepad(pGamepad0)) &&
	//	fValueX0 == 0 && fValueY0 == 0 &&
	//	FAILED(CKananLibrary::GetMoveByGamepad(pGamepad1)) &&
	//	fValueX1 == 0 && fValueY1 == 0)
	//{
	//	return;
	//}

	// マップが選択されていないとき
	if (!m_bMapSelect)
	{
		// 左に傾けたとき
		if (fValueX0 < 0)
		{
			// マップ番号1減算
			m_nMapID -= 1;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (fValueX0 > 0)
		{// 右に傾けたとき
		 // マップ番号1加算
			m_nMapID += 1;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// マップIDが0より小さい時
		if (m_nMapID < 0)
		{
			// 0にする
			m_nMapID = 0;
		}

		// まっぷIDが3より大きい時
		if (m_nMapID > 1)
		{
			// 3にする
			m_nMapID = 1;
		}

		switch (m_nMapID)
		{
		case 0:
			// マップ1
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		case 1:
			// マップ2
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		}
		// 決定ボタンを押したとき
		if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_X))
		{
			// マップを選択した状態にする
			m_bMapSelect = true;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
}
//==================================================================================================================
// キーボード処理
//==================================================================================================================
void CUI_mapSelect::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// マップが選択されていないとき
	if (!m_bMapSelect)
	{
		// 1Pが左入力をしたとき
		if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
		{
			// マップID小さくなる
			m_nMapID -= 1;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
		{// 1Pが右入力をしたとき
		 // マップID大きくなる
			m_nMapID += 1;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// マップIDが0より小さい時
		if (m_nMapID < 0)
		{
			// 0にする
			m_nMapID = 0;
		}

		// まっぷIDが3より大きい時
		if (m_nMapID > 1)
		{
			// 3にする
			m_nMapID = 1;
		}

		switch (m_nMapID)
		{
		case 0:
			// マップ1
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		case 1:
			// マップ2
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		}

		// 1Pが決定ボタンを押したとき
		if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
		{
			// マップを選んだ状態にする
			m_bMapSelect = true;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
}

//==================================================================================================================
// ロゴ作成
//==================================================================================================================
void CUI_mapSelect::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// 位置設定
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// 大きさ設定
	m_pScene2D[nCnt]->SetCol(col);									// 色設定
}
