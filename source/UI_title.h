//==================================================================================================================
//
// UI_title[UI_title.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_TITLE_H_
#define _UI_TITLE_H_

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
class CUI_title : public CScene
{
public:
	CUI_title(PRIORITY type);		// コンストラクタ
	~CUI_title();					// デストラクタ
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static CUI_title *Create(void);	// 生成処理
	static HRESULT Load(void);		// テクスチャ情報ロード処理
	static void Unload(void);		// テクスチャ情報アンロード処理

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ロゴ設定処理

protected:

private:
	//=============================================================================
	// 列挙型定義
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_TITLE = 0,			// タイトル
		LOGOTYPE_ENTER,				// エンターロゴ
		LOGOTYPE_MAX				// 最大
	} UI_TITLE_TYPE;

	void TitleUpdate(void);	// タイトルの更新処理

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// テクスチャ情報
	static char *m_apFileName[LOGOTYPE_MAX];			// ロゴのファイル名

	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2Dの情報

	D3DXVECTOR3 TitlePos;								// タイトルUIの位置

	int m_nCntBound;									// タイトルUIバウンドカウンタ

	float m_fCntUITitle0;								// タイトルUI用カウンタ0
	float m_fCntUITitle1;								// タイトルUI用カウンタ1
	float m_fCntEnter;									// エンター用カウンタ

	bool m_bUITitle0;									// タイトルを動かすかどうか0
	bool m_bUITitle1;									// タイトルを動かすかどうか1
	bool m_bUIEnter;									// エンターのα値用変数

};
#endif