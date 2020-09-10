//==================================================================================================================
//
// �v���C���[����[player.cpp]
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

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define HEIGHT_CEILING	(400.0f)			// �V��̍���
#define HEIGHT_FLOOR	(0.0f)				// ���̍���

#define BLOWAWAYFORCE_SMASH		(100.0f)	// ������΂���(�X�}�b�V���U��)
#define BLOWAWAYFORCE_NORMAL	(8.0f)		// ������΂���(�ʏ�U��)

#define TIME_LIFT_BEGIN			(60)		// �����グ�J�n�̃��[�V��������
#define TIME_MAX_DOWN			(60)		// �ő�܂Ń_�E���ł��鎞��

#define TIME_MAX_CHARGE			(100)		// �ő�܂Ń`���[�W�ł��鎞��

#define TIME_JUMP_TO_FALL		(15)		// �W�����v���痎���܂ł̎���

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
CHitPoint *CPlayer::m_pHitPoint = NULL;				// HP���

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CPlayer::CPlayer(PRIORITY type = CScene::PRIORITY_PLAYER) : CCharacter(type)
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CPlayer::~CPlayer()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CPlayer::Init(void)
{
	// �v�f�̏�����
	m_bTrans = false;
	m_nCntState = 0;
	m_nCntGap = 0;
	m_nAttackFrame = 0;

	// �ő�|���S�����܂ŃJ�E���g
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_bIn[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < CStone::STONE_ID_MAX; nCnt++)
	{
		m_bGetStoneType[nCnt] = false;
	}

	// ������
	CCharacter::Init();
	CCharacter::SetModelType(m_type);

	// �����蔻��̐ݒ�
	this->m_nBoxColliderID = C3DBoxCollider::SetColliderInfo(&this->GetPos(), this, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::ID_CHARACTER);

	m_pHitPoint = CHitPoint::Create(m_nPlayer, m_param.fMaxLife);
	m_pHitPoint->SetnPlayerNum(m_nPlayer);

	// �e����
	m_pShadow = CShadow::Create();
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();

	m_pHitPoint = nullptr;	// �ϐ�NULL
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CPlayer::Update(void)
{
	CGame::GAMESTATE gameState = CManager::GetRenderer()->GetGame()->GetGameState();
	if (m_stateStand != STANDSTATE_SMASHBLOWAWAY&& m_stateStand != STANDSTATE_BLOWAWAY && m_stateStand != STANDSTATE_DAUNTED &&
		(gameState == CGame::GAMESTATE_NORMAL || gameState == CGame::GAMESTATE_BEFORE))
	{
		// ����
		Control();
	}


	// �X�V
	CCharacter::Update();

	// �����蔻��
	Collision();

	// ���[�V��������
	Motion();

	// �v���C���[�̔ԍ��ݒ�
	SetnPlayer(m_nPlayer);

#ifdef _DEBUG
	char cText[8];
	sprintf(cText, "%d / ", m_pModelCharacter->GetAllFrame() - m_nAttackFrame);
	CDebugProc::Print(cText);
	sprintf(cText, "%d\n", m_pModelCharacter->GetAllFrame());
	CDebugProc::Print(cText);

	CDebugProc::Print("�v���C���[�̈ʒu [%.4f][%.4f][%.4f]\n", m_pos.x, m_pos.y, m_pos.z);

	ShowDebugInfo();
#endif
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//==================================================================================================================
// ��������
//==================================================================================================================
CPlayer *CPlayer::Create(int nPlayer, CHARACTER_TYPE type)
{
	// �V�[�����I�Ɋm��
	CPlayer *pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	// ���s
	if (!pPlayer)
		return nullptr;

	// �v���C���[�̃L�����^�C�v��ݒ�
	pPlayer->m_type = type;
	// �v���C���[�ԍ��̕ۑ�
	pPlayer->m_nPlayer = nPlayer;
	// ������
	pPlayer->Init();

	// �l��Ԃ�
	return pPlayer;
}

//==================================================================================================================
// ����
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
// �����蔻�菈��
//==================================================================================================================
void CPlayer::Collision(void)
{
	bool bSmashBlowAway = (m_stateStand == STANDSTATE_SMASHBLOWAWAY);

	// �ǂ̎擾
	CWall *pWall = CGame::GetWall();
	// �o�͂�����_
	D3DXVECTOR3 out_intersect = ZeroVector3;
	// �o�͂����@��
	D3DXVECTOR3 out_nor = ZeroVector3;

	CPolyCollMana *pPolyCollMana = CGame::GetpolyCollMana();

	for (int nCntPolyColli = 0; nCntPolyColli < CPolygonCollider::GetNumPoly(CGame::GetStageType()); nCntPolyColli++)
	{
		// �|���S���R���C�_�[�̎擾
		CPolygonCollider*pPolyColli = pPolyCollMana->GetPolyColl(nCntPolyColli);

		// �|���S���R���C�_�[�|�C���^��NULL��������
		if (pPolyColli == NULL)
		{// �X�L�b�v
			continue;
		}

		// �|���S���͈͓̔��ɂ���Ƃ�
		if (pPolyColli->Test3DInPolygon(&m_pos))
		{
			// �͈͓��ɂ���
			m_bIn[nCntPolyColli] = true;
		}
		else
		{
			// �͈͓��ɂ��Ȃ�
			m_bIn[nCntPolyColli] = false;
		}

		// �|���S���R���C�_�[�̏Փ˔���
		if (pPolyColli->Collision(&m_pos, &m_posOld, &m_move, &out_intersect, &out_nor, bSmashBlowAway) == true)
		{
			if (bSmashBlowAway == false)
			{
#ifdef _DEBUG
				CDebugProc::Print("����Ă���\n");
#endif
				// �n�ʂɏ���Ă�����A�ړ��ʂ��Ȃ���
				if (m_move.y <= -5.0f)
					m_move.y = -5.0f;

				SetLanding();
			}
			// �o�͂��ꂽ�@�����[������Ȃ�������
			// �o�͂��ꂽ��_���[������Ȃ���
			if (bSmashBlowAway == true &&
				out_nor != ZeroVector3 &&
				out_intersect != ZeroVector3)
			{
				// �_���[�W
				this->Damage(2);
				// ����������
				this->m_rotDest.y = atan2f(out_nor.x, out_nor.z);
				// ��]����
				CKananLibrary::InterpolationFloat(m_rotDest.y);
				// ��u�Ō�����ς���
				this->m_rot.y = this->m_rotDest.y;
				CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos, &this->m_move, &out_intersect, &this->m_move, &out_nor);
			}
		}
	}

	out_intersect = ZeroVector3;
	out_nor = ZeroVector3;

	// �����蔻��ʒu�̍X�V
	C3DBoxCollider::ChangePosition(this->m_nBoxColliderID, this->m_pos, MYLIB_3DVECTOR_ZERO);
	// �����蔻��
	if (C3DBoxCollider::CollisionBox(this->m_nBoxColliderID, this->m_pos, m_move, &out_intersect,&out_nor, bSmashBlowAway))
	{
		// �o�͂��ꂽ�@�����[������Ȃ�������
		// �o�͂��ꂽ��_���[������Ȃ���
		if (bSmashBlowAway == true &&
			out_nor != ZeroVector3 &&
			out_intersect != ZeroVector3)
		{
			// �_���[�W
			this->Damage(2);
			// ����������
			this->m_rotDest.y = atan2f(out_nor.x, out_nor.z);
			// ��]����
			CKananLibrary::InterpolationFloat(m_rotDest.y);
			// ��u�Ō�����ς���
			this->m_rot.y = this->m_rotDest.y;
			CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos, &this->m_move, &out_intersect, &this->m_move, &out_nor);
		}
		else if(bSmashBlowAway == false)
		{
			SetLanding();
		}
	}



	// �ő�|���S�����܂ŃJ�E���g
	for (int nCntPolyColli = 0; nCntPolyColli < CPolygonCollider::GetNumPoly(CGame::GetStageType()); nCntPolyColli++)
	{
		// �|���S���R���C�_�[�̎擾
		CPolygonCollider*pPolyColli = pPolyCollMana->GetPolyColl(nCntPolyColli);

		// �͈͊O�̂Ƃ�
		if (!m_bIn[nCntPolyColli])
		{
			// �e����
			Shadow();
		}
		else
		{
			bool bJump = false;
			if (m_stateStand == STANDSTATE_JUMP)
				bJump = true;
			// �e�ʒu�ݒ�
			m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, pPolyColli->GetfHeight(), m_pos.z), m_move, bJump);
			
			break;
		}
	}

	out_intersect = ZeroVector3;
	out_nor = ZeroVector3;

	// �ǂƂ̓����蔻��
	if (pWall->Collision(&m_pos, &m_posOld, &out_intersect, &out_nor, bSmashBlowAway) == true)
	{
		// �o�͂��ꂽ�@�����[������Ȃ�������
		// �o�͂��ꂽ��_���[������Ȃ���
		if (bSmashBlowAway == true &&
			out_nor != ZeroVector3 &&
			out_intersect != ZeroVector3)
		{
			// �_���[�W
			this->Damage(2);
			// ����������
			this->m_rotDest.y = atan2f(out_nor.x, out_nor.z);
			// ��]����
			CKananLibrary::InterpolationFloat(m_rotDest.y);
			// ��u�Ō�����ς���
			this->m_rot.y = this->m_rotDest.y;
			CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos, &this->m_move, &out_intersect, &this->m_move, &out_nor);
		}
	}

	// ��������
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
// �X�}�b�V��
//==================================================================================================================
void CPlayer::Smash(CInputGamepad *pGamepad, CInputKeyboard *pKey)
{
	// �ϐg���Ă��Ȃ���΁A�������Ȃ�
	if (!m_bTrans)
		return;

	// �`���[�W���Ƀ{�^���𗣂��ƁA�X�}�b�V��
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

	// �ϐg���Ƀ{�^���𒷉����ŁA�X�}�b�V���`���[�W
	if ((pGamepad && pGamepad->GetbConnect() && pGamepad->GetPress(CInputGamepad::JOYPADKEY_B)) ||
		(pKey && ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_SMASH)) || (m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_SMASH)))) &&
		m_stateStand != STANDSTATE_SMASH)
	{
		m_stateStand = STANDSTATE_SMASHCHARGE;
		m_nCntState++;
	}
}

