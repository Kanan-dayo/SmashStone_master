//*************************************************************************************************************
//
// �|���S���R���C�_�[����[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "CapsuleCollider.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "debugProc.h"
#include "CharEffectOffset.h"
#include "3DBoxCollider.h"
#include "3DParticle.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define CAPCOLLI_SLICES		5
#define CAPCOLLI_STACKS		5

#define CAPCOLLI_BOTTOMUNITVEC_NUMVTX	(3)		// ��ʂ̒P�ʃx�N�g���v�Z�ɕK�v�Ȓ��_��

#define CAPCOLLI_FILENAME						"data/TEXT/ColliInfo/Cylinder.csv"	// �t�@�C����
#define CAPCOLLI_OPENMODE						"r"		// �t�@�C�����J�����[�h
#define CAPCOLLI_WORDSIZE						16		// ���[�h�T�C�Y

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-------------------------------------------------------------------------------------------------------------
CCapsuleCollider::READINFOFILEBUFFER	CCapsuleCollider::m_ReadInfoFileBuff;		// �ǂ݂��񂾃t�@�C�����

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CCapsuleCollider::CCapsuleCollider() : CScene(PRIORITY_COLLISION)
{
	m_ColliderInfo.TopTransVec = MYLIB_VEC3_UNSET;		// ��ʂ܂ł̒����ƌ���
	m_ColliderInfo.BottomTransVec = MYLIB_VEC3_UNSET;	// ��ʂ܂ł̒����ƌ���
	m_ColliderInfo.enmTtpeID = TYPEID_NOEN;				// �^�C�vID
	m_ColliderInfo.trans.pos = MYLIB_VEC3_UNSET;
	m_ColliderInfo.trans.rot = MYLIB_VEC3_UNSET;
	m_ColliderInfo.trans.scal = MYLIB_SCALE_UNSET;
	m_ColliderInfo.pMtxParent = NULL;		// �e�̃}�g���b�N�X�|�C���^
}

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
HRESULT CCapsuleCollider::Load(void)
{
	// �ǂ݂��񂾃t�@�C�����̏�����
	m_ReadInfoFileBuff.pCell = NULL;
	m_ReadInfoFileBuff.pSetThisID = NULL;
#ifdef _DEBUG
	cout << "---------------------------------------------------------------------\n";
	cout << "CCapsuleCollider::Load �J�v�Z���R���C�_�[�̓ǂݍ��݊J�n\n";
	DWORD start = timeGetTime();			// �v���X�^�[�g����
#endif
	// �ϐ��錾
	FILE *pFile = fopen(CAPCOLLI_FILENAME, CAPCOLLI_OPENMODE);
	// �J���Ȃ�����
	if (pFile == NULL)
	{
#ifdef _DEBUG
		cout << "CCapsuleCollider::Load �J�v�Z���R���C�_�[�̓ǂݍ��݂̃t�@�C�����J���܂���ł���\n";
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
				char aSizeWord[CAPCOLLI_WORDSIZE];		// �T�C�Y�̃t���O����
				char aDiffWord[CAPCOLLI_WORDSIZE];		// �����̃t���O����
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
	cout << "CCapsuleCollider::Load �J�v�Z���R���C�_�[�̓ǂݍ��ݏI��\n";
	cout << " CCapsuleCollider::Load �J�v�Z���R���C�_�[�̓ǂݍ��� �������x = " << (end - start) << "�@[" << (end - start) * 0.001f << "sec.]\n";
#endif
	cout << "---------------------------------------------------------------------\n";
	return S_OK;

}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Unload(void)
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

	m_ReadInfoFileBuff.nNumReadInfo = 0;

}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Init(void)
{
	m_ColliderInfo.nSlices = CAPCOLLI_SLICES * 2;
	m_ColliderInfo.nStacks_1_2 = (CAPCOLLI_STACKS % 2 == 0) ? CAPCOLLI_STACKS: CAPCOLLI_STACKS + 1;
	// ���_��
	m_ColliderInfo.nNumVertex = (m_ColliderInfo.nStacks_1_2 + 1) * (m_ColliderInfo.nSlices + 1);
	// �����ڂ̒��_��
	m_ColliderInfo.nNumindex = 2 * m_ColliderInfo.nSlices * (m_ColliderInfo.nStacks_1_2 + 1);
	// �O�p�`�̐�
	m_ColliderInfo.nNumPolygon = m_ColliderInfo.nNumVertex - 2;


	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���_�̍쐬
	MakeVertex(pDevice);
	// �C���f�b�N�X�̍쐬
	MakeIndex(pDevice);

	// �J�v�Z�����̏�����
	InitCapsInfo();
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Uninit(void)
{
	// ���_�o�b�t�@��NULL����Ȃ���
	if (m_ColliderInfo.pVtexBuff != NULL)
	{
		// ���_�o�b�t�@�̊J��
		m_ColliderInfo.pVtexBuff->Release();
		m_ColliderInfo.pVtexBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@��NULL����Ȃ���
	if (m_ColliderInfo.pIdxBuff != NULL)
	{
		// �C���f�b�N�X�o�b�t�@�̊J��
		m_ColliderInfo.pIdxBuff->Release();
		m_ColliderInfo.pIdxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Update(void)
{
	if (m_ColliderInfo.pScene != NULL)
	{	
		this->Collision();
		this->CollisionStone();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[�̏��擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾
	D3DXMATRIX mtxTrans, mtxRot;							// �v�Z�p�}�g���b�N�X

	// ���C�e�B���O���[�h����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Fill Mode �̐ݒ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	// �J�����O���Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_ColliderInfo.trans.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_ColliderInfo.trans.rot.y,
		m_ColliderInfo.trans.rot.x,
		m_ColliderInfo.trans.rot.z);
	D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld,
		&m_ColliderInfo.trans.mtxWorld,
		&mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_ColliderInfo.trans.pos.x,
		m_ColliderInfo.trans.pos.y,
		m_ColliderInfo.trans.pos.z);

	D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld,
		&m_ColliderInfo.trans.mtxWorld,
		&mtxTrans);

	D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld,
		&m_ColliderInfo.trans.mtxWorld,
		&mtxTrans);


	// �e���f���̏�񂪂���Ƃ�
	if (m_ColliderInfo.pMtxParent != NULL)
	{
		D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld
			, &m_ColliderInfo.trans.mtxWorld
			, m_ColliderInfo.pMtxParent);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_ColliderInfo.trans.mtxWorld);

	// �J�v�Z���ʒu�̌v�Z
	CalCapPosition();

#ifdef _DEBUG

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_ColliderInfo.pVtexBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_ColliderInfo.pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_ColliderInfo.nNumindex, 0, m_ColliderInfo.nNumPolygon);
#endif
	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Fill Mode �̐ݒ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	// �����_���[�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����

}

