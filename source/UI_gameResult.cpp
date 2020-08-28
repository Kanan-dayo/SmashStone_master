//==================================================================================================================
//
// �Q�[�����U���g��UI���o [ UI_gameResult.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS							// �x�����

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "UI_gameResult.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "game.h"
#include "fade.h"
#include "kananlibrary.h"
#include "polygon2D.h"
#include "inputGamepad.h"
#include "inputKeyboard.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define POS_END_VICTORY		(D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f))	// victory�̈ړ���̈ʒu
#define SIZE_END_VICTORY	(D3DXVECTOR3(800.0f, 260.0f, 0.0f))				// victory�̊g���̃T�C�Y
#define TIME_ZOOM_VICTORY	(30)											// victory�̈ړ��E�g��̎���
#define TIME_SHOW_VICTORY	(90)											// victory�������鎞��

#define TIME_INPUT_MAX		(10)											// �A�����͖h�~�p

//==================================================================================================================
// �ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUI_GameResult::m_pTexture[RESULTUITYPE_MAX] = {};
CPolygon2D			*CUI_GameResult::m_pPolygon[RESULTUITYPE_MAX] = {};
char				*CUI_GameResult::m_apFileName[RESULTUITYPE_MAX] =
{
	{ "data/TEXTURE/victory.png" },
	{ "data/TEXTURE/frame.png" },
	{ "data/TEXTURE/select.png" },
	{ "data/TEXTURE/rematch.png" },
	{ "data/TEXTURE/return_select.png" },
	{ "data/TEXTURE/return_title.png" }
};

D3DXVECTOR3		  CUI_GameResult::m_posBegin[RESULTUITYPE_MAX]
{
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 170.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 170.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 170.0f, 0.0f)
};

D3DXVECTOR3		  CUI_GameResult::m_sizeBegin[RESULTUITYPE_MAX]
{
	D3DXVECTOR3(2000.0f, 1000.0f, 0.0f),
	D3DXVECTOR3(400.0f, 500.0f, 0.0f),
	D3DXVECTOR3(350.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f)
};

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CUI_GameResult::CUI_GameResult()
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CUI_GameResult::~CUI_GameResult()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUI_GameResult::Init(void)
{
	// victory����
	CreateUI(RESULTUITYPE_VICTORY);

	// �v�f�̏�����
	m_fase = FASE_VICTORY;
	m_nCntAny = 0;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CUI_GameResult::Uninit(void)
{
	// UI�̐������J��Ԃ�
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		// nullcheck
		if (!m_pPolygon[nCnt])
			// �������Ȃ�
			continue;
		// �j��
		m_pPolygon[nCnt]->Uninit();
		delete m_pPolygon[nCnt];
		m_pPolygon[nCnt] = nullptr;
	}
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI_GameResult::Update(void)
{
	// ����
	if (m_fase == FASE_VICTORY)
		Victory();
	// ���U���g
	else if (m_fase == FASE_RESULT)
		Result();

	// UI�̍X�V
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI_GameResult::Draw(void)
{
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		// nullcheck
		if (!m_pPolygon[nCnt])
			// �������Ȃ�
			continue;
		// �`��
		m_pPolygon[nCnt]->Draw();
	}
}

//==================================================================================================================
// �I�u�W�F�N�g�̐���
//==================================================================================================================
CUI_GameResult * CUI_GameResult::Create(void)
{
	// �������m��
	CUI_GameResult *pUI = new CUI_GameResult;
	// ������
	pUI->Init();
	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CUI_GameResult::Load(void)
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �u���b�N�R�����g
	CKananLibrary::StartBlockComment("�Q�[���J�n���̃e�N�X�`���̓ǂݍ��݊J�n");

	// �e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
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
void CUI_GameResult::Unload(void)
{
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
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
// ����
//==================================================================================================================
void CUI_GameResult::Victory(void)
{
	// �J�E���^�̉��Z
	m_nCntAny++;

	// UI�ړ�
	if (m_nCntAny < TIME_ZOOM_VICTORY)
		MoveUI();
	// �����o���Ă��玟��
	else if (m_nCntAny >= TIME_SHOW_VICTORY)
		NextFase();
}

//==================================================================================================================
// ���U���g
//==================================================================================================================
void CUI_GameResult::Result(void)
{
	// victory���c��ŏ������ʂ�
	if (m_pPolygon[RESULTUITYPE_VICTORY])
	{
		// victory�j��
		m_pPolygon[RESULTUITYPE_VICTORY]->Uninit();
		delete m_pPolygon[RESULTUITYPE_VICTORY];
		m_pPolygon[RESULTUITYPE_VICTORY] = nullptr;

		// ����UI�𐶐�
		for (int nCnt = 1; nCnt < RESULTUITYPE_MAX; nCnt++)
		{
			CreateUI(nCnt);
		}
	}

	// ���͂̎擾 (�Q�[���p�b�h�͕������v���C���[)
	CInputKeyboard *pKey = CManager::GetInputKeyboard();
	CInputGamepad *pGamepad = CManager::GetInputGamepad((int)(CGame::GetLosePlayer()));

	// ����
	if (CKananLibrary::Decide(pKey, pGamepad))
		CursorDecide();

	static int nCntInput = 0;
	if (nCntInput < TIME_INPUT_MAX)
	{
		nCntInput++;
		return;
	}
	
	// �㏸
	if (CKananLibrary::Up(pKey, pGamepad))
	{
		CursorUp();
		nCntInput = 0;
	}
	// ���~
	else if (CKananLibrary::Down(pKey, pGamepad))
	{
		CursorDown();
		nCntInput = 0;
	}
}

//==================================================================================================================
// �J�[�\���̏㏸
//==================================================================================================================
void CUI_GameResult::CursorUp(void)
{
	// �J�[�\���ʒu�擾
	D3DXVECTOR3 posCursor = *m_pPolygon[RESULTUITYPE_CURSOR]->GetPos();
	
	// �^�C�g���ʒu��
	if (posCursor == m_posBegin[RESULTUITYPE_REMATCH])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNTITLE]);
	// �Đ�ʒu��
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNSELECT])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_REMATCH]);
	// �L�����Z���N�g�ʒu��
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNTITLE])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNSELECT]);

}

