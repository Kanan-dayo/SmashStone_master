//==================================================================================================================
//
// UI_title[UI_title.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_TITLE_H_
#define _UI_TITLE_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CScene2D;
class CInputKeyboard;
class CInputGamepad;

//==================================================================================================================
//
// ���S�N���X�̒�`
//
//==================================================================================================================
class CUI_title : public CScene
{
public:
	CUI_title(PRIORITY type);		// �R���X�g���N�^
	~CUI_title();					// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CUI_title *Create(void);	// ��������
	static HRESULT Load(void);		// �e�N�X�`����񃍁[�h����
	static void Unload(void);		// �e�N�X�`�����A�����[�h����

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ���S�ݒ菈��

protected:

private:
	//=============================================================================
	// �񋓌^��`
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_TITLE = 0,			// �^�C�g��
		LOGOTYPE_ENTER,				// �G���^�[���S
		LOGOTYPE_MAX				// �ő�
	} UI_TITLE_TYPE;

	void TitleUpdate(void);	// �^�C�g���̍X�V����

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// �e�N�X�`�����
	static char *m_apFileName[LOGOTYPE_MAX];			// ���S�̃t�@�C����

	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2D�̏��

	D3DXVECTOR3 TitlePos;								// �^�C�g��UI�̈ʒu

	int m_nCntBound;									// �^�C�g��UI�o�E���h�J�E���^

	float m_fCntUITitle0;								// �^�C�g��UI�p�J�E���^0
	float m_fCntUITitle1;								// �^�C�g��UI�p�J�E���^1
	float m_fCntEnter;									// �G���^�[�p�J�E���^

	bool m_bUITitle0;									// �^�C�g���𓮂������ǂ���0
	bool m_bUITitle1;									// �^�C�g���𓮂������ǂ���1
	bool m_bUIEnter;									// �G���^�[�̃��l�p�ϐ�

};
#endif