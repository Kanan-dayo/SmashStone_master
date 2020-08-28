//==================================================================================================================
//
// �Q�[�����U���g��UI���� [ UI_gameResult.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_RESULT_H_
#define _UI_RESULT_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"

//==================================================================================================================
//
// �|�[�Y�N���X�̒�`
//
//==================================================================================================================
// �O���錾
class CPolygon2D;

class CUI_GameResult
{
public:
	CUI_GameResult();		// �R���X�g���N�^
	~CUI_GameResult();		// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	static CUI_GameResult *Create(void);	// ��������
	static HRESULT Load(void);				// ���[�h
	static void Unload(void);				// �A�����[�h

protected:

private:
	typedef enum
	{
		RESULTUITYPE_VICTORY = 0,	// victory
		RESULTUITYPE_FRAME,			// �g
		RESULTUITYPE_CURSOR,		// �J�[�\��
		RESULTUITYPE_REMATCH,		// �Đ�
		RESULTUITYPE_RETURNSELECT,	// �L�����Z���N�g
		RESULTUITYPE_RETURNTITLE,		// �^�C�g����
		RESULTUITYPE_MAX			// �ő吔
	} TEX_TYPE_GAMERESULT;			// �Q�[�����U���g�̃e�N�X�`���̃^�C�v

	typedef enum
	{
		FASE_VICTORY,	// victory�\��
		FASE_RESULT,	// ���U���g�\��
		FASE_MAX		// �ő吔
	} GAMESTART_FASE;	// UI�\���̏���

	void Victory(void);
	void Result(void);
	void CursorUp(void);
	void CursorDown(void);
	void CursorDecide(void);
	void CreateUI(const int type);								// UI�̐���
	void MoveUI(void);											// UI�̈ړ�
	void NextFase(void) { m_fase++; m_nCntAny = 0; }			// ���̃t�F�[�Y�Ɉڍs
	static LPDIRECT3DTEXTURE9 m_pTexture[RESULTUITYPE_MAX];		// �e�N�X�`�����
	static CPolygon2D		  *m_pPolygon[RESULTUITYPE_MAX];	// �e�N�X�`��������2DUI
	static char				  *m_apFileName[RESULTUITYPE_MAX];	// �e�N�X�`���̃t�@�C����
	static D3DXVECTOR3		  m_posBegin[RESULTUITYPE_MAX];		// UI�̏����ʒu
	static D3DXVECTOR3		  m_sizeBegin[RESULTUITYPE_MAX];	// UI�̏����T�C�Y
	int m_fase;													// �e�N�X�`�������̏���
	int m_nCntAny;												// UI�̈ړ���\���ȂǂɎg��
};
#endif
#pragma once
