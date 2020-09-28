//==================================================================================================================
//
// �����_���[[renderer.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "renderer.h"
#include "scene.h"
#include "debugProc.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "sound.h"
#include "kananlibrary.h"
#include "modelCharacter.h"
#include "motion.h"
#include "charaParam.h"
#include "inputKeyboard.h"
#include "mapSelect.h"
#include "inputKeyboard.h"
#include "objManager.h"
#include "UI_game.h"
#include "ImGui/imgui.h"			// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"	// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"	// Imgui�̎����ɕK�v

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define NAME_CAPTION_IMGUI	("Debug")	// ImGui�L���v�V������

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
CFade *CRenderer::m_pFade = NULL;						// Fade�̏��
CDebugProc *CRenderer::m_pDebugProc = NULL;				// �f�o�b�O�\�����
CCamera *CRenderer::m_pCamera = NULL;					// �J�����̏��
CLight *CRenderer::m_pLight = NULL;						// ���C�g�̏��
CGame *CRenderer::m_pGame = NULL;						// �Q�[�����
CTitle *CRenderer::m_pTitle = NULL;						// �^�C�g�����
CResult *CRenderer::m_pResult = NULL;					// ���U���g���
CTutorial *CRenderer::m_pTutorial = NULL;				// �`���[�g���A�����
CSound *CRenderer::m_pSound = NULL;						// �����
CMapSelect *CRenderer::m_pMapSelect = NULL;				// �}�b�v�I����ʂ̏��
CRenderer::MODE CRenderer::m_mode = CRenderer::MODE_GAME;			// �ŏ��̉��

