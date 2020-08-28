//==================================================================================================================
//
// �Q�[���J�n����UI���o [ UI_gameStart.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS							// �x�����

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "UI_gameStart.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "game.h"
#include "fade.h"
#include "kananlibrary.h"
#include "polygon2D.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define TIME_CREATE_ROUND		   (60)								// �t�F�[�h�C�����Round�̐�������
#define TIME_ZOOM_GAMESTART_ROUND  (20)								// UI�g��̎���
#define TIME_DELAY_GAMESTART_ROUND (TIME_ZOOM_GAMESTART_ROUND + 30) // fight�O�̃f�B���C
#define TIME_ZOOM_GAMESTART_FIGHT  (20)								// UI�g��̎���
#define TIME_DELAY_GAMESTART_FIGHT (50)								// ��ʊO�Ɉړ�����O�̃f�B���C
#define TIME_LEAVE_GAMESTART_FIGHT (20)								// UI�������鎞��
#define TIME_END_GAMESTART		   (70)								// ���̏������I���鎞��

#define POS_SLIDE_FIGHT			   (D3DXVECTOR3(SCREEN_WIDTH + 450.0f, 0.0f, 0.0f))	// fight�̂����l

//==================================================================================================================
// �ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUI_GameStart::m_pTexture[GAMEUITEX_MAX] = {};
CPolygon2D			*CUI_GameStart::m_pPolygon[STARTUITYPE_MAX] = {};
char				*CUI_GameStart::m_apFileName[GAMEUITEX_MAX] =
{
	{ "data/TEXTURE/round.png" },
	{ "data/TEXTURE/round_1.png" },
	{ "data/TEXTURE/round_2.png" },
	{ "data/TEXTURE/round_3.png" },
	{ "data/TEXTURE/fight.png" }
};

D3DXVECTOR3			CUI_GameStart::m_sizeBegin[STARTUITYPE_MAX]
{
	D3DXVECTOR3(100.0f, 50.0f, 0.0f),
	D3DXVECTOR3(900.0f, 900.0f, 0.0f),
	D3DXVECTOR3(750.0f, 250.0f, 0.0f)
};

D3DXVECTOR3			CUI_GameStart::m_sizeEnd[STARTUITYPE_MAX]
{
	D3DXVECTOR3(700.0f, 250.0f, 0.0f),
	D3DXVECTOR3(200.0f, 250.0f, 0.0f),
	D3DXVECTOR3(750.0f, 250.0f, 0.0f)
};

