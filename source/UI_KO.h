//==================================================================================================================
//
// KO��UI���� [ UI_KO.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_KO_H_
#define _UI_KO_H_

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

class CUIKO
{
public:
	CUIKO();	// �R���X�g���N�^
	~CUIKO();				// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	static CUIKO *Create(void);		// ��������
	static HRESULT Load(void);		// ���[�h
	static void Unload(void);		// �A�����[�h

protected:

private:
	typedef enum
	{
		KOUITEX_K = 0,	// K
		KOUITEX_O,		// O
		KOUITEX_BACK,	// �w�i
		KOUI_MAX		// �ő吔
	} KO_TEX_TYPE;		// KO�e�N�X�`���̃^�C�v

	void MoveUI(void);				// UI�̈ړ�
	void CreateUI(int type);		// UI�̐���
	void NextFase(void) { m_nCntFase++; m_nCntAny = 0; }	// ���̃t�F�[�Y�Ɉڍs
	static LPDIRECT3DTEXTURE9 m_pTexture[KOUI_MAX];	// �e�N�X�`�����
	static CPolygon2D *m_pPolygon[KOUI_MAX];			// �e�N�X�`��������2DUI
	static char *m_apFileName[KOUI_MAX];			// �e�N�X�`���̃t�@�C����
	static D3DXVECTOR3 m_sizeBegin[KOUI_MAX];		// �ŏ��̃e�N�X�`���̃T�C�Y
	static D3DXVECTOR3 m_sizeEnd[KOUI_MAX];			// �Ō�̃e�N�X�`���̃T�C�Y
	static D3DXVECTOR3 m_posBegin[KOUI_MAX];		// �ŏ��̃e�N�X�`���̈ʒu
	static D3DXVECTOR3 m_posEnd[KOUI_MAX];			// �Ō�̃e�N�X�`���̈ʒu
	int m_nCntFase;									// �e�N�X�`�������̏���
	int m_nCntAny;									// UI�̈ړ���\���ȂǂɎg��
};
#endif
