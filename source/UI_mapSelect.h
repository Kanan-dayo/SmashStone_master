//==================================================================================================================
//
// UI_mapSelect[UI_mapSelect.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_MAPSELECT_H_
#define _UI_MAPSELECT_H_

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
class CUI_mapSelect : public CScene
{
public:
	//=============================================================================
	// �񋓌^��`
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_WORLDMAP = 0,		// ���E�n�}
		LOGOTYPE_MAPFRAME,			// �}�b�v�I��g
		LOGOTYPE_MAP1,				// �}�b�v1
		LOGOTYPE_MAP2,				// �}�b�v2
		LOGOTYPE_MAX				// �ő�
	} UI_MAPSELECT_TYPE;

	CUI_mapSelect(PRIORITY type);	// �R���X�g���N�^
	~CUI_mapSelect();				// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CUI_mapSelect *Create(void);// ��������
	static HRESULT Load(void);		// �e�N�X�`����񃍁[�h����
	static void Unload(void);		// �e�N�X�`�����A�����[�h����

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ���S�ݒ菈��

	static int GetMapID(void) { return m_nMapID; }	// �}�b�vID�̎擾
	bool GetSelectMap(void) { return m_bMapSelect; }// �}�b�v�̑I���󋵂��擾

protected:

private:
	void MapSelectUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// �}�b�v�I����ʂ̍X�V����
	void ControlGamepad(CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// �Q�[���p�b�h����
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// �L�[�{�[�h����

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// �e�N�X�`�����
	static char *m_apFileName[LOGOTYPE_MAX];			// ���S�̃t�@�C����
	static int m_nMapID;								// �}�b�v�ԍ�

	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2D�̏��

	int m_nCharaNum[MAX_PLAYER];						// �L�����ԍ�

	bool m_bMapSelect;									// �}�b�v��I���������ǂ���

};
#endif