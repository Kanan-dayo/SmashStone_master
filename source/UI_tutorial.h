//==================================================================================================================
//
// UI_tutorial[UI_tutorial.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_TUTORIAL_H_
#define _UI_TUTORIAL_H_

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
class CUI_tutorial : public CScene
{
public:
	CUI_tutorial(PRIORITY type);	// �R���X�g���N�^
	~CUI_tutorial();				// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CUI_tutorial *Create(void);// ��������
	static HRESULT Load(void);		// �e�N�X�`����񃍁[�h����
	static void Unload(void);		// �e�N�X�`�����A�����[�h����

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ���S�ݒ菈��

	int GetCharaNum(int nPlayer) { return m_nCharaNum[nPlayer]; }	// �v���C���[�̃^�C�v���擾
	bool GetCharaDecide(int nPlayer) { return m_bCharaDecide[nPlayer]; }

protected:

private:
	//=============================================================================
	// �񋓌^��`
	//=============================================================================
	typedef enum
	{
		UITYPE_GEAR0 = 0,		// ����0
		UITYPE_CLOCK0,			// ���v�y��0
		UITYPE_CLOCKHANDS0,		// ���v�̐j0
		UITYPE_CLOCKGEAR0,		// ���v�̎���0
		UITYPE_GEAR1,			// ����0
		UITYPE_CLOCK1,			// ���v�y��0
		UITYPE_CLOCKHANDS1,		// ���v�̐j0
		UITYPE_CLOCKGEAR1,		// ���v�̎���0
		UITYPE_CHARAFULLBG,		// �L�����N�^�[�S���w�i
		UITYPE_CHARAFULL,		// �L�����N�^�[�S��
		UITYPE_1PCHARABG,		// �L�����N�^�[1P�w�i
		UITYPE_2PCHARABG,		// �L�����N�^�[2P�w�i
		UITYPE_1PCHARA,			// 1P�L�����N�^�[
		UITYPE_2PCHARA,			// 2P�L�����N�^�[
		UITYPE_1PCHARA_FREAM,	// 1P�L�����N�^�[�g��
		UITYPE_2PCHARA_FREAM,	// 2P�L�����N�^�[�g��
		UITYPE_1PCHARA_NAME,	// 1P�L�����N�^�[�l�[��
		UITYPE_2PCHARA_NAME,	// 2P�L�����N�^�[�l�[��
		UITYPE_1PREADY,			// 1P�L�����N�^�[��������
		UITYPE_2PREADY,			// 2P�L�����N�^�[��������
		UITYPE_SELECTICON,		// �v���C���[�Z���N�g�A�C�R��
		UITYPE_MAX				// �ő�
	} UI_TUTORIAL_TYPE;

	void TutorialUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// �`���[�g���A���̍X�V����
	void ControlGamepad(CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// �Q�[���p�b�h����
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// �L�[�{�[�h����

	static LPDIRECT3DTEXTURE9 m_pTexture[UITYPE_MAX];	// �e�N�X�`�����
	static char *m_apFileName[UITYPE_MAX];			// ���S�̃t�@�C����

	CScene2D *m_pScene2D[UITYPE_MAX];					// scene2D�̏��

	int m_nCharaNum[MAX_PLAYER];						// �L�����ԍ�
	int m_nCntRot[MAX_PLAYER];							// ���v�̐j��]�p�J�E���^
	int m_nCntWait[MAX_PLAYER];							// �ҋ@���ԗp�J�E���^

	float m_fPosMove[MAX_PLAYER];						// �ʒu�ړ��p�J�E���^
	float m_fPos[MAX_PLAYER];							// ���݂̘g���e�N�X�`���ʒuX
	float m_fPosDiff[MAX_PLAYER];						// �ڕW�̘g���e�N�X�`���ʒuX
	float m_fPosOld[MAX_PLAYER];						// �O��̘g���e�N�X�`���ʒuX
	float m_fRotGear[MAX_PLAYER];						// ���Ԃ̉�]�i�[�ϐ�
	float m_fPosCul[MAX_PLAYER];						// �ʒu�v�Z�p�ϐ�
	float m_fDiff[MAX_PLAYER];							// 1�t���[���O�Ƃ̋���
	float m_fAngle[MAX_PLAYER];							// ���Ԃ̉�]�p�x
	float m_fRad[MAX_PLAYER];							// ���W�A���l

	bool m_bUIClockHands[MAX_PLAYER];					// ���v�̐j�������Ă��邩�ǂ���
	bool m_bCharaDecide[MAX_PLAYER];					// �����̃L�����N�^�[��I���������ǂ���
	bool m_bStickReturn[MAX_PLAYER];					// �p�b�h�̃X�e�B�b�N��߂������ǂ���

};
#endif