//*************************************************************************************************************
//
// �V�����_�[�R���C�_�[����[CylinderCoillider.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "CylinderCollider.h"
#include "DebugProc.h"
#include "renderer.h"
#include "3DBoxCollider.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define CYLICOILLI_NUMSURFACEH			(6)		// ���̌�
#define CYLICOILLI_NUMSURFACEV			(1)		// �c�̌�
#define CYLICOILLI_BOTTOMUNITVEC_NUMVTX	(3)		// ��ʂ̒P�ʃx�N�g���v�Z�ɕK�v�Ȓ��_��

#define CYLICOILLI_FILENAME						"data/TEXT/ColliInfo/Cylinder.csv"	// �t�@�C����
#define CYLICOILLI_OPENMODE						"r"		// �t�@�C���̊J�����[�h

#define CYLICOILLI_WORDSIZE						16		// ���[�h�T�C�Y

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider::READINFOFILEBUFFER	CCylinderCoillider::m_ReadInfoFileBuff;		// �ǂ݂��񂾃t�@�C�����

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider::CCylinderCoillider() :CScene(CScene::PRIORITY_COLLISION)
{
	// ���ꂼ��̑���������
	m_nNumVertex     = (CYLICOILLI_NUMSURFACEH + 1)*(CYLICOILLI_NUMSURFACEV + 1);
	m_nNumIndex      = ((CYLICOILLI_NUMSURFACEH * 2) + 2) * CYLICOILLI_NUMSURFACEV + 2 * ((CYLICOILLI_NUMSURFACEV - 1));
	m_nNumPolygon    = (CYLICOILLI_NUMSURFACEV * CYLICOILLI_NUMSURFACEH * 2 + 4 * (CYLICOILLI_NUMSURFACEV - 1));

	m_nNumVertex = (CYLICOILLI_NUMSURFACEH + 1)*(CYLICOILLI_NUMSURFACEV + 1);
	m_nNumIndex = ((CYLICOILLI_NUMSURFACEH * 2) + 2) * CYLICOILLI_NUMSURFACEV + 2 * ((CYLICOILLI_NUMSURFACEV - 1));
	m_nNumPolygon = (CYLICOILLI_NUMSURFACEV * CYLICOILLI_NUMSURFACEH * 2 + 4 * (CYLICOILLI_NUMSURFACEV - 1));

	m_TopPoint       = MYLIB_3DVECTOR_ZERO;
	m_BottomPoint    = MYLIB_3DVECTOR_ZERO;
	m_BottomTransVec = MYLIB_3DVECTOR_ZERO;
	m_TopTransVec    = MYLIB_3DVECTOR_ZERO;
	m_Difference     = MYLIB_3DVECTOR_ZERO;
	m_fRadius        = MYLIB_FLOAT_UNSET;
	m_BottomNormal   = MYLIB_3DVECTOR_ZERO;
}

