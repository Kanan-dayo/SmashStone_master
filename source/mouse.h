// ===================================================================
//
// �}�E�X���� [ mouse.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "main.h"
#include "input.h"

// ===================================================================
// �}�N����`
// ===================================================================
#define NUM_BUTTON_MAX (8)

// ===================================================================
// �N���X��`
// ===================================================================
class CMouse : public CInput
{
public:
	CMouse();
	~CMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ������
	void Uninit(void);								// �I��
	void Update(void);								// �X�V
	bool GetPress(int nKey);						// �v���X�擾
	bool GetPressTrigger(int nKey);					// �g���K�[�擾
	LONG GetposX(void);								// �J�[�\�����W(X)�̎擾
	LONG GetposY(void);								// �J�[�\�����W(Y)�̎擾
	HWND GetWnd(void);								// hWnd�̎擾

private:
	DIMOUSESTATE2 m_mouseState;						// �}�E�X�̏��
	float m_posX;									// �}�E�X���W(X)
	float m_posY;									// �}�E�X���W(Y)

	LPDIRECTINPUTDEVICE8	m_pDevice = NULL;		// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^

	BYTE	m_aButtonState[NUM_BUTTON_MAX];			// �}�E�X�̓��͏�񃏁[�N
	BYTE	m_aButtonStateTrigger[NUM_BUTTON_MAX];	// �}�E�X�̃g���K�[

	HWND	m_hWnd;									// �E�B���h�E�n���h��
};

#endif