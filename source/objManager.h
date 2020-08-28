//=============================================================================
//
// �I�u�W�F�N�g�}�l�[�W���[����  [ objectManager.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "object.h"
#include "objectLift.h"
#include "scene.h"
#include <vector>
#include <memory>							// �X�}�[�g�|�C���^�̎g�p�ɕK�v
#include "kananlibrary.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CObject;

class CObjectManager : public CScene
{
public:
	typedef struct
	{	// �I�u�W�F�N�g�I�t�Z�b�g
		int nType;			// �^�C�v
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ��]
		D3DXVECTOR3 scale;	// �g�k
	} OBJECT_OFFSET;

	typedef struct
	{
		int nNumObject;						// �z�u����I�u�W�F�N�g��
		OBJECT_OFFSET *objOffset;			// �I�u�W�F�N�g�I�t�Z�b�g�i�[�p
		std::vector<CObject*> pObject;		// �I�u�W�F�N�g���
		std::vector<CObjectLift*> pObjLift;	// ���Ă�I�u�W�F�N�g���
		D3DXVECTOR3 stonePos[5];			// �X�g�[����z�u����ʒu
	} STAGEINFO;

	CObjectManager(CScene::PRIORITY nPriority);	// �R���X�g���N�^
	~CObjectManager();	// �f�X�g���N�^

	void Init();									// ������
	void Uninit();									// �I��
	void Update();									// �X�V
	void Draw();									// �`��
	static CObjectManager *Create(STAGETYPE type);	// ����

	static HRESULT Load(void);					// ���f���̃��[�h
	static void Unload(void);					// ���f���̔j��

	static MODEL_VTX GetModelVtx(const int & nType) { return m_objInfo[nType].modelVtx; }	// �^�C�v���Ƃ̃��f�����_���̎擾
	static STAGETYPE GetStageType(void) { return m_stageType; }								// �X�e�[�W�̃^�C�v�擾
	static STAGEINFO GetStageInfo(void) { return m_stageInfo[m_stageType]; }				// �X�e�[�W�̏��擾

	std::vector<CObject*> GetObjAll(void) { return m_stageInfo[m_stageType].pObject; }		// �I�u�W�F�N�g�̎擾
	CObject* GetObj(int nIndex) { return m_stageInfo[m_stageType].pObject[nIndex]; }		// �I�u�W�F�N�g�̎擾
	int		GetNumObject(void) { return (int)m_stageInfo[m_stageType].pObject.size(); }		// �I�u�W�F�N�g�����̎擾

	std::vector<CObjectLift*> GetObjLiftAll(void) { return m_stageInfo[m_stageType].pObjLift; }	// ���Ă�I�u�W�F�N�g�̎擾
	CObjectLift* GetObjLift(int nIndex) { return m_stageInfo[m_stageType].pObjLift[nIndex]; }	// ���Ă�I�u�W�F�N�g�̎擾
	int		GetNumObjLift(void) { return (int)m_stageInfo[m_stageType].pObjLift.size(); }		// ���Ă�I�u�W�F�N�g�����̎擾

	static D3DXVECTOR3 GetDefaultStonePos(int nPos) { return m_stageInfo[m_stageType].stonePos[nPos]; }	// �X�g�[���̔z�u�ʒu�̎擾

#ifdef _DEBUG
	void ShowObjectManagerInfo(void);			// ImGui�̍X�V
	static bool GetShowObjWindow(void) { return m_bShowAnother; }			// �I�u�W�F�E�B���h�E�\���̗L�����擾
	static void SetShowObjWindow(bool bShow) { m_bShowAnother = bShow; }	// �\���̐ݒ�
#endif

private:
	static void CreateTexture();	// �e�N�X�`������
	static void CreateObjInfo();	// �I�u�W�F�N�g��񐶐�

	static HRESULT LoadModel(void);				// ���f���ǂݍ���
	static HRESULT LoadOffset(void);			// �I�t�Z�b�g�ǂݍ���
	static HRESULT SetOffset(void);				// �I�t�Z�b�g�ݒ�

	static char					m_cModelFile[MAX_TEXT];				// ���f�����̃t�@�C����
	static char					m_aFileName[STAGE_MAX][MAX_TEXT];	// ���f���z�u�̃t�@�C����
	static STAGEINFO			m_stageInfo[STAGE_MAX];				// �X�e�[�W�̏��
	static STAGETYPE			m_stageType;						// �X�e�[�W�̃^�C�v
	static std::vector<OBJINFO>	m_objInfo;							// �I�u�W�F�N�g�̃��f�����
	static int					m_nNumTexture;						// �e�N�X�`������
	static int					*m_pModelIndex;						// �e�N�X�`�������蓖�Ă郂�f���ԍ�

#ifdef _DEBUG
	// �f�o�b�O���[�h�̍\����
	typedef enum
	{
		MODE_GAME = 0,	// �Q�[��
		MODE_DEBUG		// �f�o�b�O
	} DEBUG_MODE;

	HRESULT SaveOffset(void);						// �I�t�Z�b�g���̃Z�[�u

	static int	m_stateMode;						// ���[�h�̏��
	static bool m_bShowAnother;						// �ʂ̃E�B���h�E
	CObject		*m_pFakeObject;						// �U�̃I�u�W�F�N�g
	static bool m_bObjUse;							// �U�I�u�W�F���g�p���Ă��邩
	static int	m_nFakeType;						// �U�I�u�W�F�̃^�C�v
	static std::string	*m_pObjName;				// ImGui�p�̃I�u�W�F�N�g��
#endif
};
#endif