//-------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider::~CCylinderCoillider()
{
	m_pPareMtxWorld = NULL;
}

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
HRESULT CCylinderCoillider::Load(void)
{
	// �ǂ݂��񂾃t�@�C�����̏�����
	m_ReadInfoFileBuff.nNumReadInfo = MYLIB_INT_UNSET;
	m_ReadInfoFileBuff.pCell = NULL;
	m_ReadInfoFileBuff.pSetThisID = NULL;
#ifdef _DEBUG
	cout << "---------------------------------------------------------------------\n";
	cout << "CCylinderCoillider::Load �V�����_�[�R���C�_�[�̓ǂݍ��݊J�n\n";
	DWORD start = timeGetTime();			// �v���X�^�[�g����
#endif
	// �ϐ��錾
	FILE *pFile = fopen(CYLICOILLI_FILENAME, CYLICOILLI_OPENMODE);
	// �J���Ȃ�����
	if (pFile == NULL)
	{
#ifdef _DEBUG
		cout << "CCylinderCoillider::Load �V�����_�[�R���C�_�[�̓ǂݍ��݂̃t�@�C�����J���܂���ł���\n";
		cout << "---------------------------------------------------------------------\n";
#endif
		return E_FAIL;
	}

	// �ϐ��錾
	char aRead[MYLIB_STRINGSIZE];		// �ǂݍ��ݗp
	char aComp[MYLIB_STRINGSIZE];		// ��r�p
	char aEmpty[MYLIB_STRINGSIZE];		// �v��Ȃ����̗p
	int  nCntInfo;						// ���J�E���g

	aRead[0] = MYLIB_CHAR_UNSET;		// �ǂݍ��ݗp
	aComp[0] = MYLIB_CHAR_UNSET;		// ��r�p
	aEmpty[0] = MYLIB_CHAR_UNSET;		// �v��Ȃ����̗p
	nCntInfo = MYLIB_INT_UNSET;			// ���J�E���g

#ifdef _DEBUG
	int  nCntError;						// �G���[�J�E���g
	nCntError = MYLIB_INT_UNSET;		// �G���[�J�E���g
#endif
										// �X�N���v�g������܂łƂ΂�
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%[^,]s", &aComp);
#ifdef _DEBUG
		// �G���[�J�E���g���C���N�������g
		if (++nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
	}
	// END_SCRIPT�܂Ń��[�v
	while (strcmp(aComp, "END_SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%[^,]s", &aComp);
#ifdef _DEBUG
		// �G���[�J�E���g���C���N�������g
		if (++nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
		if (m_ReadInfoFileBuff.nNumReadInfo <= 0)
		{
			// �ݒ肷���
			if (strcmp(aComp, "SET_NUM") == 0)
			{
				sscanf(aRead, "%[^,],%d", &aEmpty, &m_ReadInfoFileBuff.nNumReadInfo);
				// �P�̏�����������
				m_ReadInfoFileBuff.pCell = new READINFOFILE_CELL[m_ReadInfoFileBuff.nNumReadInfo];
				// ����������ID����������
				m_ReadInfoFileBuff.pSetThisID = new int[m_ReadInfoFileBuff.nNumReadInfo];
				// ������̏�����
				aComp[0] = '\0';
				cout << "�ǂ݂��݌����擾���܂���\n";
			}
		}
		else
		{
			// �ݒ肷�����
			if (strcmp(aComp, "SET") == 0)
			{
				// �ϐ��錾
				char aSizeWord[CYLICOILLI_WORDSIZE]	;		// �T�C�Y�̃t���O����
				char aDiffWord[CYLICOILLI_WORDSIZE];		// �����̃t���O����
				READINFOFILE_CELL *pCell;					// �Z���|�C���^

				// ������
				aSizeWord[0] = MYLIB_CHAR_UNSET;
				aDiffWord[0] = MYLIB_CHAR_UNSET;
				pCell = &m_ReadInfoFileBuff.pCell[nCntInfo];

				/* ��s���̏�����͂��� */
				//			   SET     ����   ID  ��傫��               ����                  �^�C�v
				sscanf(aRead, "%[^, ],%[^, ], %d, %[^, ], %[^, ], %[^, ],%[^, ], %[^, ], %[^, ], %d",
					&aEmpty, &aEmpty,
					&m_ReadInfoFileBuff.pSetThisID[nCntInfo],
					&aSizeWord,
					&aEmpty, &aEmpty,
					&aDiffWord,
					&aEmpty, &aEmpty,
					&pCell->nColliderType);
				// �T�C�Y��ݒ肵�Ȃ���
				if (strcmp(aSizeWord, "UNSET") != 0)
				{
					// �T�C�Y���̐���
					pCell->pSizeInfo = new SIZE_INFO;
					//             SET     ����    ID      
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty,
						&pCell->pSizeInfo->fTop,
						&pCell->pSizeInfo->fBottom,
						&pCell->pSizeInfo->fRadius);
				}
				else
				{// ����ȊO�̎�
					pCell->pSizeInfo = NULL;
				}
				// ������ݒ肵�Ȃ���
				if (strcmp(aDiffWord, "UNSET") != 0)
				{
					// �����̐���
					pCell->pDifference = new D3DXVECTOR3;
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty,
						&pCell->pDifference->x,
						&pCell->pDifference->y,
						&pCell->pDifference->z);
				}
				else
				{// ����ȊO�̎�
					pCell->pDifference = NULL;
				}
				// ���J�E���g�C���N�������g
				nCntInfo++;
				// ������̏�����
				aComp[0] = '\0';
#ifdef _DEBUG
				cout << "[" << nCntInfo << "]�ڂ̏����擾���܂���\n";
#endif
			}
		}
	}
	fclose(pFile);
