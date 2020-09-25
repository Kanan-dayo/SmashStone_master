//==================================================================================================================
//
// プレイヤー処理[player.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "inputKeyboard.h"
#include "renderer.h"
#include "player.h"
#include "debugProc.h"
#include "camera.h"
#include "meshField.h"
#include "time.h"
#include "title.h"
#include <initializer_list>
#include "tutorial.h"
#include "game.h"
#include "inputGamepad.h"
#include "character.h"
#include "scene.h"
#include "modelCharacter.h"
#include "motion.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "3DBoxCollider.h"
#include "stone.h"
#include "wall.h"
#include "CylinderCollider.h"
#include "motion.h"
#include "Reflection.h"
#include "hitpoint.h"
#include "PolygonCollider.h"
#include "CapsuleCollider.h"
#include "sound.h"
#include "polyCollMana.h"
#include "shadow.h"
#include "motion.h"
#include "3DParticle.h"
#include "modelParts.h"
#include "UI_game.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define HEIGHT_CEILING	(400.0f)			// 天井の高さ
#define HEIGHT_FLOOR	(0.0f)				// 床の高さ

#define BLOWAWAYFORCE_SMASH		(100)		// 吹き飛ばし力(スマッシュ攻撃)
#define BLOWAWAYFORCE_NORMAL	(8.0f)		// 吹き飛ばし力(通常攻撃)

#define TIME_LIFT_BEGIN			(60)		// 持ち上げ開始のモーション時間
#define TIME_MAX_DOWN			(60)		// 最大までダウンできる時間
#define TIME_MAX_CHARGE			(100)		// 最大までチャージできる時間
#define TIME_JUMP_TO_FALL		(15)		// ジャンプから落下までの時間
#define TIME_GETUP				(40)		// 起き上がりの時間
#define TIME_GETUP_ACTIVE		(35)		// 起き上がりの時間
#define TIME_AIR_ATTACK			(30)		// 空中攻撃が敵に当たるまでの時間

#define CHARGEPARTICLE_MAX_CHARGE	(12)		// 最大までチャージできる時間

#define DISTANCE_CHASE_ENEMY		(100.0f)	// 敵を追尾する距離
#define DISTANCE_AIR_CAHSE_ENEMY	(200.0f)	// 敵を追尾する距離(空中)

