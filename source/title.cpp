//==================================================================================================================
//
// titleの描画 [title.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS			// 警告回避文

//==================================================================================================================
//	インクルードファイル
//==================================================================================================================
#include "title.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "sound.h"
#include "scene2D.h"
#include "scene3D.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "character.h"
#include "inputGamepad.h"
#include "meshSphere.h"
#include "UI_title.h"

//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define MAX_FADE_COUNT 800			// フェードののカウントの最大数

//==================================================================================================================
//	静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;		// テクスチャ情報
CCamera *CTitle::m_pCamera = NULL;					// カメラ情報
CLight *CTitle::m_pLight = NULL;					// ライト情報
CMeshField *CTitle::m_pMeshField = NULL;			// メッシュフィールド情報
CMeshSphere *CTitle::m_pMeshSphere = NULL;			// メッシュ球情報
CUI_title *CTitle::m_pUI = NULL;					// UI情報

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CTitle::CTitle()
{

}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CTitle::~CTitle()
{

}

//==================================================================================================================
//	初期化処理
//==================================================================================================================
void CTitle::Init(void)
{
	CMeshField::Load();			// メッシュフィールドロード
	CMeshSphere::Load();		// メッシュ球のテクスチャロード
	CUI_title::Load();			// UIのテクスチャロード

	// ライトの生成処理
	m_pLight = CLight::Create();

	// カメラの生成処理
	m_pCamera = CCamera::Create();

	// メッシュ球の生成処理
	m_pMeshSphere = CMeshSphere::Create();

	// メッシュフィールド生成処理
	m_pMeshField = CMeshField::Create(INTEGER2(10, 10), D3DXVECTOR3(250.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// UIの生成処理
	m_pUI = CUI_title::Create();
}

//==================================================================================================================
//	終了処理
//==================================================================================================================
void CTitle::Uninit(void)
{
	// scene2Dの破棄処理
	CScene2D::ReleaseAll();

	// scene3Dの破棄処理
	CScene3D::ReleaseAll();

	CMeshField::Unload();			// 床テクスチャアンロード
	CUI_title::Unload();			// UIテクスチャアンロード

	delete m_pLight;				// メモリ削除
	m_pLight = nullptr;				// ポインタNULL

	delete m_pCamera;				// メモリ削除
	m_pCamera = nullptr;			// ポインタNULL
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CTitle::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard = nullptr;

	// ゲームパッド変数
	CInputGamepad *pInputGamepad[MAX_PLAYER];

	// 最大人数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// ゲームパッド取得
		pInputGamepad[nCnt] = CManager::GetInputGamepad(nCnt);
	}
	if (!pInputGamepad[PLAYER_ONE]->GetbConnect() ||
		!pInputGamepad[PLAYER_TWO]->GetbConnect())
		pInputKeyboard = CManager::GetInputKeyboard();

	// フェード取得
	CFade::FADE fade = CFade::GetFade();

	// カメラの更新処理
	m_pCamera->Update();

	// ライトの更新処理
	m_pLight->Update();

	// キーボードの[Enter] 又は コントローラーの[START]を押したとき
	if ((pInputGamepad[PLAYER_ONE]->GetTrigger(CInputGamepad::JOYPADKEY_START)) ||
		(pInputGamepad[PLAYER_TWO]->GetTrigger(CInputGamepad::JOYPADKEY_START)) ||
		(pInputKeyboard && pInputKeyboard->GetKeyboardTrigger(DIK_RETURN)))
	{
		// 効果音再生
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		// フェードが何もない時
		if (fade == CFade::FADE_NONE)
		{
			// フェードの設定
			CFade::SetFade(CRenderer::MODE_TUTORIAL, DEFAULT_FADE_TIME);
		}
	}
}

//==================================================================================================================
//	描画処理
//==================================================================================================================
void CTitle::Draw(void)
{
	// カメラがあるとき
	if (m_pCamera != NULL)
	{
		m_pCamera->SetCamera();			// カメラの設定
		m_pCamera->Draw();				// カメラの描画処理
	}
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CTitle * CTitle::Create(void)
{
	CTitle *pTitle = NULL;			// チュートリアル情報をNULLにする

	pTitle = new CTitle;			// 動的に確保
	pTitle->Init();					// 初期化処理

	return pTitle;					// 値を返す
}

//==================================================================================================================
// カメラ情報取得
//==================================================================================================================
CCamera * CTitle::GetCamera(void)
{
	return m_pCamera;
}
