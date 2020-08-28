//==================================================================================================================
//
// KO��UI���o [ UI_KO.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS							// �x�����

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "UI_KO.h"
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
#define TIME_ZOOM_KO		(30)								// KO�̊g��̎���
#define TIME_SLIDE_SHADOW	(10)								// �e���X���C�h���鎞��
#define TIME_AFTER_SLIDE	(60)								// �X���C�h��̗]�C
#define POS_SLIDE_SHADOW	(D3DXVECTOR3(10.0f, 10.0f, 0.0f))	// �e�Ƃ̍��W��

//==================================================================================================================
// �ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUIKO::m_pTexture[KOUI_MAX]		= {};
CPolygon2D			*CUIKO::m_pPolygon[KOUI_MAX]	= {};
char				*CUIKO::m_apFileName[KOUI_MAX]	=
{
	{ "data/TEXTURE/K.png" },
	{ "data/TEXTURE/O.png" },
	{ "data/TEXTURE/KO_back.png" }
};

D3DXVECTOR3			CUIKO::m_sizeBegin[KOUI_MAX] =
{
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2000.0f, 1200.0f, 0.0f)
};

D3DXVECTOR3			CUIKO::m_sizeEnd[KOUI_MAX] =
{
	D3DXVECTOR3(755.0f, 640.0f, 0.0f),
	D3DXVECTOR3(755.0f, 640.0f, 0.0f),
	D3DXVECTOR3(2000.0f, 1200.0f, 0.0f)
};

D3DXVECTOR3			CUIKO::m_posBegin[KOUI_MAX] =
{
	D3DXVECTOR3(120.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(1110.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, 0.0f)
};

D3DXVECTOR3			CUIKO::m_posEnd[KOUI_MAX] =
{
	D3DXVECTOR3(325.0f, SCREEN_HEIGHT / 2 + 15, 0.0f),
	D3DXVECTOR3(945.0f, SCREEN_HEIGHT / 2 + 15, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, 0.0f)
};

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CUIKO::CUIKO()
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CUIKO::~CUIKO()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUIKO::Init(void)
{
	// �������ăe�N�X�`���o�C���h
	CreateUI(KOUITEX_BACK);
	m_pPolygon[KOUITEX_BACK]->SetbShow(false);

	// �������ăe�N�X�`���o�C���h
	CreateUI(KOUITEX_K);

	// �v�f�̏�����
	m_nCntFase = 0;
	m_nCntAny  = 0;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CUIKO::Uninit(void)
{
	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
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
void CUIKO::Update(void)
{
	// �ړ����Ԃ����Z
	m_nCntAny++;

	// UI�̊g�k
	if (m_nCntAny < TIME_ZOOM_KO && m_nCntFase < KOUITEX_BACK)
		// UI�̈ړ�
		MoveUI();

	// O�̐���
	if (m_nCntAny == TIME_ZOOM_KO && m_nCntFase < KOUITEX_O)
	{
		// ���̃t�F�[�Y
		NextFase();
		// �������ăe�N�X�`���o�C���h
		CreateUI(m_nCntFase);
	}

	// O�̊g�k���I������
	if (m_nCntFase == KOUITEX_O && m_nCntAny == TIME_ZOOM_KO)
	{
		// ���̃t�F�[�Y
		NextFase();
		// �e��\��
		m_pPolygon[KOUITEX_BACK]->SetbShow(true);
	}

	// UI�̃X���C�h
	if (m_nCntFase == KOUITEX_BACK)
	{
		if (m_nCntAny < TIME_SLIDE_SHADOW)
		{
			// KO������ɂ��炷
			for (int nCnt = 0; nCnt < KOUITEX_BACK; nCnt++)
			{
				m_pPolygon[nCnt]->SetPos(*m_pPolygon[nCnt]->GetPos() - POS_SLIDE_SHADOW / TIME_SLIDE_SHADOW);
			}
			// �e�𔽑΂ɂ��炷
			m_pPolygon[KOUITEX_BACK]->SetPos(*m_pPolygon[KOUITEX_BACK]->GetPos() + POS_SLIDE_SHADOW / TIME_SLIDE_SHADOW);
		}
		else
			// ���̃t�F�[�Y
			NextFase();
	}

	if (m_nCntFase == KOUI_MAX && m_nCntAny == TIME_AFTER_SLIDE)
		CGame::SetGameState(CGame::GAMESTATE_NEXTROUND);

	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUIKO::Draw(void)
{
	// �w�i���ɕ`��
	if (m_pPolygon[KOUITEX_BACK]->GetbShow())
		m_pPolygon[KOUITEX_BACK]->Draw();

	for (int nCnt = 0; nCnt < KOUITEX_BACK; nCnt++)
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
CUIKO * CUIKO::Create(void)
{
	// �������m��
	CUIKO *pUI = new CUIKO;
	// ������
	pUI->Init();
	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CUIKO::Load(void)
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �u���b�N�R�����g
	CKananLibrary::StartBlockComment("KO���o�̃e�N�X�`���̓ǂݍ��݊J�n");

	// �e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
	{
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt])))
			std::cout << m_apFileName[nCnt] << " �̓ǂݍ��ݐ���" << std::endl;	// ����
		else
			std::cout << m_apFileName[nCnt] << " �̓ǂݍ��ݎ��s" << std::endl;	// ���s
	}

	// ����
	CKananLibrary::EndBlockComment("KO���o�̃e�N�X�`���̓ǂݍ��ݏI��");
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CUIKO::Unload(void)
{
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
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
// UI�̈ړ�
//==================================================================================================================
void CUIKO::MoveUI(void)
{
	// �ʒu�ƃT�C�Y�̍��������߂�
	D3DXVECTOR3 difPos = m_posEnd[m_nCntFase] - m_posBegin[m_nCntFase];
	D3DXVECTOR3 difSize = m_sizeEnd[m_nCntFase] - m_sizeBegin[m_nCntFase];
	// �i�X�ړ�
	m_pPolygon[m_nCntFase]->SetPos(*m_pPolygon[m_nCntFase]->GetPos() + difPos / TIME_ZOOM_KO);
	m_pPolygon[m_nCntFase]->SetSize(*m_pPolygon[m_nCntFase]->GetSize() + difSize / TIME_ZOOM_KO);
}

//==================================================================================================================
// UI�̐���
//==================================================================================================================
void CUIKO::CreateUI(int type)
{
	// ����
	m_pPolygon[type] = CPolygon2D::Create();
	// �e�N�X�`���o�C���h
	m_pPolygon[type]->BindTexture(m_pTexture[type]);
	// �ʒu�ƃT�C�Y��ݒ�
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}
