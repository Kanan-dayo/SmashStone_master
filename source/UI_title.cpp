//==================================================================================================================
//
// UI_title処理[UI_title.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_title.h"
#include "renderer.h"
#include "scene2D.h"
#include "kananlibrary.h"
#include "sound.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define TITLEUI_BEGIN_X 1760						// タイトルUI最初の横の大きさ
#define TITLEUI_BEGIN_Y 1600						// タイトルUI最初の縦の大きさ
#define TITLEUI_SMALL_SPEED 30						// タイトルUIの小さくする速度
#define TITLEUI_VALUE_Y	700							// 減少サイズの値Y
#define TITLEUI_BOUND_SPEED 10						// タイトルUIのバウンド速度
#define TITLEUI_BOUND_COUNT 60						// タイトルUIバウンドカウンタ
#define TITLEUI_FINISH_Y 250						// タイトルUIの最後の位置Y
#define TITLEUI_UP_SPEED 2							// タイトルUI上がる速度
#define ENTERUI_SIZE_X 950							// エンターUI大きさ横
#define ENTERUI_SIZE_Y 80							// エンターUI大きさ縦
#define ENTERUI_POS_Y 600							// エンターUI位置Y
#define ENTERUI_ALPHA 0.03f							// エンターUIα値変更値

//==================================================================================================================
// 静的メンバー変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_title::m_pTexture[LOGOTYPE_MAX] = {};		// テクスチャ情報
char *CUI_title::m_apFileName[LOGOTYPE_MAX] =						// 読み込むモデルのソース先
{
	{ "data/TEXTURE/title.png" },		// タイトル
	{ "data/TEXTURE/PressStart.png" },	// エンター
};

//==================================================================================================================
//
// コンストラクタ
//
//==================================================================================================================
CUI_title::CUI_title(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// デストラクタ
//
//==================================================================================================================
CUI_title::~CUI_title()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI_title::Init(void)
{
	TitlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// タイトルの位置
	m_nCntBound = 0;			// タイトルUIバウンドカウンタ
	m_fCntUITitle0 = 0;			// タイトルUI用カウンタ0
	m_fCntUITitle1 = 0;			// タイトルUI用カウンタ1
	m_fCntEnter = 0;			// エンター用カウンタ
	m_bUITitle0 = false;		// タイトルを動かすかどうか
	m_bUITitle1 = false;		// タイトルを動かすかどうか
	m_bUIEnter = false;			// エンターのα値用変数

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
void CUI_title::Uninit(void)
{

}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI_title::Update(void)
{
	// UIタイトルの更新処理
	TitleUpdate();
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI_title::Draw(void)
{

}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CUI_title *CUI_title::Create(void)
{
	// シーン動的に確保
	CUI_title *pUI = new CUI_title(CScene::PRIORITY_UI);

	// シーン初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャロード
//==================================================================================================================
HRESULT CUI_title::Load(void)
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
void CUI_title::Unload(void)
{
	// ロゴの最大種類までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// 開放
		m_pTexture[nCnt] = NULL;			// NULLにする
	}
}

//==================================================================================================================
// タイトル更新処理
//==================================================================================================================
void CUI_title::TitleUpdate(void)
{
	// タイトルUI
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + TitlePos.y, 0.0f), TITLEUI_BEGIN_X + m_fCntUITitle0 - m_fCntUITitle1,
		TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1, LOGOTYPE_TITLE, WhiteColor);

	// バウンドカウントが規定値以下のとき
	if (m_nCntBound < TITLEUI_BOUND_COUNT)
	{
		// タイトルを動かしていなとき
		if (!m_bUITitle0)
		{
			// タイトルUIの縦の長さが規定値以下のとき
			if (TITLEUI_BEGIN_Y + m_fCntUITitle0 <= TITLEUI_VALUE_Y)
			{
				// タイトルを動かす状態にする
				m_bUITitle0 = true;

				// タイトルUI縦幅を規定値にする
				m_fCntUITitle0 = TITLEUI_VALUE_Y - TITLEUI_BEGIN_Y;
			}
			else
			{
				// タイトルカウンタ減算
				m_fCntUITitle0 -= TITLEUI_SMALL_SPEED;
			}
		}
		else
		{// タイトルを動かしていいとき
		 // タイトルUIの縦の長さが[370]以上のとき
			if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 >= TITLEUI_VALUE_Y + 40)
			{
				// タイトルを最大まで拡大させた
				m_bUITitle1 = true;

			}
			else if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 <= TITLEUI_VALUE_Y - 40)
			{// タイトルUIの縦の長さが[330]以下のとき
			 // タイトルを最小まで拡小させた
				m_bUITitle1 = false;
			}

			// タイトルを最大まで拡大させたとき
			if (m_bUITitle1)
			{
				// タイトルカウンタ加算
				m_fCntUITitle1 -= TITLEUI_BOUND_SPEED;
			}
			else
			{// タイトルを最小まで拡小させたとき
			 // タイトルカウンタ減算
				m_fCntUITitle1 += TITLEUI_BOUND_SPEED;
			}

			// タイトルUIバウンドカウンタ加算
			m_nCntBound++;
		}
	}
	else
	{// バウンドカウントが規定値を超えたとき
	 // タイトルUIの位置Yが規定値以下のとき
		if (SCREEN_HEIGHT / 2 + TitlePos.y < TITLEUI_FINISH_Y)
		{
			// エンターUI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, ENTERUI_POS_Y, 0.0f), ENTERUI_SIZE_X, ENTERUI_SIZE_Y,
				LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f + m_fCntEnter));

			// エンターUIのα値が1.0以上のとき
			if (1.0f + m_fCntEnter >= 1.0f)
			{
				// エンター用α値を減らす状態にする
				m_bUIEnter = true;
			}
			else if (1.0f + m_fCntEnter <= 0.0f)
			{
				// エンター用α値を増やす状態にする
				m_bUIEnter = false;
			}

			// エンターUIを減らす状態のとき
			if (m_bUIEnter)
			{
				// カウンタ減算
				m_fCntEnter -= ENTERUI_ALPHA;
			}
			else
			{// エンターUIを増やす状態のとき
			 // カウンタ加算
				m_fCntEnter += ENTERUI_ALPHA;
			}
		}
		else
		{
			// タイトル位置加算
			TitlePos.y -= TITLEUI_UP_SPEED;
		}
	}
}

//==================================================================================================================
// ロゴ作成
//==================================================================================================================
void CUI_title::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// 位置設定
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// 大きさ設定
	m_pScene2D[nCnt]->SetCol(col);									// 色設定
}
