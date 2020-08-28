// ===================================================================
//
// XInput�Q�[���p�b�h���� [ inputGamepad.cpp ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#include "inputGamepad.h"
#include "manager.h"
#include "debugProc.h"

// ===================================================================
// �ÓI�����o�ϐ��̏�����
// ===================================================================
bool CInputGamepad::m_bAllVib = false;	// �U���ݒ�

// ===================================================================
// �R���X�g���N�^
// ===================================================================
CInputGamepad::CInputGamepad()
{

}

// ===================================================================
// �f�X�g���N�^
// ===================================================================
CInputGamepad::~CInputGamepad()
{

}

// ===================================================================
// ������
// ===================================================================
HRESULT CInputGamepad::Init(HINSTANCE hInstance, HWND hWnd, int nIndex)
{
	m_bVibration = false;

	// �v���C���[�ԍ��̕ۑ�
	m_nIndexPlayer = nIndex;

	// ������
	ZeroMemory(&m_state, sizeof(XINPUT_STATE));

	// �R���g���[���[�X�e�[�g�̎擾
	DWORD dwResult = XInputGetState(m_nIndexPlayer, &m_state);

	// �擾�ł���
	if (dwResult == ERROR_SUCCESS)
		m_bConnect = true;
	// �ł��Ȃ�����
	else
		m_bConnect = false;

	return S_OK;
}

// ===================================================================
// �I��
// ===================================================================
void CInputGamepad::Uninit(void)
{
	// �g�p���Ȃ�
	// false�ɂ��邱�Ƃ� [�{�^�����������E�X�e�B�b�N�������E�g���K�[0�E�o�C�u���[�V������~] �𑗐M����
	XInputEnable(false);
}

// ===================================================================
// �X�V
// ===================================================================
void CInputGamepad::Update(void)
{
	// �O��̏��̕ۑ�
	m_stateOld = m_state;

	// �ڑ��̊m�F
	UpdateControlState();

	// ���X�e�B�b�N�̒l���f�b�h�]�[��(���ʂȒl)��
	if ((m_state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		// 0�ɐ���
		m_state.Gamepad.sThumbLX = 0;
		m_state.Gamepad.sThumbLY = 0;
	}

	// �E�X�e�B�b�N�̒l���f�b�h�]�[��(���ʂȒl)��
	if ((m_state.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		m_state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(m_state.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		m_state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		// 0�ɐ���
		m_state.Gamepad.sThumbRX = 0;
		m_state.Gamepad.sThumbRY = 0;
	}

	// �o�C�u���[�V�����̍X�V
	UpdateVibration();
}

// ===================================================================
// �ڑ�����Ă��邩�m�F
// ===================================================================
HRESULT CInputGamepad::UpdateControlState(void)
{
	// ������
	ZeroMemory(&m_state, sizeof(XINPUT_STATE));

	// �R���g���[���[�X�e�[�g�̎擾
	DWORD dwResult = XInputGetState(m_nIndexPlayer, &m_state);

	// �擾�ł���
	if (dwResult == ERROR_SUCCESS)
		m_bConnect = true;
	// �ł��Ȃ�����
	else
		m_bConnect = false;

	return S_OK;
}

// ===================================================================
// �o�C�u���[�V�����̍X�V
// ===================================================================
void CInputGamepad::UpdateVibration(void)
{
	// �U������ݒ肪�؂�Ă���
	if (!m_bAllVib)
	{
		// �������I����
		return;
	}

	XINPUT_VIBRATION vibration; // �o�C�u���[�V�����̍\����

	// ������
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	// ���ꂼ��̎��g��ݒ�
	if (m_bVibration)
		vibration.wLeftMotorSpeed = 65535;
	else
		vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;

	// �o�C�u���[�V���������s
	XInputSetState(0, &vibration);
}

// ===================================================================
// �g���K�[����
// ===================================================================
bool CInputGamepad::GetTrigger(JOYPADKEY button)
{
	// �{�^���̏���
	if (m_state.Gamepad.wButtons & button)
	{
		// ���݂̃L�[�ƑO��̃L�[���Ⴄ
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_L2 &&
		m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_R2 &&
		m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	return false;
}

// ===================================================================
// �v���X����
// ===================================================================
bool CInputGamepad::GetPress(JOYPADKEY button)
{
	// �{�^���̏���
	if (m_state.Gamepad.wButtons & button)
		return true;

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_L2 &&
		m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;		// bLeftTrigger���}�N�����傫����Γ���

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_R2 &&
		m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;		// bRightTrigger���}�N�����傫����Γ���

	return false;
}

// ===================================================================
// ���X�e�B�b�N�̎擾
// ===================================================================
void CInputGamepad::GetStickLeft(float * pValueH, float * pValueV)
{
	// �X�e�B�b�N�̗L���͈͂Ŋ���A-1.0f �` 1.0f �ŏo�͂���
	*pValueH = m_state.Gamepad.sThumbLX / JOY_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbLY / JOY_MAX_RANGE;

}

// ===================================================================
// �E�X�e�B�b�N�̎擾
// ===================================================================
void CInputGamepad::GetStickRight(float * pValueH, float * pValueV)
{
	// �X�e�B�b�N�̗L���͈͂Ŋ���A-1.0f �` 1.0f �ŏo�͂���
	*pValueH = m_state.Gamepad.sThumbRX / JOY_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbRY / JOY_MAX_RANGE;
}
