//==================================================================================================================
//
// �J����[camera.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CInputKeyboard;

//==================================================================================================================
//
// �J�����N���X�̒�`
//
//==================================================================================================================
class CCamera
{
public:
	typedef enum
	{	// �J�����̏��
		CAMERA_GAME = 0,	// �Q�[���J����
		CAMERA_DEBUG	// �f�o�b�O�J����
	} CAMERA_MODE;

	CCamera();							// �R���X�g���N�^
	~CCamera();							// �f�X�g���N�^
	void Init();						// ����������
	void Uninit(void);					// �I������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	static CCamera *Create(void);		// ��������

	void SetCamera(void);				// �J�����̐ݒ菈��
	void SetCameraPos(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR, const D3DXVECTOR3 rot);	// �J�����̈ʒu�ݒ�
	void SetRot(const D3DXVECTOR3 &rot)		{ m_rot = rot; }		// ��]�ݒ菈��
	void SetPosV(const D3DXVECTOR3 &posV)	{ m_posV = posV; }		// ���_�ʒu�ݒ菈��
	void SetPosR(const D3DXVECTOR3 &posR)	{ m_posR = posR; }		// �����_�̐ݒ�
	static void SetCameraMode(CAMERA_MODE mode) { m_mode = mode; }	// �J�����̃��[�h�ݒ�

	D3DXVECTOR3 &GetRot(void)			{ return m_rot; }	// ��]�擾����
	D3DXVECTOR3 &GetPosV(void)			{ return m_posV; }	// ���_�ʒu�ݒ菈��
	D3DXVECTOR3 &GetPosR(void)			{ return m_posR; }	// �����_�̎擾
	float		&GetRotY(void)			{ return m_rot.y; }	// ��](Y��)�̎擾
	D3DXVECTOR3 &GetVec(void)			{ return m_vec; }	// �x�N�g���̎擾
	static CAMERA_MODE &GetCameraMode(void)	{ return m_mode; }	// �J�����̃��[�h�擾

protected:

private:
#ifdef _DEBUG
	void ShowDebugInfo(void);			// ImGui�̍X�V
	void DebugControl(void);			// �f�o�b�O���̑���
	void ControlMouse(void);			// �}�E�X�ł̑���
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// �L�[�{�[�h�ł̑���
	D3DXVECTOR3 m_posDebug;				// �f�o�b�O�p�̈ʒu
#endif

	void MoveCamera(void);				// �Q�[���̈ړ�
	static CCamera *m_pCamera;			// �J�������
	static CAMERA_MODE m_mode;			// �J�����̏��

	D3DXMATRIX m_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;				// �r���[�}�g���b�N�X

	D3DXVECTOR3 m_posV;					// ���_
	D3DXVECTOR3 m_posVDest;				// ���_�̖ڕW�n�_
	D3DXVECTOR3 m_posR;					// �����_(�Q�Ɠ_)
	D3DXVECTOR3 m_posRDest;				// �����_(�Q�Ɠ_)�̖ڕW�n�_
	D3DXVECTOR3 m_posU;					// ������x�N�g��
	D3DXVECTOR3 m_rot;					// ��]
	D3DXVECTOR3 m_rotDest;				// ��]�̖ڕW�n�_
	D3DXVECTOR3 m_vec;					// �J�����̌����Ă���x�N�g��
	D3DXVECTOR3 m_mousePos;				// �}�E�X����p

	int nCntRot;						// ��]���n�߂�J�E���^
	int m_nCntTitleFade;				// �^�C�g���t�F�[�h�̃J�E���^

	float m_fDistance;					// ����
	float m_fDisScale;					// �g���\�ȋ���

	bool m_bCameraMode;					// �J�������[�h���ǂ���
};
#endif