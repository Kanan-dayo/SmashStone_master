//=============================================================================
//
// ポリゴンコライダーマネージャー処理  [ polyCollMana.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _POLYCOLLMANA_H_
#define _POLYCOLLMANA_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "kananlibrary.h"
#include <vector>

//=============================================================================
// クラス定義
//=============================================================================
class CPolygonCollider;

class CPolyCollMana : public CScene
{
public:
	CPolyCollMana() : CScene(PRIORITY_COLLISION) {}	// コンストラクタ
	~CPolyCollMana() {}								// デストラクタ

	void Init();									// 初期化
	void Uninit();									// 終了
	void Update();									// 更新
	void Draw();									// 描画
	static CPolyCollMana *Create(void);	// 生成
	
	CPolygonCollider *GetPolyColl(int nIndex) { return m_pPolyColl[nIndex]; }

private:
	std::vector<CPolygonCollider*> m_pPolyColl;		// ポリゴンコライダー

};
#endif