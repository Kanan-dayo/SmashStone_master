//==================================================================================================================
//
// UI_tutorial処理[UI_tutorial.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_tutorial.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "scene2D.h"
#include "debugProc.h"
#include "tutorial.h"
#include "kananlibrary.h"
#include "inputGamepad.h"
#include "sound.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define CLOCKUI0_POS_X 290							// 時計UI位置X
#define CLOCKUI1_POS_X 990							// 時計UI位置X
#define CLOCKHANDSUI_WAITTIME 30					// 時計の針UI待機時間
#define CHARATEX_SISE_X 350							// キャラテクスチャ大きさX
#define CHARATEX_SISE_Y 450							// キャラテクスチャ大きさY
#define CHARAUI_POS_Y 600							// 全員キャラUI位置Y
#define CHARAFULLUI_SIZE_X 720						// 全員キャラUI大きさX
#define CHARAFULLUI_SIZE_Y 200						// 全員キャラUI大きさY
#define CHARAFREAMUI_SIZE_X 180						// キャラUI大きさX
#define CHARA1PUI_POS D3DXVECTOR3(200, 230, 0)		// 1PキャラUIの位置
#define CHARA2PUI_POS D3DXVECTOR3(1080, 230, 0)		// 2PキャラUIの位置
#define GEAR_POS_Y 530								// 歯車の位置Y
#define CLOCK_HANDS_DIFF 0.1f						// 回転の初期値
#define READY1PUI_POS D3DXVECTOR3(200, 450, 0)		// 1P準備完了位置
#define READY2PUI_POS D3DXVECTOR3(1080, 450, 0)		// 2P準備完了位置
#define READYUI_SIZE_X 200							// 準備完了大きさX
#define READYUI_SIZE_Y 160							// 準備完了大きさX
#define CHARA_SELECTUI_POS D3DXVECTOR3(640, 60, 0)	// キャラクター選択UI位置
#define CHARA_SELECTUI_SIZE_X 400					// キャラクター選択UI大きさX
#define CHARA_SELECTUI_SIZE_Y 120					// キャラクター選択UI大きさY
#define MAPUI_SIZE_X 300							// マップUI大きさX
#define MAPUI_SIZE_Y 250							// マップUI大きさY
#define MAPUI_POS_X0 650							// マップUI縦の位置0列目
#define MAPUI_POS_X1 1050							// マップUI縦の位置1列目
#define MAPUI_POS_Y0 200							// マップUI横の位置0列目
#define MAPUI_POS_Y1 500							// マップUI横の位置1列目
#define MAPFRAMEUI_SIZE_X 360						// マップ枠線大きさX
#define MAPFRAMEUI_SIZE_Y 290						// マップ枠線大きさY
#define MAPEXPLANATION_POS D3DXVECTOR3(250, SCREEN_HEIGHT / 2, 0)// マップ説明UI位置
#define MAPEXPLANATION_SIZE_X 400					// マップ説明UI大きさX
#define MAPEXPLANATION_SIZE_Y 600					// マップ説明UI大きさY
#define STONE_POS_Y 620								// 石位置縦
#define STONE_SIZE_X 60								// 石大きさX
#define STONE_SIZE_Y 50								// 石大きさY

//==================================================================================================================
// 静的メンバー変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_tutorial::m_pTexture[UITYPE_MAX] = {};		// テクスチャ情報
char *CUI_tutorial::m_apFileName[UITYPE_MAX] =						// 読み込むモデルのソース先
{
	{ "data/TEXTURE/gear.png" },		// 歯車0
	{ "data/TEXTURE/clock.png" },		// 時計土台0
	{ "data/TEXTURE/Clock hands.png" },	// 時計の針0
	{ "data/TEXTURE/clock gear.png" },	// 時計の歯車0
	{ "data/TEXTURE/gear.png" },		// 歯車1
	{ "data/TEXTURE/clock.png" },		// 時計土台1
	{ "data/TEXTURE/Clock hands.png" },	// 時計の針1
	{ "data/TEXTURE/clock gear.png" },	// 時計の歯車1
	{ "data/TEXTURE/FULLcharaBG.jpg" },	// キャラクター全員背景
	{ "data/TEXTURE/FULLchara.png" },	// キャラクター全員
	{ "data/TEXTURE/FULLcharaBG.jpg" },	// キャラクター1P背景
	{ "data/TEXTURE/FULLcharaBG.jpg" },	// キャラクター2P背景
	{ "data/TEXTURE/FULLchara.png" },	// 1Pキャラクター
	{ "data/TEXTURE/FULLchara.png" },	// 2Pキャラクター
	{ "data/TEXTURE/1Pchara.png" },		// 1Pキャラクター枠
	{ "data/TEXTURE/2Pchara.png" },		// 2Pキャラクター枠
	{ "data/TEXTURE/charaName.png" },	// 1Pキャラクターネーム
	{ "data/TEXTURE/charaName.png" },	// 2Pキャラクターネーム
	{ "data/TEXTURE/Ready.png" },		// 1Pキャラクター準備完了
	{ "data/TEXTURE/Ready.png" },		// 2Pキャラクター準備完了
	{ "data/TEXTURE/PlayerSelect.png" },// プレイヤーセレクトアイコン
};

