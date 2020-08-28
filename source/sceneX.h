//==================================================================================================================
//
// X�t�@�C���̕`��[sceneX.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"
#include "kananlibrary.h"

//==================================================================================================================
//
// �`��X�N���X�̒�`
//
//==================================================================================================================
class CSceneX : public CScene
{
public:
	CSceneX();
	CSceneX(PRIORITY type);															// �R���X�g���N�^
	~CSceneX();																		// �f�X�g���N�^
	virtual void Init(void);														// ����������
	virtual void Uninit(void);														// �I������
	virtual void Update(void);														// �X�V����
	virtual void Draw(void);														// �`�揈��
	virtual void DrawMesh(void);													// ���b�V���̕`��
	virtual void DrawShadow(void);													// �e�̕`��

	static CSceneX *Create(void);													// ��������

	void BindTex(LPDIRECT3DTEXTURE9 tex)	{ m_pModelInfo->pTexture = tex; }	// �e�N�X�`����������
	void BindModel(MODELINFO &pModelInfo)	{ m_pModelInfo = &pModelInfo; }		// ���f����������
	void SetPos(CONST D3DXVECTOR3 &pos)		{ m_pos = pos; }					// �ʒu�ݒ菈��
	void SetSize(CONST D3DXVECTOR3 &size)	{ m_size = size; }					// �傫���ݒ菈��
	void SetMove(CONST D3DXVECTOR3 &move)	{ m_move = move; }					// �ړ��ʐݒ菈��
	void SetRot(CONST D3DXVECTOR3 &rot)		{ m_rot = rot; }					// ��]�ݒ菈��
	void SetVecAxis(D3DXVECTOR3 vecAxis)	{ m_vecAxis = vecAxis; }			// ��]���ݒ菈��
	void SetbParent(CONST bool &bParent)	{ m_bParent = bParent; }			// �e�����݂���
	void SetValueRot(float ValueRot)		{ m_fValueRot = ValueRot; }			// ��]�p�ݒ菈��
	void SetMtxParent(D3DXMATRIX *mtxParent) { m_mtxParent = mtxParent; }	// �e�̃}�g���b�N�X�̐ݒ�

	bool SetCollisionBox(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 posOld);	// ��`�̓����蔻�菈��

	D3DXVECTOR3 *GetPos(void)		{ return &m_pos; }							// �ʒu�擾����
	D3DXVECTOR3 *GetPosOld(void)	{ return &m_posOld; }						// �O��̈ʒu�擾
	D3DXVECTOR3 *GetSize(void)		{ return &m_size; }							// �傫���擾����
	D3DXVECTOR3 *GetMove(void)		{ return &m_move; }							// �ړ��ʎ擾����
	D3DXVECTOR3 *GetRot(void)		{ return &m_rot; }							// ��]�擾����
	D3DXVECTOR3 *GetVecAxis(void)	{ return &m_vecAxis; }						// ��]���擾����
	D3DXMATRIX	*GetMatrix(void)	{ return &m_mtxWorld; }						// �}�g���b�N�X�̎擾
	float GetValueRot(void)			{ return m_fValueRot; }						// ��]�p�擾����

protected:
	MODELINFO *m_pModelInfo;					// ���b�V����}�e���A���̏��Q�Ɨp�|�C���^
	D3DXMATRIX m_mtxRot;						// ��]�}�g���b�N�X�i�ۑ��p�j
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXMATRIX *m_mtxParent;					// �e�̃}�g���b�N�X
	D3DXQUATERNION m_quat;						// �N�H�[�^�j�I��

	D3DXVECTOR3 m_vecAxis;						// ��]��
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// �O��̈ʒu
	D3DXVECTOR3 m_size;							// �傫��
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_rot;							// ��]

	float m_fValueRot;							// ��]�p�i��]�ʁj
	float m_fAngle;								// �p�x
	float m_fLength;							// ����
	bool  m_bParent;							// �e���f�������݂���
private:

};
#endif