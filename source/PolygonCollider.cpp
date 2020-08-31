//*************************************************************************************************************
//
// �|���S���R���C�_�[����[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "PolygonCollider.h"
#include "renderer.h"
#include "debugProc.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLLI_COL	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)

#define POLYCOLLI_STAIRS_POS		D3DXVECTOR3(-15.5f,37.0f,-48.0f)

#define POLYCOLLI_STAIRS_HALFSIZE_X 51.5f
#define POLYCOLLI_STAIRS_HALFSIZE_Y 23.0f
#define POLYCOLLI_STAIRS_HALFSIZE_Z 32.0f

#define POLYCOLLI_STAIRS_TOP_POS	D3DXVECTOR3(64.5f,23.0f,0.0f)+POLYCOLLI_STAIRS_POS

#define POLYCOLLI_STAIRS_TOP_HALFSIZE_X 13.0f
#define POLYCOLLI_STAIRS_TOP_HALFSIZE_Z POLYCOLLI_STAIRS_HALFSIZE_Z

#define POLYCOLLI_LONGSTAIRS_POS	D3DXVECTOR3(-60.0f,10.0f,-111.0f)

#define POLYCOLLI_LONGSTAIRS_HALFSIZE_X 140.0f
#define POLYCOLLI_LONGSTAIRS_HALFSIZE_Y 13.0f
#define POLYCOLLI_LONGSTAIRS_HALFSIZE_Z 20.0f

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CPolygonCollider::m_conSetingVtxPos[MAP_TYPE_MAX][POLYCOLLI_MAX][POLYCOLLI_USE_VTX] = 	// �ݒ�p���_�ʒu
{
	{// �L��
		{// �K�i
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X,POLYCOLLI_STAIRS_HALFSIZE_Y,-POLYCOLLI_STAIRS_HALFSIZE_Z),
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X,-POLYCOLLI_STAIRS_HALFSIZE_Y,-POLYCOLLI_STAIRS_HALFSIZE_Z),
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X,POLYCOLLI_STAIRS_HALFSIZE_Y,POLYCOLLI_STAIRS_HALFSIZE_Z),
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X,-POLYCOLLI_STAIRS_HALFSIZE_Y,POLYCOLLI_STAIRS_HALFSIZE_Z),
		},
		{// �K�i�̏㕔
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,-POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,-POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),

		},
		{// �ɂ₩�ȊK�i
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
		},
		{// �ɂ₩�ȊK�i�̏㕔
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
		},
	},
};

CPolygonCollider::MAP_POLYCOLII CPolygonCollider::m_PolyPos[MAP_TYPE_MAX] = { NULL };

void CPolygonCollider::CreateDateBase(void)
{
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos = new VTXPOS[POLYCOLLI_MAX];

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_0 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X, POLYCOLLI_STAIRS_HALFSIZE_Y, -POLYCOLLI_STAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_1 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X, -POLYCOLLI_STAIRS_HALFSIZE_Y, -POLYCOLLI_STAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_2 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X, POLYCOLLI_STAIRS_HALFSIZE_Y, POLYCOLLI_STAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_3 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X, -POLYCOLLI_STAIRS_HALFSIZE_Y, POLYCOLLI_STAIRS_HALFSIZE_Z);

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_0 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, -POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_1 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, -POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_2 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_3 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_0 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_1 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_2 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_3 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_0 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_1 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_2 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_3 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);


}

