//==================================================================================================================
//
// �Ǘ�[manager.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CRenderer;
class CInputKeyboard;
class CInputGamepad;
class CMouse;

//==================================================================================================================
//
// �}�l�[�W���N���X�̒�`
//
//==================================================================================================================
class CManager
{
public:
	CManager();														// �R���X�g���N�^
	~CManager();													// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);		// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��

	static CRenderer *GetRenderer(void);							// �����_���[�̎擾
	static CInputKeyboard *GetInputKeyboard(void);					// �L�[�{�[�h�̎擾
	static CInputGamepad *GetInputGamepad(int nPlayer);				// �Q�[���p�b�h�̎擾
	static CMouse *GetMouse(void) { return m_pMouse; }				// �}�E�X�̎擾

#ifdef _DEBUG
	static bool GetShowImGui(void) { return m_bShowWindow; }		// �\���̗L���̎擾
	static void SetShowImGui(bool bShow) { m_bShowWindow = bShow; }	// �\���̗L���̐ݒ�
#endif

protected:

private:
	static CMouse	*m_pMouse;										// �}�E�X�̃|�C���^
	static CRenderer *m_pRenderer;									// �����_���[���̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;						// �L�[�{�[�h���̃|�C���^
	static CInputGamepad *m_pInputGamepad[MAX_PLAYER];				// �Q�[���p�b�h���̃|�C���^

#ifdef _DEBUG
	HRESULT LoadImGuiInfo(void);			// ImGui���̓ǂݍ���
	HRESULT SaveImGuiInfo(void);			// ImGui���̕ۑ�
	static char	m_cFileImGui[64];		// ImGui�ݒ�̃t�@�C����
	static bool	m_bShowWindow;			// �\���̗L��
#endif
};
#endif