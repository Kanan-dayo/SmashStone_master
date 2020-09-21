//==================================================================================================================
//
// HP�̕`�� [hitpoint.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================

//==================================================================================================================
//	�C���N���[�h�t�@�C��
//==================================================================================================================
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "hitpoint.h"
#include "bar.h"
#include "player.h"
#include "character.h"
#include "debugProc.h"
#include "game.h"
#include "UI_game.h"

//==================================================================================================================
//	�}�N����`
//==================================================================================================================
#define ROT_ANGLE 0.08f			// ��]������p�x
#define ALPHA_DEGRESS 19.0f		// ���l�̌�����
#define MAX_HPBAR 5				// HP�o�[�̍ő�{��
#define BAR_HP_HEIGHT 110		// HP�o�[�̍���
#define BAR_HPFRAME_HEIGHT 163	// HP�t���[���̍���
#define DROP_SPEED 8.0f			// �o�[�̗�����X�s�[�h
#define DROP_DIFFRENCE 2.0f		// ������Ƃ��̂���C��
#define ROT_DIFF 53.0f			// ��]�̍�
#define BAR0_LEFT_POSX 300		// ��0�Ԗ�Bar�ʒuX
#define BAR0_RIGHT_POSX 960		// �E0�Ԗ�Bar�ʒuX
#define BAR_SPACE 40			// Bar��Bar�̊Ԋu
#define BAR0_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX, BAR_HP_HEIGHT, 0)						// 0�Ԗ�BarHP
#define BAR1_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX, BAR_HPFRAME_HEIGHT, 0)				// 1�Ԗ�Bar�t���[��
#define BAR2_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE, BAR_HP_HEIGHT, 0)			// 2�Ԗ�BarHP
#define BAR3_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE, BAR_HPFRAME_HEIGHT, 0)	// 3�Ԗ�Bar�t���[��
#define BAR4_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 2, BAR_HP_HEIGHT, 0)		// 4�Ԗ�BarHP
#define BAR5_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 2, BAR_HPFRAME_HEIGHT, 0)// 5�Ԗ�Bar�t���[��
#define BAR6_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 3, BAR_HP_HEIGHT, 0)		// 6�Ԗ�BarHP
#define BAR7_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 3, BAR_HPFRAME_HEIGHT, 0)// 7�Ԗ�Bar�t���[��
#define BAR8_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 4, BAR_HP_HEIGHT, 0)		// 8�Ԗ�BarHP
#define BAR9_LEFT_POS D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 4, BAR_HPFRAME_HEIGHT, 0)// 9�Ԗ�Bar�t���[��
#define BAR0_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX, BAR_HP_HEIGHT, 0)					// 0�Ԗ�BarHP
#define BAR1_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX, BAR_HPFRAME_HEIGHT, 0)				// 1�Ԗ�Bar�t���[��
#define BAR2_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE, BAR_HP_HEIGHT, 0)		// 2�Ԗ�BarHP
#define BAR3_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE, BAR_HPFRAME_HEIGHT, 0)	// 3�Ԗ�Bar�t���[��
#define BAR4_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 2, BAR_HP_HEIGHT, 0)	// 4�Ԗ�BarHP
#define BAR5_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 2, BAR_HPFRAME_HEIGHT, 0)// 5�Ԗ�Bar�t���[��
#define BAR6_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 3, BAR_HP_HEIGHT, 0)	// 6�Ԗ�BarHP
#define BAR7_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 3, BAR_HPFRAME_HEIGHT, 0)// 7�Ԗ�Bar�t���[��
#define BAR8_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 4, BAR_HP_HEIGHT, 0)	// 8�Ԗ�BarHP
#define BAR9_RIGHT_POS D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 4, BAR_HPFRAME_HEIGHT, 0)// 9�Ԗ�Bar�t���[��
#define NORMAL_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// ���ʂ̐F

//==================================================================================================================
//	�ÓI�����o�ϐ��̏�����
//==================================================================================================================
CPlayer *CHitPoint::m_pPlayer = NULL;		// �v���C���[�̏��
int CHitPoint::m_nInitCnt = 0;				// �������p�J�E���^

//==================================================================================================================
//	�R���X�g���N�^
//==================================================================================================================
CHitPoint::CHitPoint(PRIORITY type = PRIORITY_UI) : CScene(type)
{
	m_pBar = NULL;				// �o�[�̏��|�C���^
}