//==================================================================================================================
// �ʏ�U��
//==================================================================================================================
void CPlayer::NormalAttack(CInputGamepad *pGamepad, CInputKeyboard *pKey)
{
	// �L�[����
	if (m_stateStand != STANDSTATE_JUMP && m_stateStand != STANDSTATE_ATTACK &&
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
// �W�����v
//==================================================================================================================
bool CPlayer::Jump(CInputGamepad *pGamepad, CInputKeyboard *pKey)
{
	// �L�[����
	if (m_stateStand != STANDSTATE_JUMP && m_stateStand != STANDSTATE_ATTACK &&
		((pGamepad && pGamepad->GetbConnect() && pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A)) ||
		(pKey && ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardTrigger(ONE_JUMP)) || (m_nPlayer == PLAYER_TWO && pKey->GetKeyboardTrigger(TWO_JUMP))))))
	{
		m_stateStand = STANDSTATE_JUMP;
		// �ړ��l��ݒ�
		m_move.y = m_param.moveParam.fJumpPower;
		// �W�����v���s
		return true;
	}
	// �W�����v���ĂȂ�
	return false;
}

//==================================================================================================================
// �ړ����͏���
//==================================================================================================================
bool CPlayer::InputKeyMove(CInputKeyboard * pKey)
{
	bool bMove = false;
	// �ϐ��錾
	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();	// �J�����擾
	D3DXVECTOR3 rotDest = GetRotDest();										// �ړI�̌������i�[����ϐ�
	float		CameraRotY = pCamera->GetRotY();								// �J������Y����]�̎擾

	// A�L�[������
	if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_LEFT)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_LEFT))
	{
		// W�L�[������
		if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_UP)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_UP))
		{
			// ����ړ�
			m_move.x += sinf(-D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// �ړI�̌���������
			rotDest.y = D3DX_PI * 0.75f + CameraRotY;
		}
		// S�L�[������
		else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_DOWN))
		{
			// �����ړ�
			m_move.x += sinf(-D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// �ړI�̌���������
			rotDest.y = D3DX_PI * 0.25f + CameraRotY;
		}
		// A�L�[�̂�
		else
		{
			// ���ړ�
			m_move.x += sinf(-D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// �ړI�̌���������
			rotDest.x = 0.0f;
			rotDest.y = D3DX_PI * 0.5f + CameraRotY;
		}

		// �W�����v���łȂ���΁A����
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}
	// D�L�[������
	else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_RIGHT)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_RIGHT))
	{
		// W�L�[������
		if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_UP)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_UP))
		{
			// �E��ړ�
			m_move.x += sinf(D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// �ړI�̌���������
			rotDest.y = -D3DX_PI * 0.75f + CameraRotY;
		}
		// S�L�[������
		else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_DOWN))
		{
			// �E���ړ�
			m_move.x += sinf(D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// �ړI�̌���������
			rotDest.y = -D3DX_PI * 0.25f + CameraRotY;
		}
		// D�L�[�̂�
		else
		{
			// �E�ړ�
			m_move.x += sinf(D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// �ړI�̌���������
			rotDest.y = -D3DX_PI * 0.5f + CameraRotY;
		}
		
		// �W�����v���łȂ���΁A����
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}
	// W�L�[������
	else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_UP)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_UP))
	{
		// ��ړ�
		m_move.x += sinf(D3DX_PI * 1.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		m_move.z -= cosf(D3DX_PI * 1.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		// �ړI�̌���������
		rotDest.y = -D3DX_PI * 1.0f + CameraRotY;
	
		// �W�����v���łȂ���΁A����
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}
	// S�L�[������
	else if ((m_nPlayer == PLAYER_ONE && pKey->GetKeyboardPress(ONE_DOWN)) ||
		m_nPlayer == PLAYER_TWO && pKey->GetKeyboardPress(TWO_DOWN))
	{
		// ���ړ�
		m_move.x += sinf(D3DX_PI * 0.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		m_move.z -= cosf(D3DX_PI * 0.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		// �ړI�̌���������
		rotDest.y = CameraRotY;
	
		// �W�����v���łȂ���΁A����
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}

	// ��]�̕␳
	CKananLibrary::InterpolationRot(&rotDest);

	// �ړI�̉�]�̐ݒ�
	SetRotDest(rotDest);

	// �ړ����Ă��Ȃ�
	return bMove;
}

//==================================================================================================================
// �Q�[���p�b�h�ɂ��ړ�����
//==================================================================================================================
bool CPlayer::InputPadMove(CInputGamepad * pGamepad)
{
	// �ړ�����
	bool bMove = false;

	// ���X�e�B�b�N�擾
	float fValueX, fValueY;
	pGamepad->GetStickLeft(&fValueX, &fValueY);

	D3DXVECTOR3 rotDest = GetRotDest();	// �ړI�̌������i�[����ϐ�
	float		CameraRotY = CManager::GetRenderer()->GetGame()->GetCamera()->GetRotY();	// �J������Y����]�̎擾
	float		fSpeed = 0.0f;			// �v���C���[�̑��x
	float		fAngle = 0.0f;			// �X�e�B�b�N�p�x�̌v�Z�p�ϐ�

	// �p�x�̌v�Z���ĕ␳
	fAngle = atan2f(fValueX, fValueY);
	CKananLibrary::InterpolationFloat(fAngle);

	// �X�e�B�b�N�̓|���ŃX�s�[�h������
	if (abs(fValueX) > abs(fValueY))
		fSpeed = (abs(fValueX));	// ���̓|��
	else
		fSpeed = (abs(fValueY));	// �c�̓|��

	// �X�e�B�b�N�̊p�x�ɂ���ăv���C���[�ړ�
	m_move.x += sinf(fAngle + CameraRotY) * fSpeed * m_param.moveParam.fRunSpeed;
	m_move.z += cosf(fAngle + CameraRotY) * fSpeed * m_param.moveParam.fRunSpeed;

	// �ړI�̌���������
	if (fValueX != 0 || fValueY != 0)
	{
		rotDest.y = D3DX_PI + fAngle + CameraRotY;
		// �W�����v���łȂ���΁A����
		if (m_stateStand != STANDSTATE_JUMP)
			m_stateStand = STANDSTATE_WALK;
	}

	// ��]�̕␳
	CKananLibrary::InterpolationRot(&rotDest);

	// �ړI�̉�]�̐ݒ�
	SetRotDest(rotDest);

	return bMove;
}

//==================================================================================================================
// �e�֌W�̍X�V����
//==================================================================================================================
void CPlayer::Shadow(void)
{
	bool bJump = false;
	if (m_stateStand == STANDSTATE_JUMP)
		bJump = true;

	// �e�ʒu�ݒ�
	m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, C3DBoxCollider::GetHeight(), m_pos.z), m_move, bJump);
}

//==================================================================================================================
// ���[�V�����Ǘ��֐�
//==================================================================================================================
void CPlayer::Motion(void)
{
	switch (m_stateStand)
	{
		// �j���[�g����
	case STANDSTATE_NEUTRAL:
		MotionNeutral();
		break;
		// ����
	case STANDSTATE_DAUNTED:
		MotionDaunted();
		break;
		// �������
	case STANDSTATE_BLOWAWAY:
		MotionBlowAway();
		break;
	case STANDSTATE_SMASHBLOWAWAY:
		MotionSmashBlowAway();
		break;
		// �_�E��
	case STANDSTATE_DOWN:
		MotionDown();
		break;
		// �N���オ��
	case STANDSTATE_GETUP:
		MotionGetUp();
		break;
		// ���s
	case STANDSTATE_WALK:
		MotionWalk();
		break;
		// �W�����v
	case STANDSTATE_JUMP:
		MotionJump();
		break;
		// �U��
	case STANDSTATE_ATTACK:
		MotionAttack();
		break;
	case STANDSTATE_SMASHCHARGE:
		MotionSmashCharge();
		break;
	case STANDSTATE_SMASH:
		MotionSmash();
		break;
	}
}

//==================================================================================================================
// �j���[�g�������[�V����
//==================================================================================================================
void CPlayer::MotionNeutral(void)
{
	// ���[�V�����ؑ�
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_NEUTRAL)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_NEUTRAL);
		m_nCntState = 0;
	}
}