D3DXVECTOR3			CUI_GameStart::m_posBegin[STARTUITYPE_MAX]
{
	D3DXVECTOR3(500.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(700.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(-450.0f, SCREEN_HEIGHT / 2, 0.0f)
};

D3DXVECTOR3			CUI_GameStart::m_posEnd[STARTUITYPE_MAX]
{
	D3DXVECTOR3(500.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(1000.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2 + 50.0f, SCREEN_HEIGHT / 2, 0.0f)
};

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CUI_GameStart::CUI_GameStart()
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CUI_GameStart::~CUI_GameStart()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUI_GameStart::Init(void)
{
	// �v�f�̏�����
	m_fase = FASE_NONE;
	m_nCntAny = 0;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CUI_GameStart::Uninit(void)
{
	for (int nCnt = 0; nCnt < STARTUITYPE_MAX; nCnt++)
	{
		if (!m_pPolygon[nCnt])
			continue;
		m_pPolygon[nCnt]->Uninit();
		delete m_pPolygon[nCnt];
		m_pPolygon[nCnt] = nullptr;
	}
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI_GameStart::Update(void)
{
	// �J�E���^�̉��Z
	m_nCntAny++;

	if (m_fase == FASE_NONE)
	{
		if (m_nCntAny == TIME_CREATE_ROUND)
		{
			// ���̃t�F�[�Y�Ɉڍs
			NextFase();
			// ���݂̃��E���h�̔ԍ�����
			CreateUI(m_fase);
		}
	}
	else if (m_fase == FASE_ROUND)
	{
		// UI�̈ړ��E�g�k
		if (m_nCntAny < TIME_ZOOM_GAMESTART_ROUND)
			MoveUI(m_fase, TIME_ZOOM_GAMESTART_ROUND);
		else
		{
			// ���̃t�F�[�Y�Ɉڍs
			NextFase();
			// ���݂̃��E���h�̔ԍ�����
			CreateUI(m_fase);
		}
	}
	else if (m_fase == FASE_RONNDNUM)
	{
		// UI�̈ړ��E�g�k
		if (m_nCntAny < TIME_ZOOM_GAMESTART_ROUND)
			MoveUI(m_fase, TIME_ZOOM_GAMESTART_ROUND);
		else if (m_nCntAny == TIME_DELAY_GAMESTART_ROUND)
		{
			// ���܂ŏo���Ă������̂��\���ɂ���
			m_pPolygon[STARTUITYPE_NUMROUND]->SetbShow(false);
			m_pPolygon[STARTUITYPE_ROUND]->SetbShow(false);
			// ���̃t�F�[�Y�Ɉڍs
			NextFase();
			// ���݂̃��E���h�̔ԍ�����
			CreateUI(m_fase);
		}
	}
	else if (m_fase == FASE_FIGHT)
	{
		// UI�̈ړ��E�g�k
		if (m_nCntAny < TIME_ZOOM_GAMESTART_FIGHT)
			MoveUI(m_fase, TIME_ZOOM_GAMESTART_FIGHT);
		// UI�̉�ʊO�ړ�
		else if (m_nCntAny < TIME_LEAVE_GAMESTART_FIGHT + TIME_DELAY_GAMESTART_FIGHT && m_nCntAny > TIME_DELAY_GAMESTART_FIGHT)
			m_pPolygon[m_fase]->SetPos(*m_pPolygon[m_fase]->GetPos() + POS_SLIDE_FIGHT / TIME_LEAVE_GAMESTART_FIGHT);
		// ���[�h��؂�ւ��ď������I��
		else if (m_nCntAny > TIME_END_GAMESTART)
			CGame::SetGameState(CGame::GAMESTATE_NORMAL);
	}

	for (int nCnt = 0; nCnt < STARTUITYPE_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI_GameStart::Draw(void)
{	
	for (int nCnt = 0; nCnt < STARTUITYPE_MAX; nCnt++)
	{
		// ������Ώ������Ȃ�
		if (!m_pPolygon[nCnt])
			continue;
		// �`��
		m_pPolygon[nCnt]->Draw();
	}
}

//==================================================================================================================
// �I�u�W�F�N�g�̐���
//==================================================================================================================
CUI_GameStart * CUI_GameStart::Create(void)
{
	// �������m��
	CUI_GameStart *pUI = new CUI_GameStart;
	// ������
	pUI->Init();
	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CUI_GameStart::Load(void)
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �u���b�N�R�����g
	CKananLibrary::StartBlockComment("�Q�[���J�n���̃e�N�X�`���̓ǂݍ��݊J�n");

	// �e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < GAMEUITEX_MAX; nCnt++)
	{
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt])))
			std::cout << m_apFileName[nCnt] << " �̓ǂݍ��ݐ���" << std::endl;	// ����
		else
			std::cout << m_apFileName[nCnt] << " �̓ǂݍ��ݎ��s" << std::endl;	// ���s
	}

	// ����
	CKananLibrary::EndBlockComment("�Q�[���J�n���̃e�N�X�`���̓ǂݍ��ݏI��");
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CUI_GameStart::Unload(void)
{
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < GAMEUITEX_MAX; nCnt++)
	{
		// �e�N�X�`��������Ƃ�
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();	// �e�N�X�`���̊J��
			m_pTexture[nCnt] = NULL;		// NULL�ɂ���
		}
	}
}

//==================================================================================================================
// UI�̐���
//==================================================================================================================
void CUI_GameStart::CreateUI(const int type)
{
	// ����
	m_pPolygon[type] = CPolygon2D::Create();
	// �e�N�X�`���o�C���h
	if (type == FASE_RONNDNUM)
		m_pPolygon[type]->BindTexture(m_pTexture[CGame::GetRound() + 1]);
	else if (type == FASE_FIGHT)
		m_pPolygon[type]->BindTexture(m_pTexture[GAMEUITEX_FIGHT]);
	else
		m_pPolygon[type]->BindTexture(m_pTexture[type]);

	// �ʒu�ƃT�C�Y��ݒ�
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}

//==================================================================================================================
// UI�̈ړ�
//==================================================================================================================
void CUI_GameStart::MoveUI(const int type, const int time)
{
	// �ʒu�ƃT�C�Y�̍��������߂�
	D3DXVECTOR3 difPos = m_posEnd[type] - m_posBegin[type];
	D3DXVECTOR3 difSize = m_sizeEnd[type] - m_sizeBegin[type];
	// �i�X�ړ�
	m_pPolygon[type]->SetPos(*m_pPolygon[type]->GetPos() + difPos / (float)time);
	m_pPolygon[type]->SetSize(*m_pPolygon[type]->GetSize() + difSize / (float)time);
}