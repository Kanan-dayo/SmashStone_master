//=============================================================================
//
// �I�u�W�F�N�g���� [ object.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

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
class CObject
{
public:
	CObject();
	~CObject();

	void Init(void);								// ������
	void Uninit(void);								// �I��
	void Update(void);								// �X�V
	void Draw(void);								// �`��
	void DrawAlpha(void);							// �����`��

	static CObject *Create(void);					// ����

	void SetObjInfo(const D3DXVECTOR3 & pos,
		const D3DXVECTOR3 & rot,
		const D3DXVECTOR3 & scale,
		MODELINFO *pModelInfo,
		const int & type);					// �I�u�W�F�N�g�̏��ݒ�

	bool CollObject(D3DXVECTOR3 *pos,
		const D3DXVECTOR3 & posOld,
		D3DXVECTOR3 *move,
		const MODEL_VTX & modelVtx);				// �I�u�W�F�N�g�̏Փˏ���


	void SetCollider(void);							// �R���C�_�[�̐ݒ�

	D3DXVECTOR3 *GetPos(void)		{ return &m_pos; }			// �ʒu�̎擾
	D3DXVECTOR3 *GetPosOld(void)	{ return &m_posOld; }		// �O��̓���̎擾
	D3DXVECTOR3 *GetRot(void)		{ return &m_rot; }			// �����̎擾
	D3DXVECTOR3 *GetMove(void)		{ return &m_move; }			// �O��̌����̎擾
	D3DXVECTOR3 *GetScale(void)		{ return &m_scale; }		// �g�嗦�̎擾
	D3DXMATRIX	*GetMtx(void)		{ return &m_mtxWorld; }		// ���[���h�}�g���b�N�X�̎擾
	bool GetbColl(void)				{ return m_bCollision; }	// �����蔻����s����
	int	 GetType(void)				{ return m_nType; }			// �^�C�v�擾

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
	D3DXVECTOR3 m_scale;		// ���f���̊g�嗦
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	MODELINFO	m_pModelInfo;	// ���f�����
	int			m_nType;		// �I�u�W�F�N�g�^�C�v
	bool		m_bCollision;	// �����蔻�菈�����s����
	int			m_nColliderID;	// �R���C�_�[ID

#ifdef _DEBUG
	bool	m_bRelease;		// �����[�X���邩�ǂ���
#endif
};

#endif