//==================================================================================================================
//	�f�X�g���N�^
//==================================================================================================================
CHitPoint::~CHitPoint(void)
{
	m_nInitCnt = 0;				// �������p�J�E���^������
}

//==================================================================================================================
//	����������
//==================================================================================================================
void CHitPoint::Init(void)
{
	// ������
	m_pos = D3DXVECTOR3(0, 0, 0);	// �ʒu
	m_rot0 = D3DXVECTOR3(0, 0, 0);	// ��]
	m_rot1 = D3DXVECTOR3(0, 0, 0);	// ��]
	m_rot2 = D3DXVECTOR3(0, 0, 0);	// ��]
	m_rot3 = D3DXVECTOR3(0, 0, 0);	// ��]
	m_nCntCol0 = 0;					// ��]�p�J�E���^
	m_nCntCol1 = 0;					// ��]�p�J�E���^
	m_nCntCol2 = 0;					// ��]�p�J�E���^
	m_nCntCol3 = 0;					// ��]�p�J�E���^
	m_nCntPos0 = 21;				// �ʒu�ύX�p�J�E���^
	m_nCntPos1 = 21;				// �ʒu�ύX�p�J�E���^
	m_nCntPos2 = 21;				// �ʒu�ύX�p�J�E���^
	m_nCntPos3 = 21;				// �ʒu�ύX�p�J�E���^
	m_fNowHP = m_fMaxHP;			// ���݂�HP
	m_fHeight = MAX_HEIGHT;			// ����

	// �ő�{���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_HPBAR; nCnt++)
	{
		m_bBar[nCnt] = true;			// �S��ture�ɂ���
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
			m_pBar->SetVertexBar(0, BAR1_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(1, BAR0_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(2, BAR3_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(3, BAR2_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(4, BAR5_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(5, BAR4_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(6, BAR7_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(7, BAR6_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(8, BAR9_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(9, BAR8_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		}
		else if (m_nInitCnt == 1)
		{// �������p�J�E���^��1�̂Ƃ�
			// Bar�t���[��
			m_pBar->SetVertexBar(0, BAR1_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(1, BAR0_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(2, BAR3_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(3, BAR2_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(4, BAR5_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(5, BAR4_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(6, BAR7_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(7, BAR6_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Bar�t���[��
			m_pBar->SetVertexBar(8, BAR9_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(9, BAR8_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		}
	}

	// �������p�J�E���^���Z
	m_nInitCnt++;
}

//==================================================================================================================
//	�I������
//==================================================================================================================
void CHitPoint::Uninit(void)
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
void CHitPoint::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// Bar�̍X�V����
	m_pBar->Update();

	// �v���C���[�̏��|�C���^�擾
	m_pPlayer = CGame::GetPlayer(m_nPlayerNum);

	float NowHP = m_pPlayer->GetLife();		// ���݂�HP

	// HP�o�[����{�ڂ̂Ƃ�
	if (NowHP > (m_fMaxHP / 5) * 4)
	{
		// �o�[0�̍X�V����
		Bar0Update(NowHP);
	}

	// HP�o�[����{�ڂ̂Ƃ�
	if (NowHP > (m_fMaxHP / 5) * 3)
	{
		// �o�[1�̍X�V����
		Bar1Update(NowHP);
	}

	// HP�o�[���O�{�ڂ̂Ƃ�
	if (NowHP > (m_fMaxHP / 5) * 2)
	{
		// �o�[2�̍X�V����
		Bar2Update(NowHP);
	}

	// HP�o�[���l�{�ڂ̂Ƃ�
	if (NowHP > (m_fMaxHP / 5) * 1)
	{
		// �o�[3�̍X�V����
		Bar3Update(NowHP);
	}

	// HP�o�[���ܖ{�ڂ̂Ƃ�
	if (NowHP >= 0)
	{
		// �o�[4�̍X�V����
		Bar4Update(NowHP);
	}

}

//==================================================================================================================
//	�`�揈��
//==================================================================================================================
void CHitPoint::Draw(void)
{
	// UI��\�������Ă����Ƃ�
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
CHitPoint * CHitPoint::Create(int nNumber, float fMaxHP)
{
	CHitPoint *pHitPoint = NULL;				// �|�C���^�ϐ�

	pHitPoint = new CHitPoint(PRIORITY_UI);		// �������𓮓I�m��
	pHitPoint->m_fMaxHP = fMaxHP;				// �v���C���[�ő�HP���
	pHitPoint->Init();							// ����������

	return pHitPoint;							// �l��Ԃ�
}

//==================================================================================================================
//	�ʒu�̐ݒ�
//==================================================================================================================
void CHitPoint::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================
//	�v���C���[�ԍ��ݒ菈��
//==================================================================================================================
void CHitPoint::SetnPlayerNum(int nPlayerNum)
{
	m_nPlayerNum = nPlayerNum;
}

//==================================================================================================================
//	�ʒu�̎擾
//==================================================================================================================
D3DXVECTOR3 CHitPoint::GetPos(void)
{
	return m_pos;
}

//==================================================================================================================
//	�o�[0�̍X�V����
//==================================================================================================================
void CHitPoint::Bar0Update(float NowHP)
{
	float fHeight = 0.0f;									// ���݂̉摜�̍���
	m_fNowHP = (NowHP - m_fMaxHP / 5 * 4) / (m_fMaxHP / 5);	// HP�̔䗦�v�Z
	fHeight = m_fNowHP * m_fHeight;							// �`�悷�ׂ��摜����������Ȃ̂�
	m_rot0.z = 0.0f;										// ��]Z������
	m_nCntCol0 = 0;											// ���l�����J�E���^������
	m_nPlayerNum = m_pPlayer->GetnPlayer();					// �v���C���[�ԍ��擾

	// �v���C���[�ԍ���0�̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// �o�[��0�Ԗڂ�true�̂Ƃ�
		if (m_bBar[0])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos0 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(1, D3DXVECTOR3(BAR0_LEFT_POSX, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos0, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(0, D3DXVECTOR3(BAR0_LEFT_POSX, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos0, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos0++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(1, BAR0_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(0, BAR1_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[0�Ԗڂ�false�̂Ƃ�
			m_nCntPos0 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[0] = true;	// 0�Ԗڂ�true;
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�̂Ƃ�
	 // �o�[��0�Ԗڂ�true�̂Ƃ�
		if (m_bBar[0])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos0 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(1, D3DXVECTOR3(BAR0_RIGHT_POSX, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos0, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(0, D3DXVECTOR3(BAR0_RIGHT_POSX, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos0, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos0++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(1, BAR0_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(0, BAR1_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[0�Ԗڂ�false�̂Ƃ�
			m_nCntPos0 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[0] = true;	// 0�Ԗڂ�true;
		}
	}
}

//==================================================================================================================
//	�o�[1�̍X�V����
//==================================================================================================================
void CHitPoint::Bar1Update(float NowHP)
{
	float fHeight = 0.0f;			// ���݂̉摜�̍���
	m_rot1.z = 0.0f;				// ��]Z������
	m_nCntCol1 = 0;					// ���l�����J�E���^������

	// ���݂�HP�{�����l�{�ȉ��̂Ƃ�
	if (NowHP < (m_fMaxHP / 5) * 4)
	{
		m_bBar[0] = false;			// 0�Ԗڂ�false
	}

	// HP�o�[��{�ڂ�����Ƃ�
	if (m_bBar[0])
	{
		// HP�̔䗦�v�Z
		m_fNowHP = 1.0f;
	}
	else
	{// HP�o�[��{�ڂ������Ƃ�
		// HP�̔䗦�v�Z
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 3) / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// �`�悷�ׂ��摜����������Ȃ̂�

	// �v���C���[�̔ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// �o�[��1�Ԗڂ�true�̂Ƃ�
		if (m_bBar[1])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos1 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(3, D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos1, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(2, D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos1, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos1++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(3, BAR2_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(2, BAR3_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[��1�Ԗڂ�false�̂Ƃ�
			m_nCntPos1 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[1] = true;	// 1�Ԗڂ�true;
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�Ԗڂ̂Ƃ�
	 // �o�[��1�Ԗڂ�true�̂Ƃ�
		if (m_bBar[1])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos1 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(3, D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos1, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(2, D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos1, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos1++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(3, BAR2_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(2, BAR3_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[��1�Ԗڂ�false�̂Ƃ�
			m_nCntPos1 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[1] = true;	// 1�Ԗڂ�true;

		}

	}

	// �v���C���[�̔ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// HP�o�[0�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[0])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot0.z <= D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(1, BAR0_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(0, BAR1_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot0.z += ROT_ANGLE;

				// ���l���Z
				m_nCntCol0++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�Ԗڂ̂Ƃ�
	 // HP�o�[0�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[0])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot0.z >= -D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(1, BAR0_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(0, BAR1_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot0.z -= ROT_ANGLE;

				// ���l���Z
				m_nCntCol0++;
			}
		}
	}
}

//==================================================================================================================
//	�o�[2�̍X�V����
//==================================================================================================================
void CHitPoint::Bar2Update(float NowHP)
{
	float fHeight = 0.0f;	// ���݂̉摜�̍���
	m_rot2.z = 0.0f;		// ��]Z������
	m_nCntCol2 = 0;			// ���l�����J�E���^������

	// ���݂�HP�{�����O�{�ȉ��̂Ƃ�
	if (NowHP < (m_fMaxHP / 5) * 3)
	{
		m_bBar[0] = false;			// 0�Ԗڂ�false
		m_bBar[1] = false;			// 1�Ԗڂ�false
	}

	// HP�o�[��{�ڂ�����Ƃ�
	if (m_bBar[1])
	{
		// HP�̔䗦�v�Z
		m_fNowHP = 1.0f;
	}
	else
	{// HP�o�[��{�ڂ������Ƃ�
		// HP�̔䗦�v�Z
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 2) / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// �`�悷�ׂ��摜����������Ȃ̂�

	// �v���C���[�̔ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// �o�[1��true�̂Ƃ�
		if (m_bBar[2])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos2 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(5, D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 2, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos2, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(4, D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 2, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos2, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos2++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(5, BAR4_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(4, BAR5_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[1��false�̂Ƃ�
			m_nCntPos2 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[2] = true;	// 2�Ԗڂ�true;
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�Ԗڂ̂Ƃ�
	 // �o�[1��true�̂Ƃ�
		if (m_bBar[2])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos2 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(5, D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 2, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos2, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(4, D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 2, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos2, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos2++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(5, BAR4_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(4, BAR5_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[1��false�̂Ƃ�
			m_nCntPos2 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[2] = true;	// 2�Ԗڂ�true;
		}
	}

	// �v���C���[�̔ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// HP�o�[1�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[1])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot1.z <= D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(3, BAR2_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(2, BAR3_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot1.z += ROT_ANGLE;

				// ���l���Z
				m_nCntCol1++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�Ԗڂ̂Ƃ�
	 // HP�o�[1�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[1])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot1.z >= -D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(3, BAR2_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(2, BAR3_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot1.z -= ROT_ANGLE;

				// ���l���Z
				m_nCntCol1++;
			}
		}
	}
}

//==================================================================================================================
//	�o�[3�̍X�V����
//==================================================================================================================
void CHitPoint::Bar3Update(float NowHP)
{
	float fHeight = 0.0f;	// ���݂̉摜�̍���
	m_rot3.z = 0.0f;		// ��]Z������
	m_nCntCol3 = 0;			// ���l�����J�E���^������

	// ���݂�HP�{������{�ȉ��̂Ƃ�
	if (NowHP < (m_fMaxHP / 5) * 2)
	{
		m_bBar[0] = false;			// 0�Ԗڂ�false
		m_bBar[1] = false;			// 1�Ԗڂ�false
		m_bBar[2] = false;			// 2�Ԗڂ�false
	}

	// HP�o�[��{�ڂ�����Ƃ�
	if (m_bBar[2])
	{
		// HP�̔䗦�v�Z
		m_fNowHP = 1.0f;
	}
	else
	{// HP�o�[��{�ڂ������Ƃ�
		// HP�̔䗦�v�Z
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 1) / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// �`�悷�ׂ��摜����������Ȃ̂�

	// �v���C���[�ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// �o�[3��true�̂Ƃ�
		if (m_bBar[3])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos3 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(7, D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 3, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos3, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(6, D3DXVECTOR3(BAR0_LEFT_POSX - BAR_SPACE * 3, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos3, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos3++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(7, BAR6_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(6, BAR7_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[3��false�̂Ƃ�
			m_nCntPos3 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[3] = true;	// 3�Ԗڂ�true;
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�ԍ���1�Ԗڂ̂Ƃ�
	 // �o�[3��true�̂Ƃ�
		if (m_bBar[3])
		{
			// �o�[�̈ʒuY���K��l���ȉ��̂Ƃ�
			if (DROP_SPEED * m_nCntPos3 < BAR_HPFRAME_HEIGHT)
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(7, D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 3, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos3, 0),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(6, D3DXVECTOR3(BAR0_RIGHT_POSX + BAR_SPACE * 3, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos3, 0),
					NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// �ʒu���Z
				m_nCntPos3++;
			}
			else
			{
				// BarHP�̐ݒ�
				m_pBar->SetVertexBar(7, BAR6_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Bar�t���[��
				m_pBar->SetVertexBar(6, BAR7_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// �o�[3��false�̂Ƃ�
			m_nCntPos3 = 0;		// �ʒu�ύX�p�J�E���^������
			m_bBar[3] = true;	// 3�Ԗڂ�true;
		}
	}

	// �v���C���[�̔ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// HP�o�[2�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[2])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot2.z <= D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(5, BAR4_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(4, BAR5_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(5, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot2.z += ROT_ANGLE;

				// ���l���Z
				m_nCntCol2++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�Ԗڂ̂Ƃ�
	 // HP�o�[2�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[2])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot2.z >= -D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(5, BAR4_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(4, BAR5_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(5, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot2.z -= ROT_ANGLE;

				// ���l���Z
				m_nCntCol2++;
			}
		}
	}
}

//==================================================================================================================
//	�o�[4�̍X�V����
//==================================================================================================================
void CHitPoint::Bar4Update(float NowHP)
{
	float fHeight = 0.0f;	// ���݂̉摜�̍���

	// ���݂�HP�{������{�ȉ��̂Ƃ�
	if (NowHP < (m_fMaxHP / 5) * 1)
	{
		m_bBar[0] = false;			// 0�Ԗڂ�false
		m_bBar[1] = false;			// 1�Ԗڂ�false
		m_bBar[2] = false;			// 2�Ԗڂ�false
		m_bBar[3] = false;			// 3�Ԗڂ�false
		m_bBar[4] = true;			// 4�Ԗڂ�true
	}

	// HP�o�[��{�ڂ�����Ƃ�
	if (m_bBar[3])
	{
		// HP�̔䗦�v�Z
		m_fNowHP = 1.0f;
	}
	else
	{// HP�o�[��{�ڂ������Ƃ�
	 // HP�̔䗦�v�Z
		m_fNowHP = NowHP / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// �`�悷�ׂ��摜����������Ȃ̂�

	// �v���C���[�̔ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// �o�[��4�Ԗڂ�true�̂Ƃ�
		if (m_bBar[4])
		{
			// BarHP�̐ݒ�
			m_pBar->SetVertexBar(9, BAR8_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
			// Bar�t���[��
			m_pBar->SetVertexBar(8, BAR9_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�Ԗڂ̂Ƃ�
	 // �o�[��4�Ԗڂ�true�̂Ƃ�
		if (m_bBar[4])
		{
			// BarHP�̐ݒ�
			m_pBar->SetVertexBar(9, BAR8_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
			// Bar�t���[��
			m_pBar->SetVertexBar(8, BAR9_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
		}

	}

	// �v���C���[�̔ԍ���0�Ԗڂ̂Ƃ�
	if (m_nPlayerNum == 0)
	{
		// HP�o�[3�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[3])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot3.z <= D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(7, BAR6_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(6, BAR7_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(7, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot3.z += ROT_ANGLE;

				// ���l���Z
				m_nCntCol3++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// �v���C���[�̔ԍ���1�Ԗڂ̂Ƃ�
	 // HP�o�[3�Ԗڂ�false�̂Ƃ�
		if (!m_bBar[3])
		{
			// �p�xZ��D3DX_PI/2�ȉ��̂Ƃ�
			if (m_rot3.z >= -D3DX_PI / 2)
			{
				// �o�[��]�ݒ�
				m_pBar->RotBar(7, BAR6_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(6, BAR7_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// �o�[�F�ݒ�
				m_pBar->SetColBar(7, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));

				// �p�x���Z
				m_rot3.z -= ROT_ANGLE;

				// ���l���Z
				m_nCntCol3++;
			}
		}
	}
}