#ifdef _DEBUG
bool CRenderer::m_bDisColl = false;						// �����蔻��̉����t���O
bool CRenderer::m_bDisplay = false;						// �f�o�b�O�e�L�X�g�\���̃t���O
#endif

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CRenderer::CRenderer()
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CRenderer::~CRenderer()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��(VSync��҂��ĕ`��)
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �N���C�A���g�̈�𒼂��ɍX�V����

	// Direct3D�f�o�C�X�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,											// �f�o�C�X�^�C�v
		hWnd,													// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,					// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,													// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&m_pD3DDevice)))										// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}
#ifdef _DEBUG
	//�f�o�b�O���S�̏���������
	m_pDebugProc->Init();
#endif

	// �t�F�[�h�̃��[�h
	CFade::Load();

	// �t�F�[�h��������
	m_pFade = CFade::Create(m_mode);

	// ���I�m��
	m_pSound = new CSound;

	// ��������
	m_pSound->Init(hWnd);

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@(�J�����ƃI�u�W�F�N�g�̋������L��)���g�p
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�B�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

#ifdef _DEBUG
	// ImGui�̏�����
	InitImGui(d3dpp, hWnd);
#endif

	// ���[�h
	CCharaParam::Load();
	CModelCharacter::Load();
	CMotion::Load();
	CObjectManager::Load();

	// ���[�h�̊J�n
	StartMode();

	// �l��Ԃ�
	return S_OK;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���S�̏I������
	m_pDebugProc->Uninit();
#endif

	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();						// �J��
		m_pD3DDevice = NULL;							// NULL�ɂ���
	}
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();								// �J��
		m_pD3D = NULL;									// NULL�ɂ���
	}

	// �t�F�[�h������Ƃ�
	if (m_pFade != NULL)
	{
		// �I������
		m_pFade->Uninit();

		// �������폜
		delete m_pFade;

		// �|�C���^NULL
		m_pFade = nullptr;
	}

	// ��������Ƃ�
	if (m_pSound != NULL)
	{
		// ���̏I������
		m_pSound->Uninit();

		// �������폜
		delete m_pSound;

		// �|�C���^NULL
		m_pSound = nullptr;
	}

	CModelCharacter::Unload();
	CMotion::UnLoad();
	CObjectManager::Unload();

	switch (m_mode)
	{
		// �^�C�g���̂Ƃ�
	case MODE_TITLE:
		// �^�C�g���̏I������
		m_pTitle->Uninit();
		delete m_pTitle;		// �������̍폜
		m_pTitle = nullptr;		// �|�C���^NULL
		break;

		// �`���[�g���A���̂Ƃ�
	case MODE_TUTORIAL:
		// �`���[�g���A���̏I������
		m_pTutorial->Uninit();
		delete m_pTutorial;		// �������̍폜
		m_pTutorial = nullptr;	// �|�C���^NULL
		break;

		// �}�b�v�I����ʂ̂Ƃ�
	case MODE_MAPSELECT:
		// �}�b�v�I����ʂ̏I������
		m_pMapSelect->Uninit();
		delete m_pMapSelect;	// �������̍폜
		m_pMapSelect = nullptr;	// �|�C���^NULL
		break;

		// �Q�[���̂Ƃ�
	case MODE_GAME:
		// �Q�[���̏I������
		m_pGame->Uninit();
		delete m_pGame;			// �������̍폜
		m_pGame = nullptr;		// �|�C���^NULL
		break;
	}

#ifdef _DEBUG
	// Imgui�̏I��
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CRenderer::Update(void)
{
#ifdef _DEBUG
	// Imgui�̐V�����g�����
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// �L���v�V������
	ImGui::Begin(NAME_CAPTION_IMGUI);
#endif

	// �Q�[���̏�Ԏ擾
	CGame::GAMESTATE gameState = CGame::GetGameState();

	// �Q�[����Ԃ��|�[�Y����Ȃ��Ƃ�
	if (gameState != CGame::GAMESTATE_PAUSE && 
		gameState != CGame::GAMESTATE_KO)
	{
		// Scene�ŊǗ����邷�ׂĂ̍X�V����
		CScene::UpdateAll();
	}

	// �t�F�[�h�̍X�V����
	m_pFade->Update();

	switch (m_mode)
	{
		// �^�C�g���̂Ƃ�
	case MODE_TITLE:
		// �X�V����
		m_pTitle->Update();
		break;

		// �`���[�g���A���̂Ƃ�
	case MODE_TUTORIAL:
		// �X�V����
		m_pTutorial->Update();
		break;

		// �}�b�v�I����ʂ̂Ƃ�
	case MODE_MAPSELECT:
		// �X�V����
		m_pMapSelect->Update();
		break;

		// �Q�[���̂Ƃ�
	case MODE_GAME:
		// �X�V����
		m_pGame->Update();
		break;
	}

#ifdef _DEBUG
	// �f�o�b�O�R�}���h
	DebugCommand();

	// ImGui�̍X�V
	UpdateImGui();
#endif
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	// �X�e���V���o�b�t�@��0�ɃN���A
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 50, 50, 0), 1.0f, 0);

	// Direc3D�ɂ��`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// Scene�ŊǗ����邷�ׂĂ̕`�揈��
		CScene::DrawAll();

		switch (m_mode)
		{
			// �^�C�g���̂Ƃ�
		case MODE_TITLE:
			// �`�揈��
			m_pTitle->Draw();
			break;

			// �`���[�g���A���̂Ƃ�
		case MODE_TUTORIAL:
			// �`�揈��
			m_pTutorial->Draw();
			break;

			// �}�b�v�I����ʂ̂Ƃ�
		case MODE_MAPSELECT:
			// �`�揈��
			m_pMapSelect->Draw();
			break;

			// �Q�[���̂Ƃ�
		case MODE_GAME:
			// �`�揈��
			m_pGame->Draw();
			break;
		}

		// �t�F�[�h������Ƃ�
		if (m_pFade != NULL)
		{
			// �t�F�[�h�̕`�揈��
			m_pFade->Draw();
		}

#ifdef _DEBUG
		// FPS�J�E���^�擾
		m_nCountFPS = GetFPS();

		// �f�o�b�O�\��������Ƃ�
		if (m_bDisplay)
		{
			// FPS�\��
			m_pDebugProc->Print("FPS:%d\n", m_nCountFPS);

			// �f�o�b�O���S�̕`��
			m_pDebugProc->Draw();
		}
		// �f�o�b�O�̕����񏉊���
		m_pDebugProc->StrInit();
		// ImGui�̕`��
		DrawImGui();
#endif

		// �J�n���ꂽ�V�[���̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	HRESULT result =  m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

#ifdef _DEBUG
	// �f�o�C�X�̃��Z�b�g
	if (result == D3DERR_DEVICELOST && 
		m_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
#endif
}

