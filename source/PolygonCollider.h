//*************************************************************************************************************
//
// �|���S���R���C�_�[����[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _POLYGONCOLLIDER_H_
#define _POLYGONCOLLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"
#include "kananlibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLLI_USE_VTX	4

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CPolygonCollider
{
public:
	enum
	{
		MAP_TYPE_NONE = -1,	// ����
		MAP_TYPE_FIELD,		// �L��
		MAP_TYPE_PARK,		// ����
		MAP_TYPE_MAX		// �ő�
	};

	typedef struct VTXPOS
	{
		D3DXVECTOR3 vtx_0;			// ���_0
		D3DXVECTOR3 vtx_1;			// ���_1
		D3DXVECTOR3 vtx_2;			// ���_2
		D3DXVECTOR3 vtx_3;			// ���_3
	}VTXPOS;
	
	typedef struct MAP_POLYCOLII
	{
		VTXPOS *pFieldVtxPos;		// �t�B�[���h�̒��_���W
	} MAP_POLYCOLII;

	typedef struct
	{	// �|���S�����
		VTXPOS		vtxPos;				// ���_���W
		int			nPolyID;			// �|���S����ID
		MODELINFO	modelInfo;			// ���f�����
		D3DXVECTOR3 pos;				// �|���S���̍��W
	} POLGONINFO;

	typedef struct
	{	// �|���S���R���C�_�[���
		POLGONINFO		*polyInfo;			// �|���S�����	
		int				 nNumPolygon;		// �|���S����
	} POLYCOLLINFO;

	CPolygonCollider() {}											// �R���X�g���N�^
	~CPolygonCollider() {}											// �f�X�g���N�^

	static HRESULT Load(void);										// ���[�h
	static void Unload(void);										// �A�����[�h
	static CPolygonCollider* Create(int nStageType, int nIndex);	// ����

	void Init(void);												// ������
	void Uninit(void);												// �I��
	void Update(void) {};											// �X�V
	void Draw(void);												// �`��

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection);		// �Փ˔���

	bool Test3DInsidePolygon(D3DXVECTOR3 *pPos);				// �|���S���̒��ɂ��邩�e�X�g

	static int GetNumPoly(int nStage) { return m_polyCollInfo[nStage].nNumPolygon; }	// �X�e�[�W�̃|���S�����̎擾

private:
	static HRESULT LoadFileName(void);	// �t�@�C�����̃��[�h
	static HRESULT LoadModelFile(void);	// ���f�����̃��[�h
	void SetVtxPos(VTXPOS &pVertexsPos);													// ���_�ʒu�̐ݒ�
	HRESULT MakeVertex(void);																// ���_���̍쐬
	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuff;													// �o�b�t�@
	VTXPOS					 m_VtxPos;														// ���_�ʒu
	D3DXVECTOR3              m_SurfaceNor;													// �ʂ̖@��
	static POLYCOLLINFO		 m_polyCollInfo[MAP_TYPE_MAX];									// �|���S���R���C�_�[���
};

#endif