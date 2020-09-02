//*************************************************************************************************************
//
// ポリゴンコライダー処理[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "PolygonCollider.h"
#include "renderer.h"
#include "debugProc.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLL_FILENAME ("data/COLLIDER/Polygon.csv")

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CPolygonCollider::POLYCOLLINFO	CPolygonCollider::m_polyCollInfo[MAP_TYPE_MAX] = {};

//-------------------------------------------------------------------------------------------------------------
// ロード
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::Load(void)
{
	// ファイル名のロード
	if (FAILED(LoadFileName()))
		return E_FAIL;
	// モデルのロード
	if (FAILED(LoadModelFile()))
		return E_FAIL;

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// ファイル名のロード
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::LoadFileName(void)
{
	// ブロックコメント
	CKananLibrary::StartBlockComment("ポリゴンコライダーのファイル読み込み開始");
	DWORD start = timeGetTime();		// 計測スタート時間

	// ファイルオープン
	FILE *pFile = fopen(POLYCOLL_FILENAME, "r");

	// nullcheck
	if (!pFile)
	{
		// 失敗
		CKananLibrary::EndBlockComment("ファイル読み込み失敗");
		return E_FAIL;
	}

	// テキスト保存用
	char cHeadText[MAX_TEXT] = {};
	char cReadText[MAX_TEXT] = {};
	char cDieText[MAX_TEXT] = {};
	
	// スクリプトが来るまで繰り返す
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cReadText, sizeof(cReadText), pFile);
		// 読み込んど文字列代入
		sscanf(cReadText, "%[^,]s", &cHeadText);
	}

	// ステージカウンタ
	int nCntStage = 0;
	// ポリゴンカウンタ
	int nCntPolygon = 0;

	// END_SCRIPTまでループ
	while (strcmp(cHeadText, "END_SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cReadText, sizeof(cReadText), pFile);
		// 読み込んど文字列代入
		sscanf(cReadText, "%[^,]s", &cHeadText);

		if (nCntStage < MAP_TYPE_MAX)
		{
			// SET_STAGEが来るまで繰り返す
			while (strcmp(cHeadText, "SET_STAGE") != 0)
			{
				// 1行読み込む
				fgets(cReadText, sizeof(cReadText), pFile);
				// 読み込んど文字列代入
				sscanf(cReadText, "%[^,]s", &cHeadText);
				// 読み込んだポリゴン数を初期化
				nCntPolygon = 0;
			}

			// END_SETSTAGEが来るまでループ
			while (strcmp(cHeadText, "END_SETSTAGE") != 0)
			{
				// 1行読み込む
				fgets(cReadText, sizeof(cReadText), pFile);
				char *pRead = &cReadText[0];
				while (*pRead == ',')
				{
					pRead++;
				}

				// 読み込んど文字列代入
				sscanf(pRead, "%[^,]s", &cHeadText);

				// 設定する個数
				if (strcmp(cHeadText, "SET_NUM") == 0)
				{
					// 数を取得し、メモリ確保
					sscanf(pRead, "%[^,],%d", &cDieText, &m_polyCollInfo[nCntStage].nNumPolygon);
					m_polyCollInfo[nCntStage].polyInfo = new POLGONINFO[m_polyCollInfo[nCntStage].nNumPolygon];
				}
				// 設定するもの
				if (strcmp(cHeadText, "SET") == 0)
				{
					while (*pRead == '"')
					{
						pRead++;
					}

					/* 一行分の情報を解析する */
					//			   SET     メモ   ID   パス
					sscanf(pRead, "%[^, ], %[^, ], %d, %f, %f, %f, %s",
						&cDieText, &cDieText,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].nPolyID,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].pos.x,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].pos.y,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].pos.z,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].modelInfo.cModelName);

					// 情報カウントインクリメント
					nCntPolygon++;
				}
			}

			if (strcmp(cHeadText, "END_SETSTAGE") == 0)
			{
				// ステージ数を加算
				nCntStage++;
			}
		}
	}

	// ブロックコメント
	DWORD end = timeGetTime();			// 計測スタート時間
#ifdef _DEBUG
	cout << "読み込み終了" << endl;
	cout << "読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]" << endl;
