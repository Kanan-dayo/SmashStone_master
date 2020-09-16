//*************************************************************************************************************
//
// �ǂ̏���[wall.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "wall.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "game.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define WALL_FILENAME_1	"data/TEXT/wall/WallInfo_stage1.txt"	// �t�@�C����
#define WALL_FILENAME_2	"data/TEXT/wall/WallInfo_stage2.txt"	// �t�@�C����
#define WALL_OPENMODE	"r"								// �J�����[�h

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CWall::m_pAllTexture[WALLTEX::WALLTEX_MAX]			 = {};					// �S�Ẵe�N�X�`�����
D3DXVECTOR2        CWall::m_Length										 = MYLIB_VEC2_UNSET;	// ���S����̋���
D3DXVECTOR3        CWall::m_CenterPos									 = MYLIB_VEC3_UNSET;	// ���S�ʒu
float              CWall::m_fHeight										 = MYLIB_FLOAT_UNSET;	// ����
char			   CWall::m_cFileName[STAGE_MAX][MYLIB_STRINGSIZE]		 =						// �t�@�C����
{
	"data/TEXT/wall/WallInfo_stage1.txt",
	"data/TEXT/wall/WallInfo_stage2.txt"
};	

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
HRESULT CWall::Load(void)
{
	// �ϐ��錾
	FILE*             pFile;						// �t�@�C���̃|�C���^
	char              aRead[MYLIB_STRINGSIZE];		// �ǂݍ��ݗp
	char              aComp[MYLIB_STRINGSIZE];		// ��r�p
	char              aEmpty[MYLIB_STRINGSIZE];		// �v��Ȃ����̗p
	char              aFileName[MYLIB_STRINGSIZE];	// �t�@�C����
	int               nCntFileName;					// �t�@�C�����J�E���g
	D3DXVECTOR3       CenterPos;					// ���S�ʒu
	D3DXVECTOR2       Length;						// ���S����̋���
	float             fHeight;						// ����
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�̃|�C���^

	// ������
	aRead[0]     = MYLIB_CHAR_UNSET;
	aComp[0]     = MYLIB_CHAR_UNSET;
	aEmpty[0]    = MYLIB_CHAR_UNSET;
	aFileName[0] = MYLIB_CHAR_UNSET;
	nCntFileName = MYLIB_INT_UNSET;
	fHeight      = MYLIB_FLOAT_UNSET;

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �t�@�C�����J��
	pFile = fopen(m_cFileName[CGame::GetStageType()], WALL_OPENMODE);

	// �J���Ȃ�������
	if (pFile == NULL)
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	int  nCntError;						// �G���[�J�E���g
	nCntError = MYLIB_INT_UNSET;		// �G���[�J�E���g�̏�����
#endif
	// �X�N���v�g������܂łƂ΂�
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%s", &aComp);
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
		sscanf(aRead, "%s", &aComp);
#ifdef _DEBUG
		// �G���[�J�E���g���C���N�������g
		if (++nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
		// �t�@�C��������e�N�X�`����ǂݍ���
		if (sscanf(aRead, "FILENAME = %s", &aFileName) == 1)
		{
			// �e�N�X�`���̓ǂݍ���
			if (D3DXCreateTextureFromFile(pDevice, &aFileName[0], &m_pAllTexture[nCntFileName]) != D3D_OK)
			{// ���s

			}
			else
			{// ����

			}
			// �t�@�C�������C���N�������g
			nCntFileName++;
		}
		// ���S�ʒu�̓ǂݍ���
		else if (sscanf(aRead, "CENTER = %f %f %f", &CenterPos.x, &CenterPos.y, &CenterPos.z) == 3)
		{
			CWall::m_CenterPos = CenterPos;
		}
		// ���S����̋����̓ǂݍ���
		else if (sscanf(aRead, "LENGTH = %f %f", &Length.x, &Length.y) == 2)
		{
			CWall::m_Length = Length;
		}
		// �����̓ǂݍ���
		else if (sscanf(aRead, "HEIGHT = %f", &fHeight) == 1)
		{
			CWall::m_fHeight = fHeight;
		}
	}

	// �t�@�C�������
	fclose(pFile);
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void CWall::Unload(void)
{
	// �e�N�X�`�����̊J��
	for (int nCntTexture = 0; nCntTexture < WALLTEX::WALLTEX_MAX; nCntTexture++)
	{
		if (m_pAllTexture[nCntTexture] != NULL)
		{
			m_pAllTexture[nCntTexture]->Release();
			m_pAllTexture[nCntTexture] = NULL;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CWall * CWall::Create(WALLTEX enmWallTex)
{
	// �ǃN���X�̐���
	CWall *pWall = new CWall;
	// ���s������
	if (pWall == NULL)
	{
		return NULL;
	}
	// ������
	pWall->Init();
	// �e�N�X�`���̘A��
	pWall->BindTexType(enmWallTex);
	return pWall;
}

//-------------------------------------------------------------------------------------------------------------
// �Փ˔���
//-------------------------------------------------------------------------------------------------------------
bool CWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection)
{
	// �ϐ��錾
	SINGLEINFO *pSingleInfo = &m_SingleInfo[0];	// �P�̏��̃|�C���^
	bool bColli = false;						// �Փ˃t���O

	// +X�̈ʒu�̎�
	if (pSingleInfo[CWall::SETINGPOS_POSIX].trans.pos.x <= pPos->x)
	{
		// �Փ˃t���O�𗧂Ă�
		bColli = true;
		
		// ���˃t���O�������Ă���Ƃ�
		if (bReflection == true)
		{// ��_�����߂�
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &m_SingleInfo[SETINGPOS_POSIX]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_POSIX].nor;
			}
		}

		pPos->x = pSingleInfo[CWall::SETINGPOS_POSIX].trans.pos.x;
	}
	// -X�̈ʒu�̎�
	else if (pSingleInfo[CWall::SETINGPOS_NEGX].trans.pos.x >= pPos->x)
	{
		// �Փ˃t���O�𗧂Ă�
		bColli = true;

		// ���˃t���O�������Ă���Ƃ�
		if (bReflection == true)
		{// ��_�����߂�
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &m_SingleInfo[SETINGPOS_NEGX]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_NEGX].nor;
			}
		}

		pPos->x = pSingleInfo[CWall::SETINGPOS_NEGX].trans.pos.x;
	}
	// +Y�̈ʒu�̎�
	if (pSingleInfo[CWall::SETINGPOS_POSIZ].trans.pos.z <= pPos->z)
	{
		// �Փ˃t���O�𗧂Ă�
		bColli = true;

		// ���˃t���O�������Ă���Ƃ�
		if (bReflection == true)
		{// ��_�����߂�
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &pSingleInfo[SETINGPOS_POSIZ]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_POSIZ].nor;
			}
		}

		pPos->z = pSingleInfo[CWall::SETINGPOS_POSIZ].trans.pos.z;

	}
	// -Y�ʒu�̎�
	else if (pSingleInfo[CWall::SETINGPOS_NEGZ].trans.pos.z >= pPos->z)
	{
		// �Փ˃t���O�𗧂Ă�
		bColli = true;

		// ���˃t���O�������Ă���Ƃ�
		if (bReflection == true)
		{// ��_�����߂�
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &pSingleInfo[SETINGPOS_NEGZ]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_NEGZ].nor;
			}
			
		}

		pPos->z = pSingleInfo[CWall::SETINGPOS_NEGZ].trans.pos.z;
	}
	// �Փ˃t���O��Ԃ�
	return bColli;
}