#define VALUE_DIFPOS_Y			(30.0f)		// 空中攻撃時の位置のずれ

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CPlayer::CPlayer(PRIORITY type = CScene::PRIORITY_PLAYER) : CCharacter(type)
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CPlayer::~CPlayer()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CPlayer::Init(void)
{
	// 要素の初期化
	m_bTrans = false;
	m_nCntState = 0;
	m_nCntGap = 0;
	m_nAttackFrame = 0;
	m_nCntParticle = 0;
	m_nCntChargeParticle = 0;
	m_nCntTimingChargeParticle = 0;
	m_nTimingChargeParticle = 0;
	m_nCntSmashDashParticle = 0;
	m_fMotionMove = 0.0f;
	m_vecP_to_E = ZeroVector3;
	m_AirMove = ZeroVector3;

	// 最大ポリゴン数までカウント
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_bIn[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < CStone::STONE_ID_MAX; nCnt++)
	{
		m_bGetStoneType[nCnt] = false;
	}

	// 初期化
	CCharacter::Init();
	CCharacter::SetModelType(m_type);

	// 当たり判定の設定
	this->m_nBoxColliderID = C3DBoxCollider::SetColliderInfo(&this->GetPos(), this, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::ID_CHARACTER);

	m_pHitPoint = CHitPoint::Create(m_nPlayer, m_param.fMaxLife);
	m_pHitPoint->SetnPlayerNum(m_nPlayer);

	// 影生成
	m_pShadow = CShadow::Create();
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();

	if (m_pHitPoint)
	{
		m_pHitPoint = nullptr;	// 変数NULL
	}
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CPlayer::Update(void)
{
	CGame::GAMESTATE gameState = CManager::GetRenderer()->GetGame()->GetGameState();
	if (m_stateStand != STANDSTATE_SMASHBLOWAWAY &&
		m_stateStand != STANDSTATE_BLOWAWAY && 
		m_stateStand != STANDSTATE_DAUNTED &&
		m_stateStand != STANDSTATE_DOWN && 
		m_stateStand != STANDSTATE_GETUP &&
		m_stateStand != STANDSTATE_GETUP_ACTIVE &&
		(gameState == CGame::GAMESTATE_NORMAL || gameState == CGame::GAMESTATE_BEFORE))
	{
		// 操作
		Control();
	}

	// ストーンパーティクルの更新
	UpdateStoneParticle();

	// 更新
	CCharacter::Update();

	// 当たり判定
	Collision();

	// モーション処理
	StandMotion();

	// 変身処理
	Trans();

#ifdef _DEBUG
	char cText[8];
	sprintf(cText, "%d / ", m_pModelCharacter->GetAllFrame() - m_nAttackFrame);
	CDebugProc::Print(cText);
	sprintf(cText, "%d\n", m_pModelCharacter->GetAllFrame());
	CDebugProc::Print(cText);

	CDebugProc::Print("プレイヤーの位置 [%.4f][%.4f][%.4f]\n", m_pos.x, m_pos.y, m_pos.z);

	ShowDebugInfo();
#endif
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CPlayer *CPlayer::Create(int nPlayer, CHARACTER_TYPE type)
{
	// シーン動的に確保
	CPlayer *pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	// 失敗
	if (!pPlayer)
		return nullptr;

	// プレイヤーのキャラタイプを設定
	pPlayer->m_type = type;
	// プレイヤー番号の保存
	pPlayer->m_nPlayer = nPlayer;
	// 初期化
	pPlayer->Init();

	// 値を返す
	return pPlayer;
}

//==================================================================================================================
// 変身
//==================================================================================================================
void CPlayer::Trans(void)
{
	// 変身していない
	if (!m_bTrans && m_nCntTrans != 0)
	{
		// 変身時間を初期化
		m_nCntTrans = 0;
		// 処理を終える
		return;
	}

	// 変身後、最初のみ
	if (m_bTrans && m_nCntTrans == 0)
	{
		// モデルを変身用にバインド
		m_pModelCharacter->ModelRebind(m_typeTrans);
		// モデルタイプを再設定
		m_pModelCharacter->SetModelType(m_typeTrans);
		// ストーンUIを消す
		CUI_game::TransPlayer(m_nPlayer);
	}

	// 変身時間を加算
	if (m_bTrans)
		m_nCntTrans++;
	// 変身時間中であれば、処理を終える
	if (m_nCntTrans < TIME_TRANS)
		return;

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

	// 変身終了後のUI処理
	CUI_game::FinishTrans(m_nPlayer);
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

//==================================================================================================================
// 操作
//==================================================================================================================
void CPlayer::Control(void)
{
	CInputGamepad *pGamepad = CManager::GetInputGamepad(m_nPlayer);
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pGamepad->GetbConnect())
		ControlGamepad(pGamepad);
	else
		ControlKeyboard(pKeyboard);
}

//==================================================================================================================
// 当たり判定処理
//==================================================================================================================
void CPlayer::Collision(void)
{
	bool bSmashBlowAway = (m_stateStand == STANDSTATE_SMASHBLOWAWAY);

	// 壁の取得
	CWall *pWall = CGame::GetWall();
	// 出力される交点
	D3DXVECTOR3 out_intersect = ZeroVector3;
	// 出力される法線
	D3DXVECTOR3 out_nor = ZeroVector3;

	CPolyCollMana *pPolyCollMana = CGame::GetpolyCollMana();

	for (int nCntPolyColli = 0; nCntPolyColli < CPolygonCollider::GetNumPoly(CGame::GetStageType()); nCntPolyColli++)
	{
		// ポリゴンコライダーの取得
		CPolygonCollider*pPolyColli = pPolyCollMana->GetPolyColl(nCntPolyColli);

		// ポリゴンコライダーポインタがNULLだった時
		if (pPolyColli == NULL)
		{// スキップ
			continue;
		}

		// ポリゴンの範囲内にいるとき
		if (pPolyColli->Test3DInPolygon(&m_pos))
		{
			// 範囲内にいる
			m_bIn[nCntPolyColli] = true;
		}
		else
		{
			// 範囲内にいない
			m_bIn[nCntPolyColli] = false;
		}

		// ポリゴンコライダーの衝突判定
		if (pPolyColli->Collision(&m_pos, &m_posOld, &m_move, &out_intersect, &out_nor, bSmashBlowAway) == true)
		{
			if (bSmashBlowAway == false)
			{
#ifdef _DEBUG
				CDebugProc::Print("乗っている\n");
#endif
				// 地面に乗っていたら、移動量をなくす
				if (m_move.y <= -5.0f)
					m_move.y = -5.0f;

				SetLanding();
			}
			// 出力された法線がゼロじゃない時かつ
			// 出力された交点がゼロじゃない時
			if (bSmashBlowAway == true &&
				out_nor != ZeroVector3 &&
				out_intersect != ZeroVector3)
			{
				// ダメージ
				this->Damage(2);
				// 向きを決定
				this->m_rotDest.y = atan2f(out_nor.x, out_nor.z);
				// 回転を補間
				CKananLibrary::InterpolationFloat(m_rotDest.y);
				// 一瞬で向きを変える
				this->m_rot.y = this->m_rotDest.y;
				CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos, &this->m_move, &out_intersect, &this->m_move, &out_nor);
			}
		}
	}

	//out_intersect = ZeroVector3;
	//out_nor = ZeroVector3;

	// 当たり判定位置の更新
	C3DBoxCollider::ChangePosition(this->m_nBoxColliderID, this->m_pos, MYLIB_3DVECTOR_ZERO);
	// 当たり判定
	if (C3DBoxCollider::CollisionBox(this->m_nBoxColliderID, this->m_pos, m_move, &out_intersect,&out_nor, bSmashBlowAway))
	{
		// 出力された法線がゼロじゃない時かつ
		// 出力された交点がゼロじゃない時
		if (bSmashBlowAway == true &&
			out_nor != ZeroVector3 &&
			out_intersect != ZeroVector3)
		{
			// ダメージ
			this->Damage(2);
			// 向きを決定
			this->m_rotDest.y = atan2f(out_nor.x, out_nor.z);
			// 回転を補間
			CKananLibrary::InterpolationFloat(m_rotDest.y);
			// 一瞬で向きを変える
			this->m_rot.y = this->m_rotDest.y;
			CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos, &this->m_move, &out_intersect, &this->m_move, &out_nor);
		}
		else if(bSmashBlowAway == false)
		{
			SetLanding();
		}
	}



	// 最大ポリゴン数までカウント
	for (int nCntPolyColli = 0; nCntPolyColli < CPolygonCollider::GetNumPoly(CGame::GetStageType()); nCntPolyColli++)
	{
		// ポリゴンコライダーの取得
		CPolygonCollider*pPolyColli = pPolyCollMana->GetPolyColl(nCntPolyColli);

		// 範囲外のとき
		if (!m_bIn[nCntPolyColli])
		{
			// 影処理
			Shadow();
		}
		else
		{
			bool bJump = false;
			if (m_stateStand == STANDSTATE_JUMP)
				bJump = true;
			// 影位置設定
			m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, pPolyColli->GetfHeight(), m_pos.z), m_move, bJump);
			
			break;
		}
	}

	//out_intersect = ZeroVector3;
	//out_nor = ZeroVector3;

	// 壁との当たり判定
	if (pWall->Collision(&m_pos, &m_posOld, &out_intersect, &out_nor, bSmashBlowAway) == true)
	{
		// 出力された法線がゼロじゃない時かつ
		// 出力された交点がゼロじゃない時
		if (bSmashBlowAway == true &&
			out_nor != ZeroVector3 &&
			out_intersect != ZeroVector3)
		{
			// ダメージ
			this->Damage(2);
			// 向きを決定
			this->m_rotDest.y = atan2f(out_nor.x, out_nor.z);
			// 回転を補間
			CKananLibrary::InterpolationFloat(m_rotDest.y);
			// 一瞬で向きを変える
			this->m_rot.y = this->m_rotDest.y;
			CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos, &this->m_move, &out_intersect, &this->m_move, &out_nor);
		}
	}

	// 高さ制限
	if (m_pos.y > HEIGHT_CEILING)
	{
		m_pos.y = HEIGHT_CEILING;
		m_move.y *= -1;
	}

	if (m_pos.y < HEIGHT_FLOOR)
	{
		m_pos.y = HEIGHT_FLOOR;
		m_move.y *= -1;
	}
}

//==================================================================================================================
// スマッシュ
//==================================================================================================================
void CPlayer::Smash(CInputGamepad *pGamepad, CInputKeyboard *pKey)
{
#ifdef _RELEASE
	// 変身していなければ、処理しない
	if (!m_bTrans)
		return;
#endif

	// チャージ中にボタンを離すと、スマッシュ
	if (m_stateStand == STANDSTATE_SMASHCHARGE &&
		((pGamepad && pGamepad->GetbConnect() && !pGamepad->GetPress(CInputGamepad::JOYPADKEY_B)) ||
		(pKey && ((m_nPlayer == PLAYER_ONE && !pKey->GetKeyboardPress(ONE_SMASH)) || (m_nPlayer == PLAYER_TWO && !pKey->GetKeyboardPress(TWO_SMASH))))))
	{
		m_stateStand = STANDSTATE_SMASH;
		return;
	}
	else if (m_nCntState >= TIME_MAX_CHARGE)
	{
		m_stateStand = STANDSTATE_SMASH;
		return;
	}

	// 変身中にボタンを長押しで、スマッシュチャージ
	if ((pGamepad && pGamepad->GetbConnect() && pGamepad->GetPress(CInputGamepad::JOYPADKEY_B)) ||
		(pKey && ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_SMASH)) || (m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_SMASH)))) &&
		m_stateStand != STANDSTATE_SMASH)
	{
		m_stateStand = STANDSTATE_SMASHCHARGE;
		m_nCntState++;
	}
}

