//*************************************************************************************************************
//
// ポリゴンコライダー処理[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _POLYGONCOLLIDER_H_
#define _POLYGONCOLLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"
#include "kananlibrary.h"

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLLI_USE_VTX	4

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CPolygonCollider
{
public:
	enum
	{
		MAP_TYPE_NONE = -1,	// 無し
		MAP_TYPE_FIELD,		// 広場
		MAP_TYPE_PARK,		// 公園
		MAP_TYPE_MAX		// 最大
	};

	typedef struct VTXPOS
	{
		D3DXVECTOR3 vtx_0;			// 頂点0
		D3DXVECTOR3 vtx_1;			// 頂点1
		D3DXVECTOR3 vtx_2;			// 頂点2
		D3DXVECTOR3 vtx_3;			// 頂点3
	}VTXPOS;
	
	typedef struct MAP_POLYCOLII
	{
		VTXPOS *pFieldVtxPos;		// フィールドの頂点座標
	} MAP_POLYCOLII;

	typedef struct
	{	// ポリゴン情報
		VTXPOS		vtxPos;				// 頂点座標
		int			nPolyID;			// ポリゴンのID
		MODELINFO	modelInfo;			// モデル情報
		D3DXVECTOR3 pos;				// ポリゴンの座標
	} POLGONINFO;

	typedef struct
	{	// ポリゴンコライダー情報
		POLGONINFO		*polyInfo;			// ポリゴン情報	
		int				 nNumPolygon;		// ポリゴン数
	} POLYCOLLINFO;

	CPolygonCollider() {}											// コンストラクタ
	~CPolygonCollider() {}											// デストラクタ

	static HRESULT Load(void);										// ロード
	static void Unload(void);										// アンロード
	static CPolygonCollider* Create(int nStageType, int nIndex);	// 生成

	void Init(void);												// 初期化
	void Uninit(void);												// 終了
	void Update(void) {};											// 更新
	void Draw(void);												// 描画

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection);		// 衝突判定

	bool Test3DInsidePolygon(D3DXVECTOR3 *pPos);				// ポリゴンの中にいるかテスト

	static int GetNumPoly(int nStage) { return m_polyCollInfo[nStage].nNumPolygon; }	// ステージのポリゴン数の取得

private:
	static HRESULT LoadFileName(void);	// ファイル名のロード
	static HRESULT LoadModelFile(void);	// モデル名のロード
	void SetVtxPos(VTXPOS &pVertexsPos);													// 頂点位置の設定
	HRESULT MakeVertex(void);																// 頂点情報の作成
	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuff;													// バッファ
	VTXPOS					 m_VtxPos;														// 頂点位置
	D3DXVECTOR3              m_SurfaceNor;													// 面の法線
	static POLYCOLLINFO		 m_polyCollInfo[MAP_TYPE_MAX];									// ポリゴンコライダー情報
};

#endif