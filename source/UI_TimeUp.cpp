//==================================================================================================================
//
// KO��TimeUp���o [ UI_TimeUp.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "UI_TimeUp.h"
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
#define TIME_GATHER_ONEPLACE	(30)														// 1�ӏ��ɏW�܂鎞��
#define TIME_SHOW_UI			(60)														// �����鎞��
#define TIME_DISTRIBUTED		(30)														// ���U���鎞��

#define POS_GATHER_ONEPLACE		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))	// �W�܂���W
#define SIZE_GATHER_ONEPLACE	(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))	// �W�܂�Ƃ��̃T�C�Y

//==================================================================================================================
// �ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUITimeUp::m_pTexture		= NULL;
char				*CUITimeUp::m_apFileName = "data/TEXTURE/time_up.png";
CPolygon2D			*CUITimeUp::m_pPolygon[TIMEUPUI_MAX]	= {};

D3DXVECTOR3			CUITimeUp::m_sizeBegin[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f)
};

D3DXVECTOR3			CUITimeUp::m_sizeEnd[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(500.0f, 250.0f, 0.0f),
	D3DXVECTOR3(500.0f, 250.0f, 0.0f),
	D3DXVECTOR3(500.0f, 250.0f, 0.0f)
};

D3DXVECTOR3			CUITimeUp::m_posBegin[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(120.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(1160.0f, SCREEN_HEIGHT / 2, 0.0f)
};

D3DXVECTOR3			CUITimeUp::m_posEnd[TIMEUPUI_MAX] =
{
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(600.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(760.0f, SCREEN_HEIGHT / 2, 0.0f)
};

D3DXCOLOR   CUITimeUp::m_col[TIMEUPUI_MAX] =
{
	D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.0f),
	D3DXCOLOR(0.5f, 1.0f, 0.5f, 0.0f),
	D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.0f),
};

CUITimeUp::TIMEUP_STATE CUITimeUp::m_stateTimeUp = CUITimeUp::TIMEUPSTATE_GATHER;

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CUITimeUp::CUITimeUp()
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CUITimeUp::~CUITimeUp()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUITimeUp::Init(void)
{
	// �������ăe�N�X�`���o�C���h
	CreateUI(TIMEUPUITEX_1);
	CreateUI(TIMEUPUITEX_2);
	CreateUI(TIMEUPUITEX_3);

	// �v�f�̏�����
	m_nCntAny  = 0;
	m_stateTimeUp = TIMEUPSTATE_GATHER;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CUITimeUp::Uninit(void)
{
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		// ������Ώ������Ȃ�
		if (!m_pPolygon[nCnt])
			continue;
		// �I���E�j��
		m_pPolygon[nCnt]->Uninit();
		delete m_pPolygon[nCnt];
		m_pPolygon[nCnt] = nullptr;
	}
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUITimeUp::Update(void)
{
	// ��ԃJ�E���^�����Z
	m_nCntAny++;

	// ��Ԃŏ����𕪂���
	switch (m_stateTimeUp)
	{
	case TIMEUPSTATE_GATHER: Gather(); break;			// �W�܂�
	case TIMEUPSTATE_SHOW: Show(); break;				// ������
	case TIMEUPSTATE_DISTRIBUTED: Distributed(); break;	// ���U����
	}

	// UI�̍X�V
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUITimeUp::Draw(void)
{
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
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
CUITimeUp * CUITimeUp::Create(void)
{
	// �������m��
	CUITimeUp *pUI = new CUITimeUp;
	// ������
	pUI->Init();
	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CUITimeUp::Load(void)
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �u���b�N�R�����g
	CKananLibrary::StartBlockComment("TimeUp���o�̃e�N�X�`���̓ǂݍ��݊J�n");

	// �e�N�X�`���̓ǂݍ���
	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, m_apFileName, &m_pTexture)))
		std::cout << m_apFileName << " �̓ǂݍ��ݐ���" << std::endl;	// ����
	else
		std::cout << m_apFileName << " �̓ǂݍ��ݎ��s" << std::endl;	// ���s

// ����
	CKananLibrary::EndBlockComment("TimeUp���o�̃e�N�X�`���̓ǂݍ��ݏI��");
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CUITimeUp::Unload(void)
{
	// �e�N�X�`��������Ƃ�
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();	// �e�N�X�`���̊J��
		m_pTexture = NULL;		// NULL�ɂ���
	}
}

