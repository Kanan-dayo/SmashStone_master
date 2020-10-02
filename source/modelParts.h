//=============================================================================
//
// ���f���̃p�[�c����  [ modelParts.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _MODELPARTS_H_
#define _MODELPARTS_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "sceneX.h"
#include "motion.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CModelParts
{
public:
	enum
	{
		PARTSNAME_NOEN = -1,		// ����
		PARTSNAME_BODY,				// ��
		PARTSNAME_HEAD,				// ��
		PARTSNAME_UPARM_R,			// ��r�E
		PARTSNAME_LOWARM_R,			// �O�r�E
		PARTSNAME_UPARM_L,			// ��r��
		PARTSNAME_LOWARM_L,			// �O�r��
		PARTSNAME_THIGH_R,			// ���ډE
		PARTSNAME_LEG_R,			// ���E
		PARTSNAME_THIGH_L,			// ���ڍ�
		PARTSNAME_LEG_L,			// ����
		PARTSNAME_MAX				// �ő吔
	};

	CModelParts();
	virtual ~CModelParts();

	void Init(void);																	// ������
	void Uninit(void);																	// �I��
	void Update(void);																	// �X�V
	void Draw(void);																	// �`��
	void DrawInvincible(int nInvincibleTime);											// �`��
	void DrawShadow(void);																// �e�̕`��

	void SetPartsTexInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture);		// �p�[�c�̏��ݒ�
	void SetPartsInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// �p�[�c�̏��ݒ�
	void SetMotionRotDest(CMotion::MOTION_TYPE motiontype, int nKey);					// ���[�V�����̖ړI�̉�]�̐ݒ�

	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }
	void SetUpdatePos(const D3DXVECTOR3 &pos) { m_UpdatePos = pos; }
	void SetUpdateRot(const D3DXVECTOR3 &rot) { m_UpdateRot = rot; }

	void SetMtxParent(D3DXMATRIX *pMtxParent)	{ m_mtxParent = pMtxParent; }		// �e�̃}�g���b�N�X�̐ݒ�
	void BindModelInfo(MODELINFO *pModelInfo)	{ m_pModelInfo = pModelInfo; }	// ���f�����̐ݒ�

	int			GetParent(void)		{ return m_nParent; }						// �e�ԍ��擾
	int			GetIndex(void)		{ return m_nIndex; }						// �p�[�c�̃C���f�b�N�X���擾
	D3DXMATRIX	*GetMtx(void)		{ return &m_mtxWorld; }						// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 *GetOffsetPos(void) { return &m_OffsetPos; }					// �I�t�Z�b�g�ʒu�̎擾
	D3DXVECTOR3 *GetOffsetRot(void) { return &m_OffsetRot; }					// �I�t�Z�b�g��]�̎擾
	D3DXVECTOR3 *GetPos(void)		{ return &m_pos; }							// �ʒu�̎擾
	D3DXVECTOR3 *GetRot(void)		{ return &m_rot; }							// ��]�̏���
	D3DXVECTOR3 *GetUpdatePos(void) { return &m_UpdatePos; }					// �����̈ʒu���擾
	D3DXVECTOR3 *GetUpdateRot(void) { return &m_UpdateRot; }					// �����̉�]���擾

	void ShowModelPartsInfo(void);														// ImGui�̍X�V

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;	// ���f���̃e�N�X�`��

	D3DXVECTOR3	m_pos;				// ���W
	D3DXVECTOR3	m_rot;				// ��]
	D3DXVECTOR3 m_OffsetPos;		// ���f���̃I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_OffsetRot;		// ���f���̃I�t�Z�b�g��]
	D3DXVECTOR3 m_UpdatePos;		// ���[�V�����p�̍����̍��W
	D3DXVECTOR3 m_UpdateRot;		// ���[�V�����p�̍����̉�]
	D3DXMATRIX	m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX	*m_mtxParent;		// �e�̃}�g���b�N�X
	MODELINFO	*m_pModelInfo;		// ���f�����
	int m_nIndex;					// �����̃C���f�b�N�X
	int m_nParent;					// �e�̔ԍ�

private:

};
#endif