//==================================================================================================================
// 通常攻撃
//==================================================================================================================
void CPlayer::NormalAttack(CInputGamepad *pGamepad, CInputKeyboard *pKey)
{
	// キー入力
	if ((m_stateStand == STANDSTATE_NEUTRAL || m_stateStand == STANDSTATE_WALK) &&
		((pGamepad && pGamepad->GetbConnect() && pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_X)) ||
		(pKey && ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardTrigger(ONE_ATTACK)) || (m_nPlayer == PLAYER_TWO && pKey->GetKeyboardTrigger(TWO_ATTACK))))))
	{
		m_stateStand = STANDSTATE_ATTACK;
		m_nCntState = 0;
	}
	else if (m_stateStand != STANDSTATE_JUMP && m_stateStand == STANDSTATE_ATTACK && m_bCancelAttack &&
		((pGamepad && pGamepad->GetbConnect() && pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_X)) ||
		(pKey && ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardTrigger(ONE_ATTACK)) || (m_nPlayer == PLAYER_TWO && pKey->GetKeyboardTrigger(TWO_ATTACK))))))
	{
		m_nCntState = 0;
	}
}

//==================================================================================================================
// ジャンプ
//==================================================================================================================
bool CPlayer::Jump(CInputGamepad *pGamepad, CInputKeyboard *pKey)
{
	// キー入力
	if (m_stateStand != STANDSTATE_JUMP && m_stateStand != STANDSTATE_ATTACK &&
		((pGamepad && pGamepad->GetbConnect() && pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A)) ||
		(pKey && ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardTrigger(ONE_JUMP)) || (m_nPlayer == PLAYER_TWO && pKey->GetKeyboardTrigger(TWO_JUMP))))))
	{
		m_stateStand = STANDSTATE_JUMP;
		m_stateJump = JUMPSTATE_JUMP;
		// 移動値を設定
		m_move.y = m_param.moveParam.fJumpPower;
		// ジャンプ実行
		return true;
	}
	// ジャンプしてない
	return false;
}

//==================================================================================================================
// ジャンプ
//==================================================================================================================
bool CPlayer::AirAttack(CInputGamepad * pGamepad, CInputKeyboard * pKey)
{
	if (m_stateStand == STANDSTATE_JUMP && 
		((pGamepad && pGamepad->GetbConnect() && pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_X)) || 
		( pKey && ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardTrigger(ONE_ATTACK)) ||
		(m_nPlayer == PLAYER_TWO && pKey->GetKeyboardTrigger(TWO_ATTACK))))))
	{
		m_stateJump = JUMPSTATE_ATTACK;
		// ジャンプ攻撃実行
		return true;
	}
	// ジャンプ攻撃していない
	return false;
}

//==================================================================================================================
// 移動入力処理
//==================================================================================================================
bool CPlayer::InputKeyMove(CInputKeyboard * pKey)
{
	bool bMove = false;
	// 変数宣言
	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();	// カメラ取得
	D3DXVECTOR3 rotDest = GetRotDest();										// 目的の向きを格納する変数
	float		CameraRotY = pCamera->GetRotY();								// カメラのY軸回転の取得

	// Aキー長押し
	if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_LEFT)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_LEFT))
	{
		// Wキー長押し
		if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_UP)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_UP))
		{
			// 左上移動
			m_move.x += sinf(-D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = D3DX_PI * 0.75f + CameraRotY;
		}
		// Sキー長押し
		else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_DOWN))
		{
			// 左下移動
			m_move.x += sinf(-D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = D3DX_PI * 0.25f + CameraRotY;
		}
		// Aキーのみ
		else
		{
			// 左移動
			m_move.x += sinf(-D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.x = 0.0f;
			rotDest.y = D3DX_PI * 0.5f + CameraRotY;
		}

		// ジャンプ中でなければ、歩く
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}
	// Dキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_RIGHT)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_RIGHT))
	{
		// Wキー長押し
		if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_UP)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_UP))
		{
			// 右上移動
			m_move.x += sinf(D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.75f + CameraRotY;
		}
		// Sキー長押し
		else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_DOWN))
		{
			// 右下移動
			m_move.x += sinf(D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.25f + CameraRotY;
		}
		// Dキーのみ
		else
		{
			// 右移動
			m_move.x += sinf(D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.5f + CameraRotY;
		}
		
		// ジャンプ中でなければ、歩く
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}
	// Wキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_UP)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_UP))
	{
		// 上移動
		m_move.x += sinf(D3DX_PI * 1.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		m_move.z -= cosf(D3DX_PI * 1.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		// 目的の向きを決定
		rotDest.y = -D3DX_PI * 1.0f + CameraRotY;
	
		// ジャンプ中でなければ、歩く
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}
	// Sキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_DOWN)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_DOWN))
	{
		// 下移動
		m_move.x += sinf(D3DX_PI * 0.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		m_move.z -= cosf(D3DX_PI * 0.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		// 目的の向きを決定
		rotDest.y = CameraRotY;
	
		// ジャンプ中でなければ、歩く
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}

	// 回転の補正
	CKananLibrary::InterpolationRot(&rotDest);

	// 目的の回転の設定
	SetRotDest(rotDest);

	// 移動していない
	return bMove;
}

//==================================================================================================================
// ゲームパッドによる移動入力
//==================================================================================================================
bool CPlayer::InputPadMove(CInputGamepad * pGamepad)
{
	// 移動判定
	bool bMove = false;

	// 左スティック取得
	float fValueX, fValueY;
	pGamepad->GetStickLeft(&fValueX, &fValueY);

	D3DXVECTOR3 rotDest = GetRotDest();	// 目的の向きを格納する変数
	float		CameraRotY = CManager::GetRenderer()->GetGame()->GetCamera()->GetRotY();	// カメラのY軸回転の取得
	float		fSpeed = 0.0f;			// プレイヤーの速度
	float		fAngle = 0.0f;			// スティック角度の計算用変数

	// 角度の計算して補正
	fAngle = atan2f(fValueX, fValueY);
	CKananLibrary::InterpolationFloat(fAngle);

	// スティックの倒れ具合でスピードを決定
	if (abs(fValueX) > abs(fValueY))
		fSpeed = (abs(fValueX));	// 横の倒れ具合
	else
		fSpeed = (abs(fValueY));	// 縦の倒れ具合

	// スティックの角度によってプレイヤー移動
	m_move.x += sinf(fAngle + CameraRotY) * fSpeed * m_param.moveParam.fRunSpeed;
	m_move.z += cosf(fAngle + CameraRotY) * fSpeed * m_param.moveParam.fRunSpeed;

	// 目的の向きを決定
	if (fValueX != 0 || fValueY != 0)
	{
		rotDest.y = D3DX_PI + fAngle + CameraRotY;
		// ジャンプ中でなければ、歩く
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}

	// 回転の補正
	CKananLibrary::InterpolationRot(&rotDest);

	// 目的の回転の設定
	SetRotDest(rotDest);

	return bMove;
}

//==================================================================================================================
// 影関係の更新処理
//==================================================================================================================
void CPlayer::Shadow(void)
{
	bool bJump = false;
	if (m_stateStand == STANDSTATE_JUMP)
		bJump = true;

	// 影位置設定
	m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, C3DBoxCollider::GetHeight(), m_pos.z), m_move, bJump);
}

//==================================================================================================================
// モーション管理関数
//==================================================================================================================
void CPlayer::StandMotion(void)
{
	switch (m_stateStand)
	{
		// ニュートラル
	case STANDSTATE_NEUTRAL:
		MotionNeutral();
		break;
		// 怯み
	case STANDSTATE_DAUNTED:
		MotionDaunted();
		break;
		// 吹っ飛び
	case STANDSTATE_BLOWAWAY:
		MotionBlowAway();
		break;
	case STANDSTATE_SMASHBLOWAWAY:
		MotionSmashBlowAway();
		break;
		// ダウン
	case STANDSTATE_DOWN:
		MotionDown();
		break;
		// 起き上がり
	case STANDSTATE_GETUP:
		MotionGetUp();
		break;
		// 起き上がり
	case STANDSTATE_GETUP_ACTIVE:
		MotionGetUpActive();
		break;
		// 歩行
	case STANDSTATE_WALK:
		MotionWalk();
		break;
		// ジャンプ
	case STANDSTATE_JUMP:
		JumpState();
		break;
		// 攻撃
	case STANDSTATE_ATTACK:
		MotionAttack();
		break;
	case STANDSTATE_SMASHCHARGE:
		MotionSmashCharge();
		SetChargeParticle();
		break;
	case STANDSTATE_SMASH:
		MotionSmash();
		break;
	}

	//// モーション開始時か判別
	//if (m_nCntState == 0)
	//{
	//	m_bMotionBegin = true;
	//}
	//else if (m_nCntState == 1)
	//{
	//	m_bMotionBegin = false;
	//}
}

//==================================================================================================================
// ニュートラルモーション
//==================================================================================================================
void CPlayer::MotionNeutral(void)
{
	// モーション切替
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_NEUTRAL)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_NEUTRAL);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
		// 攻撃ベクトルの初期化
		m_vecP_to_E = ZeroVector3;
		m_stateJump = JUMPSTATE_NONE;
	}
}

