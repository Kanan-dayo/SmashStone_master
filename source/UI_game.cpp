//==================================================================================================================
//
// UI_game����[UI_game.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_game.h"
#include "renderer.h"
#include "player.h"
#include "scene2D.h"
#include "kananlibrary.h"
#include "sound.h"
#include "game.h"
#include "inputKeyboard.h"
#include "debugProc.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define STONE_POS_Y 620								// �Έʒu�c
#define STONE_SIZE_X 60								// �Α傫��X
#define STONE_SIZE_Y 50								// �Α傫��Y

//==================================================================================================================
// �ÓI�����o�[�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_game::m_pTexture[LOGOTYPE_MAX] = {};		// �e�N�X�`�����
char *CUI_game::m_apFileName[LOGOTYPE_MAX] =					// �ǂݍ��ރ��f���̃\�[�X��
{
	{ "data/TEXTURE/gameBG.png" },		// �Q�[���w�i
	{ "data/TEXTURE/jewelryBG.png" },	// ��Δw�i1P
	{ "data/TEXTURE/jewelryBG.png" },	// ��Δw�i2P
	{ "data/TEXTURE/jewelryRed.png" },	// ��ΐ�
	{ "data/TEXTURE/jewelryBule.png" },	// ��ΐ�
	{ "data/TEXTURE/jewelryGreen.png" },// ��Η�
	{ "data/TEXTURE/FULLchara.png" },	// �L����1P
	{ "data/TEXTURE/FULLchara.png" },	// �L����2P
	{ "data/TEXTURE/charaName0.png" },	// 1P�̃L�����N�^�[�l�[��
	{ "data/TEXTURE/charaName1.png" },	// 2P�̃L�����N�^�[�l�[��
};

//==================================================================================================================
//
// �R���X�g���N�^
//
//==================================================================================================================
CUI_game::CUI_game(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// �f�X�g���N�^
//
//==================================================================================================================
CUI_game::~CUI_game()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUI_game::Init(void)
{
	m_bDisplay = true;		// UI��\�����邩�ǂ���

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_nCharaNum[nCnt] = 0;	// �L�����N�^�[�ԍ�
		m_bTransform[nCnt] = false;// �ϐg�������ǂ���

		for (int nCntType = 0; nCntType < CStone::STONE_ID_MAX; nCntType++)
		{
			m_bStoneID[nCnt][nCntType] = false;// �X�g�[�����擾�������ǂ���
		}
	}

	// ���S�̍ő喇���J�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// ��������
		m_pScene2D[nCnt] = CScene2D::Create();

		// �e�N�X�`����\��
		m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
	}
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CUI_game::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI_game::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// UI�Q�[���X�V����
	GameUpdate();

#ifdef _DEBUG
	// �\�����Ă���Ƃ�
	if (m_bDisplay)
	{
		// �L�[�{�[�h��F3���������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(DIK_F3))
		{
			// �\�������Ȃ��悤�ɂ���
			m_bDisplay = false;
		}
		CDebugProc::Print("UI��\��[F3]\n");

	}
	else
	{// �\�����Ă��Ȃ��Ƃ�
		// �L�[�{�[�h��F3���������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(DIK_F3))
		{
			// �\��������悤�ɂ���
			m_bDisplay = true;
		}
		CDebugProc::Print("UI�\��[F3]\n");
	}

#endif // DEBUG

}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI_game::Draw(void)
{

}

//==================================================================================================================
// ��������
//==================================================================================================================
CUI_game *CUI_game::Create(void)
{
	// �V�[�����I�Ɋm��
	CUI_game *pUI = new CUI_game(CScene::PRIORITY_UI);

	// �V�[��������
	pUI->Init();

	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`�����[�h
//==================================================================================================================
HRESULT CUI_game::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾

	//==============================�e�N�X�`���̓ǂݍ���==============================//
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
	}

	// �l��Ԃ�
	return S_OK;
}

//==================================================================================================================
// �j��
//==================================================================================================================
void CUI_game::Unload(void)
{
	// ���S�̍ő��ނ܂ŃJ�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// �J��
		m_pTexture[nCnt] = NULL;			// NULL�ɂ���
	}
}

