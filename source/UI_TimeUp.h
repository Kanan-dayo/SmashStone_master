//==================================================================================================================
//
// TimeUpのUI処理 [ UI_KO.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_TIMEUP_H_
#define _UI_TIMEUP_H_

#define _CRT_SECURE_NO_WARNINGS							// 警告回避文

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"

//==================================================================================================================
//
// ポーズクラスの定義
//
//==================================================================================================================
// 前方宣言
class CPolygon2D;

class CUITimeUp
{
public:
	CUITimeUp();			// コンストラクタ
	~CUITimeUp();			// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	static CUITimeUp *Create(void);		// 生成処理
	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

protected:

private:
	typedef enum
	{
		TIMEUPSTATE_GATHER = 0,		// 一か所に集まる
		TIMEUPSTATE_SHOW,			// UIを止めて見せる
		TIMEUPSTATE_DISTRIBUTED,	// UIの分散
	} TIMEUP_STATE;

	typedef enum
	{
		TIMEUPUITEX_1 = 0,	// TimeUp
		TIMEUPUITEX_2,		// TimeUp
		TIMEUPUITEX_3,		// TimeUp
		TIMEUPUI_MAX		// 最大数
	} TIMEUP_TEX_TYPE;		// TimeUpテクスチャのタイプ

	void Gather(void);
	void Show(void);
	void Distributed(void);
	void CreateUI(int type);		// UIの生成
	void NextFase(void) { m_nCntAny = 0; }	// 次のフェーズに移行
	static TIMEUP_STATE m_stateTimeUp;				// UIの状態
	static LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャ情報
	static char *m_apFileName;						// テクスチャのファイル名
	static CPolygon2D *m_pPolygon[TIMEUPUI_MAX];	// テクスチャ数分の2DUI
	static D3DXVECTOR3 m_sizeBegin[TIMEUPUI_MAX];	// 最初のテクスチャのサイズ
	static D3DXVECTOR3 m_sizeEnd[TIMEUPUI_MAX];		// 最後のテクスチャのサイズ
	static D3DXVECTOR3 m_posBegin[TIMEUPUI_MAX];	// 最初のテクスチャの位置
	static D3DXVECTOR3 m_posEnd[TIMEUPUI_MAX];		// 最後のテクスチャの位置
	static D3DXCOLOR   m_col[TIMEUPUI_MAX];			// 拡縮時のカラー
	int m_nCntAny;									// UIの移動や表示などに使う
};
#endif