//==================================================================================================================
// 歩きモーション
//==================================================================================================================
void CPlayer::MotionWalk(void)
{
	// モーション切替
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_WALK)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_WALK);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
	}
}

//==================================================================================================================
// ダウンモーション
//==================================================================================================================
void CPlayer::MotionDown(void)
{
	// モーション切替
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_DOWN)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_DOWN);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
		m_bInvincible = true;
	}

	// カウント加算
	m_nCntState++;

	CInputGamepad *pGamepad = CManager::GetInputGamepad(m_nPlayer);
	CInputKeyboard *pKey = CManager::GetInputKeyboard();

	// 一定時間内に、操作でアクティブな起き上がり
	if (m_nCntState < TIME_MAX_DOWN &&
		((pGamepad && pGamepad->GetbConnect() && SUCCEEDED(CKananLibrary::GetMoveByGamepad(pGamepad))) ||
		(pKey && SUCCEEDED(CKananLibrary::GetMoveByKeyboard(pKey, m_nPlayer)))))
		m_stateStand = STANDSTATE_GETUP_ACTIVE;
	// 一定時間後に、起き上がり
	else if (m_nCntState >= TIME_MAX_DOWN)
		m_stateStand = STANDSTATE_GETUP;
	// 処理の終了
	else
		return;

	// カウント初期化
	m_nCntState = 0;
}

//==================================================================================================================
// 怯みモーション
//==================================================================================================================
void CPlayer::MotionDaunted(void)
{
	// モーション切替
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_DAUNTED)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_DAUNTED);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
	}

	// 怯み時間を加算
	m_nCntState++;

	// 怯み中
	if (m_nCntState < m_nCntGap)
		return;

	// 怯み解除
	m_stateStand = STANDSTATE_NEUTRAL;
	// 後隙フレームを初期化
	m_nCntGap = 0;
}

//==================================================================================================================
// 吹っ飛びモーション
//==================================================================================================================
void CPlayer::MotionBlowAway(void)
{
	// モーション切替
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_BLOWAWAY)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_BLOWAWAY);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
		m_bInvincible = true;
	}

	// 地面に着く
	if (m_move.y <= -3.0f)
	{
		// ダウン開始
		m_stateStand = STANDSTATE_DOWN;
	}
	else if (m_move.y == 0.0f &&
		m_moveOld.y == 0.0f)
	{
		// ダウン開始
		m_stateStand = STANDSTATE_DOWN;
	}
}

//==================================================================================================================
// スマッシュ吹っ飛びモーション
//==================================================================================================================
void CPlayer::MotionSmashBlowAway(void)
{
	// モーション切替
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_BLOWAWAY)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_BLOWAWAY);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
	}

	if (abs(m_move.x) <= 5.0f &&
		abs(m_move.z) <= 5.0f &&
		abs(m_move.y) <= 5.0f)
	{
		// ダウン開始
		m_stateStand = STANDSTATE_DOWN;
	}
	else if (m_stateStand != STANDSTATE_SMASHBLOWAWAY &&
		abs(m_move.y) <= 1.0f &&
		abs(m_moveOld.y) <= 1.0f)
	{
		// ダウン開始
		m_stateStand = STANDSTATE_DOWN;
	}
}

//==================================================================================================================
// 攻撃モーション
//==================================================================================================================
void CPlayer::MotionAttack(void)
{
	// 最初だけ
	if (m_nCntState == 0)
	{
		// モーションの切り替え
		m_pModelCharacter->SetMotion((CMotion::MOTION_TYPE)(CMotion::PLAYER_ATTACK_0 + m_nAttackFlow));
		// 攻撃が当たったフラグをオフにする
		m_bAttakHit = false;
		m_bAttakHitStone = false;
		// 攻撃フレームを設定
		m_nAttackFrame = m_pModelCharacter->GetAllFrame();
		m_nAttackFlow++;

		int nEnemyID = 0;
		if (m_nPlayer == 0)
			nEnemyID = 1;

		float fDis = CKananLibrary::OutputSqrt(CGame::GetPlayer(nEnemyID)->GetPos() - m_pos);

		if (fDis <= DISTANCE_CHASE_ENEMY)
		{
			// 敵のほうを向く
			RotToEnemy();
			m_fMotionMove = CMotion::GetMotionMove((PARAM_TYPE)(m_type / 2), m_pModelCharacter->GetMotion(), m_pModelCharacter->GetNowKey());
			m_move.x += -m_vecP_to_E.x * m_fMotionMove;
			m_move.z += -m_vecP_to_E.z * m_fMotionMove;
		}
	}

	// 攻撃フレーム加算
	m_nCntState++;

	// キャンセル可能
	if (m_nCntState <= m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.end &&
		m_nCntState >= m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.start && m_nAttackFlow < 4)
		m_bCancelAttack = true;
	else
	{
		m_bCancelAttack = false;
	}

	// 攻撃終了後
	if (m_nCntState > m_nAttackFrame)
	{
		// 攻撃解除
		m_nCntState = 0;
		// モーション切り替え
		m_stateStand = STANDSTATE_NEUTRAL;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
		// 攻撃フレームを初期化
		m_nAttackFrame = 0;
		return;
	}
}

//==================================================================================================================
// スマッシュチャージモーション
//==================================================================================================================
void CPlayer::MotionSmashCharge(void)
{
	// スマッシュチャージ
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH_CHARGE)
	{
		// 敵のほうを向く
		RotToEnemy();
		m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH_CHARGE);
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SMASHCHARGE);
		// 攻撃が当たったフラグをオフにする
		m_bAttakHit = false;
	}
}