//-------------------------------------------------------------------------------------------------------------
// ��_�����߂�
//-------------------------------------------------------------------------------------------------------------
bool CWall::GetIntersection(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pOut_Intersect, CONST SINGLEINFO * pSingleInfo)
{
	// �ϐ��錾
	D3DXVECTOR3 PlanePoint;			// ���ʏ�̓_
	D3DXVECTOR3 VecPlane_pos;		// ���ʏ�̓_����ʒu�̃x�N�g��
	D3DXVECTOR3 VecPlane_posOld;	// ���ʏ�̓_����O��̈ʒu�̃x�N�g��
	float DotPlane_pos;				// �ʒu�̃x�N�g�����ʖ@������
	float DotPlane_posOld;			// �O��̈ʒu�̃x�N�g�����ʖ@������

	// ���ʏ�̓_���Z�o
	PlanePoint = pSingleInfo->trans.pos;

	// �x�N�g���̐���
	CMylibrary::CreateVector3(&VecPlane_pos, pPos, &PlanePoint);
	CMylibrary::CreateVector3(&VecPlane_posOld, pPosOld, &PlanePoint);

	// ���όv�Z
	DotPlane_pos =
		VecPlane_pos.x * pSingleInfo->plane.a +
		VecPlane_pos.y * pSingleInfo->plane.b +
		VecPlane_pos.z * pSingleInfo->plane.c;
	DotPlane_posOld =
		VecPlane_posOld.x * pSingleInfo->plane.a +
		VecPlane_posOld.y * pSingleInfo->plane.b +
		VecPlane_posOld.z * pSingleInfo->plane.c;

	// ��������
	if (DotPlane_pos == 0.0f &&
		DotPlane_posOld == 0.0f)
	{
		// ���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B

		return false;
	}

	// ��_�����߂� 
	D3DXVECTOR3 Pos_posOld = D3DXVECTOR3(pPosOld->x - pPos->x, pPosOld->y - pPos->y, pPosOld->z - pPos->z);

	//��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB
	float fLength = abs(DotPlane_pos) / (abs(DotPlane_pos) + abs(DotPlane_posOld));

	pOut_Intersect->x = pPos->x + (Pos_posOld.x * fLength);
	pOut_Intersect->y = pPos->y + (Pos_posOld.y * fLength);
	pOut_Intersect->z = pPos->z + (Pos_posOld.z * fLength);
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CWall::Init(void)
{
	// �ʒu�̐ݒ�
	m_SingleInfo[SETINGPOS_POSIX].trans.pos = D3DXVECTOR3(m_CenterPos.x + m_Length.x, m_CenterPos.y, m_CenterPos.z);	// +X�̈ʒu
	m_SingleInfo[SETINGPOS_NEGX].trans.pos  = D3DXVECTOR3(m_CenterPos.x - m_Length.x, m_CenterPos.y, m_CenterPos.z);	// -X�̈ʒu
	m_SingleInfo[SETINGPOS_POSIZ].trans.pos = D3DXVECTOR3(m_CenterPos.x, m_CenterPos.y, m_CenterPos.z + m_Length.y);	// +Z�̈ʒu
	m_SingleInfo[SETINGPOS_NEGZ].trans.pos  = D3DXVECTOR3(m_CenterPos.x, m_CenterPos.y, m_CenterPos.z - m_Length.y);	// -Z�̒n��

	// �傫���̐ݒ�
	m_SingleInfo[SETINGPOS_POSIX].size = D3DXVECTOR3(0.0f, m_fHeight, m_Length.y);
	m_SingleInfo[SETINGPOS_NEGX].size = D3DXVECTOR3(0.0f, m_fHeight, m_Length.y);
	m_SingleInfo[SETINGPOS_POSIZ].size = D3DXVECTOR3(m_Length.x, m_fHeight, 0.0f);
	m_SingleInfo[SETINGPOS_NEGZ].size = D3DXVECTOR3(m_Length.x, m_fHeight,0.0f);

	// �@���x�N�g���̐ݒ�
	m_SingleInfo[SETINGPOS_POSIX].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_SingleInfo[SETINGPOS_NEGX].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_SingleInfo[SETINGPOS_POSIZ].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_SingleInfo[SETINGPOS_NEGZ].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// �ǂ̐����[�v
	for (int nCntSingle = 0; nCntSingle < WALL_MAX; nCntSingle++)
	{
		// ��]�̐ݒ�
		m_SingleInfo[nCntSingle].trans.rot = MYLIB_VEC3_UNSET;
		// ���ʂ̐���
		D3DXPlaneFromPointNormal(&m_SingleInfo[nCntSingle].plane, &m_SingleInfo[nCntSingle].trans.pos, &m_SingleInfo[nCntSingle].nor);
	}


	// ���_���̍쐬
	MakeVertex();
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CWall::Uninit(void)
{
	if (m_pVtexBuff != NULL)
	{
		m_pVtexBuff->Unlock();
		m_pVtexBuff->Release();
		m_pVtexBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CWall::Update(void)
{
#ifdef WALL_DEBUG
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		CDebugProc::Print("�ǂ̈ʒu %.3f,%.3f,%.3f\n",
			m_SingleInfo[nCntWall].trans.pos.x,
			m_SingleInfo[nCntWall].trans.pos.y,
			m_SingleInfo[nCntWall].trans.pos.z);
		CDebugProc::Print("�ǂ̑傫�� %.3f,%.3f,%.3f\n",
			m_SingleInfo[nCntWall].size.x,
			m_SingleInfo[nCntWall].size.y,
			m_SingleInfo[nCntWall].size.z);
	}
#endif
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CWall::Draw(void)
{
		// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	SINGLEINFO *pSingleInfo;		// �G�t�F�N�g�̃|�C���^

	pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�̎擾
	pSingleInfo = &m_SingleInfo[0];						// �|�C���^�̏�����

	// ���C�e�B���O���[�h����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O���Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pSingleInfo[nCntWall].trans.mtxWorld);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pSingleInfo[nCntWall].trans.mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, m_pVtexBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��
		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
	}

	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ����(�����)���J�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-------------------------------------------------------------------------------------------------------------
// �e�N�X�`�����̘A��
//-------------------------------------------------------------------------------------------------------------
void CWall::BindTexType(WALLTEX enmTexType)
{
	// �|�C���^����
	this->m_pTexture = CWall::m_pAllTexture[enmTexType];
}

//-------------------------------------------------------------------------------------------------------------
// ���_���̍쐬
//-------------------------------------------------------------------------------------------------------------
HRESULT CWall::MakeVertex(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;			// ���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * WALL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtexBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �P�̏��̃|�C���^
	SINGLEINFO *pSingleInfo = &m_SingleInfo[0];

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++, pSingleInfo++)
	{
		// ���_�ʒu
		pVtx[0].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x - pSingleInfo->size.x, pSingleInfo->trans.pos.y - pSingleInfo->size.y, pSingleInfo->trans.pos.z - pSingleInfo->size.z);
		pVtx[1].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x + pSingleInfo->size.x, pSingleInfo->trans.pos.y - pSingleInfo->size.y, pSingleInfo->trans.pos.z + pSingleInfo->size.z);
		pVtx[2].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x - pSingleInfo->size.x, pSingleInfo->trans.pos.y + pSingleInfo->size.y, pSingleInfo->trans.pos.z - pSingleInfo->size.z);
		pVtx[3].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x + pSingleInfo->size.x, pSingleInfo->trans.pos.y + pSingleInfo->size.y, pSingleInfo->trans.pos.z + pSingleInfo->size.z);
		// �@���x�N�g��
		pVtx[0].nor = pSingleInfo->nor;
		pVtx[1].nor = pSingleInfo->nor;
		pVtx[2].nor = pSingleInfo->nor;
		pVtx[3].nor = pSingleInfo->nor;
		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		// texture���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// ���_�|�C���^�̍X�V
		pVtx += 4;
	}
	// ���_�f�[�^���A�����b�N����
	m_pVtexBuff->Unlock();

	return S_OK;

}