//==================================================================================================================
// �W�܂鏈��
//==================================================================================================================
void CUITimeUp::Gather(void)
{
	// UI�̐������J��Ԃ�
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		// �������W�ƖړI�n�̍���
		D3DXVECTOR3 difPos = POS_GATHER_ONEPLACE - m_posBegin[nCnt];
		D3DXVECTOR3 difSize = SIZE_GATHER_ONEPLACE - m_sizeBegin[nCnt];
		D3DXCOLOR   difCol = WhiteColor - m_col[nCnt];
		// �J���[�̍���
		difCol.r /= TIME_GATHER_ONEPLACE;
		difCol.g /= TIME_GATHER_ONEPLACE;
		difCol.b /= TIME_GATHER_ONEPLACE;
		difCol.a /= TIME_GATHER_ONEPLACE;

		// ���݂̏����擾
		D3DXVECTOR3 pos = m_pPolygon[nCnt]->GetPos();
		D3DXVECTOR3 size = m_pPolygon[nCnt]->GetSize();
		D3DXCOLOR col = m_pPolygon[nCnt]->GetCol();

		// �i�X�߂Â���
		m_pPolygon[nCnt]->SetPos(pos + difPos / TIME_GATHER_ONEPLACE);
		m_pPolygon[nCnt]->SetSize(size + difSize / TIME_GATHER_ONEPLACE);
		m_pPolygon[nCnt]->SetCol(col + difCol);
	}

	if (m_nCntAny < TIME_GATHER_ONEPLACE)
		return;

	// ���ԂɂȂ������ԕύX
	m_nCntAny = 0;
	m_stateTimeUp = TIMEUPSTATE_SHOW;
}

//==================================================================================================================
// �����鏈��
//==================================================================================================================
void CUITimeUp::Show(void)
{
	// �ꖇ�ڈȊO����
	if (m_pPolygon[TIMEUPUITEX_2]->GetbShow())
		m_pPolygon[TIMEUPUITEX_2]->SetbShow(false);
	if (m_pPolygon[TIMEUPUITEX_3]->GetbShow())
		m_pPolygon[TIMEUPUITEX_3]->SetbShow(false);

	if (m_nCntAny < TIME_SHOW_UI)
		return;

	// ���ԂɂȂ������ԕύX
	m_nCntAny = 0;
	m_stateTimeUp = TIMEUPSTATE_DISTRIBUTED;
}

//==================================================================================================================
// ���U���鏈��
//==================================================================================================================
void CUITimeUp::Distributed(void)
{
	// �S�ĕ\��
	if (!m_pPolygon[TIMEUPUITEX_2]->GetbShow())
		m_pPolygon[TIMEUPUITEX_2]->SetbShow(true);
	if (!m_pPolygon[TIMEUPUITEX_3]->GetbShow())
		m_pPolygon[TIMEUPUITEX_3]->SetbShow(true);

	// UI�̐������J��Ԃ�
	for (int nCnt = 0; nCnt < TIMEUPUI_MAX; nCnt++)
	{
		// �������W�ƖړI�n�̍���
		D3DXVECTOR3 difPos = m_posEnd[nCnt] - POS_GATHER_ONEPLACE;
		D3DXVECTOR3 difSize = m_sizeEnd[nCnt] - SIZE_GATHER_ONEPLACE;
		D3DXCOLOR   difCol = m_col[nCnt] - WhiteColor;
		// �J���[�̍���
		difCol.r /= TIME_DISTRIBUTED;
		difCol.g /= TIME_DISTRIBUTED;
		difCol.b /= TIME_DISTRIBUTED;
		difCol.a /= TIME_DISTRIBUTED;

		// ���݂̍��W�擾
		D3DXVECTOR3 pos = m_pPolygon[nCnt]->GetPos();
		D3DXVECTOR3 size = m_pPolygon[nCnt]->GetSize();
		D3DXCOLOR col = m_pPolygon[nCnt]->GetCol();

		// �i�X�߂Â���
		m_pPolygon[nCnt]->SetPos(pos + difPos / TIME_DISTRIBUTED);
		m_pPolygon[nCnt]->SetSize(size + difSize / TIME_DISTRIBUTED);
		m_pPolygon[nCnt]->SetCol(col + difCol);
	}

	if (m_nCntAny < TIME_DISTRIBUTED)
		return;

	// ���ԂɂȂ�����A���̏������I��
	CGame::SetGameState(CGame::GAMESTATE_KO_AFTER);
}

//==================================================================================================================
// UI�̐���
//==================================================================================================================
void CUITimeUp::CreateUI(int type)
{
	// ����
	m_pPolygon[type] = CPolygon2D::Create();
	// �e�N�X�`���o�C���h
	m_pPolygon[type]->BindTexture(m_pTexture);
	// �|���S���̏����ݒ�
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetCol(m_col[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}