//-------------------------------------------------------------------------------------------------------------
// �e�̃}�g���b�N�X�|�C���^�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::SetMtxParent(D3DXMATRIX * pMtxParent)
{
	m_ColliderInfo.pMtxParent = pMtxParent;
}

//-------------------------------------------------------------------------------------------------------------
// �V�[���̃|�C���^
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::SetScene(CScene * pScene)
{
	m_ColliderInfo.pScene = pScene;
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CCapsuleCollider * CCapsuleCollider::Create(CScene *pScene, D3DXMATRIX *pMtxParent, int nTypeID)
{
	// �ϐ��錾
	CCapsuleCollider *pCollider = new CCapsuleCollider;

	int nID = CCapsuleCollider::m_ReadInfoFileBuff.pSetThisID[nTypeID];	// ID�̎擾
	READINFOFILE_CELL *pCell = &CCapsuleCollider::m_ReadInfoFileBuff.pCell[nID];			// �Z���̃|�C���^

	pCollider->SeteumTypeID(nID);
	// ���̐ݒ�Ɠ���
	pCollider->InfoSetSync(
		pCell->pSizeInfo->fRadius,		// ���a
		pCell->pSizeInfo->fTop,			// ��ʂ܂ł̃T�C�Y
		pCell->pSizeInfo->fBottom,		// ��ʂ܂ł̃T�C�Y
		*pCell->pDifference				// ����
	);

	// �e�̃}�g���b�N�X�|�C���^�̐ݒ�
	pCollider->SetMtxParent(pMtxParent);
	// �V�[���̐ݒ�
	pCollider->SetScene(pScene);
	// �ݒ�
	pCollider->Init();
	return pCollider;
}

//-------------------------------------------------------------------------------------------------------------
// ���̐ݒ�Ɠ���
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 & diff)
{
	m_ColliderInfo.TopTransVec.y = fLengthMax;		// �����ő�
	m_ColliderInfo.BottomTransVec.y = fLengthMin;	// �����ŏ�
	m_ColliderInfo.trans.pos = diff;				// ����

	m_ColliderInfo.Capsule.fRadius = fRadius;
	m_ColliderInfo.Capsule.Segment.Point.y = fLengthMax;
	m_ColliderInfo.Capsule.Segment.Vec.y = fLengthMin - fLengthMax;
}

