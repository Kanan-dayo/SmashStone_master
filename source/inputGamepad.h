// ===================================================================
//
// XInput�Q�[���p�b�h���� [XGamepad.h]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _XGAMEPAD_H_
#define _XGAMEPAD_H_

#define _CRT_SECURE_NO_WARNINGS

// ===================================================================
//�C���N���[�h
// ===================================================================
#include "main.h"
#include "input.h"

// ===================================================================
// �}�N����`
// ===================================================================
#define JOY_MAX_RANGE (32768.0f)		// �X�e�B�b�N�L���͈�

// ===================================================================
// �N���X��`
// ===================================================================
class CInputGamepad : public CInput
{
public:
	typedef enum
	{
		JOYPADKEY_X		= XINPUT_GAMEPAD_X,					// X
		JOYPADKEY_Y		= XINPUT_GAMEPAD_Y,					// Y
		JOYPADKEY_A		= XINPUT_GAMEPAD_A,					// A
		JOYPADKEY_B		= XINPUT_GAMEPAD_B,					// B
		JOYPADKEY_L1	= XINPUT_GAMEPAD_LEFT_THUMB,		// L1
		JOYPADKEY_R1	= XINPUT_GAMEPAD_RIGHT_THUMB,		// R1
		JOYPADKEY_L2	= XINPUT_GAMEPAD_LEFT_SHOULDER,		// L2
		JOYPADKEY_R2	= XINPUT_GAMEPAD_RIGHT_SHOULDER,	// R2
		JOYPADKEY_BACK	= XINPUT_GAMEPAD_BACK,				// BACK
		JOYPADKEY_START = XINPUT_GAMEPAD_START,				// START
		JOYPADKEY_LEFT	= XINPUT_GAMEPAD_DPAD_LEFT,			// �����L�[[��]
		JOYPADKEY_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,		// �����L�[[�E]
		JOYPADKEY_UP	= XINPUT_GAMEPAD_DPAD_UP,			// �����L�[[��]
		JOYPADKEY_DOWN	= XINPUT_GAMEPAD_DPAD_DOWN,			// �����L�[[��]
		JOYPADKEY_MAX
	} JOYPADKEY;

	CInputGamepad();										// �R���X�g���N�^
	~CInputGamepad();										// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, int nIndex); // ������
	void Uninit(void);										// �I��
	void Update(void);										// �X�V

	bool GetbConnect(void) { return m_bConnect; }			// �ڑ��̎擾
	bool GetTrigger(JOYPADKEY button);
	bool GetPress(JOYPADKEY button);
	bool GetVibration(void) { return m_bVibration; }		// �o�C�u���[�V�����̎擾
	bool GetAllVib(void) { return m_bAllVib; }				// �Q�[�����̐U�����邩�̎擾
	void GetStickLeft(float *pValueH, float *pValueV);		// ���X�e�B�b�N�̎擾
	void GetStickRight(float *pValueH, float *pValueV);		// �E�X�e�B�b�N�̎擾

	void SetVibration(bool bVib) { m_bVibration = bVib; }	// �o�C�u���[�V�����̐ݒ�
	void SetAllVib(bool bVib) { m_bAllVib = bVib; }			// �Q�[�����̐U�����邩�̐ݒ�

private:
	HRESULT UpdateControlState(void);						// �ڑ�����Ă��邩�m�F
	void	UpdateVibration(void);							// �o�C�u���[�V�����̍X�V

	XINPUT_STATE		m_state;							// �R���g���[���[�̏��
	XINPUT_STATE		m_stateOld;							// �R���g���[���[�̑O��̏��
	int					m_nIndexPlayer;						// �v���C���[�̔ԍ�
	bool				m_bConnect;							// �ڑ�����Ă��邩
	bool				m_bVibration;						// �U�����邩
	static bool			m_bAllVib;							// �Q�[�����̐U�����邩�̐ݒ�
};
#endif 