//==================================================================================================================
// �J�[�\���̉��~
//==================================================================================================================
void CUI_GameResult::CursorDown(void)
{
	// �J�[�\���ʒu�擾
	D3DXVECTOR3 posCursor = *m_pPolygon[RESULTUITYPE_CURSOR]->GetPos();

	// �L�����Z���N�g�ʒu��
	if (posCursor == m_posBegin[RESULTUITYPE_REMATCH])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNSELECT]);
	// �^�C�g���ʒu��
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNSELECT])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNTITLE]);
	// �Đ�ʒu��
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNTITLE])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_REMATCH]);
}

//==================================================================================================================
// �I���������̂̌���
//==================================================================================================================
void CUI_GameResult::CursorDecide(void)
{
	// �J�[�\���ʒu�擾
	D3DXVECTOR3 posCursor = *m_pPolygon[RESULTUITYPE_CURSOR]->GetPos();

	// ������x�Q�[����
	if (posCursor == m_posBegin[RESULTUITYPE_REMATCH])
		CGame::SetNextMode(CRenderer::MODE_GAME);
	// �^�C�g���ʒu��
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNSELECT])
		CGame::SetNextMode(CRenderer::MODE_TUTORIAL);
	// �Đ�ʒu��
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNTITLE])
		CGame::SetNextMode(CRenderer::MODE_TITLE);
}

//==================================================================================================================
// UI�̐���
//==================================================================================================================
void CUI_GameResult::CreateUI(const int type)
{
	// ����
	m_pPolygon[type] = CPolygon2D::Create();
	m_pPolygon[type]->BindTexture(m_pTexture[type]);
	// �ʒu�ƃT�C�Y��ݒ�
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}

//==================================================================================================================
// UI�̈ړ�
//==================================================================================================================
void CUI_GameResult::MoveUI(void)
{
	// �ʒu�ƃT�C�Y�̍��������߂�
	D3DXVECTOR3 difPos = POS_END_VICTORY - m_posBegin[RESULTUITYPE_VICTORY];
	D3DXVECTOR3 difSize = SIZE_END_VICTORY - m_sizeBegin[RESULTUITYPE_VICTORY];
	// �i�X�ړ�
	m_pPolygon[RESULTUITYPE_VICTORY]->SetPos(*m_pPolygon[RESULTUITYPE_VICTORY]->GetPos() + difPos / (float)TIME_ZOOM_VICTORY);
	m_pPolygon[RESULTUITYPE_VICTORY]->SetSize(*m_pPolygon[RESULTUITYPE_VICTORY]->GetSize() + difSize / (float)TIME_ZOOM_VICTORY);
}