//==================================================================================================================
// ���[�h�̐ݒ�
//==================================================================================================================
void CRenderer::SetMode(MODE mode)
{
	// �Ȃɂ��Ȃ��Ȃ珈�����Ȃ�
	if (mode == CRenderer::MODE_NONE)
		return;

	// �w�肵�������~�߂�
	switch (mode)
	{
		// �^�C�g���̂Ƃ�
	case MODE_TITLE:
		// �����~�߂�
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMRESULT);
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMGAME);
		break;

		// �`���[�g���A���̂Ƃ�
	case MODE_TUTORIAL:
		// �����~�߂�
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTITLE);
		break;

		// �}�b�v�I����ʂ̂Ƃ�
	case MODE_MAPSELECT:
		// �����~�߂�
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTITLE);
		break;

		// �Q�[���Ƃ�
	case MODE_GAME:
		// �����~�߂�
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTUTORIAL);
		break;

		// �����L���O�̂Ƃ�
	case MODE_RESULT:
		// �����~�߂�
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMGAME);
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTITLE);

		break;
	}

	switch (m_mode)
	{
		// �^�C�g���̂Ƃ�
	case MODE_TITLE:
		// �����~�߂�
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE);
		// �I������
		m_pTitle->Uninit();
		// �j��
		delete m_pTitle;
		// NULL�ɂ���
		m_pTitle = nullptr;
		break;

		// �`���[�g���A���̂Ƃ�
	case MODE_TUTORIAL:
		// �����~�߂�
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_MAPSELECT);
		// �I������
		m_pTutorial->Uninit();
		// �j��
		delete m_pTutorial;
		// NULL�ɂ���
		m_pTutorial = nullptr;
		break;

		// �}�b�v�I����ʂ̂Ƃ�
	case MODE_MAPSELECT:
		// �����~�߂�
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_MAPSELECT);
		// �I������
		m_pMapSelect->Uninit();
		// �j��
		delete m_pMapSelect;
		// NULL�ɂ���
		m_pMapSelect = nullptr;
		break;

		// �Q�[���̂Ƃ�
	case MODE_GAME:
		// �����~�߂�
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TRANS);
		// �I������
		m_pGame->Uninit();
		// �j��
		delete m_pGame;
		// NULL�ɂ���
		m_pGame = nullptr;
		break;
	}

	// ���[�h�ϐ�����
	m_mode = mode;

	// ���[�h�̊J�n
	CManager::GetRenderer()->StartMode();
}

//==================================================================================================================
// �I�����[�h�̊J�n
//==================================================================================================================
void CRenderer::StartMode()
{
	switch (m_mode)
	{
		// �^�C�g���̂Ƃ�
	case MODE_TITLE:
		// �^�C�g���̐���
		m_pTitle = CTitle::Create();
		// BGM�Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
		break;

		// �`���[�g���A���̂Ƃ�
	case MODE_TUTORIAL:
		// �`���[�g���A���̐���
		m_pTutorial = CTutorial::Create();
		// BGM�Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_MAPSELECT);
		break;

		// �}�b�v�I����ʂ̂Ƃ�
	case MODE_MAPSELECT:
		// �}�b�v�I����ʂ̐���
		m_pMapSelect = CMapSelect::Create();
		// BGM�Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_MAPSELECT);
		break;

		// �Q�[���̂Ƃ�
	case MODE_GAME:
		// �Q�[���̐���
		m_pGame = CGame::Create();
		// BGM�Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
		break;
	}
}

//==================================================================================================================
// ���[�h�̎擾
//==================================================================================================================
CRenderer::MODE CRenderer::GetMode(void)
{
	return m_mode;
}

//==================================================================================================================
// �Q�[�����擾
//==================================================================================================================
CGame * CRenderer::GetGame(void)
{
	return m_pGame;
}

//==================================================================================================================
// �����擾
//==================================================================================================================
CSound * CRenderer::GetSound(void)
{
	return m_pSound;
}

//==================================================================================================================
// �f�o�C�X�擾
//==================================================================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	// �l��Ԃ�
	return m_pD3DDevice;
}
 
#ifdef _DEBUG
//==================================================================================================================
// �f�o�b�O�̕`��
//==================================================================================================================
void CRenderer::DrawImGui(void)
{
	if (CManager::GetShowImGui())
	{
		// ImGui�E�B���h�E�͏�ɒʏ�`��
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		// Imgui�̕`��
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// ���̕`����@�ɖ߂�
		CKananLibrary::GetWire() ?
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME) :	// ���C���[�t���[��
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);		// �ʏ�
	}
}

//==================================================================================================================
// �f�o�C�X�̃N���[���A�b�v
//==================================================================================================================
void CRenderer::CleanupDeviceD3D()
{
	if (m_pD3DDevice) 
		{ m_pD3DDevice->Release(); m_pD3DDevice = NULL; }
	if (m_pD3D) 
		{ m_pD3D->Release(); m_pD3D = NULL; }
}

//==================================================================================================================
// �f�o�C�X�̃��Z�b�g
//==================================================================================================================
void CRenderer::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = m_pD3DDevice->Reset(&m_d3dpp);

	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