//==================================================================================================================
// �������[�V����
//==================================================================================================================
void CPlayer::MotionWalk(void)
{
	// ���[�V�����ؑ�
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_WALK)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_WALK);
		m_nCntState = 0;
	}
}

//==================================================================================================================
// �W�����v���[�V����
//==================================================================================================================
void CPlayer::MotionJump(void)
{
	// �U���̏�Ԃ�������
	m_nAttackFlow = 0;
	// �J�E���^�����Z
	m_nCntState++;
	
	// �ŏ��̓W�����v���[�V����
	if (m_nCntState <= TIME_JUMP_TO_FALL && m_pModelCharacter->GetMotion() != CMotion::PLAYER_JUMP)
		m_pModelCharacter->SetMotion(CMotion::PLAYER_JUMP);
	// �ȍ~�͗������[�V����
	else if (m_nCntState > TIME_JUMP_TO_FALL && m_pModelCharacter->GetMotion() != CMotion::PLAYER_FALL)
		m_pModelCharacter->SetMotion(CMotion::PLAYER_FALL);
}

//==================================================================================================================
// �_�E�����[�V����
//==================================================================================================================
void CPlayer::MotionDown(void)
{
	// ���[�V�����ؑ�
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_DOWN)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_DOWN);
		m_nCntState = 0;
	}

	// �J�E���g���Z
	m_nCntState++;

	CInputGamepad *pGamepad = CManager::GetInputGamepad(m_nPlayer);
	CInputKeyboard *pKey = CManager::GetInputKeyboard();

	// ��莞�ԓ��ɁA����ŃA�N�e�B�u�ȋN���オ��
	if (m_nCntState < TIME_MAX_DOWN &&
		((pGamepad && pGamepad->GetbConnect() && SUCCEEDED(CKananLibrary::GetMoveByGamepad(pGamepad))) ||
		(pKey && SUCCEEDED(CKananLibrary::GetMoveByKeyboard(pKey, m_nPlayer)))))
		m_stateStand = STANDSTATE_GETUP_ACTIVE;
	// ��莞�Ԍ�ɁA�N���オ��
	else if (m_nCntState >= TIME_MAX_DOWN)
		m_stateStand = STANDSTATE_GETUP;
	// �����̏I��
	else
		return;

	// �J�E���g������
	m_nCntState = 0;
}

