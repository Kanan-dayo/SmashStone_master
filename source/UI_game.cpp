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
#include "polygon2D.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define STONE_POS_Y 620								// 石位置縦
#define STONE_SIZE_X 60								// 石大きさX
#define STONE_SIZE_Y 50								// 石大きさY

#define TIME_END_ZOOM (20)							// ズームが終了する時間
#define POS_GAMEBG	(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))	// ゲーム背景の座標
#define SIZE_GAMEBG	(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))			// ゲーム背景のサイズ

#define SIZE_WINICON_BEGIN	(D3DXVECTOR3(300.0f, 300.0f, 0.0f))					// 勝利アイコンの最初のサイズ
#define SIZE_WINICON_END	(D3DXVECTOR3(70.0f, 70.0f, 0.0f))					// 勝利アイコンの最後のサイズ

#define UVSIZE_CHARAICON	(D3DXVECTOR2(0.25f, 1.0f))							// キャラアイコンのUVサイズ
#define UVSIZE_CHARANAME	(D3DXVECTOR2(1.0f, 0.25f))							// キャラ名のUVサイズ

//==================================================================================================================
// 静的メンバー変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_game::m_pTexBG					= NULL;		// テクスチャ情報
LPDIRECT3DTEXTURE9 CUI_game::m_pTexture[LOGOTYPE_MAX]	= {};		// テクスチャ情報
LPDIRECT3DTEXTURE9 CUI_game::m_pTexIcon[ICONTYPE_MAX]   = {};	
char CUI_game::m_FileBG[MAX_TEXT] = "data/TEXTURE/gameBG.png";		// ゲーム背景
char *CUI_game::m_apFileName[LOGOTYPE_MAX] =						// 読み込むモデルのソース先
{
	{ "data/TEXTURE/jewelryBG.png" },	// 宝石背景
	{ "data/TEXTURE/jewelryRed.png" },	// 宝石赤
	{ "data/TEXTURE/jewelryGreen.png" },// 宝石緑
	{ "data/TEXTURE/jewelryBule.png" },	// 宝石青
	{ "data/TEXTURE/FULLchara.png" },	// キャラアイコン
	{ "data/TEXTURE/charaName0.png" },	// キャラクターネーム
};

char *CUI_game::m_apFileIcon[ICONTYPE_MAX] = 
{
	{ "data/TEXTURE/winIcon/red.png" },		// 赤アイコン
	{ "data/TEXTURE/winIcon/green.png" },	// 緑アイコン
	{ "data/TEXTURE/winIcon/blue.png" }		// 青アイコン
};

CPolygon2D *CUI_game::m_pPolygon[MAX_PLAYER][LOGOTYPE_MAX] = {};	// プレイヤー関連の画像
CPolygon2D *CUI_game::m_pPolyIcon[ICONTYPE_MAX] = {};

D3DXVECTOR3 CUI_game::m_posUI[MAX_PLAYER][LOGOTYPE_MAX] =			// UIの座標
{
	{	
		D3DXVECTOR3(200, STONE_POS_Y, 0) ,
		D3DXVECTOR3(125, STONE_POS_Y, 0) ,
		D3DXVECTOR3(200, STONE_POS_Y, 0) ,
		D3DXVECTOR3(275, STONE_POS_Y, 0) ,
		D3DXVECTOR3(55, 70, 0) ,
		D3DXVECTOR3(205, 155, 0) 
	},
	{
		D3DXVECTOR3(1080, STONE_POS_Y, 0),
		D3DXVECTOR3(1005, STONE_POS_Y, 0),
		D3DXVECTOR3(1080, STONE_POS_Y, 0),
		D3DXVECTOR3(1155, STONE_POS_Y, 0),
		D3DXVECTOR3(1225, 70, 0),
		D3DXVECTOR3(1075, 155, 0)
	}
};