//==================================================================================================================
//
// コンストラクタ
//
//==================================================================================================================
CUI_tutorial::CUI_tutorial(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// デストラクタ
//
//==================================================================================================================
CUI_tutorial::~CUI_tutorial()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI_tutorial::Init(void)
{
	m_nCharaNum[0] = 0;			// キャラ番号
	m_nCharaNum[1] = 3;			// キャラ番号
	m_fPos[0] = 370;			// 現在の枠線テクスチャの位置X
	m_fPos[1] = 910;			// 現在の枠線テクスチャの位置X

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_nCntRot[nCnt] = 0;	// 時計の針の回転用カウンタ
		m_nCntWait[nCnt] = 0;	// 待機時間用カウンタ
		m_fPosMove[nCnt] = 0;	// 位置移動変数
		m_fPosDiff[nCnt] = 0;	// 目標の枠線テクスチャの位置X
		m_fPosOld[nCnt] = 0;	// 前回の枠線テクスチャの位置X
		m_fRotGear[nCnt] = 0;	// 歯車の回転格納変数
		m_fPosCul[nCnt] = 0;	// 位置計算用変数
		m_fDiff[nCnt] = 0;		// 1フレーム前との距離
		m_fAngle[nCnt] = 0;		// 歯車の回転角度
		m_fRad[nCnt] = 0;		// ラジアン値
		m_bUIClockHands[nCnt] = false;// 時計の針が動いたかどうか0
		m_bCharaDecide[nCnt] = false;// 自分のキャラクターを選択したかどうか
		m_bStickReturn[nCnt] = false;// パッドスティックを戻したかどうか
	}

	// ロゴの最大枚数カウント
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
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
void CUI_tutorial::Uninit(void)
{

}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI_tutorial::Update(void)
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

	// UIチュートリアル更新処理
	TutorialUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI_tutorial::Draw(void)
{

}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CUI_tutorial *CUI_tutorial::Create(void)
{
	// シーン動的に確保
	CUI_tutorial *pUI = new CUI_tutorial(CScene::PRIORITY_UI);

	// シーン初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャロード
//==================================================================================================================
HRESULT CUI_tutorial::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得

	//==============================テクスチャの読み込み==============================//
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
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
void CUI_tutorial::Unload(void)
{
	// ロゴの最大種類までカウント
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// 開放
		m_pTexture[nCnt] = NULL;			// NULLにする
	}
}

//==================================================================================================================
// チュートリアル更新処理
//==================================================================================================================
void CUI_tutorial::TutorialUpdate(CInputKeyboard * pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1)
{
	// キャラクター全員UI
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, CHARAUI_POS_Y, 0.0f), CHARAFULLUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_CHARAFULL, WhiteColor);
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, CHARAUI_POS_Y, 0.0f), CHARAFULLUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_CHARAFULLBG, WhiteColor);

	// ゲームパッド有効時
	if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
		// ゲームパッド操作
		ControlGamepad(pGamepad0, pGamepad1);
	// ゲームパッド無効時
	else
		// キーボード操作
		ControlKeyboard(pKeyboard);

	// 時計土台0UI
	SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCK0, WhiteColor);
	// 時計の針0UI
	SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKHANDS0, WhiteColor);
	// 時計の針回転設定
	m_pScene2D[UITYPE_CLOCKHANDS0]->SetRot(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0), -D3DX_PI / 6 - m_nCntRot[0] * CLOCK_HANDS_DIFF, ZeroVector3);
	// 時計の針0UI
	SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKGEAR0, WhiteColor);

	// 時計の針が動いていい状態のとき
	if (m_bUIClockHands[0])
	{
		// 時計の針が最大回転値より大きいとき
		if ((-D3DX_PI / 6) * 5 < (-D3DX_PI / 6) - m_nCntRot[0] * CLOCK_HANDS_DIFF)
		{
			// 回転させていく
			m_nCntRot[0]++;
		}
		else
		{
			// 待機時間加算
			m_nCntWait[0]++;

			// 待機時間が0.5秒経ったとき
			if (m_nCntWait[0] >= CLOCKHANDSUI_WAITTIME)
			{
				m_bUIClockHands[0] = false;
			}
		}
	}
	else
	{
		// 待機時間用カウンタ初期化
		m_nCntWait[0] = 0;

		// 回転させていく
		m_nCntRot[0]--;

		// 回転用カウンタが[0]以下になったとき
		if (m_nCntRot[0] <= 0)
		{
			// 回転用カウンタ初期化
			m_nCntRot[0] = 0;
		}
	}

	// 時計土台1UI
	SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCK1, WhiteColor);
	// 時計の針1UI
	SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKHANDS1, WhiteColor);
	m_pScene2D[UITYPE_CLOCKHANDS1]->SetRot(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0), D3DX_PI / 6 + m_nCntRot[1] * CLOCK_HANDS_DIFF, ZeroVector3);
	// 時計の針1UI
	SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKGEAR1, WhiteColor);

	// 時計の針が動いていい状態のとき
	if (m_bUIClockHands[1])
	{
		// 時計の針が最大回転値より大きいとき
		if ((D3DX_PI / 6) * 5 > (D3DX_PI / 6) + m_nCntRot[1] * CLOCK_HANDS_DIFF)
		{
			// 回転させていく
			m_nCntRot[1]++;
		}
		else
		{
			// 待機時間加算
			m_nCntWait[1]++;

			// 待機時間が0.5秒経ったとき
			if (m_nCntWait[1] >= CLOCKHANDSUI_WAITTIME)
			{
				m_bUIClockHands[1] = false;
			}
		}
	}
	else
	{
		// 待機時間用カウンタ初期化
		m_nCntWait[1] = 0;

		// 回転させていく
		m_nCntRot[1]--;

		// 回転用カウンタが[0]以下になったとき
		if (m_nCntRot[1] <= 0)
		{
			// 回転用カウンタ初期化
			m_nCntRot[1] = 0;
		}
	}

	// 最大人数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// 現在の位置を前回の位置に代入
		m_fPosOld[nCnt] = m_fPos[nCnt];

		// キャラ番号が0より小さくなったとき
		if (m_nCharaNum[nCnt] < 0)
		{
			// 0に戻す
			m_nCharaNum[nCnt] = 0;
		}

		// キャラ番号が3より大きくなったとき
		if (m_nCharaNum[nCnt] > 3)
		{
			// 3に戻す
			m_nCharaNum[nCnt] = 3;
		}
		// 目標位置格納変数
		m_fPosDiff[nCnt] = 370 + (float)m_nCharaNum[nCnt] * CHARAFREAMUI_SIZE_X;
	}

	// 最大人数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		float fRadius = CHARAFULLUI_SIZE_Y / 2;											// 歯車テクスチャの半径
		m_fPosCul[nCnt] = ((m_fPosDiff[nCnt] - m_fPos[nCnt]) / 60) * m_fPosMove[nCnt];	// 計算用変数
		m_fDiff[nCnt] = fabsf(m_fPos[nCnt] + m_fPosCul[nCnt] - m_fPosOld[nCnt]);		// 1フレーム後の距離
		m_fAngle[nCnt] = 180 * m_fDiff[nCnt] / (D3DX_PI * fRadius);						// 角度算出
		m_fRad[nCnt] = D3DX_PI * m_fAngle[nCnt] / 360;					// ラジアン値

		// 位置の差が正のとき
		if (m_fPosCul[nCnt] > 0)
		{
			m_fRotGear[nCnt] = m_fRotGear[nCnt] + m_fRad[nCnt];					// 角度格納(加算)
		}
		else if (m_fPosCul[nCnt] < 0)
		{// 位置の差が負のとき
			m_fRotGear[nCnt] = m_fRotGear[nCnt] - m_fRad[nCnt];					// 角度格納(減算)
		}
	}

	// 1Pキャラクター枠線UI
	SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_1PCHARA_FREAM, WhiteColor);
	// 歯車0UI
	SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, UITYPE_GEAR0, WhiteColor);
	// 回転設定
	m_pScene2D[UITYPE_GEAR0]->SetRot(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0), m_fRotGear[0], ZeroVector3);

	// 2Pキャラクター枠線UI
	SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_2PCHARA_FREAM, WhiteColor);
	// 歯車1UI
	SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, UITYPE_GEAR1, WhiteColor);
	// 回転設定
	m_pScene2D[UITYPE_GEAR1]->SetRot(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0), m_fRotGear[1], ZeroVector3);

	// 最大人数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// 位置が同じじゃないとき
		if (m_fPosDiff[nCnt] != m_fPos[nCnt])
		{
			// 位置移動用カウンタ加算
			m_fPosMove[nCnt]++;
		}

		// 現在の位置設定
		m_fPos[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

		// 現在の位置を前回の位置に代入
		m_fPosOld[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

	}

	// 1PキャラクターUI
	SetUI(CHARA1PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_1PCHARA, WhiteColor);
	// テクスチャ設定
	m_pScene2D[UITYPE_1PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0]);
	// 1PキャラクターUI
	SetUI(CHARA1PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_1PCHARABG, WhiteColor);
	// テクスチャ設定
	m_pScene2D[UITYPE_1PCHARABG]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0]);
	// 2PキャラクターUI
	SetUI(CHARA2PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_2PCHARA, WhiteColor);
	// テクスチャ設定
	m_pScene2D[UITYPE_2PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1]);
	// 2PキャラクターUI
	SetUI(CHARA2PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_2PCHARABG, WhiteColor);
	// テクスチャ設定
	m_pScene2D[UITYPE_2PCHARABG]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1]);

	// 1PキャラクターネームUI
	SetUI(READY1PUI_POS, 400, 120, UITYPE_1PCHARA_NAME, WhiteColor);
	// テクスチャ設定
	m_pScene2D[UITYPE_1PCHARA_NAME]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[0]) * 0.25f, 0.0f + (m_nCharaNum[0]) * 0.25f, 0);
	// 2PキャラクターネームUI
	SetUI(READY2PUI_POS, 400, 120, UITYPE_2PCHARA_NAME, WhiteColor);
	// テクスチャ設定
	m_pScene2D[UITYPE_2PCHARA_NAME]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[1]) * 0.25f, 0.0f + (m_nCharaNum[1]) * 0.25f, 0);

	// 1Pキャラクター選択されているとき
	if (m_bCharaDecide[0])
	{
		// 1Pキャラクター準備完了
		SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_1PREADY, WhiteColor);
	}
	else
	{// 1Pキャラクター選択されていないとき
	 // 1Pキャラクター準備完了
		SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_1PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// 2Pキャラクター選択されているとき
	if (m_bCharaDecide[1])
	{
		// 2PキャラクターUI
		SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_2PREADY, WhiteColor);
	}
	else
	{// 2Pキャラクター選択されていないとき
	 // 2Pキャラクター準備完了
		SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_2PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// キャラクター選択アイコン
	SetUI(CHARA_SELECTUI_POS, CHARA_SELECTUI_SIZE_X, CHARA_SELECTUI_SIZE_Y, UITYPE_SELECTICON, WhiteColor);
}