#endif
	CKananLibrary::PrintBlockCommentFrame();
	// 終了
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// モデルのロード
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::LoadModelFile(void)
{
	for (int nCntStage = 0; nCntStage < MAP_TYPE_MAX; nCntStage++)
	{
		// 種類数分回す
		for (int nCntPoly = 0; nCntPoly < m_polyCollInfo[nCntStage].nNumPolygon; nCntPoly++)
		{
			// モデル情報の生成
			CKananLibrary::CreateModelInfo(&m_polyCollInfo[nCntStage].polyInfo[nCntPoly].modelInfo);

			// メッシュを格納
			LPD3DXMESH mesh = m_polyCollInfo[nCntStage].polyInfo[nCntPoly].modelInfo.mesh;

			// 頂点フォーマットのサイズを取得
			DWORD sizeFVF = D3DXGetFVFVertexSize(mesh->GetFVF());
			// 頂点バッファのポインタ
			BYTE  *pVertexBuffer;

			// 頂点バッファをロック
			mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

			// バッファから頂点座標を取得
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_2 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// サイズ分ポインタを進める
			pVertexBuffer += sizeFVF;
			// バッファから頂点座標を取得
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_0 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// サイズ分ポインタを進める
			pVertexBuffer += sizeFVF;
			// バッファから頂点座標を取得
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_1 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// サイズ分ポインタを進める
			pVertexBuffer += sizeFVF;
			// バッファから頂点座標を取得
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_3 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// サイズ分ポインタを進める
			pVertexBuffer += sizeFVF;

			// 頂点バッファをアンロック
			mesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// アンロード
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Unload(void)
{
	for (int nCntStage = 0; nCntStage < MAP_TYPE_MAX; nCntStage++)
	{
		for (int nCntPoly = 0; nCntPoly < m_polyCollInfo[nCntStage].nNumPolygon; nCntPoly++)
		{
			if (m_polyCollInfo[nCntStage].polyInfo)
			{
				delete[] m_polyCollInfo[nCntStage].polyInfo;
				m_polyCollInfo[nCntStage].polyInfo = nullptr;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CPolygonCollider * CPolygonCollider::Create(int nStageType, int nIndex)
{
	// 生成
	CPolygonCollider *pCPolyColli = new CPolygonCollider;
	// 頂点位置の設定
	pCPolyColli->SetVtxPos(m_polyCollInfo[nStageType].polyInfo[nIndex].vtxPos);
	// 初期化
	pCPolyColli->Init();
	return pCPolyColli;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Init(void)
{
	// 変数宣言
	D3DXVECTOR3 vecA = MYLIB_VEC3_UNSET;				// Aベクトル
	D3DXVECTOR3 vecB = MYLIB_VEC3_UNSET;				// Bベクトル

	// 法線ベクトルを求める
	vecA = m_VtxPos.vtx_1 - m_VtxPos.vtx_2;					// Aベクトル算出
	vecB = m_VtxPos.vtx_0 - m_VtxPos.vtx_1;					// Bベクトル算出
	D3DXVec3Cross(&m_SurfaceNor, &vecA, &vecB);			// 直交ベクトル算出
	D3DXVec3Normalize(&m_SurfaceNor, &m_SurfaceNor);	// 正規化する

	// 頂点情報の作成
	MakeVertex();
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Uninit(void)
{
	// 頂点バッファがNULLじゃなかった時
	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Draw(void)
{
	// 変数宣言
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得
	D3DXMATRIX mtxWorld;									// 計算用マトリックス

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライティングモード無効

	// Fill Mode の設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

#ifdef  _DEBUG
	// 可視化中のみ
	if (CRenderer::GetbDisColl())
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
#endif

	// Fill Mode の設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード無効
}

//-------------------------------------------------------------------------------------------------------------
// 衝突判定
//-------------------------------------------------------------------------------------------------------------
bool CPolygonCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection)
{
	if (Test3DInsidePolygon(pPos))
	{
#ifdef _DEBUG
		CDebugProc::Print("中にいる\n");
#endif
		if (bReflection == true)
		{
			pPos->y = m_VtxPos.vtx_2.y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos.vtx_2.x) -
					m_SurfaceNor.z * (pPos->z - m_VtxPos.vtx_2.z)) / m_SurfaceNor.y;
			*pOut_Intersect = *pPos;
			*SurfaceNor = m_SurfaceNor;
		}
		else
		{
			pPos->y = m_VtxPos.vtx_2.y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos.vtx_2.x) -
				m_SurfaceNor.z * (pPos->z - m_VtxPos.vtx_2.z))	/ m_SurfaceNor.y;
		}
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// ポリゴンの中にいるかテスト
//-------------------------------------------------------------------------------------------------------------
bool CPolygonCollider::Test3DInsidePolygon(D3DXVECTOR3 * pPos)
{
	if (CMylibrary::Test3DInsidePolygon(m_VtxPos.vtx_0, m_VtxPos.vtx_1, m_VtxPos.vtx_2, *pPos, -m_SurfaceNor))
	{
		return true;
	}
	else if(CMylibrary::Test3DInsidePolygon(m_VtxPos.vtx_1, m_VtxPos.vtx_3, m_VtxPos.vtx_2, *pPos, -m_SurfaceNor))
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::SetVtxPos(VTXPOS &pVertexsPos)
{
	m_VtxPos = pVertexsPos;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::MakeVertex(void)
{
	// 変数宣言
	CRenderer *pRenderer = CManager::GetRenderer();					// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();				// デバイスの取得

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYCOLLI_USE_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標の設定
	pVtx[0].pos = m_VtxPos.vtx_0;
	pVtx[1].pos = m_VtxPos.vtx_1;
	pVtx[2].pos = m_VtxPos.vtx_2;
	pVtx[3].pos = m_VtxPos.vtx_3;

	// 法線ベクトルの設定
	pVtx[0].nor = m_SurfaceNor;
	pVtx[1].nor = m_SurfaceNor;
	pVtx[2].nor = m_SurfaceNor;
	pVtx[3].nor = m_SurfaceNor;

	// 頂点カラー
	pVtx[0].col = WhiteColor;
	pVtx[1].col = WhiteColor;
	pVtx[2].col = WhiteColor;
	pVtx[3].col = WhiteColor;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	return E_NOTIMPL;
}
