//*************************************************************************************************************
//
// �J�v�Z���R���C�_�[����[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CAPSULECOLLIDER_H_
#define _CAPSULECOLLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLLI_USE_VTX	4

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CCapsuleCollider : public CScene
{
public:
	// �񋓌^��`
	typedef enum
	{
		TYPEID_NOEN = -1,	// ����
		COLLIPARTS_BODY,	// ��
		TYPEID_FOREARM_R,	// �E�O�r
		TYPEID_UPPERARM_R,	// �E��r
		TYPEID_FOREARM_L,	// �O�r
		TYPEID_UPPERARM_L,	// ��r
		TYPEID_MAX			// �ő吔
	}TYPEID;

	// �\���̒�`
	typedef struct
	{
		LPDIRECT3DVERTEXBUFFER9		pVtexBuff;			// ���_�o�b�t�@�̃|�C���^
		LPDIRECT3DINDEXBUFFER9		pIdxBuff;			// �C���f�b�N�X�̃o�b�t�@�̃|�C���^
		int							nSlices;			// ���𒆐S�Ƃ���������
		int							nStacks_1_2;		// ���������̕�����
		int							nNumVertex;			// ���_��
		int							nNumindex;			// �C���f�b�N�X��
		int							nNumPolygon;		// �|���S����
		D3DXVECTOR3					TopTransVec;		// ��ʂ܂ł̒����ƌ���
		D3DXVECTOR3					BottomTransVec;		// ��ʂ܂ł̒����ƌ���
		CAPSULE						Capsule;			// �J�v�Z�����
		TYPEID						enmTtpeID;			// �^�C�vID
		TRANSFORM					trans;				// �g�����X���
		D3DXMATRIX*					pMtxParent;			// �e�̃}�g���b�N�X�|�C���^
		CScene*						pScene;				// �e�̃V�[���|�C���^
	}CAPSULECOLLIDERINFO;

	// �V�����_�[�̑傫����ݒ肷����
	typedef struct
	{
		float fTop;		// ��ʂ܂ł̃T�C�Y
		float fBottom;	// ��ʂ܂ł̃T�C�Y
		float fRadius;	// ���a
	} SIZE_INFO;

	// �ǂ݂��񂾃t�@�C���̒P�̂̏��
	typedef struct
	{
		SIZE_INFO	*pSizeInfo;		// �T�C�Y���̃|�C���^
		D3DXVECTOR3	*pDifference;	// �����̃|�C���^
		int			nColliderType;	// �Փˎ��
	} READINFOFILE_CELL;

	// �ǂ݂��񂾃t�@�C���̑S�̏��
	typedef struct
	{
		int				   nNumReadInfo;			// �ǂ݂��ޏ��̌�
		int*			   pSetThisID;				// ����������ID�̃|�C���^
		READINFOFILE_CELL* pCell;					// �P�̏��|�C���^
	} READINFOFILEBUFFER;
	

	// �R���X�g���N�^
	CCapsuleCollider();
	// �f�X�g���N�^
	~CCapsuleCollider() {}


	// �ǂݍ���
	static HRESULT Load(void);
	// �J��
	static void Unload(void);

	void Init(void);																		// ������
	void Uninit(void);																		// �I��
	void Update(void);																		// �X�V
	void Draw(void);																		// �`��
	void SetMtxParent(D3DXMATRIX *pMtxParent);												// �e�̃}�g���b�N�X�|�C���^�̐ݒ�
	void SetScene(CScene *pScene);															// �V�[���̐ݒ�

	static CCapsuleCollider *Create(CScene *pScene,D3DXMATRIX *pMtxParent, int nTypeID);	// ����
	void InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 &diff);	// ���̐ݒ�Ɠ���
	void InitCapsInfo(void);																// �J�v�Z�����̏�����
	bool Collision(void);																	// �Փ˔���
	bool CollisionStone(void);																// �X�g�[���Ƃ̏Փ˔���

	void CalCapPosition(void);																// �J�v�Z���ʒu�̌v�Z

	inline void SeteumTypeID(const int &nTypeID) { m_ColliderInfo.enmTtpeID = (TYPEID)nTypeID;}

private:
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);		// ���_�̍쐬
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);		// �C���f�b�N�X�̍쐬

	static READINFOFILEBUFFER	m_ReadInfoFileBuff;	// �ǂ݂��񂾃t�@�C�����
	CAPSULECOLLIDERINFO m_ColliderInfo;				// ���
};

#endif