//==================================================================================================================
// �f�o�b�O�R�}���h
//==================================================================================================================
void CRenderer::DebugCommand(void)
{
	// �J�����Œ�
	if (CInputKeyboard::GetKeyboardTrigger(DIK_F1))
	{
		if (CCamera::GetCameraMode() == CCamera::CAMERA_GAME)
			CCamera::SetCameraMode(CCamera::CAMERA_DEBUG);
		else if (CCamera::GetCameraMode() == CCamera::CAMERA_DEBUG)
			CCamera::SetCameraMode(CCamera::CAMERA_GAME);
	}

	// ImGui
	if (CInputKeyboard::GetKeyboardTrigger(DIK_F2))
	{
		CManager::GetShowImGui() ?
			CManager::SetShowImGui(false) :
			CManager::SetShowImGui(true);
	}

	// �Q�[��UI
	if (CInputKeyboard::GetKeyboardTrigger(DIK_F3))
	{
		CUI_game::GetbDisp() ?
			CUI_game::SetbDisp(false) :
			CUI_game::SetbDisp(true);
	}

	// �f�o�b�O�e�L�X�g
	if (CInputKeyboard::GetKeyboardTrigger(DIK_F4))
	{
		m_bDisplay ?
			m_bDisplay = false :
			m_bDisplay = true;
	}

	// �R���W����
	if (CInputKeyboard::GetKeyboardTrigger(DIK_F5))
	{
		m_bDisColl ?
			m_bDisColl = false :
			m_bDisColl = true;
	}

	/* �f�o�b�O�e�L�X�g�Ƃ��āA�󋵁E�R�}���h��\�� */
	if (CCamera::GetCameraMode() == CCamera::CAMERA_GAME)
		CDebugProc::Print("�J�������[�h : �Q�[��   [ F1�Őؑ� ]\n");
	else if (CCamera::GetCameraMode() == CCamera::CAMERA_DEBUG)
		CDebugProc::Print("�J�������[�h : �f�o�b�O [ F1�Őؑ� ]\n");

	CManager::GetShowImGui() ?
		CDebugProc::Print("ImGui�\����	 [ F2�Ŕ�\�� ]\n") :
		CDebugProc::Print("ImGui��\���� [ F2�ŕ\�� ]\n");

	CUI_game::GetbDisp() ?
		CDebugProc::Print("�Q�[��UI�\����	[ F3�Ŕ�\�� ]\n") :
		CDebugProc::Print("�Q�[��UI��\���� [ F3�ŕ\�� ]\n");

	m_bDisplay ?
		CDebugProc::Print("�f�o�b�O�e�L�X�g�\����	[ F4�Ŕ�\�� ]\n") :
		CDebugProc::Print("�f�o�b�O�e�L�X�g��\���� [ F4�ŕ\�� ]\n");

	m_bDisColl ?
		CDebugProc::Print("�����蔻��\����	  [ F5�Ŕ�\�� ]\n") :
		CDebugProc::Print("�����蔻���\���� [ F5�ŕ\�� ]\n");
}

//==================================================================================================================
// ImGui�̏�����
//==================================================================================================================
void CRenderer::InitImGui(D3DPRESENT_PARAMETERS d3dpp, HWND hWnd)
{
	// �v���[���e�[�V�����p�����[�^��ۑ�
	m_d3dpp = d3dpp;
	
	// �v�f�̏�����
	m_nCntWire = 0;
	m_bWire = false;
	
	// ImGui�̏�����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	// Imgui�̃X�^�C�� (������) �̌���
	ImGui::StyleColorsDark();
	
	// �v���b�g�t�H�[���ɍ��킹���Z�b�g�A�b�v���s��
	ImGui_ImplWin32_Init(hWnd);
	// ImgGui�̃����_���[���o�C���h
	ImGui_ImplDX9_Init(m_pD3DDevice);
}

//==================================================================================================================
// ImGui�̍X�V
//==================================================================================================================
void CRenderer::UpdateImGui(void)
{
	// �f�o�b�O�̊�{���̍X�V
	CKananLibrary::ShowDebugInfo();

	// �Q�[�����[�h��
	if (m_mode == MODE_GAME)
		// �L�����p�����[�^�[�̏��X�V
		CCharaParam::UpdateImGui();

	ImGui::End();

	// �g�����I�������K������
	ImGui::EndFrame();
	// �X�e���V���e�X�g���I����
	m_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}
#endif
