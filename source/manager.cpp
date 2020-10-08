//==================================================================================================================
//
// �Ǘ�[manager.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "manager.h"
#include "scene2D.h"
#include "scene3D.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "mouse.h"
#include "kananlibrary.h"
#include "objManager.h"
#include "charaParam.h"
#include "3DEffect.h"
#include "CharEffectOffset.h"
#include "3DParticle.h"
#include "camera.h"
#include "UI_game.h"

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
CRenderer *CManager::m_pRenderer = NULL;				// �����_���[���
CInputKeyboard *CManager::m_pInputKeyboard = NULL;		// �L�[�{�[�h���
CInputGamepad *CManager::m_pInputGamepad[MAX_PLAYER] = {};		// �Q�[���p�b�h���
CMouse *CManager::m_pMouse = nullptr;

#ifdef _DEBUG
char CManager::m_cFileImGui[64]	= "data/TEXT/DebugInfo.txt";
bool CManager::m_bShowWindow	= false;
bool CManager::m_bDispUI = false;
#endif

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CManager::CManager()
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CManager::~CManager()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow)
{
	// �����_���[���I�Ɋm��
	m_pRenderer = new CRenderer;

	// �L�[�{�[�h���I�Ɋm��
	m_pInputKeyboard = new CInputKeyboard;

	// �R���g���[���[���I�Ɋm��
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pInputGamepad[nCnt] = new CInputGamepad;
	}

	m_pMouse = new CMouse;

	// ����������
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return-1;
	}

	// �L�[�{�[�h������
	m_pInputKeyboard->Init(hInstance, hWnd);

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �R���g���[���[������
		m_pInputGamepad[nCnt]->Init(hInstance, hWnd, nCnt);
		if (!m_pInputGamepad[nCnt]->GetbConnect())
		{
			printf("�Q�[���p�b�h[%d]���ǂݍ��܂�Ă��܂���\n", nCnt);
		}
	}

	m_pMouse->Init(hInstance, hWnd);

	// 3D�G�t�F�N�g���̓ǂݍ���
	C3DEffect::Load();
	// �����G�t�F�N�g�̓ǂݍ���
	CCharEffectOffset::Load();
	// �p�[�e�B�N���̓ǂݍ���
	C3DParticle::Load();

#ifdef _DEBUG
	// ImGui���̃��[�h
	LoadImGuiInfo();
#endif
	
	// �l��Ԃ�
	return S_OK;
}

//==================================================================================================================
// �I��
//==================================================================================================================
void CManager::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���̃Z�[�u
	SaveDebugInfo();
#endif

	// �S�Ă�j��
	CScene::ReleaseAll();
	// �p�[�e�B�N���̓ǂݍ���
	C3DParticle::Unload();
	// �����G�t�F�N�g�̊J��
	CCharEffectOffset::Unload();
	// 3D�G�t�F�N�g���̊J��
	C3DEffect::Unload();


	// �����_���[������Ƃ�
	if (m_pRenderer != NULL)
	{
		// �����_���[�I������
		m_pRenderer->Uninit();

		// �����_���[�j��
		delete m_pRenderer;

		// NULL�ɂ���
		m_pRenderer = NULL;
	}

	// �L�[�{�[�h������Ƃ�
	if (m_pInputKeyboard != NULL)
	{
		// �L�[�{�[�h�̏I������
		m_pInputKeyboard->Uninit();

		// �j��
		delete m_pInputKeyboard;

		// NULL�ɂ���
		m_pInputKeyboard = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// nullcheck
		if (m_pInputGamepad)
		{
			// �j��
			m_pInputGamepad[nCnt]->Uninit();
			delete m_pInputGamepad[nCnt];
			m_pInputGamepad[nCnt] = nullptr;
		}
	}

	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CManager::Update(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();					// �����_���[�̏��擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();				// �f�o�C�X���擾����

	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = GetInputKeyboard();

	// �����_���[������Ƃ�
	if (m_pRenderer != NULL)
	{
		// �����_���[�X�V����
		m_pRenderer->Update();
	}

	// �L�[�{�[�h������Ƃ�
	if (m_pInputKeyboard != NULL)
	{
		// �L�[�{�[�h�X�V����
		m_pInputKeyboard->Update();
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pInputGamepad[nCnt]->Update();
	}

	if (m_pMouse)
		m_pMouse->Update();
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CManager::Draw(void)
{
	// �����_���[������Ƃ�
	if (m_pRenderer != NULL)
	{
		// �����_���[�̕`�揈��
		m_pRenderer->Draw();
	}
}

//==================================================================================================================
// �����_���[���擾
//==================================================================================================================
CRenderer *CManager:: GetRenderer(void)
{
	return m_pRenderer;
}

//==================================================================================================================
// �L�[�{�[�h���擾
//==================================================================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//==================================================================================================================
// �R���g���[���[���擾
//==================================================================================================================
CInputGamepad * CManager::GetInputGamepad(int nPlayer)
{
	return m_pInputGamepad[nPlayer];
}

#ifdef _DEBUG
//==================================================================================================================
// ImGui���̓ǂݍ���
//==================================================================================================================
HRESULT CManager::LoadImGuiInfo(void)
{
	// �ϐ��錾
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";

	CKananLibrary::StartBlockComment("ImGui�̐ݒ�̓ǂݍ��݊J�n");

	// �t�@�C�����J��
	pFile = fopen(m_cFileImGui, "r");

	// nullcheck
	if (!pFile)
	{
		// ���s
		printf("�t�@�C�����J���܂���ł���\n");
		return E_FAIL;
	}

	// �X�N���v�g������܂ŌJ��Ԃ�
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		// ��s���ǂݍ���
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);
	}

	// �G���h�X�N���v�g������܂ŌJ��Ԃ�
	while (strcmp(cHeadText, "END_SCRIPT") != 0)
	{
		// ��s���ǂݍ���
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);

		// ���s
		if (strcmp(cHeadText, "\n") == 0)
			continue;
		// �\���̗L��
		else if (strcmp(cHeadText, "SHOW_DEBUG_WINDOW") == 0)
		{
			// �\���̓ǂݍ���
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// �\��
			if (bShow == 1)
				m_bShowWindow = true;
		}
		// �\���̗L��
		else if (strcmp(cHeadText, "SHOW_OBJECT_WINDOW") == 0)
		{
			// �\���̓ǂݍ���
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// �\��
			if (bShow == 1)
				CObjectManager::SetShowObjWindow(true);
		}
		// �\���̗L��
		else if (strcmp(cHeadText, "SHOW_CHARAPARAM_WINDOW") == 0)
		{
			// �\���̓ǂݍ���
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// �\��
			if (bShow == 1)
				CCharaParam::SetShowWindow(true);
		}
		// �\���̗L��
		else if (strcmp(cHeadText, "SHOW_GAME_UI") == 0)
		{
			// �\���̓ǂݍ���
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// �\��
			if (bShow == 1)
			{
				CManager::SetShowUI(true);
				CUI_game::SetbDisp(true);
			}
		}
		// �\���̗L��
		else if (strcmp(cHeadText, "SHOW_DEBUG_TEXT") == 0)
		{
			// �\���̓ǂݍ���
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// �\��
			if (bShow == 1)
				CRenderer::SetbDisplay(true);
		}
		// �\���̗L��
		else if (strcmp(cHeadText, "SHOW_COLLIDER") == 0)
		{
			// �\���̓ǂݍ���
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// �\��
			if (bShow == 1)
				CRenderer::SetbDisColl(true);
		}
	}
	// �t�@�C�������
	fclose(pFile);

	// ����
	CKananLibrary::EndBlockComment("�p�����[�^�[�t�@�C���̓ǂݍ��ݏI��");
	return S_OK;
}

