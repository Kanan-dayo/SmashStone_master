//=============================================================================
//
// �L�����N�^�[����  [ character.cpp ]
// Author : Seiya Takahashi
//
//=============================================================================


//=============================================================================
// �C���N���[�h
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
// �}�N����`
//=============================================================================	
#define SPEED_ROT			(0.1f)		// ��]�̃X�s�[�h
#define TIME_MAX_DOWN		(60)		// �ő�܂Ń_�E���ł��鎞��

#define INERTIA_SMASH		(0.001f)	// �X�}�b�V��������ю��̊���
#define INERTIA_BLOWAWAY	(0.02f)		// �ʏ퐁����ю��̊���

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
int CCharacter::m_nNumCharacter = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCharacter::CCharacter(PRIORITY nPriority) : CScene(nPriority)
{
	// �v�f�̏�����
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
	m_nDebugInvicible = 0;
	m_stateStand		= STANDSTATE_NEUTRAL;
	m_stateJump			= JUMPSTATE_NONE;
	m_StateLift			= STATE_NONE;
	m_bAttakHit			= false;
	m_bAttakHitStone	= false;
	m_bInvincible		= false;
	
	// ���������Z
	m_nNumCharacter++;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharacter::~CCharacter()
{
	// nullcheck
	if (m_pModelCharacter)
	{
		// ���f���L�����N�^�[��j��
		delete m_pModelCharacter;
		m_pModelCharacter = nullptr;
	}
}

//=============================================================================
// ������
//=============================================================================
void CCharacter::Init()
{
	m_nCntInvicible = 0;
	m_nMaxInvicible = 0;
	m_nDebugInvicible = 0;
}

//=============================================================================
// �I��
//=============================================================================
void CCharacter::Uninit()
{
	//���f���̏I������
	m_pModelCharacter->Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CCharacter::Update()
{
	m_param = CCharaParam::GetPlayerParam((PARAM_TYPE)(m_type / 2));
	
	if (m_bInvincible)
		m_nDebugInvicible++;
	else
	{
		if (m_nDebugInvicible != 0)
			m_nDebugInvicible = 0;
	}

	// �ړ�����
	Move();

	// ��]����
	Rot();

	// ���[���h�}�g���b�N�X�̐ݒ�
	m_pModelCharacter->SetCharacterMtx(&m_mtxWorld);

	// ���f���̍X�V
	m_pModelCharacter->Update();

	// ���[���h�}�g���b�N�X�̌v�Z
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾

	// �L�[�{�[�h��[5]���������Ƃ�
	if (pKeyboard->GetKeyboardPress(DIK_5))
	{
		// �̗͂����炷
		m_nLife -= 1;
	}
	// �L�[�{�[�h��[6]���������Ƃ�
	if (pKeyboard->GetKeyboardPress(DIK_6))
	{
		// �̗͂𑝂₷
		m_nLife += 1;
	}
	// �L�[�{�[�h��[6]���������Ƃ�
	if (pKeyboard->GetKeyboardPress(DIK_7))
	{
		// �̗͂𑝂₷
		m_nLife = m_param.fMaxLife;
	}

#endif // _DEBUG

	if (m_nLife <= 0)
		m_nLife = 0;
	if (m_nLife >= m_param.fMaxLife)
		m_nLife = m_param.fMaxLife;
}

//=============================================================================
// �`��
//=============================================================================
void CCharacter::Draw()
{
	// ���f���̕`��
	if (m_bInvincible)
		m_pModelCharacter->DrawInvincible(m_nDebugInvicible);
	else
		m_pModelCharacter->Draw();
}

//=============================================================================
// ���f���̐ݒ�
//=============================================================================
void CCharacter::SetModelType(CHARACTER_TYPE type)
{
	// ���f���̐���
	m_pModelCharacter = CModelCharacter::Create(type);

	m_type = type;
	m_typeTrans = (CHARACTER_TYPE)(type + 1);

	// �^�C�v���Ƃɍő�HP�ݒ�
	m_param = CCharaParam::GetPlayerParam((PARAM_TYPE)(m_type / 2));
	m_nLife = m_param.fMaxLife;

	// ���[���h�}�g���b�N�X�̐ݒ�
	m_pModelCharacter->SetCharacterMtx(&m_mtxWorld);

	// �V�����_�[�R���C�_�[�̐ݒ�
	SetCylinderCoillider();
}

//=============================================================================
// �V�����_�[�R���C�_�[�̐ݒ�
//=============================================================================
void CCharacter::SetCylinderCoillider(void)
{
	// �p�[�c�̃|�C���^
	CModelParts *pParts = m_pModelCharacter->GetModelParts();

	// �E�O�r�̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_FOREARM_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LOWARM_R].GetMtx(), CCapsuleCollider::TYPEID_FOREARM_R);
	// �E��r�̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_UPPERARM_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_UPARM_R].GetMtx(), CCapsuleCollider::TYPEID_UPPERARM_R);
	// ���O�r�̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_FOREARM_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LOWARM_L].GetMtx(), CCapsuleCollider::TYPEID_FOREARM_L);
	// ����r�̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_UPPERARM_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_UPARM_L].GetMtx(), CCapsuleCollider::TYPEID_UPPERARM_L);

	// �E���ڂ̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_FEMUR_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_THIGH_R].GetMtx(), CCapsuleCollider::TYPEID_THIGH_R);
	// �E���ڕ��̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_LOWERLEG_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LEG_R].GetMtx(), CCapsuleCollider::TYPEID_LOWERLEG_R);
	// �����ڂ̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_FEMUR_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_THIGH_L].GetMtx(), CCapsuleCollider::TYPEID_THIGH_L);
	// �����ڕ��̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_LOWERLEG_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LEG_L].GetMtx(), CCapsuleCollider::TYPEID_LOWERLEG_L);

	// �̂̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_BODY] =
		CCapsuleCollider::Create(this, &m_mtxWorld, CCapsuleCollider::TYPEID_BODY);

	// �X�}�b�V���̐ݒ�
	m_pCapColi[CCharacter::COLLIPARTS_SMASH] =
		CCapsuleCollider::Create(this, &m_mtxWorld, CCapsuleCollider::TYPEID_SMASH);

}

#define CASE(Key)	break;case (Key):
#define DEFAULT		break;default:


//=============================================================================
// ���f���C���f�b�N�X����^�C�vID�ɕϊ�
//=============================================================================
void CCharacter::ConfromFromModelIndexToYypeID(int * pOut, const int * pIn)
{
	switch ((CModelCharacter::PARTSNAME)*pIn)
	{
		CASE(CModelCharacter::UPARM_R)       *pOut = (int)CCapsuleCollider::TYPEID_UPPERARM_R;
		CASE(CModelCharacter::FOREARM_R)     *pOut = (int)CCapsuleCollider::TYPEID_FOREARM_R;
		CASE(CModelCharacter::UPARM_L)       *pOut = (int)CCapsuleCollider::TYPEID_UPPERARM_L;
		CASE(CModelCharacter::FOREARM_L)     *pOut = (int)CCapsuleCollider::TYPEID_FOREARM_L;
		CASE(CModelCharacter::THIGH_R)       *pOut = (int)CCapsuleCollider::TYPEID_THIGH_R;
		CASE(CModelCharacter::LOWERLEG_R)    *pOut = (int)CCapsuleCollider::TYPEID_LOWERLEG_R;
		CASE(CModelCharacter::THIGH_L)       *pOut = (int)CCapsuleCollider::TYPEID_THIGH_L;
		CASE(CModelCharacter::LOWERLEG_L)    *pOut = (int)CCapsuleCollider::TYPEID_LOWERLEG_L;
		CASE(CMODELCHARACTER_PARTSNAME_SMASH)*pOut = (int)CCapsuleCollider::TYPEID_SMASH;
			DEFAULT *pOut = -1; return;
	}
}

//=============================================================================
// �����s��
//=============================================================================
void CCharacter::VictoryAction(void)
{
}

//=============================================================================
// �ړ��̏���
//=============================================================================
void CCharacter::Move(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;
	m_moveOld = m_move;
	
	// ����
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

	// �d��
	CKananLibrary::Gravity(m_move.y);

	//�ړ��ʉ��Z
	m_pos += m_move;
}

//=============================================================================
// ��]�̏���
//=============================================================================
void CCharacter::Rot(void)
{
	// ��]�̍��������߂�
	m_rotDif = m_rotDest - m_rot;

	// ��]�̕��
	CKananLibrary::InterpolationRot(&m_rotDif);

	// ���f���̉�]
	m_rot += m_rotDif * SPEED_ROT;

	// ��]�̕��
	CKananLibrary::InterpolationRot(&m_rot);
}
