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
#include "stone.h"

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CPolygon2D;
class CStone;

//==================================================================================================================
//
// ロゴクラスの定義
//
//==================================================================================================================
class CUI_game
{
public:
	CUI_game();		// コンストラクタ
	~CUI_game();					// デストラクタ
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static CUI_game *Create(void);	// 生成処理
	static HRESULT Load(void);		// テクスチャ情報ロード処理
	static void Unload(void);		// テクスチャ情報アンロード処理

	static void CreateUI(int nPlayer, int type, D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ロゴ設定処理
	static void DeleteUI(int nPlayer, int type);									// ロゴ破棄

	void CreateBG();											// 背景の生成

	static void SetbDisp(bool bDisp)	{ m_bDisplay = bDisp; }	// 表示の設定
	static bool GetbDisp(void)			{ return m_bDisplay; }	// 表示の取得

	static void CatchStone(int nPlayer, CStone::STONE_ID type);
	static void ReleaseStone(int nPlayer, CStone::STONE_ID type);
	static void TransPlayer(int nPlayer);
	static void FinishTrans(int nPlayer);

protected:

private:
	// プレイヤーテクスチャの種類
	typedef enum
	{
		LOGOTYPE_JEWELRYBG,			// 宝石背景
		LOGOTYPE_JEWELRYRED,		// 宝石赤
		LOGOTYPE_JEWELRYGREEN,		// 宝石緑
		LOGOTYPE_JEWELRYBULE,		// 宝石青
		LOGOTYPE_PLAYERICON,		// プレイヤーアイコン
		LOGOTYPE_CHARANAME,			// キャラクターネーム
		LOGOTYPE_MAX				// 最大
	} UI_GAME_TYPE;

	void SetDefaultUI(void);							// 開始時のUIをセット

	static LPDIRECT3DTEXTURE9 m_pTexBG;						// テクスチャ情報
	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];		// テクスチャ情報
	static char m_FileBG[MAX_TEXT];							// ゲームBGのファイル名
	static char *m_apFileName[LOGOTYPE_MAX];				// ロゴのファイル名

	CPolygon2D *m_pPolyBG;									// 表示するBG画像
	static CPolygon2D *m_pPolygon[MAX_PLAYER][LOGOTYPE_MAX];// 表示するプレイヤー画像
	static D3DXVECTOR3 m_posUI[MAX_PLAYER][LOGOTYPE_MAX];	// UIの座標
	static D3DXVECTOR3 m_sizeUI[MAX_PLAYER][LOGOTYPE_MAX];	// UIのサイズ

	static bool m_bDisplay;									// UIを表示するかどうか
};
#endif