//==================================================================================================================
// ���݃��[�V����
//==================================================================================================================
void CPlayer::MotionDaunted(void)
{
	// ���[�V�����ؑ�
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_DAUNTED)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_DAUNTED);
		m_nCntState = 0;
		// �U���̏�Ԃ�������
		m_nAttackFlow = 0;
	}

	// ���ݎ��Ԃ����Z
	m_nCntState++;

	// ���ݒ�
	if (m_nCntState < m_nCntGap)
		return;

	// ���݉���
	m_stateStand = STANDSTATE_NEUTRAL;
	// �㌄�t���[����������
	m_nCntGap = 0;
}

//==================================================================================================================
// ������у��[�V����
//==================================================================================================================
void CPlayer::MotionBlowAway(void)
{
	// ���[�V�����ؑ�
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_BLOWAWAY)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_BLOWAWAY);
		m_nCntState = 0;
		// �U���̏�Ԃ�������
		m_nAttackFlow = 0;
	}

	// �n�ʂɒ���
	if (m_move.y <= -3.0f)
	{
		// �_�E���J�n
		m_stateStand = STANDSTATE_DOWN;
	}
}

//==================================================================================================================
// �X�}�b�V��������у��[�V����
//==================================================================================================================
void CPlayer::MotionSmashBlowAway(void)
{
	// ���[�V�����ؑ�
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_BLOWAWAY)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_BLOWAWAY);
		m_nCntState = 0;
		// �U���̏�Ԃ�������
		m_nAttackFlow = 0;
	}

	if (abs(m_move.x) <= 5.0f &&
		abs(m_move.z) <= 5.0f &&
		abs(m_move.y) <= 5.0f)
	{
		// �_�E���J�n
		m_stateStand = STANDSTATE_DOWN;
	}
}

