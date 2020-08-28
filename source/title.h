//==================================================================================================================
//
// �^�C�g������[title.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _TITLE_H
#define _TITLE_H

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CCamera;
class CLight;
class CPolygon;
class CMeshField;
class CMeshSphere;
class CModel;
class CUI;
class CCharacter;

//==================================================================================================================
//
// �^�C�g���N���X�̒�`
//
//==================================================================================================================
class CTitle
{
public:
	
	CTitle();									// �R���X�g���N�^
	~CTitle();									// �f�X�g���N�^
	void Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static CTitle *Create(void);				// ��������
	static CCharacter *GetCharacter(void);		// �L�����N�^�[�̏��擾����
	static CCamera *GetCamera(void);			// �J�����̏��擾����
	static bool GetbModeNext(void) { return m_bNextScreen; }// ���̉�ʂɍs�����ǂ����擾����

	void SetNextMode(int nNextMode);			// ���̃��[�h�ݒ菈��

protected:

private:

	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`�����
	static CCamera *m_pCamera;				// �J�����̏��
	static CLight *m_pLight;				// ���C�g�̏��
	static CPolygon *m_pPolygon;			// �|���S�����
	static CMeshField *m_pMeshField;		// ���b�V���t�B�[���h�̏��
	static CMeshSphere *m_pMeshSphere;		// ���b�V�����̏��|�C���^
	static CUI *m_pUI;						// UI�̏��|�C���^
	static CCharacter *m_pCharacter;		// �L�����N�^�[�̏��|�C���^
	static bool m_bNextScreen;				// ���̃��[�h�ɂ������ǂ���

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuff;	// ���_�o�b�t�@

	int m_nNextMode;						// ���̃��[�h

};

#endif
