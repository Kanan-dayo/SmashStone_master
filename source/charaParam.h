// ===================================================================
//
// �L�����N�^�[�p�����[�^�[���� [ charaParam.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CHARAPARAM_H_
#define _CHARAPARAM_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "main.h"
#include "kananlibrary.h"

// ===================================================================
// �}�N����`
// ===================================================================

// ===================================================================
// �N���X��`
// ===================================================================
class CCharaParam
{
public:
	typedef enum
	{
		ATTACK_NORMAL_1 = 0,	// �ʏ�U��1
		ATTACK_NORMAL_2,		// �ʏ�U��2
		ATTACK_NORMAL_3,		// �ʏ�U��3
		ATTACK_NORMAL_4,		// �ʏ�U��4
		ATTACK_AIR_PUNCH,		// �󒆃p���`�U��
		ATTACK_AIR_KICK,		// �󒆃L�b�N�U��
		ATTACK_SMASH,			// �X�}�b�V���U��
		ATTACK_MAX				// �S�Ă̍U��
	} PLAYER_ATTACK_TYPE;

	typedef enum
	{
		BLOWAWAY_DAUNTED = 0,	// ����
		BLOWAYAY_NORMAL,		// �������
		BLOWAWAY_SMASH,			// �X�}�b�V���������
		BLOWAWAY_MAX			// �ő吔
	} BLOWAWAY_TYPE;			// ������т̃^�C�v

	typedef struct INT_START_END
	{	// int�^�̊J�n�`�I���t���[���̍\����
		int start;	// �J�n
		int end;	// �I��

		// �R���X�g���N�^
		INT_START_END() {};
		INT_START_END(int nStart, int nEnd)
		{
			start = nStart;
			end = nEnd;
		}
	} INT_START_END;

	typedef struct
	{	// �U���̃p�����[�^�[
		INT_START_END	CancelFrame;	// �L�����Z���t���[��
		int				nDamage;		// �_���[�W
		int				nDaunted;		// ���݃t���[��
		BLOWAWAY_TYPE	blowType;		// ������т̎��
	} MOTION_PARAM;

	typedef struct
	{	// �ړ��̃p�����[�^�[
		float fRunSpeed;	// �ړ���
		float fJumpPower;	// �W�����v��
	} MOVE_PARAM;

	typedef struct
	{	// �v���C���[�̃p�����[�^�[
		float		 fMaxLife;					// �ő�HP
		float		 fAttackPower;				// �U����
		float		 fDefensePower;				// �h���
		MOVE_PARAM	 moveParam;					// �ړ��̃p�����[�^�[
		MOTION_PARAM motionParam[ATTACK_MAX];	// ���[�V�����̃p�����[�^�[
	} PLAYER_PARAM;

	CCharaParam();			// �R���X�g���N�^
	~CCharaParam();			// �f�X�g���N�^

	static HRESULT         Load(void);						// �S�L�����N�^�[�̃p�����[�^�[���[�h
	static HRESULT		   Save(const PARAM_TYPE type);		// �e�L�����N�^�[�̃p�����[�^�[�Z�[�u
	static PLAYER_PARAM	   GetPlayerParam(PARAM_TYPE type) { return m_playerParam[type]; }	// �p�����[�^�[�̎擾
	static inline int	   &GetAttackDamage(PARAM_TYPE type, PLAYER_ATTACK_TYPE attackType)
		{ return m_playerParam[type].motionParam[attackType].nDamage; }						// �_���[�W�ʂ̎擾
	static inline int	   &GetAttackDaunted(PARAM_TYPE type, PLAYER_ATTACK_TYPE attackType)
		{ return m_playerParam[type].motionParam[attackType].nDaunted; }					// ���݃t���[���̎擾

#ifdef _DEBUG
	static void UpdateImGui(void);										// ImGui�̍X�V
	static bool GetShowWindow(void) { return m_bShowWindow; }			// �E�B���h�E�\���̎擾
	static void SetShowWindow(bool bShow) { m_bShowWindow = bShow; }	// �E�B���h�E�\���̐ݒ�
#endif

protected:
	static PLAYER_PARAM m_playerParam[PARAM_MAX];	// �v���C���[�̃p�����[�^�[

private:
	static char m_aFileName[PARAM_MAX][64];	// �ϐg�O�L���������̃p�����[�^�[�t�@�C����

#ifdef _DEBUG
	static void ShowCharaParam(const char cName[16], PARAM_TYPE type);
	static bool m_bShowWindow;				// ImGui�E�B���h�E��\�����邩
#endif
};

#endif
