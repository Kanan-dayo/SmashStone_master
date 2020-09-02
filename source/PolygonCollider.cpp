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
#define POLYCOLL_FILENAME ("data/COLLIDER/Polygon.csv")

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
CPolygonCollider::POLYCOLLINFO	CPolygonCollider::m_polyCollInfo[MAP_TYPE_MAX] = {};

//-------------------------------------------------------------------------------------------------------------
// ���[�h
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::Load(void)
{
	// �t�@�C�����̃��[�h
	if (FAILED(LoadFileName()))
		return E_FAIL;
	// ���f���̃��[�h
	if (FAILED(LoadModelFile()))
		return E_FAIL;

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �t�@�C�����̃��[�h
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::LoadFileName(void)
{
	// �u���b�N�R�����g
	CKananLibrary::StartBlockComment("�|���S���R���C�_�[�̃t�@�C���ǂݍ��݊J�n");
	DWORD start = timeGetTime();		// �v���X�^�[�g����

	// �t�@�C���I�[�v��
	FILE *pFile = fopen(POLYCOLL_FILENAME, "r");

	// nullcheck
	if (!pFile)
	{
		// ���s
		CKananLibrary::EndBlockComment("�t�@�C���ǂݍ��ݎ��s");
		return E_FAIL;
	}

	// �e�L�X�g�ۑ��p
	char cHeadText[MAX_TEXT] = {};
	char cReadText[MAX_TEXT] = {};
	char cDieText[MAX_TEXT] = {};
	
	// �X�N���v�g������܂ŌJ��Ԃ�
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(cReadText, sizeof(cReadText), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cReadText, "%[^,]s", &cHeadText);
	}

	// �X�e�[�W�J�E���^
	int nCntStage = 0;
	// �|���S���J�E���^
	int nCntPolygon = 0;

	// END_SCRIPT�܂Ń��[�v
	while (strcmp(cHeadText, "END_SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(cReadText, sizeof(cReadText), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cReadText, "%[^,]s", &cHeadText);

		if (nCntStage < MAP_TYPE_MAX)
		{
			// SET_STAGE������܂ŌJ��Ԃ�
			while (strcmp(cHeadText, "SET_STAGE") != 0)
			{
				// 1�s�ǂݍ���
				fgets(cReadText, sizeof(cReadText), pFile);
				// �ǂݍ���Ǖ�������
				sscanf(cReadText, "%[^,]s", &cHeadText);
				// �ǂݍ��񂾃|���S������������
				nCntPolygon = 0;
			}

			// END_SETSTAGE������܂Ń��[�v
			while (strcmp(cHeadText, "END_SETSTAGE") != 0)
			{
				// 1�s�ǂݍ���
				fgets(cReadText, sizeof(cReadText), pFile);
				char *pRead = &cReadText[0];
				while (*pRead == ',')
				{
					pRead++;
				}

				// �ǂݍ���Ǖ�������
				sscanf(pRead, "%[^,]s", &cHeadText);

				// �ݒ肷���
				if (strcmp(cHeadText, "SET_NUM") == 0)
				{
					// �����擾���A�������m��
					sscanf(pRead, "%[^,],%d", &cDieText, &m_polyCollInfo[nCntStage].nNumPolygon);
					m_polyCollInfo[nCntStage].polyInfo = new POLGONINFO[m_polyCollInfo[nCntStage].nNumPolygon];
				}
				// �ݒ肷�����
				if (strcmp(cHeadText, "SET") == 0)
				{
					while (*pRead == '"')
					{
						pRead++;
					}

					/* ��s���̏�����͂��� */
					//			   SET     ����   ID   �p�X
					sscanf(pRead, "%[^, ], %[^, ], %d, %f, %f, %f, %s",
						&cDieText, &cDieText,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].nPolyID,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].pos.x,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].pos.y,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].pos.z,
						&m_polyCollInfo[nCntStage].polyInfo[nCntPolygon].modelInfo.cModelName);

					// ���J�E���g�C���N�������g
					nCntPolygon++;
				}
			}

			if (strcmp(cHeadText, "END_SETSTAGE") == 0)
			{
				// �X�e�[�W�������Z
				nCntStage++;
			}
		}
	}

	// �u���b�N�R�����g
	DWORD end = timeGetTime();			// �v���X�^�[�g����
#ifdef _DEBUG
	cout << "�ǂݍ��ݏI��" << endl;
	cout << "�ǂݍ��� �������x = " << (end - start) << "�@[" << (end - start) * 0.001f << "sec.]" << endl;