//==================================================================================================================
// ゲームパッド操作
//==================================================================================================================
void CUI_tutorial::ControlGamepad(CInputGamepad * pGamepad0, CInputGamepad *pGamepad1)
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

	// 自分のキャラクターを選択してないとき
	if (!m_bCharaDecide[0])
	{
		// スティックを戻しているとき
		if (m_bStickReturn[0])
		{
			// 左に傾けたとき
			if (fValueX0 < 0)
			{
				// キャラ番号減算
				m_nCharaNum[0] -= 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[0] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[0] = 0;

				// スティックを戻したかどうか
				m_bStickReturn[0] = false;

				// 効果音再生
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (fValueX0 > 0)
			{// 右に傾けたとき
			 // キャラ番号加算
				m_nCharaNum[0] += 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[0] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[0] = 0;

				// スティックを戻したかどうか
				m_bStickReturn[0] = false;

				// 効果音再生
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// 決定ボタンを押したとき
			if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_B))
			{
				// キャラクターを選択した状態にする
				m_bCharaDecide[0] = true;
				// 効果音再生
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}

		// スティック角度0のとき
		if (fValueX0 == 0)
		{
			// スティックを戻している状態にする
			m_bStickReturn[0] = true;
		}
	}
	else
	{// 1Pが自分のキャラクターが選択されているとき
	 // 戻るボタンを押したとき
		if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_A))
		{
			// キャラクターを選択していない状態にする
			m_bCharaDecide[0] = false;
		}
	}


	// 2Pのキャラクターが選ばれていないとき
	if (!m_bCharaDecide[1])
	{
		// スティックを戻しているとき
		if (m_bStickReturn[1])
		{
			// 左に傾けたとき
			if (fValueX1 < 0)
			{
				// キャラ番号減算
				m_nCharaNum[1] -= 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[1] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[1] = 0;

				// スティックが戻っていない状態にする
				m_bStickReturn[1] = false;

				// 効果音再生
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (fValueX1 > 0)
			{
				// キャラ番号加算
				m_nCharaNum[1] += 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[1] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[1] = 0;

				// スティックが戻っていない状態にする
				m_bStickReturn[1] = false;

				// 効果音再生
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// 決定ボタンを押したとき
			if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_B))
			{
				// キャラクターを選択した状態にする
				m_bCharaDecide[1] = true;
				// 効果音再生
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}

		// スティックが傾いていないとき
		if (fValueX1 == 0)
		{
			// スティックが戻っている状態にする
			m_bStickReturn[1] = true;
		}
	}
	else
	{// 2Pが自分のキャラクターを選択しているとき
	 // 戻るボタンを押したとき
		if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_A))
		{
			// キャラクターを選択していない状態にする
			m_bCharaDecide[1] = false;
		}
	}
}
//==================================================================================================================
// キーボード処理
//==================================================================================================================
void CUI_tutorial::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// 自分のキャラクターを選択してないとき
	if (!m_bCharaDecide[0])
	{
		// キーボードの[A]が押されたとき
		if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
		{
			// キャラ番号減算
			m_nCharaNum[0] -= 1;

			// 時計の針が動いていい状態にする
			m_bUIClockHands[0] = true;

			// 位置移動用カウンタ初期化
			m_fPosMove[0] = 0;

			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
		{// キーボードの[D]が押されたとき
		 // キャラ番号加算
			m_nCharaNum[0] += 1;

			// 時計の針が動いていい状態にする
			m_bUIClockHands[0] = true;

			// 位置移動用カウンタ初期化
			m_fPosMove[0] = 0;

			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// 1Pが決定ボタンをおしたとき
		if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
		{
			// キャラクターを選択した状態にする
			m_bCharaDecide[0] = true;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
	else
	{// 1Pが自分のキャラクターを選択しているとき
	 // 1Pが決定ボタンをおしたとき
		if (pKeyboard->GetKeyboardTrigger(ONE_JUMP))
		{
			// キャラクターを選択していない状態にする
			m_bCharaDecide[0] = false;
		}
	}

	// 2Pのキャラクターが選ばれていないとき
	if (!m_bCharaDecide[1])
	{
		// キーボードの[←]が押されたとき
		if (pKeyboard->GetKeyboardTrigger(TWO_LEFT))
		{
			// キャラ番号減算
			m_nCharaNum[1] -= 1;

			// 時計の針が動いていい状態にする
			m_bUIClockHands[1] = true;

			// 位置移動用カウンタ初期化
			m_fPosMove[1] = 0;

			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(TWO_RIGHT))
		{// キーボードの[→]が押されたとき
		 // キャラ番号加算
			m_nCharaNum[1] += 1;

			// 時計の針が動いていい状態にする
			m_bUIClockHands[1] = true;

			// 位置移動用カウンタ初期化
			m_fPosMove[1] = 0;

			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// 2Pが決定ボタンをおしたとき
		if (pKeyboard->GetKeyboardTrigger(TWO_ATTACK))
		{
			// キャラクターを選んだ状態にする
			m_bCharaDecide[1] = true;
			// 効果音再生
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
	else
	{// 2Pが自分のキャラクターを選択しているとき
	 // 2Pが決定ボタンをおしたとき
		if (pKeyboard->GetKeyboardTrigger(TWO_JUMP))
		{
			// キャラクターを選択していない状態にする
			m_bCharaDecide[1] = false;
		}
	}
}

//==================================================================================================================
// ロゴ作成
//==================================================================================================================
void CUI_tutorial::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// 位置設定
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// 大きさ設定
	m_pScene2D[nCnt]->SetCol(col);									// 色設定
}
