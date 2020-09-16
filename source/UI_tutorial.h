//==================================================================================================================
//
// UI_tutorial[UI_tutorial.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_TUTORIAL_H_
#define _UI_TUTORIAL_H_

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
class CUI_tutorial : public CScene
{
public:
	CUI_tutorial(PRIORITY type);	// コンストラクタ
	~CUI_tutorial();				// デストラクタ
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static CUI_tutorial *Create(void);// 生成処理
	static HRESULT Load(void);		// テクスチャ情報ロード処理
	static void Unload(void);		// テクスチャ情報アンロード処理

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ロゴ設定処理

	int GetCharaNum(int nPlayer) { return m_nCharaNum[nPlayer]; }	// プレイヤーのタイプを取得
	bool GetCharaDecide(int nPlayer) { return m_bCharaDecide[nPlayer]; }

protected:

private:
	//=============================================================================
	// 列挙型定義
	//=============================================================================
	typedef enum
	{
		UITYPE_GEAR0 = 0,		// 歯車0
		UITYPE_CLOCK0,			// 時計土台0
		UITYPE_CLOCKHANDS0,		// 時計の針0
		UITYPE_CLOCKGEAR0,		// 時計の歯車0
		UITYPE_GEAR1,			// 歯車0
		UITYPE_CLOCK1,			// 時計土台0
		UITYPE_CLOCKHANDS1,		// 時計の針0
		UITYPE_CLOCKGEAR1,		// 時計の歯車0
		UITYPE_CHARAFULLBG,		// キャラクター全員背景
		UITYPE_CHARAFULL,		// キャラクター全員
		UITYPE_1PCHARABG,		// キャラクター1P背景
		UITYPE_2PCHARABG,		// キャラクター2P背景
		UITYPE_1PCHARA,			// 1Pキャラクター
		UITYPE_2PCHARA,			// 2Pキャラクター
		UITYPE_1PCHARA_FREAM,	// 1Pキャラクター枠線
		UITYPE_2PCHARA_FREAM,	// 2Pキャラクター枠線
		UITYPE_1PCHARA_NAME,	// 1Pキャラクターネーム
		UITYPE_2PCHARA_NAME,	// 2Pキャラクターネーム
		UITYPE_1PREADY,			// 1Pキャラクター準備完了
		UITYPE_2PREADY,			// 2Pキャラクター準備完了
		UITYPE_SELECTICON,		// プレイヤーセレクトアイコン
		UITYPE_MAX				// 最大
	} UI_TUTORIAL_TYPE;

	void TutorialUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// チュートリアルの更新処理
	void ControlGamepad(CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// ゲームパッド操作
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// キーボード操作

	static LPDIRECT3DTEXTURE9 m_pTexture[UITYPE_MAX];	// テクスチャ情報
	static char *m_apFileName[UITYPE_MAX];			// ロゴのファイル名

	CScene2D *m_pScene2D[UITYPE_MAX];					// scene2Dの情報

	int m_nCharaNum[MAX_PLAYER];						// キャラ番号
	int m_nCntRot[MAX_PLAYER];							// 時計の針回転用カウンタ
	int m_nCntWait[MAX_PLAYER];							// 待機時間用カウンタ

	float m_fPosMove[MAX_PLAYER];						// 位置移動用カウンタ
	float m_fPos[MAX_PLAYER];							// 現在の枠線テクスチャ位置X
	float m_fPosDiff[MAX_PLAYER];						// 目標の枠線テクスチャ位置X
	float m_fPosOld[MAX_PLAYER];						// 前回の枠線テクスチャ位置X
	float m_fRotGear[MAX_PLAYER];						// 歯車の回転格納変数
	float m_fPosCul[MAX_PLAYER];						// 位置計算用変数
	float m_fDiff[MAX_PLAYER];							// 1フレーム前との距離
	float m_fAngle[MAX_PLAYER];							// 歯車の回転角度
	float m_fRad[MAX_PLAYER];							// ラジアン値

	bool m_bUIClockHands[MAX_PLAYER];					// 時計の針が動いているかどうか
	bool m_bCharaDecide[MAX_PLAYER];					// 自分のキャラクターを選択したかどうか
	bool m_bStickReturn[MAX_PLAYER];					// パッドのスティックを戻したかどうか

};
#endif