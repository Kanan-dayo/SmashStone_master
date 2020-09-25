//==================================================================================================================
//
// UI_mapSelect����[UI_mapSelect.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_mapSelect.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "scene2D.h"
#include "kananlibrary.h"
#include "inputGamepad.h"
#include "sound.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define MAPUI_SIZE_X 600							// �}�b�vUI�傫��X
#define MAPUI_SIZE_Y 400							// �}�b�vUI�傫��Y
#define MAPUI_POS_X0 310							// �}�b�vUI�c�̈ʒu0���
#define MAPUI_POS_X1 950							// �}�b�vUI�c�̈ʒu1���
#define MAPUI_POS_Y0 360							// �}�b�vUI���̈ʒu0���
#define MAPUI_POS_Y1 500							// �}�b�vUI���̈ʒu1���
#define MAPFRAMEUI_SIZE_X 640						// �}�b�v�g���傫��X
#define MAPFRAMEUI_SIZE_Y 440						// �}�b�v�g���傫��Y

//==================================================================================================================
// �ÓI�����o�[�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_mapSelect::m_pTexture[LOGOTYPE_MAX] = {};		// �e�N�X�`�����
char *CUI_mapSelect::m_apFileName[LOGOTYPE_MAX] =						// �ǂݍ��ރ��f���̃\�[�X��
{
	{ "data/TEXTURE/worldMap.jpg" },	// ���E�n�}
	{ "data/TEXTURE/MapFrame.jpg" },	// �}�b�v�I��g
	{ "data/TEXTURE/map1.jpg" },		// �}�b�v1
	{ "data/TEXTURE/map2.jpg" },		// �}�b�v2
};

int CUI_mapSelect::m_nMapID = 0;	// �}�b�vID

//==================================================================================================================
//
// �R���X�g���N�^
//
//==================================================================================================================
CUI_mapSelect::CUI_mapSelect(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// �f�X�g���N�^
//
//==================================================================================================================
CUI_mapSelect::~CUI_mapSelect()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUI_mapSelect::Init(void)
{
	m_nMapID = 0;				// �}�b�v�ԍ�
	m_bMapSelect = false;		// �}�b�v��I���������ǂ���

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_nCharaNum[nCnt] = 0;	// �L�����N�^�[�ԍ�
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
void CUI_mapSelect::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI_mapSelect::Update(void)
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

	// UI�}�b�v�I����ʍX�V����
	MapSelectUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI_mapSelect::Draw(void)
{

}

//==================================================================================================================
// ��������
//==================================================================================================================
CUI_mapSelect *CUI_mapSelect::Create(void)
{
	// �V�[�����I�Ɋm��
	CUI_mapSelect *pUI = new CUI_mapSelect(CScene::PRIORITY_UI);

	// �V�[��������
	pUI->Init();

	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`�����[�h
//==================================================================================================================
HRESULT CUI_mapSelect::Load(void)
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
void CUI_mapSelect::Unload(void)
{
	// ���S�̍ő��ނ܂ŃJ�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// �J��
		m_pTexture[nCnt] = NULL;			// NULL�ɂ���
	}
}

//==================================================================================================================
// �}�b�v�I����ʂ̍X�V����
//==================================================================================================================
void CUI_mapSelect::MapSelectUpdate(CInputKeyboard * pKeyboard, CInputGamepad * pGamepad0, CInputGamepad * pGamepad1)
{
	// ���E�n�}
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_WORLDMAP, WhiteColor);

	// �}�b�v1
	SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP1, WhiteColor);

	// �}�b�v2
	SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP2, WhiteColor);

	// �Q�[���p�b�h�L����
	if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
		// �Q�[���p�b�h����
		ControlGamepad(pGamepad0, pGamepad1);
	// �Q�[���p�b�h������
	else
		// �L�[�{�[�h����
		ControlKeyboard(pKeyboard);
}

//==================================================================================================================
// �Q�[���p�b�h����
//==================================================================================================================
void CUI_mapSelect::ControlGamepad(CInputGamepad * pGamepad0, CInputGamepad *pGamepad1)
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

	// �}�b�v���I������Ă��Ȃ��Ƃ�
	if (!m_bMapSelect)
	{
		// ���ɌX�����Ƃ�
		if (fValueX0 < 0)
		{
			// �}�b�v�ԍ�1���Z
			m_nMapID -= 1;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (fValueX0 > 0)
		{// �E�ɌX�����Ƃ�
		 // �}�b�v�ԍ�1���Z
			m_nMapID += 1;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// �}�b�vID��0��菬������
		if (m_nMapID < 0)
		{
			// 0�ɂ���
			m_nMapID = 0;
		}

		// �܂���ID��3���傫����
		if (m_nMapID > 1)
		{
			// 3�ɂ���
			m_nMapID = 1;
		}

		switch (m_nMapID)
		{
		case 0:
			// �}�b�v1
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		case 1:
			// �}�b�v2
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		}
		// ����{�^�����������Ƃ�
		if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_X))
		{
			// �}�b�v��I��������Ԃɂ���
			m_bMapSelect = true;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
}
//==================================================================================================================
// �L�[�{�[�h����
//==================================================================================================================
void CUI_mapSelect::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// �}�b�v���I������Ă��Ȃ��Ƃ�
	if (!m_bMapSelect)
	{
		// 1P�������͂������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
		{
			// �}�b�vID�������Ȃ�
			m_nMapID -= 1;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
		{// 1P���E���͂������Ƃ�
		 // �}�b�vID�傫���Ȃ�
			m_nMapID += 1;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// �}�b�vID��0��菬������
		if (m_nMapID < 0)
		{
			// 0�ɂ���
			m_nMapID = 0;
		}

		// �܂���ID��3���傫����
		if (m_nMapID > 1)
		{
			// 3�ɂ���
			m_nMapID = 1;
		}

		switch (m_nMapID)
		{
		case 0:
			// �}�b�v1
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		case 1:
			// �}�b�v2
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, WhiteColor);
			break;
		}

		// 1P������{�^�����������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
		{
			// �}�b�v��I�񂾏�Ԃɂ���
			m_bMapSelect = true;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
}

//==================================================================================================================
// ���S�쐬
//==================================================================================================================
void CUI_mapSelect::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// �ʒu�ݒ�
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// �傫���ݒ�
	m_pScene2D[nCnt]->SetCol(col);									// �F�ݒ�
}