//==================================================================================================================
// �U�����[�V����
//==================================================================================================================
void CPlayer::MotionAttack(void)
{
	// �ŏ�����
	if (m_nCntState == 0)
	{
		// ���[�V�����̐؂�ւ�
		m_pModelCharacter->SetMotion((CMotion::MOTION_TYPE)(CMotion::PLAYER_ATTACK_0 + m_nAttackFlow));
		// �U�������������t���O���I�t�ɂ���
		m_bAttakHit = false;
		m_bAttakHitStone = false;
		// �U���t���[����ݒ�
		m_nAttackFrame = m_pModelCharacter->GetAllFrame();

		m_nAttackFlow++;
	}

	// �U���t���[�����Z
	m_nCntState++;

	// �L�����Z���\
	if (m_nCntState <= m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.end &&
		m_nCntState >= m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.start && m_nAttackFlow < 4)
		m_bCancelAttack = true;
	else
	{
		m_bCancelAttack = false;
	}

	// �U���I����
	if (m_nCntState > m_nAttackFrame)
	{
		// �U������
		m_nCntState = 0;
		// ���[�V�����؂�ւ�
		m_stateStand = STANDSTATE_NEUTRAL;
		// �U���̏�Ԃ�������
		m_nAttackFlow = 0;
		// �U���t���[����������
		m_nAttackFrame = 0;
		return;
	}
}

//==================================================================================================================
// �X�}�b�V���`���[�W���[�V����
//==================================================================================================================
void CPlayer::MotionSmashCharge(void)
{
	// �X�}�b�V���`���[�W
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH_CHARGE)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH_CHARGE);
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SMASHCHARGE);
		// �U�������������t���O���I�t�ɂ���
		m_bAttakHit = false;
	}
}

//==================================================================================================================
// �X�}�b�V�����[�V����
//==================================================================================================================
void CPlayer::MotionSmash(void)
{
	// �X�}�b�V��
	if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH)
	{
		m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH);
		// �U���t���[����ݒ�
		m_nAttackFrame = m_pModelCharacter->GetAllFrame();
	}
	m_nCntState++;
	if (m_nCntState >= m_nAttackFrame)
	{
		m_stateStand = STANDSTATE_NEUTRAL;
		m_nCntState = 0;
	}
}

//==================================================================================================================
// �N���オ�胂�[�V����
//==================================================================================================================
void CPlayer::MotionGetUp(void)
{
}

