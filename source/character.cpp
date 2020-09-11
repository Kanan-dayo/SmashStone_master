//=============================================================================
//
// キャラクター処理  [ character.cpp ]
// Author : Seiya Takahashi
//
//=============================================================================


//=============================================================================
// インクルード
//=============================================================================
#include "character.h"
#include "Inputkeyboard.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "modelCharacter.h"
#include "debugProc.h"
#include "motion.h"
#include "game.h"
#include "meshField.h"
#include "modelParts.h"
#include "CapsuleCollider.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================	
#define SPEED_ROT			(0.1f)		// 回転のスピード
#define TIME_MAX_DOWN		(60)		// 最大までダウンできる時間

#define INERTIA_SMASH		(0.0005f)	// スマッシュ吹き飛び時の慣性
#define INERTIA_BLOWAWAY	(0.02f)		// 通常吹き飛び時の慣性

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
int CCharacter::m_nNumCharacter = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CCharacter::CCharacter(PRIORITY nPriority) : CScene(nPriority)
{
	// 要素の初期化
	m_pModelCharacter	= nullptr;
	m_pos				= ZeroVector3;
	m_posOld			= ZeroVector3;
	m_posBegin			= ZeroVector3;
	m_rot				= ZeroVector3;
	m_rotDest			= ZeroVector3;
	m_rotDif			= ZeroVector3;
	m_move				= ZeroVector3;
	m_moveOld			= ZeroVector3;
	m_nCntTrans			= 0;
	m_nAttackFlow		= 0;
	m_nNumStone 		= 0;
	m_nLife				= 0;
	m_stateStand		= STANDSTATE_NEUTRAL;
	m_stateJump			= JUMPSTATE_NONE;
	m_StateLift			= STATE_NONE;
	m_bAttakHit			= false;
	m_bAttakHitStone	= false;
	
	// 総数を加算
	m_nNumCharacter++;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCharacter::~CCharacter()
{
	// nullcheck
	if (m_pModelCharacter)
	{
		// モデルキャラクターを破棄
		delete m_pModelCharacter;
		m_pModelCharacter = nullptr;
	}
}

//=============================================================================
// 初期化
//=============================================================================
void CCharacter::Init()
{
}

//=============================================================================
// 終了
//=============================================================================
void CCharacter::Uninit()
{
	//モデルの終了処理
	m_pModelCharacter->Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CCharacter::Update()
{
	m_param = CCharaParam::GetPlayerParam((PARAM_TYPE)(m_type / 2));
	
	// 移動処理
	Move();

	// 回転処理
	Rot();

	// 変身処理
	Trans();

	// ワールドマトリックスの設定
	m_pModelCharacter->SetCharacterMtx(&m_mtxWorld);

	// モデルの更新
	m_pModelCharacter->Update();

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// キーボードの取得

	// キーボードの[5]を押したとき
	if (pKeyboard->GetKeyboardPress(DIK_5))
	{
		// 体力を減らす
		m_nLife -= 1;
	}
	// キーボードの[6]を押したとき
	if (pKeyboard->GetKeyboardPress(DIK_6))
	{
		// 体力を増やす
		m_nLife += 1;
	}
	// キーボードの[6]を押したとき
	if (pKeyboard->GetKeyboardPress(DIK_7))
	{
		// 体力を増やす
		m_nLife = m_param.fMaxLife;
	}

#endif // _DEBUG

	if (m_nLife <= 0)
		m_nLife = 0;
	if (m_nLife >= m_param.fMaxLife)
		m_nLife = m_param.fMaxLife;
}

//=============================================================================
// 描画
//=============================================================================
void CCharacter::Draw()
{
	// モデルの描画
	m_pModelCharacter->Draw();
}

//=============================================================================
// モデルの設定
//=============================================================================
void CCharacter::SetModelType(CHARACTER_TYPE type)
{
	// モデルの生成
	m_pModelCharacter = CModelCharacter::Create(type);

	m_type = type;
	m_typeTrans = (CHARACTER_TYPE)(type + 1);

	// タイプごとに最大HP設定
	m_param = CCharaParam::GetPlayerParam((PARAM_TYPE)(m_type / 2));
	m_nLife = m_param.fMaxLife;

	// ワールドマトリックスの設定
	m_pModelCharacter->SetCharacterMtx(&m_mtxWorld);

	// シリンダーコライダーの設定
	SetCylinderCoillider();
}

//=============================================================================
// シリンダーコライダーの設定
//=============================================================================
void CCharacter::SetCylinderCoillider(void)
{
	// パーツのポインタ
	CModelParts *pParts = m_pModelCharacter->GetModelParts();

	// 右前腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_FOREARM_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LOWARM_R].GetMtx(), CCapsuleCollider::TYPEID_FOREARM_R);
	// 右上腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_UPPERARM_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_UPARM_R].GetMtx(), CCapsuleCollider::TYPEID_UPPERARM_R);
	// 左前腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_FOREARM_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LOWARM_L].GetMtx(), CCapsuleCollider::TYPEID_FOREARM_L);
	// 左上腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_UPPERARM_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_UPARM_L].GetMtx(), CCapsuleCollider::TYPEID_UPPERARM_L);
	// 体の設定
	m_pCapColi[CCharacter::COLLIPARTS_BODY] =
		CCapsuleCollider::Create(this, &m_mtxWorld, CCapsuleCollider::TYPEID_BODY);


}

#define CASE(Key)	break;case (Key):
#define DEFAULT		break;default:
//=============================================================================
// モデルインデックスからタイプIDに変換
//=============================================================================
void CCharacter::ConfromFromModelIndexToYypeID(int * pOut, const int * pIn)
{
	switch ((CModelCharacter::PARTSNAME)*pIn)
	{
		CASE(CModelCharacter::UPARM_R)   *pOut = (int)CCapsuleCollider::TYPEID_UPPERARM_R;
		CASE(CModelCharacter::FOREARM_R) *pOut = (int)CCapsuleCollider::TYPEID_FOREARM_R;
		CASE(CModelCharacter::UPARM_L)   *pOut = (int)CCapsuleCollider::TYPEID_UPPERARM_L;
		CASE(CModelCharacter::FOREARM_L) *pOut = (int)CCapsuleCollider::TYPEID_FOREARM_L;
		CASE(CModelCharacter::THIGH_R)   *pOut = (int)CCapsuleCollider::TYPEID_THIGH_R;
		CASE(CModelCharacter::LOWERLEG_R)*pOut = (int)CCapsuleCollider::TYPEID_LOWERLEG_R;
		CASE(CModelCharacter::THIGH_L)   *pOut = (int)CCapsuleCollider::TYPEID_THIGH_L;
		CASE(CModelCharacter::LOWERLEG_L)*pOut = (int)CCapsuleCollider::TYPEID_LOWERLEG_L;
			DEFAULT *pOut = -1; return;
	}
}

//=============================================================================
// 勝利行動
//=============================================================================
void CCharacter::VictoryAction(void)
{
}

//=============================================================================
// 移動の処理
//=============================================================================
void CCharacter::Move(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;
	m_moveOld = m_move;
	
	// 慣性
	if (m_stateStand == STANDSTATE_SMASHBLOWAWAY)
	{
		CMylibrary::SlowingMove(&m_move.x, INERTIA_SMASH);
		CMylibrary::SlowingMove(&m_move.y, INERTIA_SMASH);
		CMylibrary::SlowingMove(&m_move.z, INERTIA_SMASH);
	}
	else if (m_stateStand == STANDSTATE_BLOWAWAY)
	{
		CMylibrary::SlowingMove(&m_move.x, INERTIA_BLOWAWAY);
		CMylibrary::SlowingMove(&m_move.y, INERTIA_BLOWAWAY);
		CMylibrary::SlowingMove(&m_move.z, INERTIA_BLOWAWAY);
	}
	else
		CKananLibrary::InertiaMoveXZ(&m_move);

	// 重力
	CKananLibrary::Gravity(m_move.y);

	//移動量加算
	m_pos += m_move;
}

//=============================================================================
// 回転の処理
//=============================================================================
void CCharacter::Rot(void)
{
	// 回転の差分を求める
	m_rotDif = m_rotDest - m_rot;

	// 回転の補間
	CKananLibrary::InterpolationRot(&m_rotDif);

	// モデルの回転
	m_rot += m_rotDif * SPEED_ROT;

	// 回転の補間
	CKananLibrary::InterpolationRot(&m_rot);
}

//=============================================================================
// 変身の処理
//=============================================================================
void CCharacter::Trans(void)
{
	if (!m_bTrans)
	{
		// 変身時間を初期化
		m_nCntTrans = 0;
		return;
	}

	// 変身時間を加算
	m_nCntTrans++;
	if (m_nCntTrans < TIME_TRANS)
	{
		// モデルを変身用にバインド
		m_pModelCharacter->ModelRebind(m_typeTrans);
		// モデルタイプを再設定
		m_pModelCharacter->SetModelType(m_typeTrans);
		return;
	}

	// ストーンの取得数を初期化
	m_nNumStone = 0;
	// ストーンの出現数を初期化
	CGame::SetNumStone(0);	
	for (int nCnt = 0; nCnt < CStone::STONE_ID_MAX; nCnt++)
	{
		m_bGetStoneType[nCnt] = false;
		// 再配置できるようストーンを使用されていない状態にする
		CGame::RemoveTypeStone(nCnt);
	}
	// 変身時間を初期化
	m_nCntTrans = 0;
	// 変身を解除
	m_bTrans = false;
	// モデルの再バインド
	m_pModelCharacter->ModelRebind(m_type);
	// モデルタイプを再設定
	m_pModelCharacter->SetModelType(m_type);
	// BGM変更
	CRenderer::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_TRANS);
	CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
}
