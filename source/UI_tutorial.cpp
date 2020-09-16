//==================================================================================================================
//
// UI_tutorial����[UI_tutorial.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_tutorial.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "scene2D.h"
#include "debugProc.h"
#include "tutorial.h"
#include "kananlibrary.h"
#include "inputGamepad.h"
#include "sound.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define CLOCKUI0_POS_X 290							// ���vUI�ʒuX
#define CLOCKUI1_POS_X 990							// ���vUI�ʒuX
#define CLOCKHANDSUI_WAITTIME 30					// ���v�̐jUI�ҋ@����
#define CHARATEX_SISE_X 350							// �L�����e�N�X�`���傫��X
#define CHARATEX_SISE_Y 450							// �L�����e�N�X�`���傫��Y
#define CHARAUI_POS_Y 600							// �S���L����UI�ʒuY
#define CHARAFULLUI_SIZE_X 720						// �S���L����UI�傫��X
#define CHARAFULLUI_SIZE_Y 200						// �S���L����UI�傫��Y
#define CHARAFREAMUI_SIZE_X 180						// �L����UI�傫��X
#define CHARA1PUI_POS D3DXVECTOR3(200, 230, 0)		// 1P�L����UI�̈ʒu
#define CHARA2PUI_POS D3DXVECTOR3(1080, 230, 0)		// 2P�L����UI�̈ʒu
#define GEAR_POS_Y 530								// ���Ԃ̈ʒuY
#define CLOCK_HANDS_DIFF 0.1f						// ��]�̏����l
#define READY1PUI_POS D3DXVECTOR3(200, 450, 0)		// 1P���������ʒu
#define READY2PUI_POS D3DXVECTOR3(1080, 450, 0)		// 2P���������ʒu
#define READYUI_SIZE_X 200							// ���������傫��X
#define READYUI_SIZE_Y 160							// ���������傫��X
#define CHARA_SELECTUI_POS D3DXVECTOR3(640, 60, 0)	// �L�����N�^�[�I��UI�ʒu
#define CHARA_SELECTUI_SIZE_X 400					// �L�����N�^�[�I��UI�傫��X
#define CHARA_SELECTUI_SIZE_Y 120					// �L�����N�^�[�I��UI�傫��Y
#define MAPUI_SIZE_X 300							// �}�b�vUI�傫��X
#define MAPUI_SIZE_Y 250							// �}�b�vUI�傫��Y
#define MAPUI_POS_X0 650							// �}�b�vUI�c�̈ʒu0���
#define MAPUI_POS_X1 1050							// �}�b�vUI�c�̈ʒu1���
#define MAPUI_POS_Y0 200							// �}�b�vUI���̈ʒu0���
#define MAPUI_POS_Y1 500							// �}�b�vUI���̈ʒu1���
#define MAPFRAMEUI_SIZE_X 360						// �}�b�v�g���傫��X
#define MAPFRAMEUI_SIZE_Y 290						// �}�b�v�g���傫��Y
#define MAPEXPLANATION_POS D3DXVECTOR3(250, SCREEN_HEIGHT / 2, 0)// �}�b�v����UI�ʒu
#define MAPEXPLANATION_SIZE_X 400					// �}�b�v����UI�傫��X
#define MAPEXPLANATION_SIZE_Y 600					// �}�b�v����UI�傫��Y
#define STONE_POS_Y 620								// �Έʒu�c
#define STONE_SIZE_X 60								// �Α傫��X
#define STONE_SIZE_Y 50								// �Α傫��Y

