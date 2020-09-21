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
#include "polygon2D.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define STONE_POS_Y 620								// �Έʒu�c
#define STONE_SIZE_X 60								// �Α傫��X
#define STONE_SIZE_Y 50								// �Α傫��Y

#define POS_GAMEBG	(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))	// �Q�[���w�i�̍��W
#define SIZE_GAMEBG	(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))			// �Q�[���w�i�̃T�C�Y

#define UVSIZE_CHARAICON	(D3DXVECTOR2(0.25f, 1.0f))							// �L�����A�C�R����UV�T�C�Y
#define UVSIZE_CHARANAME	(D3DXVECTOR2(1.0f, 0.25f))							// �L��������UV�T�C�Y

//==================================================================================================================
// �ÓI�����o�[�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_game::m_pTexBG					= NULL;		// �e�N�X�`�����
LPDIRECT3DTEXTURE9 CUI_game::m_pTexture[LOGOTYPE_MAX]	= {};		// �e�N�X�`�����
char CUI_game::m_FileBG[MAX_TEXT] = "data/TEXTURE/gameBG.png";		// �Q�[���w�i
char *CUI_game::m_apFileName[LOGOTYPE_MAX] =						// �ǂݍ��ރ��f���̃\�[�X��
{
	{ "data/TEXTURE/jewelryBG.png" },	// ��Δw�i
	{ "data/TEXTURE/jewelryRed.png" },	// ��ΐ�
	{ "data/TEXTURE/jewelryGreen.png" },// ��Η�
	{ "data/TEXTURE/jewelryBule.png" },	// ��ΐ�
	{ "data/TEXTURE/FULLchara.png" },	// �L�����A�C�R��
	{ "data/TEXTURE/charaName0.png" },	// �L�����N�^�[�l�[��
};

CPolygon2D *CUI_game::m_pPolygon[MAX_PLAYER][LOGOTYPE_MAX] = {};	// �v���C���[�֘A�̉摜

D3DXVECTOR3 CUI_game::m_posUI[MAX_PLAYER][LOGOTYPE_MAX] =			// UI�̍��W
{
	{	
		D3DXVECTOR3(200, STONE_POS_Y, 0) ,
		D3DXVECTOR3(125, STONE_POS_Y, 0) ,
		D3DXVECTOR3(200, STONE_POS_Y, 0) ,
		D3DXVECTOR3(275, STONE_POS_Y, 0) ,
		D3DXVECTOR3(55, 70, 0) ,
		D3DXVECTOR3(205, 155, 0) 
	},
	{
		D3DXVECTOR3(1080, STONE_POS_Y, 0),
		D3DXVECTOR3(1005, STONE_POS_Y, 0),
		D3DXVECTOR3(1080, STONE_POS_Y, 0),
		D3DXVECTOR3(1155, STONE_POS_Y, 0),
		D3DXVECTOR3(1225, 70, 0),
		D3DXVECTOR3(1075, 155, 0)
	}
};