//==================================================================================================================
// ImGui���̕ۑ�
//==================================================================================================================
HRESULT CManager::SaveDebugInfo(void)
{
	// �ϐ��錾
	FILE *pFile;

	// �e�L�X�g�t�@�C�������₷������R�����g
	char cEqual[2] = "=";
	// �ϐ����p�Ə������ݗp�̕ϐ�
	char cWriteText[MAX_TEXT];

	// �R���\�[���ɕ\��
	CKananLibrary::StartBlockComment("�f�o�b�O�ݒ�̃Z�[�u�J�n");

	// �t�@�C�����J��
	pFile = fopen(m_cFileImGui, "w");

	// nullcheck
	if (!pFile)
	{
		// �R���\�[���ɕ\��
		CKananLibrary::EndBlockComment("�t�@�C�����J���܂���ł���");
		// ���s
		return E_FAIL;
	}

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs("# �f�o�b�O�ݒ�\n", pFile);												// # �f�o�b�O�ݒ�
	fputs(COMMENT_AUTHOR, pFile);													// # Author : KANAN NAGANAWA
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("SCRIPT	# �����Ȃ���\n", pFile);										// SCRIPT
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	sprintf(cWriteText, "# ImGui�E�B���h�E�\���̗L��\nSHOW_DEBUG_WINDOW %s %d\n",
		&cEqual,
		m_bShowWindow);
	fputs(cWriteText, pFile);														// SHOW_DEBUG_WINDOW = bShow
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	sprintf(cWriteText, "# �I�u�W�F�N�g���\���̗L��\nSHOW_OBJECT_WINDOW %s %d\n",
		&cEqual,
		CObjectManager::GetShowObjWindow());
	fputs(cWriteText, pFile);														// SHOW_OBJECT_WINDOW = bShow
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	sprintf(cWriteText, "# �L�����p�����[�^�[�\���̗L��\nSHOW_CHARAPARAM_WINDOW %s %d\n",
		&cEqual,
		CCharaParam::GetShowWindow());
	fputs(cWriteText, pFile);														// SHOW_CHARAPARAM_WINDOW = bShow
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	sprintf(cWriteText, "# �Q�[��UI�\���̗L��\nSHOW_GAME_UI %s %d\n",
		&cEqual,
		CManager::GetShowUI());
	fputs(cWriteText, pFile);														// SHOW_GAME_UI = bShow
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	sprintf(cWriteText, "# �f�o�b�O�e�L�X�g�\���̗L��\nSHOW_DEBUG_TEXT %s %d\n",
		&cEqual,
		(int)CRenderer::GetbDisplay());
	fputs(cWriteText, pFile);														// SHOW_DEBUG_TEXT = bDisplay
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	sprintf(cWriteText, "# �����蔻��\���̗L��\nSHOW_COLLIDER %s %d\n",
		&cEqual,
		(int)CRenderer::GetbDisColl());
	fputs(cWriteText, pFile);														// SHOW_COLLIDER = bDisColl
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	fputs(COMMENT_NEW_LINE, pFile);													// \n

	fputs("END_SCRIPT	# �����Ȃ��� \n", pFile);									// END_SCRIPT

	// �t�@�C�������
	fclose(pFile);

	// ����
	CKananLibrary::EndBlockComment("ImGui�̐ݒ�̃Z�[�u�I��");
	return S_OK;
}

#endif