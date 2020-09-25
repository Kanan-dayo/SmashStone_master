//==================================================================================================================
//
// UI_mapSelect[UI_mapSelect.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_MAPSELECT_H_
#define _UI_MAPSELECT_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CScene2D;
class CInputKeyboard;
class CInputGamepad;

//==================================================================================================================
//
// ロゴクラスの定義
//
//==================================================================================================================
class CUI_mapSelect : public CScene
{
public:
	//=============================================================================
	// 列挙型定義
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_WORLDMAP = 0,		// 世界地図
		LOGOTYPE_MAPFRAME,			// マップ選択枠
		LOGOTYPE_MAP1,				// マップ1
		LOGOTYPE_MAP2,				// マップ2
		LOGOTYPE_MAX				// 最大
	} UI_MAPSELECT_TYPE;

	CUI_mapSelect(PRIORITY type);	// コンストラクタ
	~CUI_mapSelect();				// デストラクタ
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static CUI_mapSelect *Create(void);// 生成処理
	static HRESULT Load(void);		// テクスチャ情報ロード処理
	static void Unload(void);		// テクスチャ情報アンロード処理

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ロゴ設定処理

	static int GetMapID(void) { return m_nMapID; }	// マップIDの取得
	bool GetSelectMap(void) { return m_bMapSelect; }// マップの選択状況を取得

protected:

private:
	void MapSelectUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// マップ選択画面の更新処理
	void ControlGamepad(CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// ゲームパッド操作
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// キーボード操作

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// テクスチャ情報
	static char *m_apFileName[LOGOTYPE_MAX];			// ロゴのファイル名
	static int m_nMapID;								// マップ番号

	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2Dの情報

	int m_nCharaNum[MAX_PLAYER];						// キャラ番号

	bool m_bMapSelect;									// マップを選択したかどうか

};
#endif