//==================================================================================================================
//
// UI_game[UI_game.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_GAME_H_
#define _UI_GAME_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "scene.h"
#include "stone.h"

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CScene2D;

//==================================================================================================================
//
// ロゴクラスの定義
//
//==================================================================================================================
class CUI_game : public CScene
{
public:
	//=============================================================================
	// 列挙型定義
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_GAMEBG,			// ゲーム背景
		LOGOTYPE_JEWELRYBG1P,		// 宝石背景1P
		LOGOTYPE_JEWELRYBG2P,		// 宝石背景2P
		LOGOTYPE_JEWELRYRED,		// 宝石赤
		LOGOTYPE_JEWELRYBULE,		// 宝石青
		LOGOTYPE_JEWELRYGREEN,		// 宝石緑
		LOGOTYPE_PLAYER1,			// プレイヤーアイコン1P
		LOGOTYPE_PLAYER2,			// プレイヤーアイコン2P
		LOGOTYPE_CHARANAME0,		// 1Pのキャラクターネーム
		LOGOTYPE_CHARANAME1,		// 2Pのキャラクターネーム
		LOGOTYPE_MAX				// 最大
	} UITYPE;

	CUI_game(PRIORITY type);		// コンストラクタ
	~CUI_game();					// デストラクタ
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static CUI_game *Create(void);	// 生成処理
	static HRESULT Load(void);		// テクスチャ情報ロード処理
	static void Unload(void);		// テクスチャ情報アンロード処理

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ロゴ設定処理

protected:

private:
	void GameUpdate(void);								// ゲームの更新処理

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// テクスチャ情報
	static char *m_apFileName[LOGOTYPE_MAX];			// ロゴのファイル名

	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2Dの情報

	int m_nCharaNum[MAX_PLAYER];						// キャラ番号

	bool m_bTransform[MAX_PLAYER];						// 変身したかどうか
	bool m_bStoneID[MAX_PLAYER][CStone::STONE_ID_MAX];	// ストーンID

};
#endif