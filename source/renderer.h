//==================================================================================================================
//
// �����_���[[renderer.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "manager.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CFade;
class CDebugProc;
class CCamera;
class CLight;
class CGame;
class CTitle;
class CResult;
class CRanking;
class CTutorial;
class CRecord;
class CSound;
class CMapSelect;

//==================================================================================================================
//
// �����_���[�N���X�̒�`
//
//==================================================================================================================
class CRenderer
{
public:
	// ==========================================================
	// ��ʃ��[�h�̎��
	// ==========================================================
	typedef enum
	{
		MODE_NONE = -1,
		MODE_TITLE,				// �^�C�g��
		MODE_TUTORIAL,			// �`���[�g���A��
		MODE_MAPSELECT,			// �X�e�[�W�I��
		MODE_GAME,				// �Q�[��
		MODE_RESULT,			// ���U���g
		MODE_MAX				// �ő吔
	} MODE;						// ���[�h

	CRenderer();											// �R���X�g���N�^
	~CRenderer();											// �f�X�g���N�^
	HRESULT Init(HWND hWnd, BOOL bWindow);					// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static void SetMode(MODE mode);							// ���[�h�̐ݒ�
	static MODE GetMode(void);								// ���[�h�̎擾
	static CGame *GetGame(void);							// �Q�[���̎擾
	static CSound *GetSound(void);							// ���̎擾
	static CFade *GetFade(void) { return m_pFade; }			// �t�F�[�h�̎擾
	LPDIRECT3DDEVICE9 GetDevice(void);						// �f�o�C�X�擾�֐�

#ifdef _DEBUG
	static bool GetbDisColl(void) { return m_bDisColl; }	// �����蔻��̉����t���O���擾
	static bool GetbDisplay(void) { return m_bDisplay; }	// �f�o�b�O�e�L�X�g�\���t���O�̎擾

	static void SetbDisColl(bool bDisplay) { m_bDisColl = bDisplay; }	// �����蔻������̐ݒ�
	static void SetbDisplay(bool bDisplay) { m_bDisplay = bDisplay; }	// �f�o�b�O�e�L�X�g�\���̐ݒ�

	void CleanupDeviceD3D();							// �f�o�C�X�̃N���[���A�b�v
#endif

protected:

private:
	void DebugCommand(void);							// �f�o�b�O�R�}���h

	static CFade *m_pFade;								// �t�F�[�h�̏��|�C���^
	static CDebugProc *m_pDebugProc;					// �f�o�b�O���O�̏��|�C���^
	static CCamera *m_pCamera;							// �J�����̏��|�C���^
	static CLight *m_pLight;							// ���C�g�̏��|�C���^
	static CTitle *m_pTitle;							// �^�C�g�����̃|�C���^
	static CGame *m_pGame;								// �Q�[�����̃|�C���^
	static CResult *m_pResult;							// ���U���g���̃|�C���^
	static CTutorial *m_pTutorial;						// �`���[�g���A�����̃|�C���^
	static MODE m_mode;									// ���[�h���̃|�C���^
	static CSound *m_pSound;							// �����̃|�C���^
	static CMapSelect *m_pMapSelect;					// �}�b�v�I����ʂ̏��|�C���^

	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
	LPDIRECT3D9 m_pD3D = NULL;
	LPD3DXFONT	m_pFont = NULL;							// �t�H���g�ւ̃|�C���^
	CRecord *m_pRecord;									// �L�^�̏��|�C���^

	int			m_nCountFPS = 0;						// FPS�J�E���^

	void StartMode();									// �I�����[�h�̊J�n

#ifdef _DEBUG
	void InitImGui(D3DPRESENT_PARAMETERS d3dpp, HWND hWnd);	// ImGui�̏�����
	void UpdateImGui(void);									// ImGui�̍X�V
	void DrawImGui(void);									// ImGui�̕`��
	void ResetDevice();										// �f�o�C�X�̃��Z�b�g

	D3DPRESENT_PARAMETERS	m_d3dpp;					// ImGui�ɕK�v
	int						m_nCntWire;					// ���C���[�t���[����A�����͂��Ȃ����߂̃J�E���^
	bool					m_bWire;					// ���C���[�t���[��
	static bool				m_bDisplay;					// �f�o�b�O�\�������邩�ǂ���
	static bool				m_bDisColl;					// �����蔻����������邩
#endif

};
#endif