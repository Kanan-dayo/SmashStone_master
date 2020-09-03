//=============================================================================
//
// オブジェクトマネージャー処理  [ objectManager.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "object.h"
#include "objectLift.h"
#include "scene.h"
#include <vector>
#include <memory>							// スマートポインタの使用に必要
#include "kananlibrary.h"

//=============================================================================
// クラス定義
//=============================================================================
class CObject;

class CObjectManager : public CScene
{
public:
	typedef struct
	{	// オブジェクトオフセット
		int nType;			// タイプ
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 回転
		D3DXVECTOR3 scale;	// 拡縮
	} OBJECT_OFFSET;

	typedef struct
	{
		int nNumObject;						// 配置するオブジェクト数
		OBJECT_OFFSET *objOffset;			// オブジェクトオフセット格納用
		std::vector<CObject*> pObject;		// オブジェクト情報
		std::vector<CObjectLift*> pObjLift;	// 持てるオブジェクト情報
		D3DXVECTOR3 stonePos[5];			// ストーンを配置する位置
	} STAGEINFO;

	CObjectManager(CScene::PRIORITY nPriority);	// コンストラクタ
	~CObjectManager();	// デストラクタ

	void Init();									// 初期化
	void Uninit();									// 終了
	void Update();									// 更新
	void Draw();									// 描画
	static CObjectManager *Create(STAGETYPE type);	// 生成

	static HRESULT Load(void);					// モデルのロード
	static void Unload(void);					// モデルの破棄

	static MODEL_VTX GetModelVtx(const int & nType)			{ return m_objInfo[nType].modelVtx; }						// タイプごとのモデル頂点情報の取得
	static STAGEINFO GetStageInfo(int nStageType)			{ return m_stageInfo[nStageType]; }							// ステージの情報取得

	std::vector<CObject*> GetObjAll(int nStageType)			{ return m_stageInfo[nStageType].pObject; }					// オブジェクトの取得
	CObject* GetObj(int nStageType, int nIndex)				{ return m_stageInfo[nStageType].pObject[nIndex]; }			// オブジェクトの取得
	int		GetNumObject(int nStageType)					{ return (int)m_stageInfo[nStageType].pObject.size(); }		// オブジェクト総数の取得

	std::vector<CObjectLift*> GetObjLiftAll(int nStageType) { return m_stageInfo[nStageType].pObjLift; }				// 持てるオブジェクトの取得
	CObjectLift* GetObjLift(int nStageType, int nIndex)		{ return m_stageInfo[nStageType].pObjLift[nIndex]; }		// 持てるオブジェクトの取得
	int		GetNumObjLift(int nStageType)					{ return (int)m_stageInfo[nStageType].pObjLift.size(); }	// 持てるオブジェクト総数の取得

	static D3DXVECTOR3 GetDefaultStonePos(int nStageType, int nPos) { return m_stageInfo[nStageType].stonePos[nPos]; }	// ストーンの配置位置の取得

#ifdef _DEBUG
	void ShowObjectManagerInfo(void);			// ImGuiの更新
	static bool GetShowObjWindow(void) { return m_bShowAnother; }			// オブジェウィンドウ表示の有無を取得
	static void SetShowObjWindow(bool bShow) { m_bShowAnother = bShow; }	// 表示の設定
#endif

private:
	static void CreateTexture();	// テクスチャ生成
	static void CreateObjInfo();	// オブジェクト情報生成

	static HRESULT LoadModel(void);				// モデル読み込み
	static HRESULT LoadLiftModel(void);			// 持ち上げ用モデルの読み込み
	static HRESULT LoadOffset(void);			// オフセット読み込み
	//static HRESULT LoadOffset(void);			// オフセット読み込み
	static HRESULT SetOffset(int nStageType);	// オフセット設定

	static char					m_cModelFile[MAX_TEXT];				// モデル情報のファイル名
	static char					m_cLiftFile[MAX_TEXT];				// 持ち上げモデル情報のファイル名
	static char					m_aFileName[STAGE_MAX][MAX_TEXT];	// モデル配置のファイル名
	static STAGEINFO			m_stageInfo[STAGE_MAX];				// ステージの情報
	static std::vector<OBJINFO>	m_objInfo;							// オブジェクトのモデル情報
	static std::vector<OBJINFO>	m_liftObjInfo;						// オブジェクトのモデル情報
	static int					m_nNumTexture;						// テクスチャ総数
	static int					m_nNumLiftTexture;					// テクスチャ総数
	static int					*m_pModelIndex;						// テクスチャを割り当てるモデル番号

#ifdef _DEBUG
	// デバッグモードの構造体
	typedef enum
	{
		MODE_GAME = 0,	// ゲーム
		MODE_DEBUG		// デバッグ
	} DEBUG_MODE;

	HRESULT SaveOffset(void);						// オフセット情報のセーブ

	static int	m_stateMode;						// モードの状態
	static bool m_bShowAnother;						// 別のウィンドウ
	CObject		*m_pFakeObject;						// 偽のオブジェクト
	static bool m_bObjUse;							// 偽オブジェを使用しているか
	static int	m_nFakeType;						// 偽オブジェのタイプ
	static std::string	*m_pObjName;				// ImGui用のオブジェクト名
#endif
};
#endif