D3DXVECTOR3 CUI_game::m_sizeUI[MAX_PLAYER][LOGOTYPE_MAX] =			// UIのサイズ
{
	{
		D3DXVECTOR3(210, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(110, 130, 0.0f),
		D3DXVECTOR3(400, 50, 0.0f)
	},
	{
		D3DXVECTOR3(210, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(110, 130, 0.0f),
		D3DXVECTOR3(400, 50, 0.0f)
	},
};

D3DXVECTOR3 CUI_game::m_posIcon[MAX_PLAYER][MAX_WIN] =
{
	{
		D3DXVECTOR3(40.0f, 200.0f, 0.0f), D3DXVECTOR3(100.0f, 200.0f, 0.0f)
	},
	{
		D3DXVECTOR3(1240.0f, 200.0f, 0.0f), D3DXVECTOR3(1180.0f, 200.0f, 0.0f)
	}
};

bool CUI_game::m_bDisplay = true;
int CUI_game::m_nCntIcon = 0;
CUI_game::ICONSTATE_TYPE CUI_game::m_IconState[ICONTYPE_MAX] = {};

//==================================================================================================================
//
// コンストラクタ
//
//==================================================================================================================
CUI_game::CUI_game()
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
	m_nCntIcon = 0;
	m_nCntIconState = 0;

	for (int nCnt = 0; nCnt < ICONTYPE_MAX; nCnt++)
	{
		m_IconState[nCnt] = ICONSTATE_NONE;
	}

	// ゲーム開始時のUIをセット
	SetDefaultUI();
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUI_game::Uninit(void)
{
	// 終了
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
		{
			if (m_pPolygon[nCntPlayer][nCntLogo])
			{
				m_pPolygon[nCntPlayer][nCntLogo]->Uninit();
				delete m_pPolygon[nCntPlayer][nCntLogo];
				m_pPolygon[nCntPlayer][nCntLogo] = nullptr;
			}
		}
	}

	for (int nCnt = 0; nCnt < ICONTYPE_MAX; nCnt++)
	{
		if (m_pPolyIcon[nCnt])
		{
			m_pPolyIcon[nCnt]->Uninit();
			delete m_pPolyIcon[nCnt];
			m_pPolyIcon[nCnt] = nullptr;
		}
	}

	if (m_pPolyBG)
	{
		m_pPolyBG->Uninit();
		delete m_pPolyBG;
		m_pPolyBG = nullptr;
	}
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI_game::Update(void)
{
	if (m_pPolyBG)
		m_pPolyBG->Update();

	// 更新
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
		{
			if (m_pPolygon[nCntPlayer][nCntLogo])
				m_pPolygon[nCntPlayer][nCntLogo]->Update();
		}
	}

	for (int nCnt = 0; nCnt < ICONTYPE_MAX; nCnt++)
	{
		IconUpdate(nCnt);
	}
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI_game::Draw(void)
{
	// 表示しないなら、処理を終える
	if (!m_bDisplay)
		return;

	if (m_pPolyBG)
		m_pPolyBG->Draw();

	// 描画
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
		{
			if (m_pPolygon[nCntPlayer][nCntLogo])
				m_pPolygon[nCntPlayer][nCntLogo]->Draw();
		}
	}

	for (int nCnt = 0; nCnt < ICONTYPE_MAX; nCnt++)
	{
		if (m_pPolyIcon[nCnt])
			m_pPolyIcon[nCnt]->Draw();
	}
}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CUI_game *CUI_game::Create(void)
{
	// シーン動的に確保
	CUI_game *pUI = new CUI_game;

	// シーン初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//==================================================================================================================
// 勝利アイコンの生成
//==================================================================================================================
void CUI_game::CreateWinIcon(int nPlayer, int nWin)
{
	// 存在するなら、処理しない
	if (m_pPolyIcon[m_nCntIcon])
		return;

	// 生成
	m_pPolyIcon[m_nCntIcon] = CPolygon2D::Create();
	// テクスチャのバインド
	m_pPolyIcon[m_nCntIcon]->BindTexture(m_pTexIcon[m_nCntIcon]);
	// 座標設定
	m_pPolyIcon[m_nCntIcon]->SetPos(m_posIcon[nPlayer][nWin]);
	// サイズ設定
	m_pPolyIcon[m_nCntIcon]->SetSize(SIZE_WINICON_BEGIN);
	// ポリゴンの原点を設定
	m_pPolyIcon[m_nCntIcon]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
	// アイコンの状態を変更
	m_IconState[m_nCntIcon] = ICONSTATE_ZOOM;

	// 勝利アイコンのタイプを加算
	m_nCntIcon++;
}

//==================================================================================================================
// テクスチャロード
//==================================================================================================================
HRESULT CUI_game::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイスの取得

	//==============================テクスチャの読み込み==============================//
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
	}
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, m_FileBG, &m_pTexBG);

	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < ICONTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_apFileIcon[nCnt], &m_pTexIcon[nCnt]);
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
// 勝利アイコンの更新
//==================================================================================================================
void CUI_game::IconUpdate(int nIcon)
{
	// 存在しなければ、処理しない
	if (!m_pPolyIcon[nIcon])
		return;

	// 拡大中なら、拡大処理
	if (m_IconState[nIcon] == ICONSTATE_ZOOM)
		ZoomWinIcon(nIcon);

	// 更新
	m_pPolyIcon[nIcon]->Update();
}

//==================================================================================================================
// 勝利アイコンの拡大
//==================================================================================================================
void CUI_game::ZoomWinIcon(int nIcon)
{
	// 時間を加算
	m_nCntIconState++;

	// アイコンサイズの差
	D3DXVECTOR3 difSize = SIZE_WINICON_END - SIZE_WINICON_BEGIN;
	// 現在のサイズ
	D3DXVECTOR3 size = m_pPolyIcon[nIcon]->GetSize();

	m_pPolyIcon[nIcon]->SetSize(size + difSize / TIME_END_ZOOM);

	// 一定時間でズーム終了
	if (m_nCntIconState >= TIME_END_ZOOM)
	{
		m_nCntIconState = 0;
		m_IconState[nIcon] = ICONSTATE_SHOW;
	}
}