//==================================================================================================================
// スマッシュモーション
//==================================================================================================================
void CPlayer::MotionSmash(void)
{
	// スマッシュ
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH)
	{
		// 敵のほうを向く
		RotToEnemy();
		m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH);
		// 攻撃フレームを設定
		m_nAttackFrame = m_pModelCharacter->GetAllFrame();

		m_fMotionMove = CMotion::GetMotionMove((PARAM_TYPE)(m_type / 2), m_pModelCharacter->GetMotion(), m_pModelCharacter->GetNowKey());
		C3DParticle::Set(&m_pos, &m_rot, C3DParticle::OFFSETNAME::SMASHATTACKSTART);
		m_nCntChargeParticle = 0;
		m_nCntTimingChargeParticle = 0;
		m_nTimingChargeParticle = 0;
	}

	m_nCntState++;

	m_move.x += -m_vecP_to_E.x * m_fMotionMove;
	m_move.z += -m_vecP_to_E.z * m_fMotionMove;

	if (m_nCntState >= m_nAttackFrame)
	{
		m_stateStand = STANDSTATE_NEUTRAL;
		m_nCntState = 0;
	}
	else
	{
		if (m_nCntSmashDashParticle == 7)
		{
			m_nCntSmashDashParticle = 0;
			// パーツのポインタ
			CModelParts *pParts = &m_pModelCharacter->GetModelParts()[CModelParts::PARTSNAME_UPARM_R];
			D3DXVECTOR3 ParticlePos = D3DXVECTOR3(pParts->GetMtx()->_41, pParts->GetMtx()->_42, pParts->GetMtx()->_43);
			C3DParticle::Set(&ParticlePos, pParts->GetRot(), (C3DParticle::OFFSETNAME::SMASHDASH));
		}
		else
		{
			m_nCntSmashDashParticle++;
		}
	}
}

//==================================================================================================================
// 起き上がりモーション
//==================================================================================================================
void CPlayer::MotionGetUp(void)
{
	// 起き上がり
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_GETUP)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_GETUP);
		m_nCntState = 0;
	}
	m_nCntState++;

	if (m_nCntState >= TIME_GETUP)
	{
		m_stateStand = STANDSTATE_NEUTRAL;
		m_bInvincible = false;
	}
}

//==================================================================================================================
// 起き上がりモーション
//==================================================================================================================
void CPlayer::MotionGetUpActive(void)
{
	// 起き上がり
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_GETUP_ACTIVE)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_GETUP_ACTIVE);
		m_nCntState = 0;
	}
	m_nCntState++;

	if (m_nCntState >= TIME_GETUP_ACTIVE)
	{
		m_stateStand = STANDSTATE_NEUTRAL;
		m_bInvincible = false;
	}
}

//==================================================================================================================
// スマッシュの判定
//==================================================================================================================
bool CPlayer::SmashJudge(void)
{
	if (m_bTrans &&
		m_stateStand == STANDSTATE_SMASH || 
		m_stateStand == STANDSTATE_SMASHCHARGE)
	{
		return true;
	}

	return false;
}

//==================================================================================================================
// ジャンプの判定
//==================================================================================================================
bool CPlayer::JumpJudge(void)
{
	if (m_stateStand == STANDSTATE_JUMP)
	{
		return true;
	}

	return false;
}

//==================================================================================================================
// ジャンプ中の処理
//==================================================================================================================
void CPlayer::JumpState(void)
{
	switch (m_stateJump)
	{
		// ジャンプモーション
	case JUMPSTATE_JUMP:
		MotionJump();
		break;
		// 落下モーション
	case JUMPSTATE_FALL:
		MotionFall();
		break;
		// 攻撃モーション
	case JUMPSTATE_ATTACK:
		MotionAirAttack();
		break;
	}
}

//==================================================================================================================
// ジャンプモーション
//==================================================================================================================
void CPlayer::MotionJump(void)
{
	// ジャンプモーション
	if ( m_pModelCharacter->GetMotion() != CMotion::PLAYER_JUMP)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_JUMP);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
	}

	// 状態カウントを加算
	m_nCntState++;
	// 一定時間で落下モーションに移行
	if (m_nCntState >= TIME_JUMP_TO_FALL)
		m_stateJump = JUMPSTATE_FALL;
}

//==================================================================================================================
// 落下モーション
//==================================================================================================================
void CPlayer::MotionFall(void)
{
	// 落下モーション
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_FALL)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_FALL);
	}
}

//==================================================================================================================
// 空中攻撃モーション
//==================================================================================================================
void CPlayer::MotionAirAttack(void)
{
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_AIRATTACK)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_AIRATTACK);
		m_nCntState = 0;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
		// 攻撃のフラグをオフにする
		m_bAttakHit = false;
		m_bAttakHitStone = false;

		// 敵のIDを取得
		int nEnemyID = 0;
		if (m_nPlayer == 0)
			nEnemyID = 1;

		D3DXVECTOR3 difPos = CGame::GetPlayer(nEnemyID)->GetPos() - m_pos;

		// 一定の距離以内
		if (CKananLibrary::OutputSqrt(difPos) <= DISTANCE_AIR_CAHSE_ENEMY)
		{
			// プレイヤーから敵へのベクトルを取得
			m_vecP_to_E = CKananLibrary::OutputVector(CGame::GetPlayer(nEnemyID)->GetPos(), m_pos);
			// 向きを決定
			m_rotDest.y = atan2f((float)m_vecP_to_E.x, (float)m_vecP_to_E.z);
			// 即振り向き
			m_rot.y = m_rotDest.y;

			float fDisPosY = CGame::GetPlayer(nEnemyID)->GetPos().y + VALUE_DIFPOS_Y - m_pos.y + VALUE_DIFPOS_Y;

			m_move.y = 0.0f;

			// 移動値を決定
			m_AirMove.x = sqrt(difPos.x * difPos.x) / TIME_AIR_ATTACK;
			m_AirMove.y = sqrt(fDisPosY * fDisPosY) / TIME_AIR_ATTACK;
			m_AirMove.z = sqrt(difPos.z * difPos.z) / TIME_AIR_ATTACK;
		}
	}

	float fmoveY = -m_vecP_to_E.y * m_AirMove.y;
	if (fmoveY >= 0)
		fmoveY = -0.1f;

	// 移動
	m_move.x += -m_vecP_to_E.x * m_AirMove.x;
	m_move.y += fmoveY;
	m_move.z += -m_vecP_to_E.z * m_AirMove.z;
}

//==================================================================================================================
// 敵のほうを向く
//==================================================================================================================
void CPlayer::RotToEnemy(void)
{
	// 敵のIDを決定
	int nEnemyID = 0;
	if (m_nPlayer == 0)
		nEnemyID = 1;

	// プレイヤーから敵へのベクトルを取得
	m_vecP_to_E = CKananLibrary::OutputVector(CGame::GetPlayer(nEnemyID)->GetPos(), m_pos);

	m_rotDest.y = atan2f((float)m_vecP_to_E.x, (float)m_vecP_to_E.z);
}

//==================================================================================================================
// 着地処理
//==================================================================================================================
void CPlayer::SetLanding(void)
{
	if (m_stateStand == STANDSTATE_JUMP)
	{
		// ジャンプを初期化
		m_stateJump = JUMPSTATE_NONE;
		// ジャンプ解除
		m_stateStand = STANDSTATE_NEUTRAL;
	}
}