//==================================================================================================================
// �ÓI�����o�[�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_tutorial::m_pTexture[UITYPE_MAX] = {};		// �e�N�X�`�����
char *CUI_tutorial::m_apFileName[UITYPE_MAX] =						// �ǂݍ��ރ��f���̃\�[�X��
{
	{ "data/TEXTURE/gear.png" },		// ����0
	{ "data/TEXTURE/clock.png" },		// ���v�y��0
	{ "data/TEXTURE/Clock hands.png" },	// ���v�̐j0
	{ "data/TEXTURE/clock gear.png" },	// ���v�̎���0
	{ "data/TEXTURE/gear.png" },		// ����1
	{ "data/TEXTURE/clock.png" },		// ���v�y��1
	{ "data/TEXTURE/Clock hands.png" },	// ���v�̐j1
	{ "data/TEXTURE/clock gear.png" },	// ���v�̎���1
	{ "data/TEXTURE/FULLcharaBG.jpg" },	// �L�����N�^�[�S���w�i
	{ "data/TEXTURE/FULLchara.png" },	// �L�����N�^�[�S��
	{ "data/TEXTURE/FULLcharaBG.jpg" },	// �L�����N�^�[1P�w�i
	{ "data/TEXTURE/FULLcharaBG.jpg" },	// �L�����N�^�[2P�w�i
	{ "data/TEXTURE/FULLchara.png" },	// 1P�L�����N�^�[
	{ "data/TEXTURE/FULLchara.png" },	// 2P�L�����N�^�[
	{ "data/TEXTURE/1Pchara.png" },		// 1P�L�����N�^�[�g
	{ "data/TEXTURE/2Pchara.png" },		// 2P�L�����N�^�[�g
	{ "data/TEXTURE/charaName.png" },	// 1P�L�����N�^�[�l�[��
	{ "data/TEXTURE/charaName.png" },	// 2P�L�����N�^�[�l�[��
	{ "data/TEXTURE/Ready.png" },		// 1P�L�����N�^�[��������
	{ "data/TEXTURE/Ready.png" },		// 2P�L�����N�^�[��������
	{ "data/TEXTURE/PlayerSelect.png" },// �v���C���[�Z���N�g�A�C�R��
};