D3DXVECTOR3 CUI_game::m_sizeUI[MAX_PLAYER][LOGOTYPE_MAX] =			// UI�̃T�C�Y
{
	{
		D3DXVECTOR3(210, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(110, 130, 0.0f),
		D3DXVECTOR3(400, 50, 0.0f)
	},
	{
		D3DXVECTOR3(210, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(STONE_SIZE_X, STONE_SIZE_Y, 0.0f),
		D3DXVECTOR3(110, 130, 0.0f),
		D3DXVECTOR3(400, 50, 0.0f)
	},
};

bool CUI_game::m_bDisplay = true;

//==================================================================================================================
//
// �R���X�g���N�^
//
//==================================================================================================================
CUI_game::CUI_game()
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
	// �Q�[���J�n����UI���Z�b�g
	SetDefaultUI();
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CUI_game::Uninit(void)
{
	// �I��
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
		{
			if (m_pPolygon[nCntPlayer][nCntLogo])
			{
				m_pPolygon[nCntPlayer][nCntLogo]->Uninit();
				delete m_pPolygon[nCntPlayer][nCntLogo];
				m_pPolygon[nCntPlayer][nCntLogo] = nullptr;
			}
		}
	}

	if (m_pPolyBG)
	{
		m_pPolyBG->Uninit();
		delete m_pPolyBG;
		m_pPolyBG = nullptr;
	}
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI_game::Update(void)
{
	if (m_pPolyBG)
		m_pPolyBG->Update();

	// �X�V
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
		{
			if (m_pPolygon[nCntPlayer][nCntLogo])
				m_pPolygon[nCntPlayer][nCntLogo]->Update();
		}
	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI_game::Draw(void)
{
	// �\�����Ȃ��Ȃ�A�������I����
	if (!m_bDisplay)
		return;

	if (m_pPolyBG)
		m_pPolyBG->Draw();

	// �`��
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
		{
			if (m_pPolygon[nCntPlayer][nCntLogo])
				m_pPolygon[nCntPlayer][nCntLogo]->Draw();
		}
	}
}

//==================================================================================================================
// ��������
//==================================================================================================================
CUI_game *CUI_game::Create(void)
{
	// �V�[�����I�Ɋm��
	CUI_game *pUI = new CUI_game;

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�̎擾

	//==============================�e�N�X�`���̓ǂݍ���==============================//
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
	}
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, m_FileBG, &m_pTexBG);

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
// �Q�[���J�n����UI���Z�b�g
//==================================================================================================================
void CUI_game::SetDefaultUI(void)
{
	// �Q�[���w�i
	CreateBG();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �v���C���[�̃^�C�v���擾
		int nCharaType= (int)CGame::GetPlayer(nCnt)->GetCharaType() / 2;

		// �L�����A�C�R���𐶐��EUV�ݒ�
		CreateUI(nCnt, LOGOTYPE_PLAYERICON, m_posUI[nCnt][LOGOTYPE_PLAYERICON], m_sizeUI[nCnt][LOGOTYPE_PLAYERICON]);
		m_pPolygon[nCnt][LOGOTYPE_PLAYERICON]->SetAnim(D3DXVECTOR2(nCharaType * 0.25f, 0.0f), UVSIZE_CHARAICON);

		// �L�������𐶐��EUV�ݒ�
		CreateUI(nCnt, LOGOTYPE_CHARANAME, m_posUI[nCnt][LOGOTYPE_CHARANAME], m_sizeUI[nCnt][LOGOTYPE_CHARANAME]);
		m_pPolygon[nCnt][LOGOTYPE_CHARANAME]->SetAnim(D3DXVECTOR2(0.0f, nCharaType * 0.25f), UVSIZE_CHARANAME);

		// �L�������𐶐��EUV�ݒ�
		CreateUI(nCnt, LOGOTYPE_JEWELRYBG, m_posUI[nCnt][LOGOTYPE_JEWELRYBG], m_sizeUI[nCnt][LOGOTYPE_JEWELRYBG]);
	}
}

//==================================================================================================================
// ���S�쐬
//==================================================================================================================
void CUI_game::CreateUI(int nPlayer, int type, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ���݂���Ȃ�A�������Ȃ�
	if (m_pPolygon[nPlayer][type])
		return;

	// ����
	m_pPolygon[nPlayer][type] = CPolygon2D::Create();
	// �e�N�X�`���̃o�C���h
	m_pPolygon[nPlayer][type]->BindTexture(m_pTexture[type]);
	// ���W�ݒ�
	m_pPolygon[nPlayer][type]->SetPos(pos);
	// �T�C�Y�ݒ�
	m_pPolygon[nPlayer][type]->SetSize(size);
	// �|���S���̌��_��ݒ�
	m_pPolygon[nPlayer][type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);

}

//==================================================================================================================
// ���S�j��
//==================================================================================================================
void CUI_game::DeleteUI(int nPlayer, int type)
{
	if (m_pPolygon[nPlayer][type])
	{
		// �I���E�j��
		m_pPolygon[nPlayer][type]->Uninit();
		delete m_pPolygon[nPlayer][type];
		m_pPolygon[nPlayer][type] = nullptr;
	}
}

//==================================================================================================================
// �w�i�̐���
//==================================================================================================================
void CUI_game::CreateBG()
{
	// ���݂���Ȃ�A�������Ȃ�
	if (m_pPolyBG)
		return;

	// ����
	m_pPolyBG = CPolygon2D::Create();
	// �e�N�X�`���̃o�C���h
	m_pPolyBG->BindTexture(m_pTexBG);
	// ���W�ݒ�
	m_pPolyBG->SetPos(POS_GAMEBG);
	// �T�C�Y�ݒ�
	m_pPolyBG->SetSize(SIZE_GAMEBG);
	// �|���S���̌��_��ݒ�
	m_pPolyBG->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}

//==================================================================================================================
// �X�g�[���擾
//==================================================================================================================
void CUI_game::CatchStone(int nPlayer, CStone::STONE_ID type)
{
	UI_GAME_TYPE StoneType = (UI_GAME_TYPE)(LOGOTYPE_JEWELRYRED + type);

	// ���݂��Ă���΁A�������Ȃ�
	if (m_pPolygon[nPlayer][StoneType])
		return;

	// UI����
	CreateUI(nPlayer, StoneType, m_posUI[nPlayer][StoneType], m_sizeUI[nPlayer][StoneType]);
}

//==================================================================================================================
// �X�g�[���j��
//==================================================================================================================
void CUI_game::ReleaseStone(int nPlayer, CStone::STONE_ID type)
{
	UI_GAME_TYPE StoneType = (UI_GAME_TYPE)(LOGOTYPE_JEWELRYRED + type);

	// ���݂��Ă��Ȃ���΁A�������Ȃ�
	if (!m_pPolygon[nPlayer][StoneType])
		return;

	// UI�j��
	DeleteUI(nPlayer, StoneType);
}

//==================================================================================================================
// �ϐg
//==================================================================================================================
void CUI_game::TransPlayer(int nPlayer)
{
	// ��Δw�i�E��΂�S�j��
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYBG])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYBG);
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYRED])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYRED);
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYGREEN])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYGREEN);
	if (m_pPolygon[nPlayer][LOGOTYPE_JEWELRYBULE])
		DeleteUI(nPlayer, LOGOTYPE_JEWELRYBULE);
}

//==================================================================================================================
// �ϐg�I��
//==================================================================================================================
void CUI_game::FinishTrans(int nPlayer)
{
	// ��Δw�i�𐶐�
	CreateUI(nPlayer, LOGOTYPE_JEWELRYBG, m_posUI[nPlayer][LOGOTYPE_JEWELRYBG], m_sizeUI[nPlayer][LOGOTYPE_JEWELRYBG]);
}
