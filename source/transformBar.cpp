//==================================================================================================================
//
// �ϐg�o�[�̕`�� [transformBar.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================

//==================================================================================================================
//	�C���N���[�h�t�@�C��
//==================================================================================================================
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "transformBar.h"
#include "bar.h"
#include "player.h"
#include "character.h"
#include "debugProc.h"
#include "game.h"
#include "UI_game.h"

//==================================================================================================================
//	�}�N����`
//==================================================================================================================
#define BAR_HP_HEIGHT 110		// HP�o�[�̍���
#define BAR_HPFRAME_HEIGHT 163	// HP�t���[���̍���
#define NORMAL_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// ���ʂ̐F
#define BAR_WIDTH 160	// �o�[���̒���
#define BAR_HEGTH 30	// �o�[�c�̒���

//==================================================================================================================
//	�ÓI�����o�ϐ��̏�����
//==================================================================================================================
CPlayer *CTransformBar::m_pPlayer = NULL;		// �v���C���[�̏��
int CTransformBar::m_nInitCnt = 0;				// �������p�J�E���^

//==================================================================================================================
//	�R���X�g���N�^
//==================================================================================================================
CTransformBar::CTransformBar()
{
	m_pBar = NULL;				// �o�[�̏��|�C���^
}

//==================================================================================================================
//	�f�X�g���N�^
//==================================================================================================================
CTransformBar::~CTransformBar(void)
{
	m_nInitCnt = 0;				// �������p�J�E���^������
}

//==================================================================================================================
//	����������
//==================================================================================================================
void CTransformBar::Init(void)
{
	// ������
	m_pos = D3DXVECTOR3(0, 0, 0);	// �ʒu
	m_fNowHP = m_fMaxHP;			// ���݂�HP
	m_fHeight = BAR_WIDTH;			// ����

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_bTrans[nCnt] = false;	// �ϐg���Ă��邩�ǂ���
	}

	// Bar�̐���
	m_pBar = CBar::Create();

	// Bar������Ƃ�
	if (m_pBar != NULL)
	{
		// �������p�J�E���^��0�̂Ƃ�
		if (m_nInitCnt == 0)
		{
			// Bar�t���[��
			m_pBar->SetVertexBarW(10, D3DXVECTOR3(100, 620, 0), ZeroColor, 160, 35);
			// BarHP
			m_pBar->SetVertexBarW(11, D3DXVECTOR3(47, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
		}
		else if (m_nInitCnt == 1)
		{// �������p�J�E���^��1�̂Ƃ�
			// Bar�t���[��
			m_pBar->SetVertexBarWL(12, D3DXVECTOR3(1080, 620, 0), ZeroColor, 160, 35);
			// BarHP
			m_pBar->SetVertexBarWL(13, D3DXVECTOR3(1027, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
		}
	}

	// �������p�J�E���^���Z
	m_nInitCnt++;
}

//==================================================================================================================
//	�I������
//==================================================================================================================
void CTransformBar::Uninit(void)
{
	// �o�[������Ƃ�
	if (m_pBar != NULL)
	{
		//Bar�̏I������
		m_pBar->Uninit();
	}

	delete m_pBar;					// �������폜
	m_pBar = nullptr;				// �|�C���^NULL
}

//==================================================================================================================
//	�X�V����
//==================================================================================================================
void CTransformBar::Update(void)
{
	// Bar�̍X�V����
	m_pBar->Update();

	CPlayer *pPlayer0, *pPlayer1;
	pPlayer0 = CGame::GetPlayer(0);
	pPlayer1 = CGame::GetPlayer(1);

	// �ϐg���Ă��邩�ǂ���
	m_bTrans[0] = pPlayer0->GetTrans();
	m_bTrans[1] = pPlayer1->GetTrans();

	// �ϐg���Ă���Ƃ�
	if (m_bTrans[0])
	{
		// �v���C���[�̏��|�C���^�擾
		m_pPlayer = CGame::GetPlayer(0);
		float fHeight = 0.0f;									// ���݂̉摜�̍���
		float NowHP = m_fMaxHP - m_pPlayer->GetTransCnt();		// ���݂�HP
		m_fNowHP = NowHP / m_fMaxHP;							// HP�̔䗦�v�Z
		fHeight = m_fNowHP * m_fHeight;							// �`�悷�ׂ��摜����������Ȃ̂�

		// Bar�t���[��
		m_pBar->SetVertexBarW(10, D3DXVECTOR3(50, 620, 0), NORMAL_COLOR, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarW(11, D3DXVECTOR3(153, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), fHeight, BAR_HEGTH);
	}
	else
	{
		// Bar�t���[��
		m_pBar->SetVertexBarW(10, D3DXVECTOR3(50, 620, 0), ZeroColor, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarW(11, D3DXVECTOR3(153, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
	}

	// 2P���ϐg���̂Ƃ�
	if (m_bTrans[1])
	{
		// �v���C���[�̏��|�C���^�擾
		m_pPlayer = CGame::GetPlayer(1);
		float fHeight = 0.0f;									// ���݂̉摜�̍���
		float NowHP = m_fMaxHP - m_pPlayer->GetTransCnt();		// ���݂�HP
		m_fNowHP = NowHP / m_fMaxHP;							// HP�̔䗦�v�Z
		fHeight = m_fNowHP * m_fHeight;							// �`�悷�ׂ��摜����������Ȃ̂�

		// Bar�t���[��
		m_pBar->SetVertexBarWL(12, D3DXVECTOR3(1230, 620, 0), NORMAL_COLOR, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarWL(13, D3DXVECTOR3(1127, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), fHeight, BAR_HEGTH);
	}
	else
	{
		// Bar�t���[��
		m_pBar->SetVertexBarWL(12, D3DXVECTOR3(1230, 620, 0), ZeroColor, BAR_WIDTH * 2, BAR_HEGTH + 15);

		// BarHP
		m_pBar->SetVertexBarWL(13, D3DXVECTOR3(1127, 620, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f), MAX_HEIGHT, BAR_HEGTH);
	}
}

//==================================================================================================================
//	�`�揈��
//==================================================================================================================
void CTransformBar::Draw(void)
{
	// �\�����Ă��Ȃ�
	if (!CUI_game::GetbDisp())
	{
		return;
	}

	//Bar�̕`�揈��
	m_pBar->Draw();
}

//==================================================================================================================
// �I�u�W�F�N�g�̐���
//==================================================================================================================
CTransformBar * CTransformBar::Create(float fMaxHP)
{
	CTransformBar *pTransformBar = NULL;				// �|�C���^�ϐ�

	pTransformBar = new CTransformBar();				// �������𓮓I�m��
	pTransformBar->m_fMaxHP = fMaxHP;				// �v���C���[�ő�HP���
	pTransformBar->Init();							// ����������

	return pTransformBar;							// �l��Ԃ�
}

//==================================================================================================================
//	�ʒu�̐ݒ�
//==================================================================================================================
void CTransformBar::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================
//	�v���C���[�ԍ��ݒ菈��
//==================================================================================================================
void CTransformBar::SetnPlayerNum(int nPlayerNum)
{
	m_nPlayerNum = nPlayerNum;
}

//==================================================================================================================
//	�ʒu�̎擾
//==================================================================================================================
D3DXVECTOR3 CTransformBar::GetPos(void)
{
	return m_pos;
}
