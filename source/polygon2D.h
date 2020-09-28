//==================================================================================================================
//
// �V�[�����g��Ȃ�2D�|���S������ [ polygon2D.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _2DPOLYGON_H_
#define _2DPOLYGON_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================

//==================================================================================================================
//
// �`��2D�N���X�̒�`
//
//==================================================================================================================
class CPolygon2D
{
public:
	typedef enum
	{
		POSSTART_TOP_LEFT,			// �㍶
		POSSTART_TOP_CENTRAL,		// �㒆��
		POSSTART_TOP_RIGHT,			// ��E
		POSSTART_CENTRAL_LEFT,		// ������
		POSSTART_CENTRAL_CENTRAL,	// ��������
		POSSTART_CENTRAL_RIGHT,		// �����E
		POSSTART_BOTTOM_LEFT,		// ����
		POSSTART_BOTTOM_CENTRAL,	// ������
		POSSTART_BOTTOM_RIGHT,		// ���E
		POSSTART_MAX				// �ő吔
	} POSSTART_TYPE;				// �ʒu�̎n�_�̎��

	CPolygon2D();		// �R���X�g���N�^
	~CPolygon2D();		// �f�X�g���N�^
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void Draw(void);	// �`��

	static CPolygon2D *Create(void);										// ����
	void BindTexture(const LPDIRECT3DTEXTURE9 tex)	{ m_pTexture = tex; }	// �e�N�X�`���̃o�C���h

	void SetPosStart(const POSSTART_TYPE posStart)	{ m_posStart = posStart; }	// �ʒu�̎n�_
	void SetPos(const D3DXVECTOR3 & pos)			{ m_pos = pos; }			// �ʒu�ݒ菈��
	void SetbShow(bool bShow)						{ m_bShow = bShow; }		// �`�悷�邩�ݒ�
	void SetCol(const D3DXCOLOR & col);											// �F�ݒ菈��
	void SetSize(const D3DXVECTOR3 & size);										// �傫���ݒ菈��
	void SetAnim(const D3DXVECTOR2 & UV, const D3DXVECTOR2 & size);				// �e�N�X�`�����W�ݒ�

	D3DXVECTOR3 &GetPos(void)	{ return m_pos; }						// �ʒu�擾����
	D3DXVECTOR3 &GetSize(void)	{ return m_size; }						// �傫���擾����
	D3DXCOLOR	&GetCol(void)	{ return m_col; }						// �J���[�̎擾
	bool GetbShow(void)			{ return m_bShow; }						// �`�悷�邩�擾

protected:
	POSSTART_TYPE m_posStart;											// �ʒu�̎��_
	D3DXVECTOR3 m_pos;													// �ʒu
	D3DXVECTOR3 m_size;													// �傫��
	D3DXVECTOR3 m_rot;													// ��]
	D3DXCOLOR m_col;													// �J���[
	bool  m_bShow;														// �`�悷�邩
	float m_fAngle;														// �p�x
	float m_fLength;													// ����

private:
	void MakeVertexPos(void);											// ���_���W�̐ݒ�
	LPDIRECT3DTEXTURE9 m_pTexture;										// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;									// ���_�o�b�t�@

};
#endif