
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

	void Daunted(const int nGap);	// ����

	bool ReadyToHit(void);			
	bool ReadyToHitStone(void);
	bool HitConditionAttack0(const int &nCapColliID);	
	bool HitConditionAttack1(const int &nCapColliID);	
	bool HitConditionAttack2(const int &nCapColliID);	
	bool HitConditionAttack3(const int &nCapColliID);	
	bool HitConditionSmash(const int &nCapColliID);		

	void SetHitSound();	
	void CatchStone(CStone *pStone);	
	CShadow *GetShadow(void) { return m_pShadow; }	// �L�����N�^�[�̉e�擾����

	void TakeDamage(CPlayer * pAnother, const int nAttackMotion);

protected:

private:
	void Control(void);								
	void Collision(void);							
	void Smash(CInputGamepad *pGamepad, CInputKeyboard *pKey);
	void NormalAttack(CInputGamepad *pGamepad, CInputKeyboard *pKey);
	bool Jump(CInputGamepad *pGamepad, CInputKeyboard *pKey);
	bool InputKeyMove(CInputKeyboard *pKey);
	bool InputPadMove(CInputGamepad *pGamepad);
	void Shadow(void);								// �e�֌W�̍X�V����

	void Motion(void);
	void MotionNeutral(void);
	void MotionWalk(void);
	void MotionJump(void);
	void MotionDown(void);
	void MotionDaunted(void);
	void MotionBlowAway(void);
	void MotionSmashBlowAway(void);
	void MotionAttack(void);
	void MotionSmashCharge(void);
	void MotionSmash(void);
	void MotionGetUp(void);
	bool SmashJudge(void);

	void SetLanding(void);

	void CollisionAttack(void);						

	static CHitPoint *m_pHitPoint;					
	CShadow *m_pShadow;								// �e�̏��|�C���^

	void ControlGamepad(CInputGamepad *pGamepad);	
	void ControlKeyboard(CInputKeyboard *pKeyboard);
	void SetnPlayer(int nPlayerNum);				

	void AnotherPlayerAttack0(CPlayer *pAnother);	
	void AnotherPlayerAttack1(CPlayer *pAnother);	
	void AnotherPlayerAttack2(CPlayer *pAnother);	
	void AnotherPlayerAttack3(CPlayer *pAnother);	
	void AnotherPlayerSmash(CPlayer *pAnother);		

	void TakeAttack3Damage(CPlayer * pAnother);		
	void TakeSmashDamage(CPlayer * pAnother);		


	bool BlowAway(CPlayer *pAnother, const float MoveVecY, const float fBlowAwayForce);	

	int m_nPlayer;			
	int m_nBoxColliderID;	
	int m_nCntState;		// ��ԊǗ��p�̃J�E���^
	int m_nCntGap;			// ��_���[�W���̌㌄�J�E���^

	int	m_nAttackFrame;			// �U�����[�V�����؂�ւ��̃J�E���^


	bool m_bIn[3];		// �|���S���͈͓̔��t���O
	bool m_bCancelAttack;	// �U���̃L�����Z���t���O

#ifdef _DEBUG
	void ShowDebugInfo(void);									
#endif
};
#endif