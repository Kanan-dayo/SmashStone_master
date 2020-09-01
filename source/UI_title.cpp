//==================================================================================================================
//
// UI_title����[UI_title.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI_title.h"
#include "renderer.h"
#include "scene2D.h"
#include "kananlibrary.h"
#include "sound.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define TITLEUI_BEGIN_X 1760						// �^�C�g��UI�ŏ��̉��̑傫��
#define TITLEUI_BEGIN_Y 1600						// �^�C�g��UI�ŏ��̏c�̑傫��
#define TITLEUI_SMALL_SPEED 30						// �^�C�g��UI�̏��������鑬�x
#define TITLEUI_VALUE_Y	700							// �����T�C�Y�̒lY
#define TITLEUI_BOUND_SPEED 10						// �^�C�g��UI�̃o�E���h���x
#define TITLEUI_BOUND_COUNT 60						// �^�C�g��UI�o�E���h�J�E���^
#define TITLEUI_FINISH_Y 250						// �^�C�g��UI�̍Ō�̈ʒuY
#define TITLEUI_UP_SPEED 2							// �^�C�g��UI�オ�鑬�x
#define ENTERUI_SIZE_X 950							// �G���^�[UI�傫����
#define ENTERUI_SIZE_Y 80							// �G���^�[UI�傫���c
#define ENTERUI_POS_Y 600							// �G���^�[UI�ʒuY
#define ENTERUI_ALPHA 0.03f							// �G���^�[UI���l�ύX�l

//==================================================================================================================
// �ÓI�����o�[�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI_title::m_pTexture[LOGOTYPE_MAX] = {};		// �e�N�X�`�����
char *CUI_title::m_apFileName[LOGOTYPE_MAX] =						// �ǂݍ��ރ��f���̃\�[�X��
{
	{ "data/TEXTURE/title.png" },		// �^�C�g��
	{ "data/TEXTURE/PressStart.png" },	// �G���^�[
};