#ifdef _DEBUG
	DWORD end = timeGetTime();			// �v���X�^�[�g����
	cout << "CCylinderCoillider::Load �V�����_�[�R���C�_�[�̓ǂݍ��ݏI��\n";
	cout << " CCylinderCoillider::Load �V�����_�[�R���C�_�[�̓ǂݍ��� �������x = " << (end - start) << "�@[" << (end - start) * 0.001f << "sec.]\n";
#endif
	cout << "---------------------------------------------------------------------\n";
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Unload(void)
{
	// �P�̏��̌������[�v
	for (int nCntCell = 0; nCntCell < m_ReadInfoFileBuff.nNumReadInfo; nCntCell++)
	{
		// �傫���̔j��
		if (m_ReadInfoFileBuff.pCell[nCntCell].pSizeInfo != NULL)
		{
			delete m_ReadInfoFileBuff.pCell[nCntCell].pSizeInfo;
			m_ReadInfoFileBuff.pCell[nCntCell].pSizeInfo = NULL;
		}
		// �����̔j��
		if (m_ReadInfoFileBuff.pCell[nCntCell].pDifference != NULL)
		{
			delete m_ReadInfoFileBuff.pCell[nCntCell].pDifference;
			m_ReadInfoFileBuff.pCell[nCntCell].pDifference = NULL;
		}
	}
	// �P�̏���j��
	if (m_ReadInfoFileBuff.pCell != NULL)
	{
		delete[]m_ReadInfoFileBuff.pCell;
		m_ReadInfoFileBuff.pCell = NULL;
	}
	// ����������ID�̔j��
	if (m_ReadInfoFileBuff.pSetThisID != NULL)
	{
		delete[]m_ReadInfoFileBuff.pSetThisID;
		m_ReadInfoFileBuff.pSetThisID = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider * CCylinderCoillider::Create(TYPEID enmTypeId, D3DXMATRIX *pMtxWorld)
{
	// �ϐ��錾
	CCylinderCoillider *pCylinderCoillider	= new CCylinderCoillider;										// �V�����_�[�R���C�_�[�̃|�C���^
	int nID									= CCylinderCoillider::m_ReadInfoFileBuff.pSetThisID[enmTypeId];	// ID�̎擾
	READINFOFILE_CELL *pCell				= &CCylinderCoillider::m_ReadInfoFileBuff.pCell[nID];			// �Z���̃|�C���^

	// ���̐ݒ�Ɠ���
	pCylinderCoillider->InfoSetSync(
		pCell->pSizeInfo->fRadius,		// ���a
		pCell->pSizeInfo->fTop,			// ��ʂ܂ł̃T�C�Y
		pCell->pSizeInfo->fBottom,		// ��ʂ܂ł̃T�C�Y
		*pCell->pDifference				// ����
	);

	// �e���f���̐ݒ�
	pCylinderCoillider->SetPareMtxWorld(pMtxWorld);

	// �^�C�vID�̐ݒ�
	pCylinderCoillider->SetTypeID(enmTypeId);



	return pCylinderCoillider;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Init(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���_���̍쐬
	MakeVertex(pDevice);

	// �C���f�N�X���̍쐬
	MakeIndex(pDevice);
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Uninit(void)
{
	// ���_�o�b�t�@NULL����Ȃ���
	if (m_pVtexBuff != NULL)
	{
		// ���_�o�b�t�@�̊J��
		m_pVtexBuff->Release();
		m_pVtexBuff = NULL;
	}
	// �C���f�b�N�X�o�b�t�@NULL����Ȃ���
	if (m_pIdxBuff != NULL)
	{
		// �C���f�b�N�X�o�b�t�@�̊J��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Update(void)
{
	//CDebugProc::Print("�e���̃}�g���b�N�X[%.3f][%.3f][%.3f]\n", m_pPareMtxWorld->_41, m_pPareMtxWorld->_42, m_pPareMtxWorld->_43);
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�̃|�C���^
	D3DXMATRIX			mtxRot, mtxTrans;									// �v�Z�p�}�g���b�N�X

	// ���C�e�B���O���[�h����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O���Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �e���f���̏�񂪂���Ƃ�
	if (m_pPareMtxWorld != NULL)
	{
		D3DXMatrixMultiply(&m_mtxWorld
			, &m_mtxWorld
			, m_pPareMtxWorld);
	}


	/* ���[���h���W������W���Z�o */
	// ��ʂ̈ʒu
	D3DXVec3TransformCoord(&m_TopPoint, &m_TopTransVec, &m_mtxWorld);
	// ��ʂ̈ʒu
	D3DXVec3TransformCoord(&m_BottomPoint, &m_BottomTransVec, &m_mtxWorld);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtexBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
#ifdef _DEBUG
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	// �������̂�
	if (CRenderer::GetbDisColl())
	{
		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
	}
#endif
	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// �����_���[�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����

	// ��ʂ̖@����1�ӂ̃x�N�g���̎Z�o
	CalculateBottomNormal();
}

//-------------------------------------------------------------------------------------------------------------
// ���̐ݒ�Ɠ���
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 &diff)
{
	// �ݒ�1
	m_fRadius			= fRadius;		// ���a
	m_TopTransVec.y		= fLengthMax;	// �����ő�
	m_BottomTransVec.y	= fLengthMin;	// �����ŏ�
	m_TopPoint.y		= fLengthMax;	// ��ʂ̈ʒu
	m_BottomPoint.y		= fLengthMin;	// ��ʂ̈ʒu
	m_Difference		= diff;			// ����

	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	MakeVertex(pDevice);

	// �C���f�N�X���̍쐬
	MakeIndex(pDevice);
}

//-------------------------------------------------------------------------------------------------------------
// �e���f���̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::SetPareMtxWorld(D3DXMATRIX* pPareMtxWorld)
{
	m_pPareMtxWorld = pPareMtxWorld;
}

//-------------------------------------------------------------------------------------------------------------
// ���[���h�}�g���b�N�X�̎擾
//-------------------------------------------------------------------------------------------------------------
D3DXMATRIX *CCylinderCoillider::GetMtxWorld(void)
{
	return&m_mtxWorld;
}

//-------------------------------------------------------------------------------------------------------------
// ���a�̎擾
//-------------------------------------------------------------------------------------------------------------
float CCylinderCoillider::GetRadius(void)
{
	return m_fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// �ő�_�̎擾
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCylinderCoillider::GetTopPoint(void)
{
	return &m_TopPoint;
}

//-------------------------------------------------------------------------------------------------------------
// �ŏ��_�̎擾
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCylinderCoillider::GetBottomPoint(void)
{
	return  &m_BottomPoint;
}

//-------------------------------------------------------------------------------------------------------------
// ��ʂ̖@�����Z�o
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::CalculateBottomNormal(void)
{
	// �ϐ��錾
	D3DXVECTOR3 aWorkPos[CYLICOILLI_BOTTOMUNITVEC_NUMVTX];	// �v�Z�p�ʒu���
	VERTEX_3D   *pVtx;												// ���_���̐ݒ�

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// �K�v�Ȓ��_�����[�v	/* ��ʂ���ʂ̒��_ */
	for (int nCntVtx = 0; nCntVtx < CYLICOILLI_BOTTOMUNITVEC_NUMVTX; nCntVtx++)
	{
		// ���_���W���Z�o
		D3DXVec3TransformCoord(&aWorkPos[nCntVtx], &pVtx[0].pos, &m_mtxWorld);
		// ���_�|�C���^��i�߂�
		pVtx++;
	}

	// ���_�f�[�^���A�����b�N
	m_pVtexBuff->Unlock();

	// �O�όv�Z
	D3DXVec3Cross(&m_BottomNormal,
		&(aWorkPos[1] - aWorkPos[0]),
		&(aWorkPos[2] - aWorkPos[0]));
	// �P�ʃx�N�g���̐���
	D3DXVec3Normalize(&m_BottomNormal, &m_BottomNormal);
}

//-------------------------------------------------------------------------------------------------------------
// ��ʂ̖@���x�N�g���̎擾
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCylinderCoillider::GetBottomNormal(void)
{
	return &m_BottomNormal;
}

//-------------------------------------------------------------------------------------------------------------
// �^�C�vID
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::SetTypeID(TYPEID enmTypeID)
{
	m_enmTtpeID = enmTypeID;
}

//-------------------------------------------------------------------------------------------------------------
// �Փ�
//-------------------------------------------------------------------------------------------------------------
bool CCylinderCoillider::Collision(int n3DBoxColliderID)
{
	if (InsideBoxTest(n3DBoxColliderID, &m_TopPoint) == true)
	{
		return true;
	}
	if (InsideBoxTest(n3DBoxColliderID, &m_BottomPoint) == true)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// �{�b�N�X���ɂ��邩�e�X�g
//-------------------------------------------------------------------------------------------------------------
bool CCylinderCoillider::InsideBoxTest(int n3DBoxColliderID, D3DXVECTOR3 *pPos)
{
	// �ϐ��錾
	C3DBoxCollider::_3DBOXCOLLIDER *p3DBoxInfo = C3DBoxCollider::GetInfo(n3DBoxColliderID);
	// �g�p����Ă���Ƃ�
	if (p3DBoxInfo->bUse == false)
	{
		return false;
	}
	// �㑤�̗��ɂ���Ƃ�
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[0], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP]) == false)
	{
		return false;

	}
	// �E���̗��ɂ���Ƃ�
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[1], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_RIGHT]) == false)
	{
		return false;

	}
	// �����̗��ɂ���Ƃ�
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[0], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_LEFT]) == false)
	{
		return false;

	}
	// �O���̗��ɂ���Ƃ�
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[0], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_FRONT]) == false)
	{
		return false;

	}
	// ��둤�̗��ɂ���Ƃ�
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[3], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_REAR]) == false)
	{
		return false;

	}
	// �����̗��ɂ���Ƃ�
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[4], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM]) == false)
	{
		return false;

	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// ���_�̍쐬
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̐ݒ�

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtexBuff,
		NULL);

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtexBuff->Lock(0, 0, (void**)&pVtx, 0); 

	// �p�x�̌v�Z
	float fAngle = ((D3DX_PI * 2) / CYLICOILLI_NUMSURFACEH);

	// ����
	float fLength[CYLICOILLI_NUMSURFACEV + 1] = {
		m_BottomTransVec.y,		// �ŏ�
		m_TopTransVec.y			// �ő�
	};
	m_TopPoint.y    = m_TopTransVec.y;
	m_BottomPoint.y = m_BottomTransVec.y;
	// �c�̎l�p�`�̌���
	for (int nCntVertical = 0; nCntVertical < CYLICOILLI_NUMSURFACEV + 1; nCntVertical++)
	{
		// ���̎l�p�`�̌���
		for (int nCntHeng = 0; nCntHeng < CYLICOILLI_NUMSURFACEH + 1; nCntHeng++)
		{
			pVtx[0].pos.x = sinf(fAngle*nCntHeng)*m_fRadius;
			pVtx[0].pos.y = fLength[nCntVertical];
			pVtx[0].pos.z = cosf(fAngle*nCntHeng)*m_fRadius;
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx++;
		}
	}

	// ���_�f�[�^���A�����b�N
	m_pVtexBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// �C���f�b�N�X�̍쐬
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�o�b�t�@�̃|�C���^�擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	// ���s�̌�
	for (int nCntVertical = 0; nCntVertical < CYLICOILLI_NUMSURFACEV; nCntVertical++)
	{
		// �ŏ������͂���Ȃ��悤�ɂ��邽��
		if (nCntVertical > 0)
		{
			// �k�ރ|���S����
			pIdx[0] = (nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + CYLICOILLI_NUMSURFACEH + 1);
			pIdx++;
		}
		// ���̌�
		for (int nCntInd = 0; nCntInd < CYLICOILLI_NUMSURFACEH + 1; nCntInd++)
		{
			// �k�ރ|���S������������
			pIdx[0] = nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + nCntInd + CYLICOILLI_NUMSURFACEH + 1;
			pIdx[1] = nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + nCntInd;
			pIdx += 2;
		}
		// �Ōゾ������Ȃ��悤�ɂ���
		if (nCntVertical < CYLICOILLI_NUMSURFACEV - 1)
		{
			// �k�ރ|���S����
			pIdx[0] = (nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + CYLICOILLI_NUMSURFACEH);
			pIdx++;
		}
	}
	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}
