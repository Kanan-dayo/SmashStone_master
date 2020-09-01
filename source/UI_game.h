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
#include "scene.h"
#include "stone.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CScene2D;

//==================================================================================================================
//
// ���S�N���X�̒�`
//
//==================================================================================================================
class CUI_game : public CScene
{
public:
	//=============================================================================
	// �񋓌^��`
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_GAMEBG,			// �Q�[���w�i
		LOGOTYPE_JEWELRYBG1P,		// ��Δw�i1P
		LOGOTYPE_JEWELRYBG2P,		// ��Δw�i2P
		LOGOTYPE_JEWELRYRED,		// ��ΐ�
		LOGOTYPE_JEWELRYBULE,		// ��ΐ�
		LOGOTYPE_JEWELRYGREEN,		// ��Η�
		LOGOTYPE_PLAYER1,			// �v���C���[�A�C�R��1P
		LOGOTYPE_PLAYER2,			// �v���C���[�A�C�R��2P
		LOGOTYPE_CHARANAME0,		// 1P�̃L�����N�^�[�l�[��
		LOGOTYPE_CHARANAME1,		// 2P�̃L�����N�^�[�l�[��
		LOGOTYPE_MAX				// �ő�
	} UITYPE;

	CUI_game(PRIORITY type);		// �R���X�g���N�^
	~CUI_game();					// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CUI_game *Create(void);	// ��������
	static HRESULT Load(void);		// �e�N�X�`����񃍁[�h����
	static void Unload(void);		// �e�N�X�`�����A�����[�h����

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ���S�ݒ菈��

protected:

private:
	void GameUpdate(void);								// �Q�[���̍X�V����

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// �e�N�X�`�����
	static char *m_apFileName[LOGOTYPE_MAX];			// ���S�̃t�@�C����

	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2D�̏��

	int m_nCharaNum[MAX_PLAYER];						// �L�����ԍ�

	bool m_bTransform[MAX_PLAYER];						// �ϐg�������ǂ���
	bool m_bStoneID[MAX_PLAYER][CStone::STONE_ID_MAX];	// �X�g�[��ID

};
#endif