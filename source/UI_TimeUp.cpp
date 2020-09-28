//==================================================================================================================
//
// KOのTimeUp演出 [ UI_TimeUp.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "UI_TimeUp.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "game.h"
#include "fade.h"
#include "kananlibrary.h"
#include "polygon2D.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define TIME_GATHER_ONEPLACE	(30)														// 1箇所に集まる時間
#define TIME_SHOW_UI			(60)														// 見せる時間
#define TIME_DISTRIBUTED		(30)														// 分散する時間

#define POS_GATHER_ONEPLACE		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))	// 集まる座標
#define SIZE_GATHER_ONEPLACE	(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))	// 集まるときのサイズ

//==================================================================================================================
// 静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUITimeUp::m_pTexture		= NULL;
char				*CUITimeUp::m_apFileName = "data/TEXTURE/time_up.png";
CPolygon2D			*CUITimeUp::m_pPolygon[TIMEUPUI_MAX]	= {};

D3DXVECTOR3			CUITimeUp::m_sizeBegin[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f)
};

D3DXVECTOR3			CUITimeUp::m_sizeEnd[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(500.0f, 250.0f, 0.0f),
	D3DXVECTOR3(500.0f, 250.0f, 0.0f),
	D3DXVECTOR3(500.0f, 250.0f, 0.0f)
};

D3DXVECTOR3			CUITimeUp::m_posBegin[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(120.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(1160.0f, SCREEN_HEIGHT / 2, 0.0f)
};

D3DXVECTOR3			CUITimeUp::m_posEnd[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(600.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(760.0f, SCREEN_HEIGHT / 2, 0.0f)
};

D3DXCOLOR   CUITimeUp::m_col[TIMEUPUI_MAX] =
{
	D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.0f),
	D3DXCOLOR(0.5f, 1.0f, 0.5f, 0.0f),
	D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.0f),
};

CUITimeUp::TIMEUP_STATE CUITimeUp::m_stateTimeUp = CUITimeUp::TIMEUPSTATE_GATHER;

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CUITimeUp::CUITimeUp()
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CUITimeUp::~CUITimeUp()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUITimeUp::Init(void)
{
	// 生成してテクスチャバインド
	CreateUI(TIMEUPUITEX_1);
	CreateUI(TIMEUPUITEX_2);
	CreateUI(TIMEUPUITEX_3);

	// 要素の初期化
	m_nCntAny  = 0;
	m_stateTimeUp = TIMEUPSTATE_GATHER;
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUITimeUp::Uninit(void)
{
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		// 無ければ処理しない
		if (!m_pPolygon[nCnt])
			continue;
		// 終了・破棄
		m_pPolygon[nCnt]->Uninit();
		delete m_pPolygon[nCnt];
		m_pPolygon[nCnt] = nullptr;
	}
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUITimeUp::Update(void)
{
	// 状態カウンタを加算
	m_nCntAny++;

	// 状態で処理を分ける
	switch (m_stateTimeUp)
	{
	case TIMEUPSTATE_GATHER: Gather(); break;			// 集まる
	case TIMEUPSTATE_SHOW: Show(); break;				// 見せる
	case TIMEUPSTATE_DISTRIBUTED: Distributed(); break;	// 分散する
	}

	// UIの更新
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUITimeUp::Draw(void)
{
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		// 無ければ処理しない
		if (!m_pPolygon[nCnt])
			continue;
		// 描画
		m_pPolygon[nCnt]->Draw();
	}
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CUITimeUp * CUITimeUp::Create(void)
{
	// メモリ確保
	CUITimeUp *pUI = new CUITimeUp;
	// 初期化
	pUI->Init();
	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CUITimeUp::Load(void)
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ブロックコメント
	CKananLibrary::StartBlockComment("TimeUp演出のテクスチャの読み込み開始");

	// テクスチャの読み込み
	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, m_apFileName, &m_pTexture)))
		std::cout << m_apFileName << " の読み込み成功" << std::endl;	// 成功
	else
		std::cout << m_apFileName << " の読み込み失敗" << std::endl;	// 失敗

// 成功
	CKananLibrary::EndBlockComment("TimeUp演出のテクスチャの読み込み終了");
	return S_OK;
}

//==================================================================================================================
// テクスチャ情報破棄
//==================================================================================================================
void CUITimeUp::Unload(void)
{
	// テクスチャがあるとき
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();	// テクスチャの開放
		m_pTexture = NULL;		// NULLにする
	}
}