//==================================================================================================================
// 怯み処理
//==================================================================================================================
void CPlayer::Daunted(const int nGap)
{
	// カウントを設定し、怯み状態に移行
	m_pModelCharacter->ResetMotion();
	m_nCntState = 0;
	m_nCntGap = nGap;
	m_stateStand = STANDSTATE_DAUNTED;
	m_pModelCharacter->SetMotion(CMotion::PLAYER_DAUNTED);
}

//==================================================================================================================
// リセット
//==================================================================================================================
void CPlayer::ResetPlayer(void)
{
	// 値の初期化
	SetPos(m_posBegin);
	SetRot(ZeroVector3);
	SetRotDest(ZeroVector3);
	SetMove(ZeroVector3);
	SetLife(m_param.fMaxLife);
	// 状態の初期化
	m_bTrans = false;
	m_stateStand = STANDSTATE_NEUTRAL;
	m_stateJump = JUMPSTATE_NONE;
	m_nCntState = 0;
	// モデルを通常にリバインド
	m_pModelCharacter->ModelRebind(m_type);
}

//==================================================================================================================
// 攻撃判定
//==================================================================================================================
void CPlayer::CollisionAttack(void)
{
	// 違うプレイヤーの取得
	CPlayer *pAnother = GetAnotherPlayer();

	// 違うプレイヤーが攻撃を当てたフラグが立ってない時
	if (pAnother->m_bAttakHit == false)
	{
		// 別のプレイヤーのモーションを比較
		switch (pAnother->m_pModelCharacter->GetMotion())
		{
		case CMotion::PLAYER_ATTACK_0:
			// 別のプレイヤーが攻撃0をしている時
			this->AnotherPlayerAttack0(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_1:
			// 別のプレイヤーが攻撃1をしている時
			this->AnotherPlayerAttack1(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_2:
			// 別のプレイヤーが攻撃2をしている時
			this->AnotherPlayerAttack2(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_3:
			// 別のプレイヤーが攻撃3をしている時
			this->AnotherPlayerAttack3(pAnother);
			break;
		case CMotion::PLAYER_SMASH:
			// 別のプレイヤーがスマッシュ攻撃しているとき
			this->AnotherPlayerSmash(pAnother);
			break;
		}
	}
}

//==================================================================================================================
// 攻撃当てる準備かできているか
//==================================================================================================================
bool CPlayer::ReadyToHit(void)
{
	if (this->m_bAttakHit == false)
	{
		switch (this->m_pModelCharacter->GetMotion())
		{
		case CMotion::PLAYER_ATTACK_0: return true;
		case CMotion::PLAYER_ATTACK_1: return true;
		case CMotion::PLAYER_ATTACK_2: return true;
		case CMotion::PLAYER_ATTACK_3: return true;
		case CMotion::PLAYER_AIRATTACK: return true;
		case CMotion::PLAYER_SMASH:    return true;
		}
	}
	return false;
}

//==================================================================================================================
// 攻撃当てる準備かできているか
//==================================================================================================================
bool CPlayer::ReadyToHitStone(void)
{
	if (this->m_bAttakHitStone == false)
	{
		switch (this->m_pModelCharacter->GetMotion())
		{
		case CMotion::PLAYER_ATTACK_0: return true;
		case CMotion::PLAYER_ATTACK_1: return true;
		case CMotion::PLAYER_ATTACK_2: return true;
		case CMotion::PLAYER_ATTACK_3: return true;
		case CMotion::PLAYER_AIRATTACK: return true;
		case CMotion::PLAYER_SMASH:    return true;
		}
	}
	return false;
}

//==================================================================================================================
//　攻撃0を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack0(const int &nCapColliID)
{
	if(nCapColliID != CCapsuleCollider::TYPEID_FOREARM_L ||
		nCapColliID != CCapsuleCollider::TYPEID_UPPERARM_L)
	{
		return false;
	}
	int nKey = CMotion::GetAttackKey((PARAM_TYPE)(m_type / 2),this->m_pModelCharacter->GetMotion());

	return (nKey == m_pModelCharacter->GetNowKey());
}

//==================================================================================================================
//　攻撃1を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack1(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
//　攻撃2を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack2(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_L) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_L);
}

//==================================================================================================================
//　攻撃3を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack3(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
//　スマッシュ攻を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionSmash(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
//　モーションの取得
//==================================================================================================================
CMotion::MOTION_TYPE CPlayer::GetMotion(void)
{
	return m_pModelCharacter->GetMotion();
}

//==================================================================================================================
// 違うプレイヤーの取得
//==================================================================================================================
CPlayer * CPlayer::GetAnotherPlayer(void)
{
	return (this->m_nPlayer == PLAYER_ONE) ? CGame::GetPlayer(PLAYER_TWO) : CGame::GetPlayer(PLAYER_ONE);
}

//==================================================================================================================
// ゲームパッドの操作
//==================================================================================================================
void CPlayer::ControlGamepad(CInputGamepad * pGamepad)
{
	// ゲームパッドのスティック情報の取得用
	float fValueX, fValueY;

	// 左スティック取得
	pGamepad->GetStickLeft(&fValueX, &fValueY);

	// 何も入力されていなければ、処理しない
	if (FAILED(CKananLibrary::GetMoveByGamepad(pGamepad)) &&
		fValueX == 0 && fValueY == 0 &&
		!SmashJudge() &&
		!JumpJudge())
	{
		// ジャンプ中でなければ、ニュートラル
		if (m_stateStand != STANDSTATE_ATTACK)
			m_stateStand = STANDSTATE_NEUTRAL;
		return;
	}

	if (CGame::GetGameState() == CGame::GAMESTATE_NORMAL)
	{
		// スマッシュ
		Smash(pGamepad, nullptr);

		// スマッシュ系モーション中は以降の処理をしない
		if (m_stateStand == STANDSTATE_SMASHCHARGE ||
			m_stateStand == STANDSTATE_SMASH)
			// 処理を終える
			return;

		// 攻撃入力
		NormalAttack(pGamepad, nullptr);

		// 攻撃中であれば、処理を終える
		if (m_stateStand == STANDSTATE_ATTACK)
			return;

		// 空中攻撃
		AirAttack(pGamepad, nullptr);

		if (m_stateJump == JUMPSTATE_ATTACK)
			return;
	}

	// ジャンプ
	Jump(pGamepad, nullptr);

	// 移動
	InputPadMove(pGamepad);
}

//==================================================================================================================
// キーボードの操作
//==================================================================================================================
void CPlayer::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// 入力されていなければ処理を終える
	if (FAILED(CKananLibrary::GetMoveByKeyboard(pKeyboard, m_nPlayer)) &&
		!SmashJudge() &&
		!JumpJudge())
	{
		// 条件付きで、ニュートラル
		if (m_stateStand != STANDSTATE_ATTACK)
			m_stateStand = STANDSTATE_NEUTRAL;
		return;
	}

	// 攻撃はゲーム中のみ
	if (CGame::GetGameState() == CGame::GAMESTATE_NORMAL)
	{
		// スマッシュ
		Smash(nullptr, pKeyboard);

		// スマッシュ系モーション中は以降の処理をしない
		if (m_stateStand == STANDSTATE_SMASHCHARGE ||
			m_stateStand == STANDSTATE_SMASH)
			return;

		// 通常攻撃
		NormalAttack(nullptr, pKeyboard);

		// 攻撃中であれば、処理を終える
		if (m_stateStand == STANDSTATE_ATTACK)
			return;

		// 空中攻撃
		AirAttack(nullptr, pKeyboard);

		if (m_stateJump == JUMPSTATE_ATTACK)
			return;
	}

	// ジャンプ
	Jump(nullptr, pKeyboard);

	// 移動
	InputKeyMove(pKeyboard);
}

//==================================================================================================================
// ストーンの取得判定
//==================================================================================================================
void CPlayer::CatchStone(CStone *pStone)
{
	// ステージのストーン配置数を減らす
	CGame::RemoveNumStone(pStone->GetIndexPos());
	// 取得ストーンのタイプを有効
	int nStoneID = pStone->GetStoneID();
	// ストーンUIを表示
	CUI_game::CatchStone(m_nPlayer, (CStone::STONE_ID)nStoneID);
	m_bGetStoneType[nStoneID] = true;
	// ストーンの取得
	pStone->Catch();

	// ストーンの取得数を加算
	m_nNumStone++;
	// 3つ取得している
	if (m_nNumStone >= 3)
	{
		// 変身
		this->m_bTrans = true;
		// SE
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_TRANSFORM);
		// BGM
		CRenderer::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_TRANS);
	}
	// それ以外
	else
	{
		// SE
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETSTONE);
	}
}