//==================================================================================================================
// �X�}�b�V���̔���
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
// ���n����
//==================================================================================================================
void CPlayer::SetLanding(void)
{
	if (m_stateStand == STANDSTATE_JUMP)
	{
		// �W�����v��������
		m_stateJump = JUMPSTATE_NONE;
		// �W�����v����
		m_stateStand = STANDSTATE_NEUTRAL;
	}
}

//==================================================================================================================
// ���ݏ���
//==================================================================================================================
void CPlayer::Daunted(const int nGap)
{
	// �J�E���g��ݒ肵�A���ݏ�ԂɈڍs
	m_pModelCharacter->ResetMotion();
	m_nCntGap = nGap;
	m_stateStand = STANDSTATE_DAUNTED;
	m_pModelCharacter->SetMotion(CMotion::PLAYER_DAUNTED);
}

//==================================================================================================================
// �U������
//==================================================================================================================
void CPlayer::CollisionAttack(void)
{
	// �Ⴄ�v���C���[�̎擾
	CPlayer *pAnother = GetAnotherPlayer();

	// �Ⴄ�v���C���[���U���𓖂Ă��t���O�������ĂȂ���
	if (pAnother->m_bAttakHit == false)
	{
		// �ʂ̃v���C���[�̃��[�V�������r
		switch (pAnother->m_pModelCharacter->GetMotion())
		{
		case CMotion::PLAYER_ATTACK_0:
			// �ʂ̃v���C���[���U��0�����Ă��鎞
			this->AnotherPlayerAttack0(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_1:
			// �ʂ̃v���C���[���U��1�����Ă��鎞
			this->AnotherPlayerAttack1(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_2:
			// �ʂ̃v���C���[���U��2�����Ă��鎞
			this->AnotherPlayerAttack2(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_3:
			// �ʂ̃v���C���[���U��3�����Ă��鎞
			this->AnotherPlayerAttack3(pAnother);
			break;
		case CMotion::PLAYER_SMASH:
			// �ʂ̃v���C���[���X�}�b�V���U�����Ă���Ƃ�
			this->AnotherPlayerSmash(pAnother);
			break;
		}
	}
}

//==================================================================================================================
// �U�����Ă鏀�����ł��Ă��邩
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
		case CMotion::PLAYER_SMASH:    return true;
		}
	}
	return false;
}

//==================================================================================================================
// �U�����Ă鏀�����ł��Ă��邩
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
		case CMotion::PLAYER_SMASH:    return true;
		}
	}
	return false;
}

//==================================================================================================================
//�@�U��0�𓖂Ă����
//==================================================================================================================
bool CPlayer::HitConditionAttack0(const int &nCapColliID)
{
	if((nCapColliID != CCapsuleCollider::TYPEID_FOREARM_L) ||
		(nCapColliID != CCapsuleCollider::TYPEID_UPPERARM_L))
	{
		return false;
	}
	int nKey = CMotion::GetAttackKey((PARAM_TYPE)(m_type / 2),this->m_pModelCharacter->GetMotion());

	return (nKey == m_pModelCharacter->GetNowKey());
}

//==================================================================================================================
//�@�U��1�𓖂Ă����
//==================================================================================================================
bool CPlayer::HitConditionAttack1(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
//�@�U��2�𓖂Ă����
//==================================================================================================================
bool CPlayer::HitConditionAttack2(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_L) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_L);
}