//==================================================================================================================
// �Q�[���X�V����
//==================================================================================================================
void CUI_game::GameUpdate(void)
{
	CPlayer *pPlayer0, *pPlayer1;

	// �v���C���[���擾
	pPlayer0 = CGame::GetPlayer(0);
	pPlayer1 = CGame::GetPlayer(1);

	// �v���C���[��2�̂���Ƃ�
	if (pPlayer0 != NULL && pPlayer1 != NULL)
	{
		// �v���C���[�ԍ��擾
		m_nCharaNum[0] = pPlayer0->GetCharaType();
		m_nCharaNum[1] = pPlayer1->GetCharaType();

		// �ϐg�󋵎擾
		m_bTransform[0] = pPlayer0->GetTrans();
		m_bTransform[1] = pPlayer1->GetTrans();

		// �ő�΂܂ŃJ�E���g
		for (int nCntStone = 0; nCntStone < CStone::STONE_ID_MAX; nCntStone++)
		{
			// �e�v���C���[���ǂ̎�ނ̐΂��l��������
			m_bStoneID[0][nCntStone] = pPlayer0->GetStoneType(nCntStone);
			m_bStoneID[1][nCntStone] = pPlayer1->GetStoneType(nCntStone);
		}
	}

	// �Q�[���w�i
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_GAMEBG, WhiteColor, m_bDisplay);

	if (!m_bTransform[0])
	{
		// ��Δw�i1P
		SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG1P, WhiteColor, m_bDisplay);

		// 1P���Ԑ΂�������Ƃ�
		if (m_bStoneID[0][0])
		{
			// ��ΐ�
			SetUI(D3DXVECTOR3(125, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, WhiteColor, m_bDisplay);
		}

		// 1P���ΐ΂�������Ƃ�
		if (m_bStoneID[0][1])
		{
			// ��ΐ�
			SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, WhiteColor, m_bDisplay);
		}

		// 1P���΂�������Ƃ�
		if (m_bStoneID[0][2])
		{
			// ��Η�
			SetUI(D3DXVECTOR3(275, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, WhiteColor, m_bDisplay);
		}
	}
	else
	{
		// ��Δw�i1P
		SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG1P, AlphaColor, m_bDisplay);
		// ��ΐ�
		SetUI(D3DXVECTOR3(125, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, AlphaColor, m_bDisplay);
		// ��ΐ�
		SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, AlphaColor, m_bDisplay);
		// ��Η�
		SetUI(D3DXVECTOR3(275, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, AlphaColor, m_bDisplay);
	}

	if (!m_bTransform[1])
	{
		// ��Δw�i2P
		SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG2P, WhiteColor, m_bDisplay);

		// 2P���Ԑ΂�������Ƃ�
		if (m_bStoneID[1][0])
		{
			// ��ΐ�
			SetUI(D3DXVECTOR3(1005, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, WhiteColor, m_bDisplay);
		}

		// 2P���ΐ΂�������Ƃ�
		if (m_bStoneID[1][1])
		{
			// ��ΐ�
			SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, WhiteColor, m_bDisplay);
		}

		// 2P���΂�������Ƃ�
		if (m_bStoneID[1][2])
		{
			// ��Η�
			SetUI(D3DXVECTOR3(1155, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, WhiteColor, m_bDisplay);
		}
	}
	else
	{
		// ��Δw�i2P
		SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG2P, AlphaColor, m_bDisplay);
		// ��ΐ�
		SetUI(D3DXVECTOR3(1005, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, AlphaColor, m_bDisplay);
		// ��ΐ�
		SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, AlphaColor, m_bDisplay);
		// ��Η�
		SetUI(D3DXVECTOR3(1155, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, AlphaColor, m_bDisplay);
	}

	// 1P�L�����N�^�[UI
	SetUI(D3DXVECTOR3(55, 70, 0), 110, 130, LOGOTYPE_PLAYER1, WhiteColor, m_bDisplay);
	// �e�N�X�`���ݒ�
	m_pScene2D[LOGOTYPE_PLAYER1]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0] / 2);
	// 2P�L�����N�^�[UI
	SetUI(D3DXVECTOR3(1225, 70, 0), 110, 130, LOGOTYPE_PLAYER2, WhiteColor, m_bDisplay);
	// �e�N�X�`���ݒ�
	m_pScene2D[LOGOTYPE_PLAYER2]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1] / 2);

	// 1P�L�����N�^�[�l�[��UI
	SetUI(D3DXVECTOR3(205, 155, 0), 400, 50, LOGOTYPE_CHARANAME0, WhiteColor, m_bDisplay);
	// �e�N�X�`���ݒ�
	m_pScene2D[LOGOTYPE_CHARANAME0]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[0] / 2) * 0.25f, 0.0f + (m_nCharaNum[0] / 2) * 0.25f, 0);
	// 2P�L�����N�^�[�l�[��UI
	SetUI(D3DXVECTOR3(1075, 155, 0), 400, 50, LOGOTYPE_CHARANAME1, WhiteColor, m_bDisplay);
	// �e�N�X�`���ݒ�
	m_pScene2D[LOGOTYPE_CHARANAME1]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[1] / 2) * 0.25f, 0.0f + (m_nCharaNum[1] / 2) * 0.25f, 0);
}

//==================================================================================================================
// ���S�쐬
//==================================================================================================================
void CUI_game::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col, bool bDisplay)
{
	m_pScene2D[nCnt]->SetPos(pos);									// �ʒu�ݒ�
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// �傫���ݒ�

	// �\�����Ă����Ƃ�
	if (m_bDisplay)
	{
		m_pScene2D[nCnt]->SetCol(col);								// �F�ݒ�
	}
	else
	{// �\�������Ȃ��Ƃ�
		m_pScene2D[nCnt]->SetCol(ZeroColor);						// �F�ݒ�
	}
}
