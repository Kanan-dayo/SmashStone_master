
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sceneX.h"
#include "fade.h"
#include "character.h"
#include "motion.h"

#define TEXTURE_PLAYER "data/TEXTURE/field000.jpg"
#define PLAYER_MODEL "data/MODEL/testmodel.x"		
#define PLAYER_MAX_MODEL (6)						

class CCamera;
class CMotionModel;
class CMeshField;
class CInputKeyboard;
class CInputGamepad;
class CFade;
class CHitPoint;
class CStone;
class CShadow;

class CPlayer : public CCharacter
{
public:
	CPlayer(PRIORITY type);					
	~CPlayer();								
	void Init(void);						
	void Uninit(void);						
	void Update(void);						
	void Draw(void);						

	static CPlayer *Create(int nPlayer, CHARACTER_TYPE type);	

	int GetNumStone(void)	{ return m_nNumStone; }	
	int GetnPlayer(void) { return m_nPlayer; }		
	inline int GetBoxColliderID(void) { return m_nBoxColliderID; }
	inline bool & GetbMotionBegin(void) { return m_bMotionBegin; }
	CMotion::MOTION_TYPE GetMotion(void);
	CPlayer*GetAnotherPlayer(void);				

	void Daunted(const int nGap);	// 怯み
	void ResetPlayer(void);			// リセット

	bool ReadyToHit(void);			
	bool ReadyToHitStone(void);
	bool HitConditionAttack0(const int &nCapColliID);	
	bool HitConditionAttack1(const int &nCapColliID);	
	bool HitConditionAttack2(const int &nCapColliID);	
	bool HitConditionAttack3(const int &nCapColliID);	
	bool HitConditionSmash(const int &nCapColliID);		

	void SetHitSound();	
	void CatchStone(CStone *pStone);	
	CShadow *GetShadow(void) { return m_pShadow; }	// キャラクターの影取得処理

	void TakeDamage(CPlayer * pAnother, const int nAttackMotion);

protected:

private:
	void Trans(void);
	void Control(void);								
	void Collision(void);							
	void InvincibleManager(void);
	void Smash(CInputGamepad *pGamepad, CInputKeyboard *pKey);
	void NormalAttack(CInputGamepad *pGamepad, CInputKeyboard *pKey);
	bool Jump(CInputGamepad *pGamepad, CInputKeyboard *pKey);
	bool AirAttack(CInputGamepad *pGamepad, CInputKeyboard *pKey);
	bool InputKeyMove(CInputKeyboard *pKey);
	bool InputPadMove(CInputGamepad *pGamepad);
	void Shadow(void);								// 影関係の更新処理

	void StandMotion(void);
	void MotionNeutral(void);
	void MotionWalk(void);
	void MotionDown(void);
	void MotionDaunted(void);
	void MotionBlowAway(void);
	void MotionSmashBlowAway(void);
	void MotionAttack(void);
	void MotionSmashCharge(void);
	void MotionSmash(void);
	void MotionGetUp(void);
	void MotionGetUpActive(void);
	bool SmashJudge(void);
	bool JumpJudge(void);

	void JumpState(void);
	void MotionJump(void);
	void MotionFall(void);
	void MotionAirAttack(void);

	void RotToEnemy(void);
	
	void SetLanding(void);

	void CollisionAttack(void);						

	CHitPoint *m_pHitPoint;					
	CShadow *m_pShadow;								// 影の情報ポインタ

	void ControlGamepad(CInputGamepad *pGamepad);	
	void ControlKeyboard(CInputKeyboard *pKeyboard);

	void AnotherPlayerAttack0(CPlayer *pAnother);	
	void AnotherPlayerAttack1(CPlayer *pAnother);	
	void AnotherPlayerAttack2(CPlayer *pAnother);	
	void AnotherPlayerAttack3(CPlayer *pAnother);	
	void AnotherPlayerAirAttack(CPlayer *pAnother);
	void AnotherPlayerSmash(CPlayer *pAnother);		

	void TakeAttack3Damage(CPlayer * pAnother);		
	void TakeSmashDamage(CPlayer * pAnother);		


	bool BlowAway(CPlayer *pAnother, const float MoveVecY, const float fBlowAwayForce);

	void UpdateStoneParticle(void);					// ストーンパーティクルの更新

	void SetChargeParticle(void);

	int m_nPlayer;			
	int m_nBoxColliderID;	
	int m_nCntState;		// 状態管理用のカウンタ
	int m_nCntGap;			// 被ダメージ時の後隙カウンタ
	float m_fMotionMove;	// モーション時の移動量
	D3DXVECTOR3 m_AirMove;	// 空中攻撃時の移動値
	int	m_nAttackFrame;			// 攻撃モーション切り替えのカウンタ

	bool m_bIn[3];		// ポリゴンの範囲内フラグ
	bool m_bCancelAttack;	// 攻撃のキャンセルフラグ
	bool m_bMotionBegin;	// モーションの最初かどうか
	D3DXVECTOR3 m_vecP_to_E;	// プレイヤーから敵までのベクトル

	int m_nCntParticle;				// パーティクルカウント
	int m_nCntChargeParticle;		// チャージパーティクルのカウント
	int m_nTimingChargeParticle;	// チャージパーティクルのタイミング
	int m_nCntTimingChargeParticle;	// チャージパーティクルのタイミングのカウント
	int m_nCntSmashDashParticle;	// スマッシュダッシュカウント
#ifdef _DEBUG
	void ShowDebugInfo(void);									
#endif
};
#endif