//==================================================================================================================
//�@�U��3�𓖂Ă����
//==================================================================================================================
bool CPlayer::HitConditionAttack3(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
//�@�X�}�b�V���U�𓖂Ă����
//==================================================================================================================
bool CPlayer::HitConditionSmash(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
// �Ⴄ�v���C���[�̎擾
//==================================================================================================================
CPlayer * CPlayer::GetAnotherPlayer(void)
{
	return (this->m_nPlayer == PLAYER_ONE) ? CGame::GetPlayer(PLAYER_TWO) : CGame::GetPlayer(PLAYER_ONE);
}

//==================================================================================================================
// �Q�[���p�b�h�̑���
//==================================================================================================================
void CPlayer::ControlGamepad(CInputGamepad * pGamepad)
{
	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fValueX, fValueY;

	// ���X�e�B�b�N�擾
	pGamepad->GetStickLeft(&fValueX, &fValueY);

	// �������͂���Ă��Ȃ���΁A�������Ȃ�
	if (FAILED(CKananLibrary::GetMoveByGamepad(pGamepad)) &&
		fValueX == 0 && fValueY == 0 && 
		!SmashJudge())
	{
		// �W�����v���łȂ���΁A�j���[�g����
		if (m_stateStand != STANDSTATE_JUMP&&
			m_stateStand != STANDSTATE_ATTACK && 
			m_stateStand != STANDSTATE_DOWN)
			m_stateStand = STANDSTATE_NEUTRAL;
		return;
	}

	if (CGame::GetGameState() == CGame::GAMESTATE_NORMAL)
	{
		// �X�}�b�V��
		Smash(pGamepad, nullptr);

		// �X�}�b�V���n���[�V�������͈ȍ~�̏��������Ȃ�
		if (m_stateStand == STANDSTATE_SMASHCHARGE ||
			m_stateStand == STANDSTATE_SMASH)
			// �������I����
			return;

		// �U������
		NormalAttack(pGamepad, nullptr);
		
		// �U�����ł���΁A�������I����
		if (m_stateStand == STANDSTATE_ATTACK)
			return;
	}

	// �W�����v
	Jump(pGamepad, nullptr);

	// �ړ�
	InputPadMove(pGamepad);
}

//==================================================================================================================
// �L�[�{�[�h�̑���
//==================================================================================================================
void CPlayer::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// ���͂���Ă��Ȃ���Ώ������I����
	if (FAILED(CKananLibrary::GetMoveByKeyboard(pKeyboard, m_nPlayer)) &&
		!SmashJudge())
	{
		// �W�����v���łȂ���΁A�j���[�g����
		if (m_stateStand != STANDSTATE_JUMP &&
			m_stateStand != STANDSTATE_ATTACK&&
			m_stateStand != STANDSTATE_DOWN)
			m_stateStand = STANDSTATE_NEUTRAL;
		return;
	}

	// �U���̓Q�[�����̂�
	if (CGame::GetGameState() == CGame::GAMESTATE_NORMAL)
	{
		// �X�}�b�V��
		Smash(nullptr, pKeyboard);

		// �X�}�b�V���n���[�V�������͈ȍ~�̏��������Ȃ�
		if (m_stateStand == STANDSTATE_SMASHCHARGE ||
			m_stateStand == STANDSTATE_SMASH)
			return;

		// �ʏ�U��
		NormalAttack(nullptr, pKeyboard);

		// �U�����ł���΁A�������I����
		if (m_stateStand == STANDSTATE_ATTACK)
			return;
	}

	// �W�����v
	Jump(nullptr, pKeyboard);

	// �ړ�����
	InputKeyMove(pKeyboard);
}

//==================================================================================================================
// �X�g�[���̎擾����
//==================================================================================================================
void CPlayer::CatchStone(CStone *pStone)
{
	CGame::RemoveNumStone(pStone->GetIndexPos());
	// �擾�X�g�[���̃^�C�v��L��
	int nStoneID = pStone->GetStoneID();
	m_bGetStoneType[nStoneID] = true;
	// �X�g�[���̎擾
	pStone->Catch();

	// �X�g�[���̎擾�������Z
	m_nNumStone++;
	// 3�擾���Ă���
	if (m_nNumStone >= 3)
	{
		// �ϐg
		this->m_bTrans = true;
		// SE
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_TRANSFORM);
		// BGM
		CRenderer::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_TRANS);
	}
	// ����ȊO
	else
	{
		// SE
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GETSTONE);
	}
}

//==================================================================================================================
// �ʂ̃v���C���[���U��0���Ă��鎞
//==================================================================================================================
void CPlayer::AnotherPlayerAttack0(CPlayer * pAnother)
{
	// �_���[�W
	this->Damage(2);
	if (!m_bTrans)
		// ����
		this->Daunted(20);
}

//==================================================================================================================
// �ʂ̃v���C���[���U��1���Ă��鎞
//==================================================================================================================
void CPlayer::AnotherPlayerAttack1(CPlayer * pAnother)
{
	// �_���[�W
	this->Damage(2);
	if (!m_bTrans)
		// ����
		this->Daunted(20);
}

//==================================================================================================================
// �ʂ̃v���C���[���U��2���Ă��鎞
//==================================================================================================================
void CPlayer::AnotherPlayerAttack2(CPlayer * pAnother)
{
	// �_���[�W
	this->Damage(2);
	if (!m_bTrans)
		// ����
		this->Daunted(20);
}

//==================================================================================================================
// �ʂ̃v���C���[���U��3���Ă��鎞
//==================================================================================================================
void CPlayer::AnotherPlayerAttack3(CPlayer * pAnother)
{
	// �_���[�W
	this->Damage(2);
	// �ϐg���ȊO�͐������
	if (!m_bTrans)
	{
		// �������
		BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_NORMAL);
		// ������т�L��
		m_stateStand = STANDSTATE_BLOWAWAY;
		if (m_nNumStone > 0)
		{
			// �����X�g�[��������炷
			m_nNumStone--;
			// �Ĕz�u�ł���悤�X�g�[�����g�p����Ă��Ȃ���Ԃɂ���
			CGame::RemoveTypeStone(CKananLibrary::DecideRandomValue(m_nNumStone + 1, m_bGetStoneType));
			// ���炵���X�g�[���𑦐���
			CGame::AppearStone();
		}
	}
}

//==================================================================================================================
// �ʂ̃v���C���[���X�}�b�V���U�����Ă��鎞
//==================================================================================================================
void CPlayer::AnotherPlayerSmash(CPlayer * pAnother)
{
	// �_���[�W
	this->Damage(2);
	// �ϐg���ȊO�͐������
	BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_SMASH);
	// �X�}�b�V���ɂ�鐁����т����s
	m_stateStand = STANDSTATE_SMASHBLOWAWAY;

}

