//==================================================================================================================
//
// �Q�[���J�n����UI���� [ UI_gameStart.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_START_H_
#define _UI_START_H_

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

class CUI_GameStart
{
public:
	CUI_GameStart();		// �R���X�g���N�^
	~CUI_GameStart();		// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	static CUI_GameStart *Create(void);		// ��������
	static HRESULT Load(void);				// ���[�h
	static void Unload(void);				// �A�����[�h

protected:

private:
	typedef enum
	{
		STARTUITYPE_ROUND = 0,	// ���E���h
		STARTUITYPE_NUMROUND,	// ���E���h��
		STARTUITYPE_FIGHT,		// �t�@�C�g
		STARTUITYPE_MAX			// �ő吔
	} UITYPE_GAMESTART;			// UI�̎��

	typedef enum
	{
		GAMEUITEX_ROUND = 0,// Round
		GAMEUITEX_1,		// 1
		GAMEUITEX_2,		// 2
		GAMEUITEX_3,		// 3
		GAMEUITEX_FIGHT,	// Fight
		GAMEUITEX_MAX		// �ő吔
	} TEX_TYPE_GAMESTART;	// �Q�[���J�n���̃e�N�X�`���̃^�C�v

	typedef enum
	{
		FASE_NONE = -1,
		FASE_ROUND,		// ���E���h
		FASE_RONNDNUM,	// ���E���h��
		FASE_FIGHT,		// fight
		FASE_MAX		// �ő吔
	} GAMESTART_FASE;	// UI�\���̏���

	void CreateUI(const int type);							// UI�̐���
	void MoveUI(const int type, const int time);			// UI�̈ړ�
	void NextFase(void) { m_fase++; m_nCntAny = 0; }		// ���̃t�F�[�Y�Ɉڍs
	static LPDIRECT3DTEXTURE9 m_pTexture[GAMEUITEX_MAX];	// �e�N�X�`�����
	static CPolygon2D		  *m_pPolygon[STARTUITYPE_MAX];	// �e�N�X�`��������2DUI
	static char				  *m_apFileName[GAMEUITEX_MAX];	// �e�N�X�`���̃t�@�C����
	static D3DXVECTOR3		  m_sizeBegin[STARTUITYPE_MAX];	// �ŏ��̃e�N�X�`���̃T�C�Y (ROUND, ROUND��, FIGHT��3��)
	static D3DXVECTOR3		  m_sizeEnd[STARTUITYPE_MAX];	// �Ō�̃e�N�X�`���̃T�C�Y
	static D3DXVECTOR3		  m_posBegin[STARTUITYPE_MAX];	// �ŏ��̃e�N�X�`���̈ʒu
	static D3DXVECTOR3		  m_posEnd[STARTUITYPE_MAX];	// �Ō�̃e�N�X�`���̈ʒu
	int m_fase;												// �e�N�X�`�������̏���
	int m_nCntAny;											// UI�̈ړ���\���ȂǂɎg��
};
#endif