//==================================================================================================================
// 別のプレイヤーが攻撃0している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack0(CPlayer * pAnother)
{
	PARAM_TYPE type = (PARAM_TYPE)(pAnother->GetCharaType() / 2);
	// ダメージ
	this->Damage(CCharaParam::GetAttackDamage(type, CCharaParam::ATTACK_NORMAL_1));
	if (!m_bTrans)
		// 怯み
		this->Daunted(CCharaParam::GetAttackDaunted(type, CCharaParam::ATTACK_NORMAL_1));
}

//==================================================================================================================
// 別のプレイヤーが攻撃1している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack1(CPlayer * pAnother)
{
	PARAM_TYPE type = (PARAM_TYPE)(pAnother->GetCharaType() / 2);
	// ダメージ
	this->Damage(CCharaParam::GetAttackDamage(type, CCharaParam::ATTACK_NORMAL_2));
	if (!m_bTrans)
		// 怯み
		this->Daunted(CCharaParam::GetAttackDaunted(type, CCharaParam::ATTACK_NORMAL_2));
}

//==================================================================================================================
// 別のプレイヤーが攻撃2している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack2(CPlayer * pAnother)
{
	PARAM_TYPE type = (PARAM_TYPE)(pAnother->GetCharaType() / 2);
	// ダメージ
	this->Damage(CCharaParam::GetAttackDamage(type, CCharaParam::ATTACK_NORMAL_3));
	if (!m_bTrans)
		// 怯み
		this->Daunted(CCharaParam::GetAttackDaunted(type, CCharaParam::ATTACK_NORMAL_3));
}

//==================================================================================================================
// 別のプレイヤーが攻撃3している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack3(CPlayer * pAnother)
{
	PARAM_TYPE type = (PARAM_TYPE)(pAnother->GetCharaType() / 2);
	// ダメージ
	this->Damage(CCharaParam::GetAttackDamage(type, CCharaParam::ATTACK_NORMAL_4));
	// 変身中以外は吹き飛ぶ
	if (!m_bTrans)
	{
		// 吹き飛び
		BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_NORMAL);
		// 吹き飛びを有効
		m_stateStand = STANDSTATE_BLOWAWAY;
		if (m_nNumStone > 0)
		{
			// 所持ストーンを一つ減らす
			m_nNumStone--;
			// 減らすストーンのタイプを取得
			int nRemoveType = CKananLibrary::DecideRandomValue(CStone::STONE_ID_MAX, m_bGetStoneType, true);
			// ストーンのUIをなくす
			CUI_game::ReleaseStone(m_nPlayer, (CStone::STONE_ID)nRemoveType);
			// 使っていない状態に戻す
			m_bGetStoneType[nRemoveType] = false;
			// 再配置できるようストーンを使用されていない状態にする
			CGame::RemoveTypeStone(nRemoveType);
			// 減らしたストーンを即生成
			CGame::AppearStone();
		}
	}
}

//==================================================================================================================
// 別のプレイヤーが空中攻撃している時
//==================================================================================================================
void CPlayer::AnotherPlayerAirAttack(CPlayer * pAnother)
{
	PARAM_TYPE type = (PARAM_TYPE)(pAnother->GetCharaType() / 2);
	// ダメージ
	this->Damage(CCharaParam::GetAttackDamage(type, CCharaParam::ATTACK_AIR_ATTACK));
	// 変身中以外は吹き飛ぶ
	if (!m_bTrans)
	{
		// 吹き飛び
		BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_NORMAL);
		// 吹き飛びを有効
		m_stateStand = STANDSTATE_BLOWAWAY;
		m_stateJump = JUMPSTATE_NONE;
		if (m_nNumStone > 0)
		{
			// 所持ストーンを一つ減らす
			m_nNumStone--;
			// 減らすストーンのタイプを取得
			int nRemoveType = CKananLibrary::DecideRandomValue(CStone::STONE_ID_MAX, m_bGetStoneType, true);
			// ストーンのUIをなくす
			CUI_game::ReleaseStone(m_nPlayer, (CStone::STONE_ID)nRemoveType);
			// 使っていない状態に戻す
			m_bGetStoneType[nRemoveType] = false;
			// 再配置できるようストーンを使用されていない状態にする
			CGame::RemoveTypeStone(nRemoveType);
			// 減らしたストーンを即生成
			CGame::AppearStone();
		}
	}
}

//==================================================================================================================
// 別のプレイヤーがスマッシュ攻撃している時
//==================================================================================================================
void CPlayer::AnotherPlayerSmash(CPlayer * pAnother)
{
	PARAM_TYPE type = (PARAM_TYPE)(pAnother->GetCharaType() / 2);
	// ダメージ
	this->Damage(CCharaParam::GetAttackDamage(type, CCharaParam::ATTACK_SMASH));
	// 変身中以外は吹き飛ぶ
	BlowAway(pAnother, 0.2f, BLOWAWAYFORCE_SMASH);
	// スマッシュによる吹き飛びを実行
	m_stateStand = STANDSTATE_SMASHBLOWAWAY;
}

//==================================================================================================================
//ダメージを受ける
//==================================================================================================================
void CPlayer::TakeDamage(CPlayer * pAnother, const int nAttackMotion)
{
	switch (nAttackMotion)
	{
	case CMotion::PLAYER_ATTACK_0: AnotherPlayerAttack0(pAnother);return;
	case CMotion::PLAYER_ATTACK_1: AnotherPlayerAttack1(pAnother);return;
	case CMotion::PLAYER_ATTACK_2: AnotherPlayerAttack2(pAnother);return;
	case CMotion::PLAYER_ATTACK_3: AnotherPlayerAttack3(pAnother);return;
	case CMotion::PLAYER_AIRATTACK: AnotherPlayerAttack3(pAnother); return;
	case CMotion::PLAYER_SMASH:    AnotherPlayerSmash(pAnother);return;
	}

	// 当てたフラグを立てる
	pAnother->m_bAttakHit = true;
}