//==================================================================================================================
//
// �R���X�g���N�^
//
//==================================================================================================================
CUI_title::CUI_title(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// �f�X�g���N�^
//
//==================================================================================================================
CUI_title::~CUI_title()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUI_title::Init(void)
{
	TitlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �^�C�g���̈ʒu
	m_nCntBound = 0;			// �^�C�g��UI�o�E���h�J�E���^
	m_fCntUITitle0 = 0;			// �^�C�g��UI�p�J�E���^0
	m_fCntUITitle1 = 0;			// �^�C�g��UI�p�J�E���^1
	m_fCntEnter = 0;			// �G���^�[�p�J�E���^
	m_bUITitle0 = false;		// �^�C�g���𓮂������ǂ���
	m_bUITitle1 = false;		// �^�C�g���𓮂������ǂ���
	m_bUIEnter = false;			// �G���^�[�̃��l�p�ϐ�

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
void CUI_title::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI_title::Update(void)
{
	// UI�^�C�g���̍X�V����
	TitleUpdate();
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI_title::Draw(void)
{

}

//==================================================================================================================
// ��������
//==================================================================================================================
CUI_title *CUI_title::Create(void)
{
	// �V�[�����I�Ɋm��
	CUI_title *pUI = new CUI_title(CScene::PRIORITY_UI);

	// �V�[��������
	pUI->Init();

	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`�����[�h
//==================================================================================================================
HRESULT CUI_title::Load(void)
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
void CUI_title::Unload(void)
{
	// ���S�̍ő��ނ܂ŃJ�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// �J��
		m_pTexture[nCnt] = NULL;			// NULL�ɂ���
	}
}

//==================================================================================================================
// �^�C�g���X�V����
//==================================================================================================================
void CUI_title::TitleUpdate(void)
{
	// �^�C�g��UI
	SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + TitlePos.y, 0.0f), TITLEUI_BEGIN_X + m_fCntUITitle0 - m_fCntUITitle1,
		TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1, LOGOTYPE_TITLE, WhiteColor);

	// �o�E���h�J�E���g���K��l�ȉ��̂Ƃ�
	if (m_nCntBound < TITLEUI_BOUND_COUNT)
	{
		// �^�C�g���𓮂����Ă��ȂƂ�
		if (!m_bUITitle0)
		{
			// �^�C�g��UI�̏c�̒������K��l�ȉ��̂Ƃ�
			if (TITLEUI_BEGIN_Y + m_fCntUITitle0 <= TITLEUI_VALUE_Y)
			{
				// �^�C�g���𓮂�����Ԃɂ���
				m_bUITitle0 = true;

				// �^�C�g��UI�c�����K��l�ɂ���
				m_fCntUITitle0 = TITLEUI_VALUE_Y - TITLEUI_BEGIN_Y;
			}
			else
			{
				// �^�C�g���J�E���^���Z
				m_fCntUITitle0 -= TITLEUI_SMALL_SPEED;
			}
		}
		else
		{// �^�C�g���𓮂����Ă����Ƃ�
		 // �^�C�g��UI�̏c�̒�����[370]�ȏ�̂Ƃ�
			if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 >= TITLEUI_VALUE_Y + 40)
			{
				// �^�C�g�����ő�܂Ŋg�傳����
				m_bUITitle1 = true;

			}
			else if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 <= TITLEUI_VALUE_Y - 40)
			{// �^�C�g��UI�̏c�̒�����[330]�ȉ��̂Ƃ�
			 // �^�C�g�����ŏ��܂Ŋg��������
				m_bUITitle1 = false;
			}

			// �^�C�g�����ő�܂Ŋg�傳�����Ƃ�
			if (m_bUITitle1)
			{
				// �^�C�g���J�E���^���Z
				m_fCntUITitle1 -= TITLEUI_BOUND_SPEED;
			}
			else
			{// �^�C�g�����ŏ��܂Ŋg���������Ƃ�
			 // �^�C�g���J�E���^���Z
				m_fCntUITitle1 += TITLEUI_BOUND_SPEED;
			}

			// �^�C�g��UI�o�E���h�J�E���^���Z
			m_nCntBound++;
		}
	}
	else
	{// �o�E���h�J�E���g���K��l�𒴂����Ƃ�
	 // �^�C�g��UI�̈ʒuY���K��l�ȉ��̂Ƃ�
		if (SCREEN_HEIGHT / 2 + TitlePos.y < TITLEUI_FINISH_Y)
		{
			// �G���^�[UI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, ENTERUI_POS_Y, 0.0f), ENTERUI_SIZE_X, ENTERUI_SIZE_Y,
				LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f + m_fCntEnter));

			// �G���^�[UI�̃��l��1.0�ȏ�̂Ƃ�
			if (1.0f + m_fCntEnter >= 1.0f)
			{
				// �G���^�[�p���l�����炷��Ԃɂ���
				m_bUIEnter = true;
			}
			else if (1.0f + m_fCntEnter <= 0.0f)
			{
				// �G���^�[�p���l�𑝂₷��Ԃɂ���
				m_bUIEnter = false;
			}

			// �G���^�[UI�����炷��Ԃ̂Ƃ�
			if (m_bUIEnter)
			{
				// �J�E���^���Z
				m_fCntEnter -= ENTERUI_ALPHA;
			}
			else
			{// �G���^�[UI�𑝂₷��Ԃ̂Ƃ�
			 // �J�E���^���Z
				m_fCntEnter += ENTERUI_ALPHA;
			}
		}
		else
		{
			// �^�C�g���ʒu���Z
			TitlePos.y -= TITLEUI_UP_SPEED;
		}
	}
}

//==================================================================================================================
// ���S�쐬
//==================================================================================================================
void CUI_title::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// �ʒu�ݒ�
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// �傫���ݒ�
	m_pScene2D[nCnt]->SetCol(col);									// �F�ݒ�
}
