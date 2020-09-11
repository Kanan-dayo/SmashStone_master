//=============================================================================
//
// ���[�V��������  [ motion.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "kananlibrary.h"
#include <vector>

//=============================================================================
// �N���X��`
//=============================================================================
class CModelCharacter;

class CMotion
{
public:
	// �L�[
	typedef struct
	{
		D3DXVECTOR3 posDest;		// �ړI�̈ʒu
		D3DXVECTOR3 rotDest;		// �ړI�̉�]
	} KEY;

	// �L�[���
	typedef struct
	{
		KEY *pKey;					// �L�[�̃|�C���^
		int nFrame;					// �t���[����
		float fMove;				// �ړ��l
		std::vector<int> nAttackIndex;	// �U���p�[�c�̎擾
	} KEY_INFO;

	// ���[�V�����̏��
	typedef struct
	{
		KEY_INFO *pKeyInfo;			// �L�[���̃|�C���^
		int nNumKey;				// �L�[��
		int nAttackKey;				// �U���L�[
		bool bLoop;					// ���[�v���邩�ǂ���
	} MOTION_INFO;

	// ���[�V����
	typedef enum
	{
		MOTION_NONE,
		PLAYER_NEUTRAL = 0,		// �v���C���[ �j���[�g����
		PLAYER_WALK,			// �v���C���[ ����
		PLAYER_JUMP,			// �v���C���[ �W�����v
		PLAYER_FALL,			// �v���C���[ ����
		PLAYER_LIFT,			// �v���C���[ ���̎����グ��
		PLAYER_LIFT_NEUTRAL,	// �v���C���[ �������j���[�g����
		PLAYER_LIFT_WALK,		// �v���C���[ ����������
		PLAYER_THROW,			// �v���C���[ ���̓���
		PLAYER_ATTACK_0,		// �v���C���[ �U��1
		PLAYER_ATTACK_1,		// �v���C���[ �U��2
		PLAYER_ATTACK_2,		// �v���C���[ �U��3
		PLAYER_ATTACK_3,		// �v���C���[ �U��4
		PLAYER_SMASH_CHARGE,	// �v���C���[ �X�}�b�V���`���[�W
		PLAYER_SMASH,			// �v���C���[ �X�}�b�V��
		PLAYER_DAUNTED,			// �v���C���[ ���݃_���[�W
		PLAYER_BLOWAWAY,		// �v���C���[ ������у_���[�W
		PLAYER_DOWN,			// �v���C���[ �_�E��
		PLAYER_GETUP,			// �v���C���[ �N���オ��
		PLAYER_GETUP_ACTIVE,	// �v���C���[ �A�N�e�B�u�ȋN���オ��
		//PLAYER_WALLDOWN,		// �v���C���[ �ǂ͓\��t��
		MOTION_MAX				// �ő�l
	} MOTION_TYPE;

	CMotion();
	~CMotion();

	static HRESULT Load();						// ���[�V�����̃��[�h
	static void UnLoad();						// ���[�V�����̃A�����[�h

	static D3DXVECTOR3 GetRotDest(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey, int nIndex)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].pKey[nIndex].rotDest; }	// ���[�V�����̉�]�̃S�[���擾
	static D3DXVECTOR3 GetPosDest(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey, int nIndex)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].pKey[nIndex].posDest; }	// ���[�V�����̉�]�̃S�[���擾
	static int GetFrame(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].nFrame; }					// ���[�V�����̑��t���[�����̎擾
	static float GetMotionMove(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].fMove; }					// ���[�V�����̈ړ��l�擾
	static int GetNumKey(PARAM_TYPE charaType, MOTION_TYPE motiontype)
		{ return m_pMotionInfo[charaType][motiontype].nNumKey; }								// ���[�V�����̑��L�[���̎擾
	static bool GetLoop(PARAM_TYPE charaType, MOTION_TYPE motiontype)
		{ return m_pMotionInfo[charaType][motiontype].bLoop; }									// ���[�V���������[�v���邩�ǂ����擾
	static int GetAttackKey(PARAM_TYPE charaType, MOTION_TYPE motiontype) 
		{ return m_pMotionInfo[charaType][motiontype].nAttackKey; }								// �U���L�[�̎擾
	static int *GetnAttackIndex(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].nAttackIndex.data(); }		// ���[�V�����̍U���p�[�c�̎擾
	static int GetnAttackIndexSize(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].nAttackIndex.size(); }		// ���[�V�����̍U���p�[�c�̃T�C�Y�̎擾

private:
	static MOTION_INFO m_pMotionInfo[MAX_CHARACTER_TYPE][CMotion::MOTION_MAX];		// ���[�V�������
	static HRESULT LoadMotion(PARAM_TYPE charaType, MOTION_TYPE motiontype);	// ���[�V�����ǂݍ���	

	static char m_apFileName[MAX_CHARACTER_TYPE][CMotion::MOTION_MAX][MAX_TEXT];
};
#endif