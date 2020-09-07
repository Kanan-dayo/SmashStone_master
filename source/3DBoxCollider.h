//*************************************************************************************************************
//
//�{�b�N�X�R���C�_�[����[3DBoxCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _3DBOXCOLLIDER_H_
#define _3DBOXCOLLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define _3DBOXCOLLIDER_MAX				128		// �{�b�N�X�R���C�_�[�̌�
#define _3DBOXCOLLIDER_NUM_INDEX		24
#define _3DBOXCOLLIDER_NUM_PRIMITIVE	12
#define _3DBOXCOLLIDER_USE_VERTEX		8
#define _3DBOXCOLLIDER_NUM_SURFACE		6


//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C3DBoxCollider :public CScene
{
public:
	enum
	{
		SURFACE_TYPE_TOP =0,	// ��
		SURFACE_TYPE_FRONT,		// �O
		SURFACE_TYPE_REAR,		// ���
		SURFACE_TYPE_RIGHT,		// �E
		SURFACE_TYPE_LEFT,		// ��
		SURFACE_TYPE_BOTTOM,	// ��
		SURFACE_TYPE_MAX		// �ő�
	};

	enum
	{
		ID_NOEN = -1,			// ����
		ID_CHARACTER,			// �L�����N�^�[
		ID_STONE,				// �X�g�[��

		ID_FLOWER,				// �A�ؔ�
		ID_ROOF,				// ����
		ID_STAIRS,				// �K�i
		ID_WOODENBOX,			// �؂̔�
		ID_LOOOSESTAIRS,		// �ɂ��K�i
		ID_FOUNTAIN_DOWN,		// �����̉�
		ID_FOUNTAIN_W_UP,		// �����̐��������̕���
		ID_FOUNTAIN_IRON,		// �S

		ID_MAX					// �ő吔
	};

	enum
	{
		TOP_OBJECT = ID_FLOWER,	// �I�u�W�F�N�g�̐擪ID
	};

	typedef enum
	{
		COLLIDER_TYPE_NORMAL = 0,			// �ʏ�
		COLLIDER_TYPE_CYLINDER,				// �~��
		COLLIDER_TYPE_SPHERE,				// ����
		COLLIDER_TYPE_CYLINDER_CANRIDE,		// �~���̏�����
		COLLIDER_TYPE_MAX					// �ő�
	} COLLIDER_TYPE;

	typedef enum
	{
		COLLIDER_SUB_NORMAL = 0,	// �ʏ�
		COLLIDER_SUB_OVERRAP,		// �d�Ȃ��Ă��邩����
		COLLIDER_SUB_MAX			// �ő�
	} COLLIDER_SUBTYPE;

	typedef struct
	{
		D3DXVECTOR3			difference;		// ����
		D3DXVECTOR3			size;			// �傫��
		D3DXVECTOR3			pos;			// �ʒu
		D3DXVECTOR3			posOld;			// �O��̈ʒu
		D3DXVECTOR3			Vertex[_3DBOXCOLLIDER_USE_VERTEX];	// ���_�ʒu
		D3DXVECTOR3			rot;			// ��]
		D3DXMATRIX			mtxWorld;		// ���[���h�}�g���b�N�X
		D3DXVECTOR3			surfaceNom[_3DBOXCOLLIDER_NUM_SURFACE];	// �ʂ̖@��
		bool				bUse;			// �g�p�t���O
		COLLIDER_TYPE		ColliderType;	// �Փˎ��
		COLLIDER_SUBTYPE	ColliderSubType;// �Փ˃T�u�^�C�v
		CScene				*pScene;		// �Ȃɂ��̂����ʂ��邽��
	} _3DBOXCOLLIDER;

	// �ǂ݂��񂾃t�@�C���̒P�̂̏��
	typedef struct
	{
		D3DXVECTOR3	*pSize;			// �傫���̃|�C���^
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
	C3DBoxCollider(CScene::PRIORITY priority);
	// �ǂݍ���
	static HRESULT Load(void);
	// �J��
	static void Unload(void);
	// ����
	static C3DBoxCollider*Create(void);
	// ������
	void Init(void);
	// �I��
	void Uninit(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);
	// �Փ˔���
	static bool Collision(D3DXVECTOR3 &pos);
	// �_���[�W���蔻��
	static bool CollisionDamage(int n3DBoxColliderID, int nNoneColisitionID = -1);
	// �Փ˂�������������
	static bool Collisionoverlap(int n3DBoxColliderID,int *pHitID, int nNoneColisitionID = -1);
	// �u���b�N��̏Փ˔���
	static bool CollisionBox(int n3DBoxColliderID, D3DXVECTOR3 &pos, D3DXVECTOR3 &move, D3DXVECTOR3 *pOut_intersect = NULL, D3DXVECTOR3 *pOut_nor = NULL, bool bReflection = false);
	// �ݒ�
	static int Set(D3DXVECTOR3 &size, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &difference, COLLIDER_TYPE colliType, COLLIDER_SUBTYPE ColliderSubType, CScene * pScene);
	// �V�[���̃|�C���^��ݒ�
	static void SetScene(int n3DBoxColliderID, CScene *pScene = NULL);
	// �V�[���̃|�C���^���擾
	static CScene *GetScene(int n3DBoxColliderID);

	// ���̕ύX
	static void ChangeInfo(int nIndex, D3DXVECTOR3 &size, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
	// ���̕ύX
	static void ChangeInfo(int nIndex, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
	// �ʒu�̕ύX
	static void ChangePosition(int nID,D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
	// �ʂ̖@���̐ݒ�
	static void SetSurfaceNom(VERTEX_3D *pVtx, int nIndex);
	static void SetVertexPosResult(int nIndex);
	// �v�Z��̒��_�ʒu�̐ݒ�
	static void SetVertexPosResult(VERTEX_3D *pVtx, int nIndex);
	// �R���C�_�[���̎擾
	static _3DBOXCOLLIDER *GetInfo(int nIndex = 0);
	// �ݒ����
	static void unset(int nID);
	// ���̏�����
	void InitColliderInfo(void);
	// �R���C�_�[���̓ǂݍ���
	//static int LoadColliderLoad(TRANSFORM *trans, int nID = -1, D3DXVECTOR3 *vtxMax = NULL, D3DXVECTOR3 *vtxMin = NULL, bool bLoad = true, COLLIDER_TYPE Collidertype = COLLIDER_TYPE::COLLIDER_TYPE_OVERRAP);
	// �R���C�_�[����ݒ肷��
	static int SetColliderInfo(D3DXVECTOR3 *pPos, CScene *pScene, COLLIDER_SUBTYPE ColliderSubType = COLLIDER_SUB_NORMAL, int nID = ID_CHARACTER);
	// �C�ӂ�ID���R���C�_�[ID�ɕϊ�
	static void ConvertColliderID(int *pID);
	// �����蔻���̍����擾
	static float GetHeight(void) { return m_fHeight; }
private:
	/* �����o�֐� */
	// ���_�̍쐬
	HRESULT MakeVertex(void);
	// �C���f�b�N�X�̍쐬
	HRESULT MakeIndex(void);
	// ���_�ʒu�̐ݒ�
	static void SetVertexPosition(VERTEX_3D *pVtx,int nIndex);
	/* �����o�ϐ� */
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// ���_�o�b�t�@�ւ̃|�C���^
	static LPDIRECT3DTEXTURE9		m_pTexture;							// �e�N�X�`�����̃|�C���^
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;							// �C���f�b�N�X�̃o�b�t�@�̃|�C���^
	static READINFOFILEBUFFER		m_ReadInfoFileBuff;					// �ǂ݂��񂾃t�@�C�����
	static _3DBOXCOLLIDER			m_ColliderInfo[_3DBOXCOLLIDER_MAX];	// �R���C�_�[���
	static float m_fHeight;												// �����蔻���̍���
};
#endif