//-------------------------------------------------------------------------------------------------------------
// �J�v�Z�����̏�����
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::InitCapsInfo(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �Փ˔���
//-------------------------------------------------------------------------------------------------------------
bool CCapsuleCollider::Collision(void)
{
	// �̂̎�
	if (m_ColliderInfo.enmTtpeID == COLLIPARTS_BODY)
	{// �������Ȃ�
		return false;
	}
	// �ϐ��錾
	CPlayer          *pOwn            = (CPlayer *)m_ColliderInfo.pScene;						// ���̃R���C�_�[�������Ă���v���C���[
	CPlayer          *pOthers         = pOwn->GetAnotherPlayer();								// ���̑��̃v���C���[
	CCapsuleCollider *pOthersCapColli = pOthers->GetCapCollider(CCharacter::COLLIPARTS_BODY);	// ���̑��̃v���C���[�̃R���C�_�[���

	if (pOwn->ReadyToHit(m_ColliderInfo.enmTtpeID) == false)
	{
		return false;
	}

	D3DXVECTOR3 HitPos;
#ifdef _DEBUG
	CDebugProc::Print("COLLIPARTS [%d]", m_ColliderInfo.enmTtpeID);

	// 2�����̍ŒZ�������߂�
	if (CMylibrary::colCapsuleCapsule(m_ColliderInfo.Capsule, pOthersCapColli->m_ColliderInfo.Capsule, HitPos) == true)
	{
		CDebugProc::Print("�������Ă�\n");
		CCharEffectOffset::Set(&HitPos, CCharEffectOffset::STR_�h���b);
		C3DParticle::Set(&HitPos, &pOwn->GetRot(), C3DParticle::OFFSETNAME::HIT);
		pOwn->SetAttakHit(true);
	}
	else
	{
		CDebugProc::Print("�������ĂȂ�\n");
	}
#else

	// 2�����̍ŒZ�������߂�
	if (CMylibrary::colCapsuleCapsule(m_ColliderInfo.Capsule, pOthersCapColli->m_ColliderInfo.Capsule, HitPos) == true)
	{
		CCharEffectOffset::Set(&HitPos, CCharEffectOffset::STR_�h���b);
		C3DParticle::Set(&HitPos, &pOwn->GetRot(), C3DParticle::OFFSETNAME::HIT);
		pOwn->SetAttakHit(true);
	}

#endif // _DEBUG



	return false;
}

//-------------------------------------------------------------------------------------------------------------
// 石との衝突�E琁E//-------------------------------------------------------------------------------------------------------------
bool CCapsuleCollider::CollisionStone(void)
{
	CPlayer*                         pOwn = (CPlayer *)m_ColliderInfo.pScene;
	C3DBoxCollider::_3DBOXCOLLIDER * pBoxColli = C3DBoxCollider::GetInfo();	

	if (pOwn->ReadyToHit(m_ColliderInfo.enmTtpeID) == false)
	{
		return false;
	}

	for (int nCntBox = 0; nCntBox < _3DBOXCOLLIDER_MAX; nCntBox++)
	{
		if (pBoxColli[nCntBox].pScene == NULL)
		{
			continue;
		}
		if (pBoxColli[nCntBox].bUse == false ||
			pBoxColli[nCntBox].pScene->GetPriority() != CScene::PRIORITY_STONE)
		{
			continue;
		}

		D3DXVECTOR3 HitPos;

		if (CMylibrary::colCapsuleSphere(m_ColliderInfo.Capsule, pBoxColli[nCntBox].pos, ikuminLib::VEC3(pBoxColli[nCntBox].size).Length(), HitPos) == true)
		{
			CStone *pStone = (CStone *)pBoxColli[nCntBox].pScene;
			pOwn->SetAttakHit(true);
			if (pStone->ApplyDamage() == true)
			{
				CCharEffectOffset::Set(pStone->GetPos(), CCharEffectOffset::OFFSETNAME::STR_�L�[��);
				pOwn->CatchStone(pStone);
			}
			else
			{
				CCharEffectOffset::Set(&HitPos, CCharEffectOffset::STR_�S�b);
			}
		}
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// �J�v�Z���ʒu�̌v�Z
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::CalCapPosition(void)
{
	// �ϐ��錾
	D3DXVECTOR3 BottomPoint;	// ��ʂ̈ʒu

	// ��ʂ̈ʒu
	D3DXVec3TransformCoord(&m_ColliderInfo.Capsule.Segment.Point, &m_ColliderInfo.TopTransVec, &m_ColliderInfo.trans.mtxWorld);
	// ��ʂ̈ʒu
	D3DXVec3TransformCoord(&BottomPoint, &m_ColliderInfo.BottomTransVec, &m_ColliderInfo.trans.mtxWorld);

	// �x�N�g���ɎZ�o
	m_ColliderInfo.Capsule.Segment.Vec = BottomPoint - m_ColliderInfo.Capsule.Segment.Point;
}


//-------------------------------------------------------------------------------------------------------------
// ���_�̍쐬
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_ColliderInfo.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_ColliderInfo.pVtexBuff,
		NULL);

	// �ϐ��錾
	D3DXVECTOR3 TopPoint = m_ColliderInfo.Capsule.Segment.Point;			// ��ʂ̈ʒu
	D3DXVECTOR3 BottomPoint = m_ColliderInfo.Capsule.Segment.GetEndPoint();	// ��ʂ̈ʒu
	float fSlicesPI = (D3DX_PI * 2.0f) / m_ColliderInfo.nSlices;			// �������ɑΉ������~����
	float fStacksPI = (D3DX_PI * 2.0f) / m_ColliderInfo.nStacks_1_2;		// �������ɑΉ������~����
	// �c���J�E���g
	int nCompSlices = (CAPCOLLI_SLICES / 2);
	nCompSlices += (CAPCOLLI_SLICES % 2 == 0) ? 0 : 1;

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_ColliderInfo.pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSlices = 0; nCntSlices < m_ColliderInfo.nSlices + 1; nCntSlices++)
	{
		float fSlicesSinValue = sinf((fSlicesPI * nCntSlices));							// �T�C���̒l
		float fPos_y = cosf(fSlicesPI * nCntSlices) *  m_ColliderInfo.Capsule.fRadius;	// ���a��Y���W�̈ʒu
		D3DXVECTOR3 pos = (nCntSlices <  nCompSlices) ? TopPoint : BottomPoint;			// ���a�̒��S�ʒu

		// �����J�E���g
		for (int nCntStacks = 0; nCntStacks < m_ColliderInfo.nStacks_1_2 + 1; nCntStacks++)
		{
			float fStacksRadian = fStacksPI * nCntStacks;								// ���W�A���l

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pos.x+ fSlicesSinValue * cosf(fStacksRadian) * m_ColliderInfo.Capsule.fRadius;
			pVtx[0].pos.y = pos.y+ fPos_y;
			pVtx[0].pos.z = pos.z+ fSlicesSinValue * sinf(fStacksRadian) * m_ColliderInfo.Capsule.fRadius;
			// ���_���ɖ@���̌v�Z���ʑ��
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �e�N�X�`���`�ʂ̈ʒu
			pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�ԍ����Z
			pVtx++;
		}
	}

	// ���_�f�[�^���A�����b�N����
	m_ColliderInfo.pVtexBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// �C���f�b�N�X�̍쐬
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_ColliderInfo.nNumindex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_ColliderInfo.pIdxBuff,
		NULL);

	WORD *pIdx;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int nNumber = 0;		// �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N���C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_ColliderInfo.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �c���J�E���g
	for (int nCntSlices = 0; nCntSlices < m_ColliderInfo.nSlices; nCntSlices++)
	{
		// �����J�E���g
		for (int nCntStacks = 0; nCntStacks < m_ColliderInfo.nStacks_1_2 + 1; nCntStacks++)
		{
			// ���ɃC���f�b�N�X�i�[
			pIdx[nNumber++] = nCntSlices * (m_ColliderInfo.nStacks_1_2 + 1) + nCntStacks;
			pIdx[nNumber++] = pIdx[nNumber - 1] + m_ColliderInfo.nStacks_1_2 + 1;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_ColliderInfo.pIdxBuff->Unlock();
}
