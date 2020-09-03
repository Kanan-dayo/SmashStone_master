#pragma once
//=============================================================================
//
// ���Ă�I�u�W�F�N�g���� [ objectLift.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _OBJECTLIFT_H_
#define _OBJECTLIFT_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "kananlibrary.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �N���X��`
//=============================================================================
class CObjectLift
{
public:
	typedef enum
	{
		LIFTTYPE_BOX = 0,	// ���^�I�u�W�F�N�g
		LIFTTYPE_SPHERE,	// ���^�I�u�W�F�N�g
		LIFTTYPE_MAX		// �ő吔
	} LIFTCOLLTYPE;

	CObjectLift();
	~CObjectLift();

	void Init(void);								// ������
	void Uninit(void);								// �I��
	void Update(void);								// �X�V
	void Draw(void);								// �`��
	void DrawAlpha(void);							// �����`��

	static CObjectLift *Create(void);					// ����

	void SetObjInfo(const D3DXVECTOR3 & pos,
		const D3DXVECTOR3 & rot,
		MODELINFO *pModelInfo,
		const int & type);							// �I�u�W�F�N�g�̏��ݒ�

	void SetCollider(void);							// �R���C�_�[�̐ݒ�

	D3DXVECTOR3 &GetPos(void)		{ return m_pos; }			// �ʒu�̎擾
	D3DXVECTOR3 &GetPosOld(void)	{ return m_posOld; }		// �O��̓���̎擾
	D3DXVECTOR3 &GetposBegin(void)	{ return m_posBegin; }		// �O��̓���̎擾
	D3DXVECTOR3 &GetRot(void)		{ return m_rot; }			// �����̎擾
	D3DXVECTOR3 &GetMove(void)		{ return m_move; }			// �O��̌����̎擾
	D3DXMATRIX	&GetMtx(void)		{ return m_mtxWorld; }		// ���[���h�}�g���b�N�X�̎擾
	int	 &GetType(void)				{ return m_nType; }			// �^�C�v�擾

#ifdef _DEBUG
	bool GetRelease(void) { return m_bRelease; }		// �����[�X���邩�ǂ���
	void ShowObjectInfo(char cPrintText[16]);
#endif

private:
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_posOld;		// �O��̈ʒu
	D3DXVECTOR3 m_posBegin;		// �J�n�ʒu
	D3DXVECTOR3 m_move;			// �ړ���
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_rotBegin;		// �J�n����
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	MODELINFO	m_pModelInfo;	// ���f�����
	int			m_nType;		// �I�u�W�F�N�g�^�C�v
	int			m_nColliderID;	// �R���C�_�[ID

#ifdef _DEBUG
	bool	m_bRelease;		// �����[�X���邩�ǂ���
#endif
};

#endif