//==================================================================================================================
//�_���[�W���󂯂�
//==================================================================================================================
void CPlayer::TakeDamage(CPlayer * pAnother, const int nAttackMotion)
{
	switch (nAttackMotion)
	{
	case CMotion::PLAYER_ATTACK_0: AnotherPlayerAttack0(pAnother);return;
	case CMotion::PLAYER_ATTACK_1: AnotherPlayerAttack1(pAnother);return;
	case CMotion::PLAYER_ATTACK_2: AnotherPlayerAttack2(pAnother);return;
	case CMotion::PLAYER_ATTACK_3: AnotherPlayerAttack3(pAnother);return;
	case CMotion::PLAYER_SMASH:    AnotherPlayerSmash(pAnother);return;
	}

	// ���Ă��t���O�𗧂Ă�
	pAnother->m_bAttakHit = true;
}

//==================================================================================================================
// �U��3�̃_���[�W���󂯂�
//==================================================================================================================
void CPlayer::TakeAttack3Damage(CPlayer * pAnother)
{
	// �_���[�W
	this->Damage(2);
	// �ϐg���ȊO�͐������
	if (!m_bTrans)
	{
		// �������
		BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_NORMAL);
		// ������т�L��
		m_stateStand = STANDSTATE_BLOWAWAY;
		if (m_nNumStone > 0)
		{
			// �����X�g�[��������炷
			m_nNumStone--;
			// �Ĕz�u�ł���悤�X�g�[�����g�p����Ă��Ȃ���Ԃɂ���
			CGame::RemoveTypeStone(CKananLibrary::DecideRandomValue(m_nNumStone + 1, m_bGetStoneType));
			// ���炵���X�g�[���𑦐���
			CGame::AppearStone();
		}
	}
}

//==================================================================================================================
// �X�}�b�V���U���̃_���[�W���󂯂�
//==================================================================================================================
void CPlayer::TakeSmashDamage(CPlayer * pAnother)
{
	// �_���[�W
	this->Damage(2);
	// �ϐg���ȊO�͐������
	BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_SMASH);
	// �X�}�b�V���ɂ�鐁����т����s
	m_stateStand = STANDSTATE_SMASHBLOWAWAY;
	// ���Ă��t���O�𗧂Ă�
	pAnother->m_bAttakHit = true;
}


//==================================================================================================================
// 
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
	case CMotion::PLAYER_SMASH:
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SMASHHIT);
		break;
	}

}

//==================================================================================================================
// �v���C���[�ԍ��ݒ菈��
//==================================================================================================================
void CPlayer::SetnPlayer(int nPlayerNum)
{
	m_nPlayer = nPlayerNum;
}

//==================================================================================================================
// �������
//==================================================================================================================
inline bool CPlayer::BlowAway(CPlayer *pAnother, const float MoveVecY, const float fBlowAwayForce)
{
	// NULL���������������Ȃ�
	if (pAnother == NULL)
	{
		return false;
	}
	// �ϐ��錾
	D3DXVECTOR3 MoveVec;	// �ړ��x�N�g��

	MoveVec.x = sinf(pAnother->m_rot.y + D3DX_PI);
	MoveVec.y = MoveVecY;
	MoveVec.z = cosf(pAnother->m_rot.y + D3DX_PI);

	// �Ԃ���у��[�V����
	m_pModelCharacter->ResetMotion();
	m_pModelCharacter->SetMotion(CMotion::PLAYER_BLOWAWAY);
	// ����������
	m_rotDest.y = pAnother->m_rot.y + D3DX_PI;
	// ��]�̕��
	CKananLibrary::InterpolationFloat(m_rotDest.y);
	// ��u�Ō�����ς���
	m_rot.y = m_rotDest.y;

	// �ړ��l�ɉ��Z
	this->m_move.x = MoveVec.x * fBlowAwayForce;
	this->m_move.z = MoveVec.z * fBlowAwayForce;
	this->m_move.y = MoveVec.y * fBlowAwayForce;

	return true;
}

#ifdef _DEBUG
//==================================================================================================================
// ImGui�̍X�V
//==================================================================================================================
void CPlayer::ShowDebugInfo()
{
	char cHead[8];
	sprintf(cHead, "Player%d", m_nPlayer);			// �v���C���[�ԍ��𕶎���ɒǉ�

	if (ImGui::CollapsingHeader(cHead))
	{
		int nAllFrame = m_pModelCharacter->GetAllFrame();
		// ���̕\��
		CKananLibrary::ShowOffsetInfo(GetPos(), GetRot(), GetMove());
		ImGui::Text("nLife       : %f", m_nLife);
		ImGui::Text("AttackFlow  : %d", m_nAttackFlow);
		ImGui::Text("AttackFrame : %d / %d", m_nCntState, m_nAttackFrame);
		if (m_stateStand == STANDSTATE_ATTACK)
		ImGui::Text("CancelFrame : %d ~ %d", m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.start,
			m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.end);
		ImGui::Text("GetNumStone : %d", m_nNumStone);
		if (m_bTrans)
			ImGui::Text("TransTime   : %d", TIME_TRANS - m_nCntTrans);
	}
}
#endif
