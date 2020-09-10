//=============================================================================
//
// ���f���̃L�����N�^�[����  [ modelCharacter.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _MODELCHARACTER_H_
#define _MODELCHARACTER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "sceneX.h"
#include "motion.h"
#include "kananlibrary.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CModelParts;

class CModelCharacter
{
public:

	CModelCharacter();
	~CModelCharacter();

	HRESULT Init();													// ������
	void Uninit();													// �I��
	void Update();													// �X�V
	void Draw();													// �`��
	static CModelCharacter *Create(CHARACTER_TYPE type);			// ����
	static HRESULT Load(void);										// ���f���̃��[�h
	static void Unload(void);										// ���f���̔j��

	void SetCharacterMtx(D3DXMATRIX *mtx);							// �}�g���b�N�X�̐ݒ�
	void SetModelType(CHARACTER_TYPE type) { m_type = type; }							// �L�����N�^�[�̃^�C�v
	void SetMotion(CMotion::MOTION_TYPE motiontype);				// ���[�V������ݒ�
	void ResetMotion();												// ���[�V�����̃��Z�b�g

	void ModelRebind(CHARACTER_TYPE type);							// ���f���̍ăo�C���h

	static int GetPartsNum(CHARACTER_TYPE type) { return m_pModelCharacter[type].nNumParts; }	// �L�����N�^�[�̃p�[�c�����擾
	CMotion::MOTION_TYPE GetMotion(void)		{ return m_motion; }							// ���[�V�������擾
	CModelParts *GetModelParts(void)			{ return m_pModelParts; }						// ���f���p�[�c�̎擾
	int GetAllFrame(void)						{ return m_nAllFrame; }							// ���݂̃��[�V�����̑��t���[�������擾
	int GetNowKey(void)							{ return m_nKey; }								// ���݂̃L�[�����擾
	bool AttackKeyCondition(void);																	// ���݂̃L�[���U����Ԃ��ǂ���

private:
	static MODELCHARACTER m_pModelCharacter[CHARACTER_MAX];			// �S�Ẵ��f���L�����N�^�̏��i�[
	CModelParts *m_pModelParts;										// ���f���̃p�[�c�̃|�C���^
	CHARACTER_TYPE m_type;											// ���f���̎��
	D3DXMATRIX *m_CharacterMtx;										// �L�����N�^�[�̃}�g���b�N�X
	CMotion::MOTION_TYPE m_motion;									// ���[�V�������
	int m_nFrame;													// ���݂̃t���[��
	int m_nKey;														// ���݂̃L�[
	int m_nAllFrame;												// ���݂̃��[�V�����̑��t���[��

	HRESULT LoadOffset(CHARACTER_TYPE type);						// �I�t�Z�b�g�ǂݍ���
	static HRESULT LoadFileName(CHARACTER_TYPE type);				// ���f�����ǂݍ���

#ifdef _DEBUG
	void ShowModelCharaInfo(void);									// ImGui�̍X�V
#endif

	static char m_aFileName[CHARACTER_MAX][64];
};
#endif