void CPolygonCollider::DeleteDateBase(void)
{
	for (int nCntDate = 0; nCntDate < MAP_TYPE_MAX;nCntDate++)
	{
		if (m_PolyPos[nCntDate].pFieldVtxPos == NULL)
		{
			continue;
		}
		delete []m_PolyPos[nCntDate].pFieldVtxPos;
		m_PolyPos[nCntDate].pFieldVtxPos = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CPolygonCollider * CPolygonCollider::Create(int nType)
{
	// ����
	CPolygonCollider *pCPolyColli = new CPolygonCollider;
	// ���_�ʒu�̐ݒ�
	pCPolyColli->SetVtxPos(&m_conSetingVtxPos[0][nType][0]);
	// ������
	pCPolyColli->Init();
	return pCPolyColli;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Init(void)
{
	// �ϐ��錾
	D3DXVECTOR3 vecA = MYLIB_VEC3_UNSET;				// A�x�N�g��
	D3DXVECTOR3 vecB = MYLIB_VEC3_UNSET;				// B�x�N�g��

	// �@���x�N�g�������߂�
	vecA = m_VtxPos[1] - m_VtxPos[2];					// A�x�N�g���Z�o
	vecB = m_VtxPos[0] - m_VtxPos[1];					// B�x�N�g���Z�o
	D3DXVec3Cross(&m_SurfaceNor, &vecA, &vecB);			// �����x�N�g���Z�o
	D3DXVec3Normalize(&m_SurfaceNor, &m_SurfaceNor);	// ���K������

	// ���_���̍쐬
	MakeVertex();
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Uninit(void)
{
	// ���_�o�b�t�@��NULL����Ȃ�������
	if (m_pVtxBuff != NULL)
	{
		// ���_�o�b�t�@�̊J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Draw(void)
{
	// �ϐ��錾
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾
	D3DXMATRIX mtxWorld;									// �v�Z�p�}�g���b�N�X

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ���C�e�B���O���[�h����

	// Fill Mode �̐ݒ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

#ifdef  _DEBUG
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
#endif

	// Fill Mode �̐ݒ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h����


}

//-------------------------------------------------------------------------------------------------------------
// �Փ˔���
//-------------------------------------------------------------------------------------------------------------
bool CPolygonCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection)
{
	if (Test3DInsidePolygon(pPos))
	{
#ifdef _DEBUG
		CDebugProc::Print("���ɂ���\n");
#endif
		if (bReflection == true)
		{
			pPos->y = m_VtxPos[2].y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos[2].x) -
					m_SurfaceNor.z * (pPos->z - m_VtxPos[2].z)) / m_SurfaceNor.y;
			*pOut_Intersect = *pPos;
			*SurfaceNor = m_SurfaceNor;
		}
		else
		{
			pPos->y = m_VtxPos[2].y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos[2].x) -
				m_SurfaceNor.z * (pPos->z - m_VtxPos[2].z))	/ m_SurfaceNor.y;
		}
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// �|���S���̒��ɂ��邩�e�X�g
//-------------------------------------------------------------------------------------------------------------
bool CPolygonCollider::Test3DInsidePolygon(D3DXVECTOR3 * pPos)
{
	if (CMylibrary::Test3DInsidePolygon(m_VtxPos[0], m_VtxPos[1], m_VtxPos[2], *pPos, -m_SurfaceNor))
	{
		return true;
	}
	else if(CMylibrary::Test3DInsidePolygon(m_VtxPos[1], m_VtxPos[3], m_VtxPos[2], *pPos, -m_SurfaceNor))
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// ���_�ʒu�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::SetVtxPos(D3DXVECTOR3 * pVertexsPos)
{
	for (int nCntVtx = 0; nCntVtx < POLYCOLLI_USE_VTX; nCntVtx++)
	{
		m_VtxPos[nCntVtx] = pVertexsPos[nCntVtx];
	}
}

//-------------------------------------------------------------------------------------------------------------
// ���_�̍쐬
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::MakeVertex(void)
{
	// �ϐ��錾
	CRenderer *pRenderer = CManager::GetRenderer();					// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();				// �f�o�C�X�̎擾

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYCOLLI_USE_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���W�̐ݒ�
	pVtx[0].pos = m_VtxPos[0];
	pVtx[1].pos = m_VtxPos[1];
	pVtx[2].pos = m_VtxPos[2];
	pVtx[3].pos = m_VtxPos[3];

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = m_SurfaceNor;
	pVtx[1].nor = m_SurfaceNor;
	pVtx[2].nor = m_SurfaceNor;
	pVtx[3].nor = m_SurfaceNor;

	// ���_�J���[
	pVtx[0].col = POLYCOLLI_COL;
	pVtx[1].col = POLYCOLLI_COL;
	pVtx[2].col = POLYCOLLI_COL;
	pVtx[3].col = POLYCOLLI_COL;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();

	return E_NOTIMPL;
}
