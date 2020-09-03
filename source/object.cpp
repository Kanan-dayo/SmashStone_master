//=============================================================================
//
// オブジェクト処理 [ object.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "objManager.h"
#include "3DBoxCollider.h"

#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================
#ifdef _DEBUG
#define COLOR_ALPHA		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))	// 透明なカラー
#endif

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject()
{
	// 要素の初期化
	m_bCollision	= false;
	m_pos			= ZeroVector3;
	m_posOld		= ZeroVector3;
	m_posBegin		= ZeroVector3;
	m_move			= ZeroVector3;
	m_rot			= ZeroVector3;
	m_rotBegin		= ZeroVector3;
	m_scale			= OneVector3;
	m_nColliderID	= -1;
#ifdef _DEBUG
	m_bRelease = false;
#endif
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// 初期化
//=============================================================================
void CObject::Init(void)
{
}

//=============================================================================
// 終了
//=============================================================================
void CObject::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CObject::Update(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;

	// 位置更新
	m_pos += m_move;
}

//=============================================================================
// 描画
//=============================================================================
void CObject::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マテリアル格納用
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	// スケール編集用
	D3DXMATRIX mtxScale;

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// スケールの計算
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetTexture(0, NULL);

	// テクスチャの設定
	if (m_pModelInfo.bTex)
		pDevice->SetTexture(0, m_pModelInfo.pTexture);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelInfo.matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo.matNum; nCntMat++)
	{
		// 半透明にする
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		// 描画
		m_pModelInfo.mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

#ifdef _DEBUG
//=============================================================================
// 透明描画
//=============================================================================
void CObject::DrawAlpha(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マテリアル格納用
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	// スケール編集用
	D3DXMATRIX mtxScale;

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// スケールの計算
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetTexture(0, NULL);

	// テクスチャの設定
	if (m_pModelInfo.bTex)
		pDevice->SetTexture(0, m_pModelInfo.pTexture);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelInfo.matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo.matNum; nCntMat++)
	{
		// 半透明にする
		pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		// 描画
		m_pModelInfo.mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}
#endif

//=============================================================================
// 生成
//=============================================================================
CObject * CObject::Create(void)
{
	// メモリ確保
	CObject *pObject = new CObject();

	// nullcheck
	if (!pObject)
		// 失敗
		return nullptr;

	// 初期化
	pObject->Init();

	// 値を返す
	return pObject;
}

//=============================================================================
// オブジェクトの情報設定
//=============================================================================
void CObject::SetObjInfo(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const D3DXVECTOR3 & scale, MODELINFO *pModelInfo, const int & type)
{
	m_pos = pos;
	m_posBegin = pos;
	m_rot = rot;
	m_rotBegin = rot;
	m_scale = scale;
	m_pModelInfo = *pModelInfo;
	m_nType = type;

	// コライダーの設定
	SetCollider();
}

//=============================================================================
// コライダーの設定
//=============================================================================
void CObject::SetCollider(void)
{
	// 設定されているとき
	if (m_nColliderID != -1)
	{// 処理を抜ける
		return;
	}
	// コライダーIDの設定
	m_nColliderID = C3DBoxCollider::SetColliderInfo(&m_pos, NULL, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::TOP_OBJECT + m_nType);
}

#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CObject::ShowObjectInfo(char cPrintText[16])
{
	// ツリー
	if (ImGui::TreeNode(cPrintText))
	{
		// 偽オブジェクト以外
		if (cPrintText != "FakeObject")
			ImGui::Text("type : %d", m_nType);

		ImGui::DragFloat3("pos", m_pos, 0.5f);
		ImGui::DragFloat3("rot", m_rot, 0.05f, -D3DX_PI, D3DX_PI);
		ImGui::DragFloat3("scale", m_scale, 0.05f, 0.0f, 100.0f);

		//ImGui::Checkbox("Collision", &m_bCollision);

		// 偽オブジェクト以外
		if (cPrintText != "FakeObject")
		{
			// 改行せずに間隔をあける
			ImGui::Dummy(ImVec2(200.0f, 0.0f));
			ImGui::SameLine();

			// 削除
			if (ImGui::Button("delete"))
				// リリースする
				m_bRelease = true;
		}

		// ツリーの最後に必ず書く
		ImGui::TreePop();
	}
}
#endif