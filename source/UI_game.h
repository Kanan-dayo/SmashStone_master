//==================================================================================================================
//
// UI_game[UI_game.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_GAME_H_
#define _UI_GAME_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "stone.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CPolygon2D;
class CStone;

//==================================================================================================================
//
// ���S�N���X�̒�`
//
//==================================================================================================================
class CUI_game
{
public:
	CUI_game();		// �R���X�g���N�^
	~CUI_game();					// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CUI_game *Create(void);	// ��������
	static HRESULT Load(void);		// �e�N�X�`����񃍁[�h����
	static void Unload(void);		// �e�N�X�`�����A�����[�h����

	static void CreateUI(int nPlayer, int type, D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ���S�ݒ菈��
	static void DeleteUI(int nPlayer, int type);									// ���S�j��

	void CreateBG();											// �w�i�̐���

	static void SetbDisp(bool bDisp)	{ m_bDisplay = bDisp; }	// �\���̐ݒ�
	static bool GetbDisp(void)			{ return m_bDisplay; }	// �\���̎擾

	static void CatchStone(int nPlayer, CStone::STONE_ID type);
	static void ReleaseStone(int nPlayer, CStone::STONE_ID type);
	static void TransPlayer(int nPlayer);
	static void FinishTrans(int nPlayer);

protected:

private:
	// �v���C���[�e�N�X�`���̎��
	typedef enum
	{
		LOGOTYPE_JEWELRYBG,			// ��Δw�i
		LOGOTYPE_JEWELRYRED,		// ��ΐ�
		LOGOTYPE_JEWELRYGREEN,		// ��Η�
		LOGOTYPE_JEWELRYBULE,		// ��ΐ�
		LOGOTYPE_PLAYERICON,		// �v���C���[�A�C�R��
		LOGOTYPE_CHARANAME,			// �L�����N�^�[�l�[��
		LOGOTYPE_MAX				// �ő�
	} UI_GAME_TYPE;

	void SetDefaultUI(void);							// �J�n����UI���Z�b�g

	static LPDIRECT3DTEXTURE9 m_pTexBG;						// �e�N�X�`�����
	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];		// �e�N�X�`�����
	static char m_FileBG[MAX_TEXT];							// �Q�[��BG�̃t�@�C����
	static char *m_apFileName[LOGOTYPE_MAX];				// ���S�̃t�@�C����

	CPolygon2D *m_pPolyBG;									// �\������BG�摜
	static CPolygon2D *m_pPolygon[MAX_PLAYER][LOGOTYPE_MAX];// �\������v���C���[�摜
	static D3DXVECTOR3 m_posUI[MAX_PLAYER][LOGOTYPE_MAX];	// UI�̍��W
	static D3DXVECTOR3 m_sizeUI[MAX_PLAYER][LOGOTYPE_MAX];	// UI�̃T�C�Y

	static bool m_bDisplay;									// UI��\�����邩�ǂ���
};
#endif