//==================================================================================================================
// 集まる処理
//==================================================================================================================
void CUITimeUp::Gather(void)
{
	// UIの数だけ繰り返す
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		// 初期座標と目的地の差分
		D3DXVECTOR3 difPos = POS_GATHER_ONEPLACE - m_posBegin[nCnt];
		D3DXVECTOR3 difSize = SIZE_GATHER_ONEPLACE - m_sizeBegin[nCnt];
		D3DXCOLOR   difCol = WhiteColor - m_col[nCnt];
		// カラーの差分
		difCol.r /= TIME_GATHER_ONEPLACE;
		difCol.g /= TIME_GATHER_ONEPLACE;
		difCol.b /= TIME_GATHER_ONEPLACE;
		difCol.a /= TIME_GATHER_ONEPLACE;

		// 現在の情報を取得
		D3DXVECTOR3 pos = m_pPolygon[nCnt]->GetPos();
		D3DXVECTOR3 size = m_pPolygon[nCnt]->GetSize();
		D3DXCOLOR col = m_pPolygon[nCnt]->GetCol();

		// 段々近づける
		m_pPolygon[nCnt]->SetPos(pos + difPos / TIME_GATHER_ONEPLACE);
		m_pPolygon[nCnt]->SetSize(size + difSize / TIME_GATHER_ONEPLACE);
		m_pPolygon[nCnt]->SetCol(col + difCol);
	}

	if (m_nCntAny < TIME_GATHER_ONEPLACE)
		return;

	// 時間になったら状態変更
	m_nCntAny = 0;
	m_stateTimeUp = TIMEUPSTATE_SHOW;
}

//==================================================================================================================
// 見せる処理
//==================================================================================================================
void CUITimeUp::Show(void)
{
	// 一枚目以外消す
	if (m_pPolygon[TIMEUPUITEX_2]->GetbShow())
		m_pPolygon[TIMEUPUITEX_2]->SetbShow(false);
	if (m_pPolygon[TIMEUPUITEX_3]->GetbShow())
		m_pPolygon[TIMEUPUITEX_3]->SetbShow(false);

	if (m_nCntAny < TIME_SHOW_UI)
		return;

	// 時間になったら状態変更
	m_nCntAny = 0;
	m_stateTimeUp = TIMEUPSTATE_DISTRIBUTED;
}

//==================================================================================================================
// 分散する処理
//==================================================================================================================
void CUITimeUp::Distributed(void)
{
	// 全て表示
	if (!m_pPolygon[TIMEUPUITEX_2]->GetbShow())
		m_pPolygon[TIMEUPUITEX_2]->SetbShow(true);
	if (!m_pPolygon[TIMEUPUITEX_3]->GetbShow())
		m_pPolygon[TIMEUPUITEX_3]->SetbShow(true);

	// UIの数だけ繰り返す
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		// 初期座標と目的地の差分
		D3DXVECTOR3 difPos = m_posEnd[nCnt] - POS_GATHER_ONEPLACE;
		D3DXVECTOR3 difSize = m_sizeEnd[nCnt] - SIZE_GATHER_ONEPLACE;
		D3DXCOLOR   difCol = m_col[nCnt] - WhiteColor;
		// カラーの差分
		difCol.r /= TIME_DISTRIBUTED;
		difCol.g /= TIME_DISTRIBUTED;
		difCol.b /= TIME_DISTRIBUTED;
		difCol.a /= TIME_DISTRIBUTED;

		// 現在の座標取得
		D3DXVECTOR3 pos = m_pPolygon[nCnt]->GetPos();
		D3DXVECTOR3 size = m_pPolygon[nCnt]->GetSize();
		D3DXCOLOR col = m_pPolygon[nCnt]->GetCol();

		// 段々近づける
		m_pPolygon[nCnt]->SetPos(pos + difPos / TIME_DISTRIBUTED);
		m_pPolygon[nCnt]->SetSize(size + difSize / TIME_DISTRIBUTED);
		m_pPolygon[nCnt]->SetCol(col + difCol);
	}

	if (m_nCntAny < TIME_DISTRIBUTED)
		return;

	// 時間になったら、この処理を終了
	CGame::SetGameState(CGame::GAMESTATE_KO_AFTER);
}

//==================================================================================================================
// UIの生成
//==================================================================================================================
void CUITimeUp::CreateUI(int type)
{
	// 生成
	m_pPolygon[type] = CPolygon2D::Create();
	// テクスチャバインド
	m_pPolygon[type]->BindTexture(m_pTexture);
	// ポリゴンの初期設定
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetCol(m_col[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}
