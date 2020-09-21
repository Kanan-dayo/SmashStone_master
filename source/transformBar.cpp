//==================================================================================================================
//
// 変身バーの描画 [transformBar.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================

//==================================================================================================================
//	インクルードファイル
//==================================================================================================================
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "transformBar.h"
#include "bar.h"
#include "player.h"
#include "character.h"
#include "debugProc.h"
#include "game.h"
#include "UI_game.h"

//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define BAR_HP_HEIGHT 110		// HPバーの高さ
#define BAR_HPFRAME_HEIGHT 163	// HPフレームの高さ
#define NORMAL_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 普通の色
#define BAR_WIDTH 160	// バー横の長さ
#define BAR_HEGTH 30	// バー縦の長さ

//==================================================================================================================
//	静的メンバ変数の初期化
//==================================================================================================================
CPlayer *CTransformBar::m_pPlayer = NULL;		// プレイヤーの情報
int CTransformBar::m_nInitCnt = 0;				// 初期化用カウンタ

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CTransformBar::CTransformBar()
{
	m_pBar = NULL;				// バーの情報ポインタ
}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CTransformBar::~CTransformBar(void)
{
	m_nInitCnt = 0;				// 初期化用カウンタ初期化
}

//==================================================================================================================
//	初期化処理
//==================================================================================================================
void CTransformBar::Init(void)
{
	// 初期化
	m_pos = D3DXVECTOR3(0, 0, 0);	// 位置
	m_fNowHP = m_fMaxHP;			// 現在のHP
	m_fHeight = BAR_WIDTH;			// 高さ

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_bTrans[nCnt] = false;	// 変身しているかどうか
	}

	// Barの生成
	m_pBar = CBar::Create();

	// Barがあるとき
	if (m_pBar != NULL)
	{
		// 初期化用カウンタが0のとき
		if (m_nInitCnt == 0)
		{
			// Barフレーム
			m_pBar->SetVertexBarW(10, D3DXVECTOR3(100, 620, 0), ZeroColor, 160, 35);
			// BarHP
			m_pBar->SetVertexBarW(11, D3DXVECTOR3(47, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
		}
		else if (m_nInitCnt == 1)
		{// 初期化用カウンタが1のとき
			// Barフレーム
			m_pBar->SetVertexBarWL(12, D3DXVECTOR3(1080, 620, 0), ZeroColor, 160, 35);
			// BarHP
			m_pBar->SetVertexBarWL(13, D3DXVECTOR3(1027, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
		}
	}

	// 初期化用カウンタ加算
	m_nInitCnt++;
}

//==================================================================================================================
//	終了処理
//==================================================================================================================
void CTransformBar::Uninit(void)
{
	// バーがあるとき
	if (m_pBar != NULL)
	{
		//Barの終了処理
		m_pBar->Uninit();
	}

	delete m_pBar;					// メモリ削除
	m_pBar = nullptr;				// ポインタNULL
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CTransformBar::Update(void)
{
	// Barの更新処理
	m_pBar->Update();

	CPlayer *pPlayer0, *pPlayer1;
	pPlayer0 = CGame::GetPlayer(0);
	pPlayer1 = CGame::GetPlayer(1);

	// 変身しているかどうか
	m_bTrans[0] = pPlayer0->GetTrans();
	m_bTrans[1] = pPlayer1->GetTrans();

	// 変身しているとき
	if (m_bTrans[0])
	{
		// プレイヤーの情報ポインタ取得
		m_pPlayer = CGame::GetPlayer(0);
		float fHeight = 0.0f;									// 現在の画像の高さ
		float NowHP = m_fMaxHP - m_pPlayer->GetTransCnt();		// 現在のHP
		m_fNowHP = NowHP / m_fMaxHP;							// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// Barフレーム
		m_pBar->SetVertexBarW(10, D3DXVECTOR3(50, 620, 0), NORMAL_COLOR, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarW(11, D3DXVECTOR3(153, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), fHeight, BAR_HEGTH);
	}
	else
	{
		// Barフレーム
		m_pBar->SetVertexBarW(10, D3DXVECTOR3(50, 620, 0), ZeroColor, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarW(11, D3DXVECTOR3(153, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
	}

	// 2Pが変身中のとき
	if (m_bTrans[1])
	{
		// プレイヤーの情報ポインタ取得
		m_pPlayer = CGame::GetPlayer(1);
		float fHeight = 0.0f;									// 現在の画像の高さ
		float NowHP = m_fMaxHP - m_pPlayer->GetTransCnt();		// 現在のHP
		m_fNowHP = NowHP / m_fMaxHP;							// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// Barフレーム
		m_pBar->SetVertexBarWL(12, D3DXVECTOR3(1230, 620, 0), NORMAL_COLOR, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarWL(13, D3DXVECTOR3(1127, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), fHeight, BAR_HEGTH);
	}
	else
	{
		// Barフレーム
		m_pBar->SetVertexBarWL(12, D3DXVECTOR3(1230, 620, 0), ZeroColor, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarWL(13, D3DXVECTOR3(1127, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
	}
}

//==================================================================================================================
//	描画処理
//==================================================================================================================
void CTransformBar::Draw(void)
{
	// 表示していない
	if (!CUI_game::GetbDisp())
	{
		return;
	}

	//Barの描画処理
	m_pBar->Draw();
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CTransformBar * CTransformBar::Create(float fMaxHP)
{
	CTransformBar *pTransformBar = NULL;				// ポインタ変数

	pTransformBar = new CTransformBar();				// メモリを動的確保
	pTransformBar->m_fMaxHP = fMaxHP;				// プレイヤー最大HP代入
	pTransformBar->Init();							// 初期化処理

	return pTransformBar;							// 値を返す
}

//==================================================================================================================
//	位置の設定
//==================================================================================================================
void CTransformBar::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================
//	プレイヤー番号設定処理
//==================================================================================================================
void CTransformBar::SetnPlayerNum(int nPlayerNum)
{
	m_nPlayerNum = nPlayerNum;
}

//==================================================================================================================
//	位置の取得
//==================================================================================================================
D3DXVECTOR3 CTransformBar::GetPos(void)
{
	return m_pos;
}