//==================================================================================================================
//
// �R���X�g���N�^
//
//==================================================================================================================
CUI_tutorial::CUI_tutorial(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// �f�X�g���N�^
//
//==================================================================================================================
CUI_tutorial::~CUI_tutorial()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUI_tutorial::Init(void)
{
	m_nCharaNum[0] = 0;			// �L�����ԍ�
	m_nCharaNum[1] = 3;			// �L�����ԍ�
	m_fPos[0] = 370;			// ���݂̘g���e�N�X�`���̈ʒuX
	m_fPos[1] = 910;			// ���݂̘g���e�N�X�`���̈ʒuX

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_nCntRot[nCnt] = 0;	// ���v�̐j�̉�]�p�J�E���^
		m_nCntWait[nCnt] = 0;	// �ҋ@���ԗp�J�E���^
		m_fPosMove[nCnt] = 0;	// �ʒu�ړ��ϐ�
		m_fPosDiff[nCnt] = 0;	// �ڕW�̘g���e�N�X�`���̈ʒuX
		m_fPosOld[nCnt] = 0;	// �O��̘g���e�N�X�`���̈ʒuX
		m_fRotGear[nCnt] = 0;	// ���Ԃ̉�]�i�[�ϐ�
		m_fPosCul[nCnt] = 0;	// �ʒu�v�Z�p�ϐ�
		m_fDiff[nCnt] = 0;		// 1�t���[���O�Ƃ̋���
		m_fAngle[nCnt] = 0;		// ���Ԃ̉�]�p�x
		m_fRad[nCnt] = 0;		// ���W�A���l
		m_bUIClockHands[nCnt] = false;// ���v�̐j�����������ǂ���0
		m_bCharaDecide[nCnt] = false;// �����̃L�����N�^�[��I���������ǂ���
		m_bStickReturn[nCnt] = false;// �p�b�h�X�e�B�b�N��߂������ǂ���
	}

	// ���S�̍ő喇���J�E���g
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
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
void CUI_tutorial::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI_tutorial::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�ϐ�
	CInputGamepad *pGamepad[MAX_PLAYER];

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �Q�[���p�b�h�擾
		pGamepad[nCnt] = CManager::GetInputGamepad(nCnt);
	}

	// UI�`���[�g���A���X�V����
	TutorialUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI_tutorial::Draw(void)
{

}

//==================================================================================================================
// ��������
//==================================================================================================================
CUI_tutorial *CUI_tutorial::Create(void)
{
	// �V�[�����I�Ɋm��
	CUI_tutorial *pUI = new CUI_tutorial(CScene::PRIORITY_UI);

	// �V�[��������
	pUI->Init();

	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`�����[�h
//==================================================================================================================
HRESULT CUI_tutorial::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾

	//==============================�e�N�X�`���̓ǂݍ���==============================//
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
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
void CUI_tutorial::Unload(void)
{
	// ���S�̍ő��ނ܂ŃJ�E���g
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// �J��
		m_pTexture[nCnt] = NULL;			// NULL�ɂ���
	}
}

//==================================================================================================================
// �`���[�g���A���X�V����
//==================================================================================================================
void CUI_tutorial::TutorialUpdate(CInputKeyboard * pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1)
{
	// �L�����N�^�[�S��UI
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, CHARAUI_POS_Y, 0.0f), CHARAFULLUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_CHARAFULL, WhiteColor);
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, CHARAUI_POS_Y, 0.0f), CHARAFULLUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_CHARAFULLBG, WhiteColor);

	// �Q�[���p�b�h�L����
	if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
		// �Q�[���p�b�h����
		ControlGamepad(pGamepad0, pGamepad1);
	// �Q�[���p�b�h������
	else
		// �L�[�{�[�h����
		ControlKeyboard(pKeyboard);

	// ���v�y��0UI
	SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCK0, WhiteColor);
	// ���v�̐j0UI
	SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKHANDS0, WhiteColor);
	// ���v�̐j��]�ݒ�
	m_pScene2D[UITYPE_CLOCKHANDS0]->SetRot(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0), -D3DX_PI / 6 - m_nCntRot[0] * CLOCK_HANDS_DIFF, ZeroVector3);
	// ���v�̐j0UI
	SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKGEAR0, WhiteColor);

	// ���v�̐j�������Ă�����Ԃ̂Ƃ�
	if (m_bUIClockHands[0])
	{
		// ���v�̐j���ő��]�l���傫���Ƃ�
		if ((-D3DX_PI / 6) * 5 < (-D3DX_PI / 6) - m_nCntRot[0] * CLOCK_HANDS_DIFF)
		{
			// ��]�����Ă���
			m_nCntRot[0]++;
		}
		else
		{
			// �ҋ@���ԉ��Z
			m_nCntWait[0]++;

			// �ҋ@���Ԃ�0.5�b�o�����Ƃ�
			if (m_nCntWait[0] >= CLOCKHANDSUI_WAITTIME)
			{
				m_bUIClockHands[0] = false;
			}
		}
	}
	else
	{
		// �ҋ@���ԗp�J�E���^������
		m_nCntWait[0] = 0;

		// ��]�����Ă���
		m_nCntRot[0]--;

		// ��]�p�J�E���^��[0]�ȉ��ɂȂ����Ƃ�
		if (m_nCntRot[0] <= 0)
		{
			// ��]�p�J�E���^������
			m_nCntRot[0] = 0;
		}
	}

	// ���v�y��1UI
	SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCK1, WhiteColor);
	// ���v�̐j1UI
	SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKHANDS1, WhiteColor);
	m_pScene2D[UITYPE_CLOCKHANDS1]->SetRot(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0), D3DX_PI / 6 + m_nCntRot[1] * CLOCK_HANDS_DIFF, ZeroVector3);
	// ���v�̐j1UI
	SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, UITYPE_CLOCKGEAR1, WhiteColor);

	// ���v�̐j�������Ă�����Ԃ̂Ƃ�
	if (m_bUIClockHands[1])
	{
		// ���v�̐j���ő��]�l���傫���Ƃ�
		if ((D3DX_PI / 6) * 5 > (D3DX_PI / 6) + m_nCntRot[1] * CLOCK_HANDS_DIFF)
		{
			// ��]�����Ă���
			m_nCntRot[1]++;
		}
		else
		{
			// �ҋ@���ԉ��Z
			m_nCntWait[1]++;

			// �ҋ@���Ԃ�0.5�b�o�����Ƃ�
			if (m_nCntWait[1] >= CLOCKHANDSUI_WAITTIME)
			{
				m_bUIClockHands[1] = false;
			}
		}
	}
	else
	{
		// �ҋ@���ԗp�J�E���^������
		m_nCntWait[1] = 0;

		// ��]�����Ă���
		m_nCntRot[1]--;

		// ��]�p�J�E���^��[0]�ȉ��ɂȂ����Ƃ�
		if (m_nCntRot[1] <= 0)
		{
			// ��]�p�J�E���^������
			m_nCntRot[1] = 0;
		}
	}

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// ���݂̈ʒu��O��̈ʒu�ɑ��
		m_fPosOld[nCnt] = m_fPos[nCnt];

		// �L�����ԍ���0��菬�����Ȃ����Ƃ�
		if (m_nCharaNum[nCnt] < 0)
		{
			// 0�ɖ߂�
			m_nCharaNum[nCnt] = 0;
		}

		// �L�����ԍ���3���傫���Ȃ����Ƃ�
		if (m_nCharaNum[nCnt] > 3)
		{
			// 3�ɖ߂�
			m_nCharaNum[nCnt] = 3;
		}
		// �ڕW�ʒu�i�[�ϐ�
		m_fPosDiff[nCnt] = 370 + (float)m_nCharaNum[nCnt] * CHARAFREAMUI_SIZE_X;
	}

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		float fRadius = CHARAFULLUI_SIZE_Y / 2;											// ���ԃe�N�X�`���̔��a
		m_fPosCul[nCnt] = ((m_fPosDiff[nCnt] - m_fPos[nCnt]) / 60) * m_fPosMove[nCnt];	// �v�Z�p�ϐ�
		m_fDiff[nCnt] = fabsf(m_fPos[nCnt] + m_fPosCul[nCnt] - m_fPosOld[nCnt]);		// 1�t���[����̋���
		m_fAngle[nCnt] = 180 * m_fDiff[nCnt] / (D3DX_PI * fRadius);						// �p�x�Z�o
		m_fRad[nCnt] = D3DX_PI * m_fAngle[nCnt] / 360;					// ���W�A���l

		// �ʒu�̍������̂Ƃ�
		if (m_fPosCul[nCnt] > 0)
		{
			m_fRotGear[nCnt] = m_fRotGear[nCnt] + m_fRad[nCnt];					// �p�x�i�[(���Z)
		}
		else if (m_fPosCul[nCnt] < 0)
		{// �ʒu�̍������̂Ƃ�
			m_fRotGear[nCnt] = m_fRotGear[nCnt] - m_fRad[nCnt];					// �p�x�i�[(���Z)
		}
	}

	// 1P�L�����N�^�[�g��UI
	SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_1PCHARA_FREAM, WhiteColor);
	// ����0UI
	SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, UITYPE_GEAR0, WhiteColor);
	// ��]�ݒ�
	m_pScene2D[UITYPE_GEAR0]->SetRot(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0), m_fRotGear[0], ZeroVector3);

	// 2P�L�����N�^�[�g��UI
	SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, UITYPE_2PCHARA_FREAM, WhiteColor);
	// ����1UI
	SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, UITYPE_GEAR1, WhiteColor);
	// ��]�ݒ�
	m_pScene2D[UITYPE_GEAR1]->SetRot(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0), m_fRotGear[1], ZeroVector3);

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �ʒu����������Ȃ��Ƃ�
		if (m_fPosDiff[nCnt] != m_fPos[nCnt])
		{
			// �ʒu�ړ��p�J�E���^���Z
			m_fPosMove[nCnt]++;
		}

		// ���݂̈ʒu�ݒ�
		m_fPos[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

		// ���݂̈ʒu��O��̈ʒu�ɑ��
		m_fPosOld[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

	}

	// 1P�L�����N�^�[UI
	SetUI(CHARA1PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_1PCHARA, WhiteColor);
	// �e�N�X�`���ݒ�
	m_pScene2D[UITYPE_1PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0]);
	// 1P�L�����N�^�[UI
	SetUI(CHARA1PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_1PCHARABG, WhiteColor);
	// �e�N�X�`���ݒ�
	m_pScene2D[UITYPE_1PCHARABG]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0]);
	// 2P�L�����N�^�[UI
	SetUI(CHARA2PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_2PCHARA, WhiteColor);
	// �e�N�X�`���ݒ�
	m_pScene2D[UITYPE_2PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1]);
	// 2P�L�����N�^�[UI
	SetUI(CHARA2PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, UITYPE_2PCHARABG, WhiteColor);
	// �e�N�X�`���ݒ�
	m_pScene2D[UITYPE_2PCHARABG]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1]);

	// 1P�L�����N�^�[�l�[��UI
	SetUI(READY1PUI_POS, 400, 120, UITYPE_1PCHARA_NAME, WhiteColor);
	// �e�N�X�`���ݒ�
	m_pScene2D[UITYPE_1PCHARA_NAME]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[0]) * 0.25f, 0.0f + (m_nCharaNum[0]) * 0.25f, 0);
	// 2P�L�����N�^�[�l�[��UI
	SetUI(READY2PUI_POS, 400, 120, UITYPE_2PCHARA_NAME, WhiteColor);
	// �e�N�X�`���ݒ�
	m_pScene2D[UITYPE_2PCHARA_NAME]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[1]) * 0.25f, 0.0f + (m_nCharaNum[1]) * 0.25f, 0);

	// 1P�L�����N�^�[�I������Ă���Ƃ�
	if (m_bCharaDecide[0])
	{
		// 1P�L�����N�^�[��������
		SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_1PREADY, WhiteColor);
	}
	else
	{// 1P�L�����N�^�[�I������Ă��Ȃ��Ƃ�
	 // 1P�L�����N�^�[��������
		SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_1PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// 2P�L�����N�^�[�I������Ă���Ƃ�
	if (m_bCharaDecide[1])
	{
		// 2P�L�����N�^�[UI
		SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_2PREADY, WhiteColor);
	}
	else
	{// 2P�L�����N�^�[�I������Ă��Ȃ��Ƃ�
	 // 2P�L�����N�^�[��������
		SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, UITYPE_2PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// �L�����N�^�[�I���A�C�R��
	SetUI(CHARA_SELECTUI_POS, CHARA_SELECTUI_SIZE_X, CHARA_SELECTUI_SIZE_Y, UITYPE_SELECTICON, WhiteColor);
}

