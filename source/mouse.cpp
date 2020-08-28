//=============================================================================
//
//	�}�E�X���� [ mouse.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "mouse.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �\���̒�`
//=============================================================================

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMouse::CMouse()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMouse::~CMouse()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	m_hWnd = hWnd;

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		std::cout << "�}�E�X�������s" << std::endl;
		return E_FAIL;
	}

	m_pDevice->SetDataFormat(&c_dfDIMouse2); // �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�

	m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j    
	m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// ���͐���J�n
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMouse::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CMouse::Update(void)
{
	POINT point;

	// �l�̏�����    
	m_mouseState.rgbButtons[0] = 0;

	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);
	m_posX = (float)point.x;
	m_posY = (float)point.y;

	// �l�̍X�V 
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouseState)))
	{
		for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// �r���I�_���a���ǂ������f
			m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouseState.rgbButtons[nCntButton]) & m_mouseState.rgbButtons[nCntButton];
			m_aButtonState[nCntButton] = m_mouseState.rgbButtons[nCntButton];	// �L�[�v���X���ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire(); // �P���ڂ�Q���ڂɃG���[���o�邪�������Ă悢�B 
	}
}

//=============================================================================
// �v���X����
//=============================================================================
bool CMouse::GetPress(int nButton)
{
	return(m_mouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �g���K�[����
//=============================================================================
bool CMouse::GetPressTrigger(int nButton)
{
	return(m_aButtonStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X��X���W�̎擾
//=============================================================================
LONG CMouse::GetposX(void)
{
	return (LONG)m_posX;
}

//=============================================================================
// �}�E�X��Y���W�̎擾
//=============================================================================
LONG CMouse::GetposY(void)
{
	return (LONG)m_posY;
}

//=============================================================================
// �E�B���h�E�n���h���̎擾
//=============================================================================
HWND CMouse::GetWnd(void)
{
	return m_hWnd;
}
