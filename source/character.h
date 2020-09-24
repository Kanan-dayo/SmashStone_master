// ===================================================================
//
// �L�����N�^�[���� [ character.h ]
// Author : Seiya Takahashi
//
// ===================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "main.h"
#include "scene.h"
#include "motion.h"
#include "charaParam.h"
#include "kananlibrary.h"
#include "stone.h"

// ===================================================================
// �}�N����`
// ===================================================================
#define TIME_TRANS		(15 * ONE_SECOND_FPS)	// �ϐg�̎���
#define LIFE_DEFAULT	(100.0f)				// ���C�t�̏����l

// ===================================================================
// �N���X��`
// ===================================================================
class CModelCharacter;
class CCapsuleCollider;
class CCharaParam;

class CCharacter : public CScene
{
public:
	enum COLLIPARTS
	{
		COLLIPARTS_NONE = -1,	// �Ȃ�
		COLLIPARTS_BODY,		// ��
		COLLIPARTS_FOREARM_R,	// �E�O�r
		COLLIPARTS_UPPERARM_R,	// �E��r
		COLLIPARTS_FOREARM_L,	// �O�r
		COLLIPARTS_UPPERARM_L,	// ��r
		COLLIPARTS_FEMUR_R,     // �E����
		COLLIPARTS_LOWERLEG_R,  // �E���ڕ�
		COLLIPARTS_FEMUR_L,     // ������
		COLLIPARTS_LOWERLEG_L,  // �����ڕ�
		COLLIPARTS_MAX			// �ő�
	};

	typedef enum
	{	// ������Ԃ̍s��
		STANDSTATE_NONE = -1,
		STANDSTATE_NEUTRAL,		// �j���[�g����
		STANDSTATE_DAUNTED,		// ����
		STANDSTATE_BLOWAWAY,	// �������
		STANDSTATE_SMASHBLOWAWAY,	// �X�}�b�V���������
		STANDSTATE_DOWN,		// �_�E��
		STANDSTATE_GETUP,		// �N���オ��
		STANDSTATE_GETUP_ACTIVE,// �A�N�e�B�u�ȋN���オ��
		STANDSTATE_WALK,		// ����
		STANDSTATE_JUMP,		// �W�����v
		STANDSTATE_ATTACK,		// �U��
		STANDSTATE_SMASHCHARGE,	// �X�}�b�V���`���[�W
		STANDSTATE_SMASH,		// �X�}�b�V��
		STANDSTATE_LIFT,		// �����グ
		STANDSTATE_MAX			// �ő�
	} CHARACTER_STANDSTATE;

	typedef enum
	{	// �W�����v��Ԃ̍s��
		JUMPSTATE_NONE = -1,
		JUMPSTATE_JUMP,			// �W�����v
		JUMPSTATE_FALL,			// ������
		JUMPSTATE_ATTACK,		// �U��
	} CHARACTER_JUMPSTATE;

	typedef enum
	{	// �������̏��
		STATE_NONE = 0,			// �����Ȃ�
		STATE_LIFT,				// �������グ
		STATE_LIFT_NEUTRAL,		// �������j���[�g����
		STATE_WALK,				// ����������
		STATE_THROW,			// ������
		STATE_LIFT_MAX			// �ő吔
	} STATE_LIFT_OBJET;

	CCharacter(CScene::PRIORITY nPriority);			// �R���X�g���N�^
	~CCharacter();									// �f�X�g���N�^

	void               Init(void);					// ������
	void               Uninit(void);				// �I��
	void               Update(void);				// �X�V
	void               Draw(void);					// �`��

	inline void        SetPos(const D3DXVECTOR3 & pos)			{ m_pos = pos; }					// �ʒu�̐ݒ�
	inline void        SetMove(const D3DXVECTOR3 & move)		{ m_move = move; }					// �ړ��l�̐ݒ�
	inline void        SetRot(const D3DXVECTOR3 & rot)			{ m_rot = rot; }					// ��]�̐ݒ�
	inline void        SetRotDest(const D3DXVECTOR3 & rotDest)	{ m_rotDest = rotDest; }			// �ړI�̉�]�̐ݒ�
	inline void        SetLife(const float &nLife)				{ m_nLife = nLife; }				// ���C�t�̐ݒ�
	inline void        SetAttakHit(const bool &bAttackHit)		{ m_bAttakHit = bAttackHit; }		// �U���𓖂Ă��t���O
	inline void        SetAttakHitStone(const bool &bAttackHit)	{ m_bAttakHitStone = bAttackHit; }	// �X�g�[���ɍU���𓖂Ă��t���O
	void               SetModelType(CHARACTER_TYPE type);											// ���f���̐ݒ�
	void               SetCylinderCoillider(void);													// �V�����_�[�R���C�_�[�̐ݒ�
	void               ConfromFromModelIndexToYypeID(int *pOut, const int *pIn);					// ���f���C���f�b�N�X����^�C�vID�ɕϊ�

