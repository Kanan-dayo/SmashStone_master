
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sceneX.h"
#include "fade.h"
#include "character.h"


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
	CPlayer*GetAnotherPlayer(void);				


	bool ReadyToHit(const int &nCapColliID);			
	bool HitConditionAttack0(const int &nCapColliID);	
	bool HitConditionAttack1(const int &nCapColliID);	
	bool HitConditionAttack2(const int &nCapColliID);	
	bool HitConditionAttack3(const int &nCapColliID);	
	bool HitConditionSmash(const int &nCapColliID);		

	void SetHitSound();	
	void CatchStone(CStone *pStone);	
	CShadow *GetShadow(void) { return m_pShadow; }	// キャラクターの影取得処理

protected:

private:
	void Control(void);								
	void Collision(void);							
	void Smash(void);								
	void NormalAttack(void);						
	void Jump(void);								
	void Lift(void);								
	void Shadow(void);								// 影関係の更新処理

	void Motion(void);
	void MotionNeutral(void);
	void MotionWalk(void);
	void MotionLift(void);
	void MotionJump(void);
	void MotionDown(void);
	void MotionDaunted(void);
	void MotionBlowAway(void);
	void MotionAttack(void);
	void MotionGetUp(void);

	void CollisionAttack(void);						

	static CHitPoint *m_pHitPoint;					
	CShadow *m_pShadow;								// 影の情報ポインタ

	void ControlGamepad(CInputGamepad *pGamepad);	
	void ControlKeyboard(CInputKeyboard *pKeyboard);
	void SetnPlayer(int nPlayerNum);				

	void AnotherPlayerAttack0(CPlayer *pAnother);	
	void AnotherPlayerAttack1(CPlayer *pAnother);	
	void AnotherPlayerAttack2(CPlayer *pAnother);	
	void AnotherPlayerAttack3(CPlayer *pAnother);	
	void AnotherPlayerSmash(CPlayer *pAnother);		

	void TakeDamage(CPlayer * pAnother);			
	void TakeAttack3Damage(CPlayer * pAnother);		
	void TakeSmashDamage(CPlayer * pAnother);		


	bool BlowAway(CPlayer *pAnother, const float MoveVecY, const float fBlowAwayForce);	

	int m_nPlayer;			
	int m_nBoxColliderID;	
	int m_nCntState;

	bool m_bIn[3];		// ポリゴンの範囲内フラグ

#ifdef _DEBUG
	void ShowDebugInfo(void);									
#endif
};
#endif