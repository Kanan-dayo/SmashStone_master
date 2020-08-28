//*************************************************************************************************************
//
// �V�����_�[�R���C�_�[����[CylinderCoillider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CYLINDERCOILLIDER_H_
#define _CYLINDERCOILLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C3DBoxCollider;
class CModel;
class CCylinderCoillider :public CScene
{
public:
	typedef enum
	{
		TYPEID_NOEN = -1,	// ����
		TYPEID_FOREARM_R,	// �E�O�r
		TYPEID_UPPERARM_R,	// �E��r
		TYPEID_FOREARM_L,	// �O�r
		TYPEID_UPPERARM_L,	// ��r
		TYPEID_MAX			// �ő吔
	}TYPEID;

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
	CCylinderCoillider();
	// �f�X�g���N�^
	~CCylinderCoillider();

	// �ǂݍ���
	static HRESULT Load(void);
	// �J��
	static void Unload(void);
	// ����
	static CCylinderCoillider *Create(TYPEID enmTypeId, D3DXMATRIX *pMtxWorld);
	// ������
	void Init(void);
	// �I��
	void Uninit(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);
	// ���̐ݒ�Ɠ���
	void InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 &diff);
	// �e�̐ݒ�
	void SetPareMtxWorld(D3DXMATRIX* pPareMtxWorld);
	// ���[���h�}�g���b�N�X�̎擾
	D3DXMATRIX *GetMtxWorld(void);
	// ���a�̎擾
	float GetRadius(void);
	// �ő�_�̎擾
	D3DXVECTOR3 *GetTopPoint(void);
	// �ŏ��_�̎擾
	D3DXVECTOR3 *GetBottomPoint(void);
	// ��ʂ̖@����1�ӂ̃x�N�g���̎Z�o
	void CalculateBottomNormal(void);
	// ��ʂ̖@���̎擾
	D3DXVECTOR3 *GetBottomNormal(void);
	// �^�C�vID�̐ݒ�
	void SetTypeID(TYPEID enmTypeID);

	// �Փ�
	bool Collision(int n3DBoxColliderID);
	// �{�b�N�X���ɂ��邩�e�X�g
	bool InsideBoxTest(int n3DBoxColliderID , D3DXVECTOR3 *pPos);

private:
	/* �����o�֐� */
	// ���_�̍쐬
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	// �C���f�b�N�X�̍쐬
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);
	/* �����o�ϐ� */
	static READINFOFILEBUFFER	m_ReadInfoFileBuff;		// �ǂ݂��񂾃t�@�C�����
	LPDIRECT3DVERTEXBUFFER9		m_pVtexBuff;			// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// �C���f�b�N�X�̃o�b�t�@�̃|�C���^
	D3DXMATRIX					m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXMATRIX*					m_pPareMtxWorld;		// �e�̃��[���h�}�g���b�N�X�|�C���^
	float						m_fRadius;				// ���a
	D3DXVECTOR3					m_Difference;			// ����
	D3DXVECTOR3					m_TopTransVec;			// ��ʂ܂ł̒����ƌ���
	D3DXVECTOR3					m_BottomTransVec;		// ��ʂ܂ł̒����ƌ���
	int							m_nNumVertex;			// �����_��
	int							m_nNumIndex;			// ���C���f�b�N�X��
	int							m_nNumPolygon;			// ���|���S����
	D3DXVECTOR3					m_TopPoint;				// ��ʈʒu
	D3DXVECTOR3					m_BottomPoint;			// ��ʈʒu
	D3DXVECTOR3					m_BottomNormal;			// ��ʂ̖@��
	TYPEID						m_enmTtpeID;			// �^�C�vID

protected:
};

#endif