//==================================================================================================================
// �Q�[���p�b�h����
//==================================================================================================================
void CUI_tutorial::ControlGamepad(CInputGamepad * pGamepad0, CInputGamepad *pGamepad1)
{
	float fValueX0, fValueY0 = 0;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fValueX1, fValueY1 = 0;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p

	// ���X�e�B�b�N�擾
	pGamepad0->GetStickLeft(&fValueX0, &fValueY0);
	pGamepad1->GetStickLeft(&fValueX1, &fValueY1);

	//// �������͂���Ă��Ȃ���΁A�������Ȃ�
	//if (FAILED(CKananLibrary::GetMoveByGamepad(pGamepad0)) &&
	//	fValueX0 == 0 && fValueY0 == 0 &&
	//	FAILED(CKananLibrary::GetMoveByGamepad(pGamepad1)) &&
	//	fValueX1 == 0 && fValueY1 == 0)
	//{
	//	return;
	//}

	// �����̃L�����N�^�[��I�����ĂȂ��Ƃ�
	if (!m_bCharaDecide[0])
	{
		// �X�e�B�b�N��߂��Ă���Ƃ�
		if (m_bStickReturn[0])
		{
			// ���ɌX�����Ƃ�
			if (fValueX0 < 0)
			{
				// �L�����ԍ����Z
				m_nCharaNum[0] -= 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[0] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[0] = 0;

				// �X�e�B�b�N��߂������ǂ���
				m_bStickReturn[0] = false;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (fValueX0 > 0)
			{// �E�ɌX�����Ƃ�
			 // �L�����ԍ����Z
				m_nCharaNum[0] += 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[0] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[0] = 0;

				// �X�e�B�b�N��߂������ǂ���
				m_bStickReturn[0] = false;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// ����{�^�����������Ƃ�
			if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_B))
			{
				// �L�����N�^�[��I��������Ԃɂ���
				m_bCharaDecide[0] = true;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}

		// �X�e�B�b�N�p�x0�̂Ƃ�
		if (fValueX0 == 0)
		{
			// �X�e�B�b�N��߂��Ă����Ԃɂ���
			m_bStickReturn[0] = true;
		}
	}
	else
	{// 1P�������̃L�����N�^�[���I������Ă���Ƃ�
	 // �߂�{�^�����������Ƃ�
		if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_A))
		{
			// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
			m_bCharaDecide[0] = false;
		}
	}


	// 2P�̃L�����N�^�[���I�΂�Ă��Ȃ��Ƃ�
	if (!m_bCharaDecide[1])
	{
		// �X�e�B�b�N��߂��Ă���Ƃ�
		if (m_bStickReturn[1])
		{
			// ���ɌX�����Ƃ�
			if (fValueX1 < 0)
			{
				// �L�����ԍ����Z
				m_nCharaNum[1] -= 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[1] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[1] = 0;

				// �X�e�B�b�N���߂��Ă��Ȃ���Ԃɂ���
				m_bStickReturn[1] = false;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (fValueX1 > 0)
			{
				// �L�����ԍ����Z
				m_nCharaNum[1] += 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[1] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[1] = 0;

				// �X�e�B�b�N���߂��Ă��Ȃ���Ԃɂ���
				m_bStickReturn[1] = false;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// ����{�^�����������Ƃ�
			if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_B))
			{
				// �L�����N�^�[��I��������Ԃɂ���
				m_bCharaDecide[1] = true;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}

		// �X�e�B�b�N���X���Ă��Ȃ��Ƃ�
		if (fValueX1 == 0)
		{
			// �X�e�B�b�N���߂��Ă����Ԃɂ���
			m_bStickReturn[1] = true;
		}
	}
	else
	{// 2P�������̃L�����N�^�[��I�����Ă���Ƃ�
	 // �߂�{�^�����������Ƃ�
		if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_A))
		{
			// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
			m_bCharaDecide[1] = false;
		}
	}
}
//==================================================================================================================
// �L�[�{�[�h����
//==================================================================================================================
void CUI_tutorial::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// �����̃L�����N�^�[��I�����ĂȂ��Ƃ�
	if (!m_bCharaDecide[0])
	{
		// �L�[�{�[�h��[A]�������ꂽ�Ƃ�
		if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
		{
			// �L�����ԍ����Z
			m_nCharaNum[0] -= 1;

			// ���v�̐j�������Ă�����Ԃɂ���
			m_bUIClockHands[0] = true;

			// �ʒu�ړ��p�J�E���^������
			m_fPosMove[0] = 0;

			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
		{// �L�[�{�[�h��[D]�������ꂽ�Ƃ�
		 // �L�����ԍ����Z
			m_nCharaNum[0] += 1;

			// ���v�̐j�������Ă�����Ԃɂ���
			m_bUIClockHands[0] = true;

			// �ʒu�ړ��p�J�E���^������
			m_fPosMove[0] = 0;

			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// 1P������{�^�����������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
		{
			// �L�����N�^�[��I��������Ԃɂ���
			m_bCharaDecide[0] = true;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
	else
	{// 1P�������̃L�����N�^�[��I�����Ă���Ƃ�
	 // 1P������{�^�����������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(ONE_JUMP))
		{
			// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
			m_bCharaDecide[0] = false;
		}
	}

	// 2P�̃L�����N�^�[���I�΂�Ă��Ȃ��Ƃ�
	if (!m_bCharaDecide[1])
	{
		// �L�[�{�[�h��[��]�������ꂽ�Ƃ�
		if (pKeyboard->GetKeyboardTrigger(TWO_LEFT))
		{
			// �L�����ԍ����Z
			m_nCharaNum[1] -= 1;

			// ���v�̐j�������Ă�����Ԃɂ���
			m_bUIClockHands[1] = true;

			// �ʒu�ړ��p�J�E���^������
			m_fPosMove[1] = 0;

			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(TWO_RIGHT))
		{// �L�[�{�[�h��[��]�������ꂽ�Ƃ�
		 // �L�����ԍ����Z
			m_nCharaNum[1] += 1;

			// ���v�̐j�������Ă�����Ԃɂ���
			m_bUIClockHands[1] = true;

			// �ʒu�ړ��p�J�E���^������
			m_fPosMove[1] = 0;

			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// 2P������{�^�����������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(TWO_ATTACK))
		{
			// �L�����N�^�[��I�񂾏�Ԃɂ���
			m_bCharaDecide[1] = true;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
	else
	{// 2P�������̃L�����N�^�[��I�����Ă���Ƃ�
	 // 2P������{�^�����������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(TWO_JUMP))
		{
			// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
			m_bCharaDecide[1] = false;
		}
	}
}

//==================================================================================================================
// ���S�쐬
//==================================================================================================================
void CUI_tutorial::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// �ʒu�ݒ�
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// �傫���ݒ�
	m_pScene2D[nCnt]->SetCol(col);									// �F�ݒ�
}