#endif
	CKananLibrary::PrintBlockCommentFrame();
	// �I��
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// ���f���̃��[�h
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::LoadModelFile(void)
{
	for (int nCntStage = 0; nCntStage < MAP_TYPE_MAX; nCntStage++)
	{
		// ��ސ�����
		for (int nCntPoly = 0; nCntPoly < m_polyCollInfo[nCntStage].nNumPolygon; nCntPoly++)
		{
			// ���f�����̐���
			CKananLibrary::CreateModelInfo(&m_polyCollInfo[nCntStage].polyInfo[nCntPoly].modelInfo);

			// ���b�V�����i�[
			LPD3DXMESH mesh = m_polyCollInfo[nCntStage].polyInfo[nCntPoly].modelInfo.mesh;

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			DWORD sizeFVF = D3DXGetFVFVertexSize(mesh->GetFVF());
			// ���_�o�b�t�@�̃|�C���^
			BYTE  *pVertexBuffer;

			// ���_�o�b�t�@�����b�N
			mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

			// �o�b�t�@���璸�_���W���擾
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_2 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// �T�C�Y���|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
			// �o�b�t�@���璸�_���W���擾
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_0 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// �T�C�Y���|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
			// �o�b�t�@���璸�_���W���擾
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_1 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// �T�C�Y���|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
			// �o�b�t�@���璸�_���W���擾
			m_polyCollInfo[nCntStage].polyInfo[nCntPoly].vtxPos.vtx_3 = *(D3DXVECTOR3*)pVertexBuffer + m_polyCollInfo[nCntStage].polyInfo[nCntPoly].pos;
			// �T�C�Y���|�C���^��i�߂�
			pVertexBuffer += sizeFVF;

			// ���_�o�b�t�@���A�����b�N
			mesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �A�����[�h
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Unload(void)
{
	for (int nCntStage = 0; nCntStage < MAP_TYPE_MAX; nCntStage++)
	{
		for (int nCntPoly = 0; nCntPoly < m_polyCollInfo[nCntStage].nNumPolygon; nCntPoly++)
		{
			if (m_polyCollInfo[nCntStage].polyInfo)
			{
				delete[] m_polyCollInfo[nCntStage].polyInfo;
				m_polyCollInfo[nCntStage].polyInfo = nullptr;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CPolygonCollider * CPolygonCollider::Create(int nStageType, int nIndex)
{
	// ����
	CPolygonCollider *pCPolyColli = new CPolygonCollider;
	// ���_�ʒu�̐ݒ�
	pCPolyColli->SetVtxPos(m_polyCollInfo[nStageType].polyInfo[nIndex].vtxPos);
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
	vecA = m_VtxPos.vtx_1 - m_VtxPos.vtx_2;					// A�x�N�g���Z�o
	vecB = m_VtxPos.vtx_0 - m_VtxPos.vtx_1;					// B�x�N�g���Z�o
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
	// �������̂�
	if (CRenderer::GetbDisColl())
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
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
			pPos->y = m_VtxPos.vtx_2.y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos.vtx_2.x) -
					m_SurfaceNor.z * (pPos->z - m_VtxPos.vtx_2.z)) / m_SurfaceNor.y;
			*pOut_Intersect = *pPos;
			*SurfaceNor = m_SurfaceNor;
		}
		else
		{
			pPos->y = m_VtxPos.vtx_2.y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos.vtx_2.x) -
				m_SurfaceNor.z * (pPos->z - m_VtxPos.vtx_2.z))	/ m_SurfaceNor.y;
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
	if (CMylibrary::Test3DInsidePolygon(m_VtxPos.vtx_0, m_VtxPos.vtx_1, m_VtxPos.vtx_2, *pPos, -m_SurfaceNor))
	{
		return true;
	}
	else if(CMylibrary::Test3DInsidePolygon(m_VtxPos.vtx_1, m_VtxPos.vtx_3, m_VtxPos.vtx_2, *pPos, -m_SurfaceNor))
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// ���_�ʒu�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::SetVtxPos(VTXPOS &pVertexsPos)
{
	m_VtxPos = pVertexsPos;
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
	pVtx[0].pos = m_VtxPos.vtx_0;
	pVtx[1].pos = m_VtxPos.vtx_1;
	pVtx[2].pos = m_VtxPos.vtx_2;
	pVtx[3].pos = m_VtxPos.vtx_3;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = m_SurfaceNor;
	pVtx[1].nor = m_SurfaceNor;
	pVtx[2].nor = m_SurfaceNor;
	pVtx[3].nor = m_SurfaceNor;

	// ���_�J���[
	pVtx[0].col = WhiteColor;
	pVtx[1].col = WhiteColor;
	pVtx[2].col = WhiteColor;
	pVtx[3].col = WhiteColor;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();

	return E_NOTIMPL;
}