//==================================================================================================================
// ゲーム開始時のUIをセット
//==================================================================================================================
void CUI_game::SetDefaultUI(void)
{
	// ゲーム背景
	CreateBG();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// プレイヤーのタイプを取得
		int nCharaType= (int)CGame::GetPlayer(nCnt)->GetCharaType() / 2;

		// キャラアイコンを生成・UV設定
		CreateUI(nCnt, LOGOTYPE_PLAYERICON, m_posUI[nCnt][LOGOTYPE_PLAYERICON], m_sizeUI[nCnt][LOGOTYPE_PLAYERICON]);
		m_pPolygon[nCnt][LOGOTYPE_PLAYERICON]->SetAnim(D3DXVECTOR2(nCharaType * 0.25f, 0.0f), UVSIZE_CHARAICON);

		// キャラ名を生成・UV設定
		CreateUI(nCnt, LOGOTYPE_CHARANAME, m_posUI[nCnt][LOGOTYPE_CHARANAME], m_sizeUI[nCnt][LOGOTYPE_CHARANAME]);
		m_pPolygon[nCnt][LOGOTYPE_CHARANAME]->SetAnim(D3DXVECTOR2(0.0f, nCharaType * 0.25f), UVSIZE_CHARANAME);

		// キャラ名を生成・UV設定
		CreateUI(nCnt, LOGOTYPE_JEWELRYBG, m_posUI[nCnt][LOGOTYPE_JEWELRYBG], m_sizeUI[nCnt][LOGOTYPE_JEWELRYBG]);
	}
}

//==================================================================================================================
// ロゴ作成
//==================================================================================================================
void CUI_game::CreateUI(int nPlayer, int type, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 存在するなら、処理しない
	if (m_pPolygon[nPlayer][type])
		return;

	// 生成
	m_pPolygon[nPlayer][type] = CPolygon2D::Create();
	// テクスチャのバインド
	m_pPolygon[nPlayer][type]->BindTexture(m_pTexture[type]);
	// 座標設定
	m_pPolygon[nPlayer][type]->SetPos(pos);
	// サイズ設定
	m_pPolygon[nPlayer][type]->SetSize(size);
	// ポリゴンの原点を設定
	m_pPolygon[nPlayer][type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);

}

//==================================================================================================================
// ロゴ破棄
//==================================================================================================================
void CUI_game::DeleteUI(int nPlayer, int type)
{
	if (m_pPolygon[nPlayer][type])
	{
		// 終了・破棄
		m_pPolygon[nPlayer][type]->Uninit();
		delete m_pPolygon[nPlayer][type];
		m_pPolygon[nPlayer][type] = nullptr;
	}
}

//==================================================================================================================
// 背景の生成
//==================================================================================================================
void CUI_game::CreateBG()
{
	// 存在するなら、処理しない
	if (m_pPolyBG)
		return;

	// 生成
	m_pPolyBG = CPolygon2D::Create();
	// テクスチャのバインド
	m_pPolyBG->BindTexture(m_pTexBG);
	// 座標設定
	m_pPolyBG->SetPos(POS_GAMEBG);
	// サイズ設定
	m_pPolyBG->SetSize(SIZE_GAMEBG);
	// ポリゴンの原点を設定
	m_pPolyBG->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}

//==================================================================================================================
// ストーン取得
//==================================================================================================================
void CUI_game::CatchStone(int nPlayer, CStone::STONE_ID type)
{
	UI_GAME_TYPE StoneType = (UI_GAME_TYPE)(LOGOTYPE_JEWELRYRED + type);

	// 存在していれば、処理しない
	if (m_pPolygon[nPlayer][StoneType])
		return;

	// UI生成
	CreateUI(nPlayer, StoneType, m_posUI[nPlayer][StoneType], m_sizeUI[nPlayer][StoneType]);
}

//==================================================================================================================
// ストーン破棄
//==================================================================================================================
void CUI_game::ReleaseStone(int nPlayer, CStone::STONE_ID type)
{
	UI_GAME_TYPE StoneType = (UI_GAME_TYPE)(LOGOTYPE_JEWELRYRED + type);

	// 存在していなければ、処理しない
	if (!m_pPolygon[nPlayer][StoneType])
		return;

	// UI破棄
	DeleteUI(nPlayer, StoneType);
}

//==================================================================================================================
// 変身
//==================================================================================================================
void CUI_game::TransPlayer(int nPlayer)
{
	// 宝石背景・宝石を全破棄
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYBG])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYBG);
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYRED])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYRED);
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYGREEN])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYGREEN);
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYBULE])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYBULE);
}

//==================================================================================================================
// 変身終了
//==================================================================================================================
void CUI_game::FinishTrans(int nPlayer)
{
	// 宝石背景を生成
	CreateUI(nPlayer, LOGOTYPE_JEWELRYBG, m_posUI[nPlayer][LOGOTYPE_JEWELRYBG], m_sizeUI[nPlayer][LOGOTYPE_JEWELRYBG]);
}