	inline CHARACTER_TYPE    GetCharaType(void)					{ return m_type; }					// �L�����^�C�v�̎擾
	inline STATE_LIFT_OBJET  GetStateLift(void)					{ return m_StateLift; }				// �������グ�̏�Ԏ擾
	inline D3DXVECTOR3       &GetPos(void)						{ return m_pos; }					// �ʒu�̎擾
	inline D3DXVECTOR3       &GetMove(void)						{ return m_move; }					// �ړ��l�̎擾
	inline D3DXVECTOR3       &GetMoveOld(void)					{ return m_moveOld; }				// �ړ��l�̎擾
	inline D3DXVECTOR3       &GetRot(void)						{ return m_rot; }					// ��]�̎擾
	inline D3DXVECTOR3       &GetRotDest(void)					{ return m_rotDest; }				// ��]��̎擾
	inline float             &GetMaxLife(void)					{ return m_param.fMaxLife; }		// �ő僉�C�t�擾
	inline float             &GetLife(void)						{ return m_nLife; }					// ���C�t�擾
	inline void              Damage(const int nDamage)			{ m_nLife -= nDamage; }				// �_���[�W����
	inline CCapsuleCollider* GetCapCollider(int nPartsIndex)	{ return m_pCapColi[nPartsIndex]; }	// �J�v�Z���R���C�_�[�̎擾
	inline CModelCharacter*  GetModelCharacter(void)			{ return m_pModelCharacter; }		// ���f���L�����N�^�|�̎擾

	inline bool				 GetStoneType(int nID)				{ return m_bGetStoneType[nID]; }	// �擾�����X�g�[���^�C�v�擾
	inline bool				 GetTrans(void)						{ return m_bTrans; }				// �ϐg�������ǂ���
	inline bool				 GetInvincible(void)				{ return m_bInvincible; }			// ���G���ǂ���
	inline int				 GetTransCnt(void)					{ return m_nCntTrans; }				// �ϐg�J�E���g�擾
	inline CHARACTER_STANDSTATE GetStandState(void)				{ return m_stateStand; }			// ������Ԃ̍s���擾
	inline CHARACTER_JUMPSTATE GetJumpState(void)				{ return m_stateJump; }				// �W�����v��Ԃ̍s���擾


	void			   VictoryAction(void);							// �����s��

protected:
	CModelCharacter *m_pModelCharacter;					// ���f���L�����N�^�[�̃|�C���^
	CHARACTER_TYPE	 m_type;							// �L�����N�^�[�̃^�C�v
	CHARACTER_TYPE	 m_typeTrans;						// �ϐg���̃^�C�v

	LPD3DXMESH		m_pMesh;							// ���b�V�����
	LPD3DXBUFFER	m_pBuffMat;							// �}�e���A�����
	DWORD			m_nNumMat;							// �}�e���A�����̐�

	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_posOld;								// �O��̈ʒu
	D3DXVECTOR3 m_posBegin;								// �����ʒu
	D3DXVECTOR3 m_move;									// �ړ��l
	D3DXVECTOR3 m_moveOld;								// �O��̈ړ��l
	D3DXVECTOR3 m_rot;									// ��]
	D3DXVECTOR3 m_rotDest;								// �ړI�̉�]
	D3DXVECTOR3 m_rotDif;								// ��]�̍�
	D3DXMATRIX	m_mtxWorld;								// ���[���h�}�g���b�N�X
	STATE_LIFT_OBJET m_StateLift;						// �������グ�̏��
	CCapsuleCollider* m_pCapColi[COLLIPARTS_MAX];	// �V�����_�[�R���C�_�[�|�C���^

	CCharaParam::PLAYER_PARAM m_param;					// �v���C���[�̃p�����[�^�[
	CHARACTER_STANDSTATE	  m_stateStand;				// ������Ԃ̍s��
	CHARACTER_JUMPSTATE		  m_stateJump;				// �W�����v��Ԃ̍s��
	int						  m_nCntTrans;				// �ǂꂮ�炢�̎��ԕϐg���Ă��邩
	int						  m_nNumStone;				// �擾�����X�g�[���̐�
	float					  m_nLife;					// ���C�t
	bool					  m_bTrans;					// �ϐg���Ă��邩
	bool					  m_bInvincible;			// ���G���ǂ���
	static int				  m_nNumCharacter;			// �L�����N�^�[�̑���
	int						  m_nAttackFlow;			// �U���̗���

	bool		m_bAttakHit;							// �U���𓖂Ă��t���O
	bool		m_bAttakHitStone;						// �X�g�[���ɍU���𓖂Ă��t���O
	bool m_bGetStoneType[CStone::STONE_ID_MAX];	// �擾�����X�g�[���̃^�C�v

private:
	void Move(void);									// �ړ��֐�
	void Rot(void);										// ��]�֐�
};

#endif