//==================================================================================================================
// 攻撃3のダメージを受ける
//==================================================================================================================
void CPlayer::TakeAttack3Damage(CPlayer * pAnother)
{
	// ダメージ
	this->Damage(2);
	// 変身中以外は吹き飛ぶ
	if (!m_bTrans)
	{
		// 吹き飛び
		BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_NORMAL);
		// 吹き飛びを有効
		m_stateStand = STANDSTATE_BLOWAWAY;
		if (m_nNumStone > 0)
		{
			// 所持ストーンを一つ減らす
			m_nNumStone--;
			// 再配置できるようストーンを使用されていない状態にする
			CGame::RemoveTypeStone(CKananLibrary::DecideRandomValue(m_nNumStone + 1, m_bGetStoneType, true));
			// 減らしたストーンを即生成
			CGame::AppearStone();
		}
	}
}

//==================================================================================================================
// スマッシュ攻撃のダメージを受ける
//==================================================================================================================
void CPlayer::TakeSmashDamage(CPlayer * pAnother)
{
	// ダメージ
	this->Damage(2);
	// 変身中以外は吹き飛ぶ
	BlowAway(pAnother, 0.0f, BLOWAWAYFORCE_SMASH);
	// スマッシュによる吹き飛びを実行
	m_stateStand = STANDSTATE_SMASHBLOWAWAY;
	// 当てたフラグを立てる
	pAnother->m_bAttakHit = true;
}


//==================================================================================================================
// 被弾時の効果音
//==================================================================================================================
void CPlayer::SetHitSound()
{
	switch (m_pModelCharacter->GetMotion())
	{
	case CMotion::PLAYER_ATTACK_0:
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HIT1);
		break;
	case CMotion::PLAYER_ATTACK_1:
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HIT1);
		break;
	case CMotion::PLAYER_ATTACK_2:
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HIT2);
		break;
	case CMotion::PLAYER_ATTACK_3:
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HIT3);
		break;
	case CMotion::PLAYER_AIRATTACK:
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_HIT3);
		break;
	case CMotion::PLAYER_SMASH:
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SMASHHIT);
		break;
	}

}

//==================================================================================================================
// 吹き飛ぶ
//==================================================================================================================
inline bool CPlayer::BlowAway(CPlayer *pAnother, const float MoveVecY, const float fBlowAwayForce)
{
	// NULLだった時処理しない
	if (pAnother == NULL)
	{
		return false;
	}
	// 変数宣言
	D3DXVECTOR3 MoveVec;	// 移動ベクトル

	MoveVec.x = sinf(pAnother->m_rot.y + D3DX_PI);
	MoveVec.y = MoveVecY;
	MoveVec.z = cosf(pAnother->m_rot.y + D3DX_PI);

	// ぶっ飛びモーション
	m_pModelCharacter->ResetMotion();
	m_pModelCharacter->SetMotion(CMotion::PLAYER_BLOWAWAY);
	// 向きを決定
	m_rotDest.y = pAnother->m_rot.y + D3DX_PI;
	// 回転の補間
	CKananLibrary::InterpolationFloat(m_rotDest.y);
	// 一瞬で向きを変える
	m_rot.y = m_rotDest.y;

	// 移動値に加算
	this->m_move.x = MoveVec.x * fBlowAwayForce;
	this->m_move.z = MoveVec.z * fBlowAwayForce;
	this->m_move.y = MoveVec.y * fBlowAwayForce;

	return true;
}

//==================================================================================================================
// ストーンパーティクルの更新
//==================================================================================================================
void CPlayer::UpdateStoneParticle(void)
{
	if (++m_nCntParticle == 1)
	{
		if (m_bGetStoneType[0] == true)
		{
			C3DParticle::Set(&m_pos, &m_rot, C3DParticle::OFFSETNAME::STONEHAVE_R);
		}
		if (m_bGetStoneType[1] == true)
		{
			C3DParticle::Set(&m_pos, &m_rot, C3DParticle::OFFSETNAME::STONEHAVE_G);
		}
		if (m_bGetStoneType[2] == true)
		{
			C3DParticle::Set(&m_pos, &m_rot, C3DParticle::OFFSETNAME::STONEHAVE_B);
		}
		m_nCntParticle = 0;
	}
}

//==================================================================================================================
// チャージパーティクルの設定
//==================================================================================================================
void CPlayer::SetChargeParticle(void)
{
	static const int aOffsetName[3] =
	{
		C3DParticle::CHARGE_R,
		C3DParticle::CHARGE_G,
		C3DParticle::CHARGE_B,
	};

	// パーツのポインタ
	CModelParts *pParts = &m_pModelCharacter->GetModelParts()[CModelParts::PARTSNAME_UPARM_R];
	D3DXVECTOR3 ParticlePos = D3DXVECTOR3(pParts->GetMtx()->_41, pParts->GetMtx()->_42, pParts->GetMtx()->_43);



	if (CHARGEPARTICLE_MAX_CHARGE == m_nCntTimingChargeParticle)
	{
		C3DParticle::Set(&ParticlePos, pParts->GetRot(), (C3DParticle::OFFSETNAME)aOffsetName[0]);
		C3DParticle::Set(&ParticlePos, pParts->GetRot(), (C3DParticle::OFFSETNAME)aOffsetName[1]);
		C3DParticle::Set(&ParticlePos, pParts->GetRot(), (C3DParticle::OFFSETNAME)aOffsetName[2]);
	}
	else if ((CHARGEPARTICLE_MAX_CHARGE - m_nCntTimingChargeParticle) == m_nTimingChargeParticle)
	{
		C3DParticle::Set(&ParticlePos, pParts->GetRot(), (C3DParticle::OFFSETNAME)aOffsetName[m_nCntChargeParticle]);
		m_nCntChargeParticle++;
		if (m_nCntChargeParticle == 3)
		{
			m_nCntChargeParticle = 0;
		}
		m_nCntTimingChargeParticle++;
		m_nTimingChargeParticle = 0;
	}
	m_nTimingChargeParticle++;

}

#ifdef _DEBUG
//==================================================================================================================
// ImGuiの更新
//==================================================================================================================
void CPlayer::ShowDebugInfo()
{
	char cHead[8];
	sprintf(cHead, "Player%d", m_nPlayer);			// プレイヤー番号を文字列に追加

	if (ImGui::CollapsingHeader(cHead))
	{
		int nAllFrame = m_pModelCharacter->GetAllFrame();
		// 情報の表示
		CKananLibrary::ShowOffsetInfo(GetPos(), GetRot(), GetMove());
		ImGui::Text("movOld (%.4f, %.4f, %.4f)", m_moveOld.x, m_moveOld.y, m_moveOld.z);
		ImGui::Text("nLife       : %f", m_nLife);
		ImGui::Text("AttackFlow  : %d", m_nAttackFlow);
		ImGui::Text("AttackFrame : %d / %d", m_nCntState, m_nAttackFrame);
		if (m_stateStand == STANDSTATE_ATTACK)
		ImGui::Text("CancelFrame : %d ~ %d", m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.start,
			m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.end);
		ImGui::Text("GetNumStone : %d", m_nNumStone);
		if (m_bTrans)
			ImGui::Text("TransTime   : %d", TIME_TRANS - m_nCntTrans);
		ImGui::Text("bInvincible : %d", m_bInvincible);
	}
}
#endif
