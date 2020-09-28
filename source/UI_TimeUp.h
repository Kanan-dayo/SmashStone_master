//==================================================================================================================
//
// TimeUp��UI���� [ UI_KO.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_TIMEUP_H_
#define _UI_TIMEUP_H_

#define _CRT_SECURE_NO_WARNINGS							// �x�����

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

class CUITimeUp
{
public:
	CUITimeUp();			// �R���X�g���N�^
	~CUITimeUp();			// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	static CUITimeUp *Create(void);		// ��������
	static HRESULT Load(void);		// ���[�h
	static void Unload(void);		// �A�����[�h

protected:

private:
	typedef enum
	{
		TIMEUPSTATE_GATHER = 0,		// �ꂩ���ɏW�܂�
		TIMEUPSTATE_SHOW,			// UI���~�߂Č�����
		TIMEUPSTATE_DISTRIBUTED,	// UI�̕��U
	} TIMEUP_STATE;

	typedef enum
	{
		TIMEUPUITEX_1 = 0,	// TimeUp
		TIMEUPUITEX_2,		// TimeUp
		TIMEUPUITEX_3,		// TimeUp
		TIMEUPUI_MAX		// �ő吔
	} TIMEUP_TEX_TYPE;		// TimeUp�e�N�X�`���̃^�C�v

	void Gather(void);
	void Show(void);
	void Distributed(void);
	void CreateUI(int type);		// UI�̐���
	void NextFase(void) { m_nCntAny = 0; }	// ���̃t�F�[�Y�Ɉڍs
	static TIMEUP_STATE m_stateTimeUp;				// UI�̏��
	static LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`�����
	static char *m_apFileName;						// �e�N�X�`���̃t�@�C����
	static CPolygon2D *m_pPolygon[TIMEUPUI_MAX];	// �e�N�X�`��������2DUI
	static D3DXVECTOR3 m_sizeBegin[TIMEUPUI_MAX];	// �ŏ��̃e�N�X�`���̃T�C�Y
	static D3DXVECTOR3 m_sizeEnd[TIMEUPUI_MAX];		// �Ō�̃e�N�X�`���̃T�C�Y
	static D3DXVECTOR3 m_posBegin[TIMEUPUI_MAX];	// �ŏ��̃e�N�X�`���̈ʒu
	static D3DXVECTOR3 m_posEnd[TIMEUPUI_MAX];		// �Ō�̃e�N�X�`���̈ʒu
	static D3DXCOLOR   m_col[TIMEUPUI_MAX];			// �g�k���̃J���[
	int m_nCntAny;									// UI�̈ړ���\���ȂǂɎg��
};
#endif
