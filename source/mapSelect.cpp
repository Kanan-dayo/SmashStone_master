//==================================================================================================================
//
// �}�b�v�I���̕`�� [mapSelect.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS											// �x�����

//==================================================================================================================
//	�C���N���[�h�t�@�C��
//==================================================================================================================
#include "tutorial.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "sound.h"
#include "scene2D.h"
#include "scene3D.h"
#include "camera.h"
#include "light.h"
#include "inputGamepad.h"
#include "UI_mapSelect.h"
#include "game.h"
#include "mapSelect.h"

//==================================================================================================================
//	�ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9 CMapSelect::m_pTexture = NULL;		// �e�N�X�`�����
CCamera *CMapSelect::m_pCamera = NULL;					// �J�������
CLight *CMapSelect::m_pLight = NULL;					// ���C�g���
CUI_mapSelect *CMapSelect::m_pUI = NULL;				// UI���

//==================================================================================================================
//	�R���X�g���N�^
//==================================================================================================================
CMapSelect::CMapSelect()
{

}

//==================================================================================================================
//	�f�X�g���N�^
//==================================================================================================================
CMapSelect::~CMapSelect()
{

}

//==================================================================================================================
//	����������
//==================================================================================================================
void CMapSelect::Init(void)
{
	m_bSelectMap = false;			// �}�b�v���I������Ă��邩�ǂ���

	CUI_mapSelect::Load();			// UI�e�N�X�`�����[�h

	// �J�����̐�������
	m_pCamera = CCamera::Create();

	// ���C�g�̐�������
	m_pLight = CLight::Create();

	// UI��������
	m_pUI = CUI_mapSelect::Create();
}

//==================================================================================================================
//	�I������
//==================================================================================================================
void CMapSelect::Uninit(void)
{
	// scene2D�̔j������
	CScene2D::ReleaseAll();

	// scene3D�̔j������
	CScene3D::ReleaseAll();

	CUI_mapSelect::Unload();		// UI�e�N�X�`���A�����[�h

	delete m_pLight;				// �������폜
	m_pLight = nullptr;				// �|�C���^NULL

	delete m_pCamera;				// �������폜
	m_pCamera = nullptr;			// �|�C���^NULL
}

//==================================================================================================================
//	�X�V����
//==================================================================================================================
void CMapSelect::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�擾
	CInputGamepad *pGamepad = CManager::GetInputGamepad(0);

	// �t�F�[�h�擾
	CFade::FADE fade = CFade::GetFade();

	// �}�b�v�̑I���󋵎擾
	m_bSelectMap = m_pUI->GetSelectMap();

	// �J�����̍X�V����
	m_pCamera->Update();

	// ���C�g�̍X�V����
	m_pLight->Update();

	// �}�b�v���I������Ă���Ƃ�
	if (m_bSelectMap)
	{
		// �t�F�[�h�������Ȃ���
		if (fade == CFade::FADE_NONE)
		{
			CGame::SetStageType(CUI_mapSelect::GetMapID());
			// �t�F�[�h�̐ݒ�
			CFade::SetFade(CRenderer::MODE_GAME, DEFAULT_FADE_TIME);
		}
	}
	else
	{
		// 1P���߂�{�^�����������Ƃ�
		if (pInputKeyboard->GetKeyboardPress(ONE_JUMP) || pGamepad->GetPress(CInputGamepad::JOYPADKEY_B))
		{
			// �t�F�[�h�������Ȃ���
			if (fade == CFade::FADE_NONE)
			{
				// �t�F�[�h�̐ݒ�
				CFade::SetFade(CRenderer::MODE_TUTORIAL, DEFAULT_FADE_TIME);
			}
		}
	}
}

//==================================================================================================================
//	�`�揈��
//==================================================================================================================
void CMapSelect::Draw(void)
{
	// �J�����̐ݒ�
	m_pCamera->SetCamera();

	// �J�����̕`�揈��
	m_pCamera->Draw();
}

//==================================================================================================================
// �I�u�W�F�N�g�̐���
//==================================================================================================================
CMapSelect * CMapSelect::Create(void)
{
	CMapSelect *pMapSelect = NULL;		// �`���[�g���A������NULL�ɂ���

	pMapSelect = new CMapSelect;		// ���I�Ɋm��
	pMapSelect->Init();					// ����������

	return pMapSelect;					// �l��Ԃ�
}
