//*************************************************************************************************************
//
// 3D�p�[�e�B�N������[3DParticle.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "3DParticle.h"

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
C3DParticle::OFFSET C3DParticle::m_Offset[C3DParticle::OFFSETNAME::OFFSET_MAX] = {};

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Load(void)
{
#ifdef _DEBUG
	DWORD start = timeGetTime();			// �v���X�^�[�g����
#endif // _DEBUG
	// �t�@�C����
	CONST_STRING pFileName[OFFSETNAME::OFFSET_MAX] = {
		{ "data/TEXT/Effect/ParticleOffset/StoneAppearRed.txt" },
		{ "data/TEXT/Effect/ParticleOffset/StoneNormalRed.txt" },
		{ "data/TEXT/Effect/ParticleOffset/StoneDisAppearRed.txt" },

		{ "data/TEXT/Effect/ParticleOffset/StoneAppearGreen.txt" },
		{ "data/TEXT/Effect/ParticleOffset/StoneNormalGreen.txt" },
		{ "data/TEXT/Effect/ParticleOffset/StoneDisAppearGreen.txt" },

		{ "data/TEXT/Effect/ParticleOffset/StoneAppearBlue.txt" },
		{ "data/TEXT/Effect/ParticleOffset/StoneNormalBlue.txt" },
		{ "data/TEXT/Effect/ParticleOffset/StoneDisAppearBlue.txt" },

		{ "data/TEXT/Effect/ParticleOffset/HitOffset.txt" },
		{ "data/TEXT/Effect/ParticleOffset/SmashAttackStart.txt" },
		{ "data/TEXT/Effect/ParticleOffset/SmashAttackHit.txt" },
		{ "data/TEXT/Effect/ParticleOffset/SmashDash.txt" },

		{ "data/TEXT/Effect/ParticleOffset/HitReflection.txt" },

		{ "data/TEXT/Effect/ParticleOffset/ChargeRed.txt" },
		{ "data/TEXT/Effect/ParticleOffset/ChargeGreen.txt" },
		{ "data/TEXT/Effect/ParticleOffset/ChargeBlue.txt" },

	};

	for (int nCntOffset = 0; nCntOffset < OFFSETNAME::OFFSET_MAX; nCntOffset++)
	{
		// �t�@�C������ǂݍ���
		LoadFromFile(pFileName[nCntOffset], nCntOffset);
	}

#ifdef _DEBUG
	DWORD end = timeGetTime();			// �v���X�^�[�g����

	cout << "\nC3DParticle::Load �I�t�Z�b�g���̓ǂݍ��ݏI��\n";
	cout << "C3DParticle::Load �I�t�Z�b�g���̓ǂݍ��� �������x = " << (end - start) << "�@[" << (end - start) * 0.001f << "sec.]\n";
#endif // _DEBUG
}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Unload(void)
{
	// �I�t�Z�b�g�̃|�C���^
	OFFSET *pOffset = &m_Offset[0];
	for (int nCntOffset = 0; nCntOffset < OFFSETNAME::OFFSET_MAX; nCntOffset++)
	{
		ReleaseParam(&pOffset[nCntOffset]);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^�̊J��
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::ReleaseParam(OFFSET *pOffset)
{
	for (int nCntParam = 0; nCntParam < pOffset->nNumParam; nCntParam++)
	{
		if (&pOffset->pParam[nCntParam] != nullptr)
		{
			// �p�����[�^�̃����o�̊J��
			ReleaseParamMemb(&pOffset->pParam[nCntParam]);
		}
	}
	delete[]pOffset->pParam;
	pOffset->pParam = nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^�����o�̊J��
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::ReleaseParamMemb(PARAM * pParam)
{
	if (pParam->pPos != nullptr)
	{
		delete pParam->pPos;
		pParam->pPos = nullptr;
	}
	if (pParam->pPosXRand != nullptr)
	{
		delete pParam->pPosXRand;
		pParam->pPosXRand = nullptr;
	}
	if (pParam->pPosYRand != nullptr)
	{
		delete pParam->pPosYRand;
		pParam->pPosYRand = nullptr;
	}
	if (pParam->pPosZRand != nullptr)
	{
		delete pParam->pPosZRand;
		pParam->pPosZRand = nullptr;
	}
	if (pParam->pLengthRand != nullptr)
	{
		delete pParam->pLengthRand;
		pParam->pLengthRand = nullptr;
	}
	if (pParam->pRadiusRand != nullptr)
	{
		delete pParam->pRadiusRand;
		pParam->pRadiusRand = nullptr;
	}
	if (pParam->pSpeedRand != nullptr)
	{
		delete pParam->pSpeedRand;
		pParam->pSpeedRand = nullptr;
	}
	if (pParam->pAngleRand != nullptr)
	{
		delete pParam->pAngleRand;
		pParam->pAngleRand = nullptr;
	}
	if (pParam->pLifeRand != nullptr)
	{
		delete pParam->pLifeRand;
		pParam->pLifeRand = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �t�@�C������ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::LoadFromFile(CONST_STRING pFileName, const int & nCntOffSet)
{
	// �ϐ��錾
	FILE *pFile;						// �t�@�C���|�C���^
	char aRead[MYLIB_STRINGSIZE];		// �ǂݍ��ݗp
	char aComp[MYLIB_STRINGSIZE];		// ��r�p
	char aWork[MYLIB_STRINGSIZE];		// ��Ɨp
	int  nCntParam;						// ���J�E���g
	int  nNumParam;						// �g���p�����[�^��
	int  nParent;						// �e�t���O

	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	aWork[0] = MYLIB_CHAR_UNSET;
	nCntParam = MYLIB_INT_UNSET;
	nNumParam = MYLIB_INT_UNSET;
	nParent = MYLIB_INT_UNSET;


	// �t�@�C�����J��
	if ((pFile = fopen(pFileName, "r")) == NULL)
	{// ���s������
		return;
	}

	// �X�N���v�g
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%s", &aComp);
	}

	// �G���h�X�N���v�g
	while (strcmp(aComp, "END_SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%s", &aComp);


		if (sscanf(aRead, "NUMPARAM = %d", &nNumParam) == 1)
		{
			m_Offset[nCntOffSet].nNumParam = nNumParam;
			m_Offset[nCntOffSet].pParam = new PARAM[m_Offset[nCntOffSet].nNumParam];
			cout << "NUMPARAM �擾\n";
		}
		else if (strcmp(aComp, "SETPARAM") == 0)
		{
			LoadParamFromFile(pFile, nCntOffSet, nCntParam);
			nCntParam++;
			aComp[0] = MYLIB_CHAR_UNSET;
		}
	}
	fclose(pFile);

}

//-------------------------------------------------------------------------------------------------------------
// �t�@�C������p�����[�^��ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::LoadParamFromFile(FILE * pFile, const int & nCntOffSet, const int & nCntParam)
{
	// �ϐ��錾
	char aRead[MYLIB_STRINGSIZE];	// �ǂݍ��ݗp
	char aComp[MYLIB_STRINGSIZE];	// ��r�p
	int nIntWork;					// int�̍�Ɨp
	D3DXVECTOR3 Float3Work;			// float3�̍�Ɨp
	RANGE RangeWork;				// range�̍�Ɨp
	float FlaotWork;				// float�̍�Ɨp
	D3DXCOLOR col;					// �F
	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	nIntWork = MYLIB_INT_UNSET;
	Float3Work = MYLIB_VEC3_UNSET;
	RangeWork = MYLIB_RANGE_UNSET;
	FlaotWork = MYLIB_FLOAT_UNSET;
	col = MYLIB_D3DXCOR_UNSET;

	cout << "�p���� == " << nCntParam << "\n";
	// �G���h�p����
	while (strcmp(aComp, "END_SETPARAM") != 0)
	{
		STRING pFIleRead = &aRead[0];
		// 1�s�ǂݍ���
		fgets(pFIleRead, MYLIB_STRINGSIZE, pFile);
		// �ǂݍ���Ǖ�������
		sscanf(pFIleRead, "%s", &aComp);

		while (*pFIleRead == '\t')
		{
			pFIleRead++;
		}

		if (sscanf(pFIleRead, "TIMER = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nTimer = nIntWork;
		}
		else if (sscanf(pFIleRead, "FRAME = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nFrame = nIntWork;
		}
		else if (sscanf(pFIleRead, "NUMBER = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nNumber = nIntWork;
		}
		else if (sscanf(pFIleRead, "TYPE = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nType = nIntWork;
		}
		else if (sscanf(pFIleRead, "TEXTYPE = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nTexType = nIntWork;
		}
		else if (sscanf(pFIleRead, "POS = %f %f %f", &Float3Work.x, &Float3Work.y, &Float3Work.z) == 3)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPos = new D3DXVECTOR3;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPos = Float3Work;
		}
		else if (sscanf(pFIleRead, "POSXRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPosXRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPosXRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "POSYRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPosYRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPosYRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "POSZRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPosZRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPosZRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "LENGTH = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fLength = FlaotWork;
		}
		else if (sscanf(pFIleRead, "LENGTHRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pLengthRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pLengthRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "COL = %f %f %f %f", &col.r, &col.g, &col.b, &col.a) == 4)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].col = col;
		}
		else if (sscanf(pFIleRead, "RADIUS = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fRadius = FlaotWork;
		}
		else if (sscanf(pFIleRead, "RADIUSRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pRadiusRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pRadiusRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "RADIUSVALUE = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fRadiusValue = FlaotWork;
		}
		else if (sscanf(pFIleRead, "ANGLE = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fAngle = FlaotWork;
		}
		else if (sscanf(pFIleRead, "ANGLERAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pAngleRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pAngleRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "SPEED = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fSpeed = FlaotWork;
		}
		else if (sscanf(pFIleRead, "SPEEDRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pSpeedRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pSpeedRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "LIFE = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nLife = nIntWork;
		}
		else if (sscanf(pFIleRead, "LIFERAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pLifeRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pLifeRand = RangeWork;
		}
		else if (sscanf(pFIleRead, "GRAVITY = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fGravity = FlaotWork;
		}
		else if (sscanf(pFIleRead, "ROT = %f %f %f", &Float3Work.x, &Float3Work.y, &Float3Work.z) == 3)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].rot = Float3Work;
		}
		else if (sscanf(pFIleRead, "PARENT = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].bParent = nIntWork != 0;
			(nIntWork == 1) ?
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.set(BIT_INDEX::PARENT) :
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.clear(BIT_INDEX::PARENT);
		}
		else if (sscanf(pFIleRead, "DIREQUAPOS = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].bDirEquaPos = nIntWork != 0;
			(nIntWork == 1) ?
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.set(BIT_INDEX::DIREQUAPOS) :
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.clear(BIT_INDEX::DIREQUAPOS);
		}
		else if (sscanf(pFIleRead, "BILLBOARD = %d", &nIntWork) == 1)
		{
			(nIntWork == 1) ?
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.set(BIT_INDEX::BILLBOARD) :
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.clear(BIT_INDEX::BILLBOARD);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Init(void)
{
	m_nTimer = MYLIB_INT_UNSET;
	m_pFrame = nullptr;
	m_pPosParent = nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Uninit(void)
{
	m_pPosParent = nullptr;
	m_pRotParent = nullptr;

	if (m_pFrame != nullptr)
	{
		delete[] m_pFrame;
		m_pFrame = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Update(void)
{
	// �ϐ��錾
	PARAM * pParam = &m_Offset[m_Name].pParam[0];	// �p�����|�C���^

	// �p�����[�^�����[�v
	for (int nCntParam = 0; nCntParam < m_Offset[m_Name].nNumParam; nCntParam++)
	{
		// �|�C���^��null�̎�
		if (&pParam[nCntParam] == nullptr)
		{
			continue;
		}

		// �^�C�}�[�������������܂��̓t���[�����ȏ�̎�
		if (pParam[nCntParam].nTimer > m_nTimer ||
			pParam[nCntParam].nFrame <= m_pFrame[nCntParam])
		{// �������X�L�b�v
			continue;
		}
		// �t���[������i�߂�
		m_pFrame[nCntParam]++;

		// �p�����[�^����G�t�F�N�g��ݒ�
		SetEffectFromParam(&pParam[nCntParam]);

	}
	// �J���ł��邩�`�F�b�N
	if (ReleaseCheck() == true)
	{
		this->Release();
	}
	// �^�C�}�[�����Z����
	m_nTimer++;
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^����G�t�F�N�gw�ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetEffectFromParam(PARAM * pParam)
{
	for (int nNumEffect = 0; nNumEffect < pParam->nNumber; nNumEffect++)
	{
		// �ݒ�p
		C3DEffect::SETINGPARAM Seting;
		// �r���{�[�h�̐ݒ�
		Seting.bBillBoard = true;
		// �F�̐ݒ�
		Seting.col = pParam->col;
		// �d�͂̐ݒ�
		Seting.fGravity = pParam->fGravity;
		// ���a�̐ݒ�
		GetRadiuFromParam(pParam, Seting.fRadius);
		// ���a�̕ω��l�̐ݒ�
		Seting.fRadiusValue = pParam->fRadiusValue;
		// �e�N�X�`���^�C�v�̐ݒ�
		Seting.nTexType = pParam->nTexType;
		// �^�C�v�̐ݒ�
		Seting.type = pParam->nType;
		// ���C�t�̐ݒ�
		GetLifeFromParam(pParam, Seting.nLife);
		// �e�|�C���^�̐ݒ�
		Seting.pParent = GetParentFromParam(pParam);
		// �r���{�[�h�t���O�̐ݒ�
		Seting.bBillBoard = pParam->Flag.comp(C3DParticle::BILLBOARD);
		// �����ƕ��ˈʒu�����킹��t���O�������Ă��鎞
		if (pParam->bDirEquaPos == true)
		{// �ړ��ʂƕ����ɕ�����ʒu�̎擾
			GetMoveAndPosAccordingDirFromParam(pParam, Seting.move, Seting.pos);
		}
		else
		{// �ʒu�̐ݒ�
			GetPosFromParam(pParam, Seting.pos);
			// �ړ��ʂ̐ݒ�
			GetMoveFromParam(pParam, Seting.move);
		}
		// �G�t�F�N�g�̐ݒ�
		C3DEffect::Set(Seting);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �ݒ�
//------------------------------------------------------------------------------------------------------------
C3DParticle* C3DParticle::Set(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pRot, CONST OFFSETNAME & name)
{
	// �p�[�e�B�N���̐���
	C3DParticle * pParticle = new C3DParticle;
	// ������
	pParticle->Init();
	// �����̐ݒ�
	pParticle->SetRot(pRot);
	// �ʒu�̐ݒ�
	pParticle->SetPos(pPos);
	// �I�t�Z�b�g���̐ݒ�
	pParticle->SetName(name);
	// �t���[���̏���
	pParticle->StandbyFrame(m_Offset[name].nNumParam);
	return pParticle;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^���甼�a���擾
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetRadiuFromParam(CONST PARAM * pParam, float &fRadius)
{
	fRadius =
		(pParam->pRadiusRand != nullptr) ?
		(float)(rand() % pParam->pRadiusRand->nMax + pParam->pRadiusRand->nMin) :
		pParam->fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^���烉�C�t���擾
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetLifeFromParam(CONST PARAM * pParam, int & nLife)
{
	nLife = 
		(pParam->pLifeRand != nullptr) ?
		(rand() % pParam->pLifeRand->nMax + pParam->pLifeRand->nMin) :
		pParam->nLife;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^����e���擾
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * C3DParticle::GetParentFromParam(CONST PARAM * pParam)
{
	return (pParam->bParent == true) ? m_pPosParent : nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^����ʒu���擾
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetPosFromParam(CONST PARAM * pParam, D3DXVECTOR3 & pos)
{
	if (pParam->pPosXRand != nullptr)
	{
		if (pParam->bParent == true)
		{
			pos.x = (float)(rand() % pParam->pPosXRand->nMax + pParam->pPosXRand->nMin) + m_pPosParent->x;
		}
		else
		{
			pos.x = (float)(rand() % pParam->pPosXRand->nMax + pParam->pPosXRand->nMin);
		}
	}
	else if (pParam->pPos != nullptr)
	{
		pos.x = (pParam->bParent == true) ?
			pParam->pPos->x + m_pPosParent->x :
			pParam->pPos->x + m_pos.x;
	}
	if (pParam->pPosYRand != nullptr)
	{
		if (pParam->bParent == true)
		{
			pos.y = (float)(rand() % pParam->pPosYRand->nMax + pParam->pPosYRand->nMin) + m_pPosParent->y;
		}
		else
		{
			pos.z = (float)(rand() % pParam->pPosZRand->nMax + pParam->pPosZRand->nMin);
		}
	}
	else if (pParam->pPos != nullptr)
	{
		pos.y = (pParam->bParent == true) ?
			pParam->pPos->y + m_pPosParent->y :
			pParam->pPos->y + m_pos.y;
	}
	if (pParam->pPosZRand != nullptr)
	{
		if (pParam->bParent == true)
		{
			pos.z = (float)(rand() % pParam->pPosZRand->nMax + pParam->pPosZRand->nMin) + m_pPosParent->z;
		}
		else
		{
			pos.z = (float)(rand() % pParam->pPosZRand->nMax + pParam->pPosZRand->nMin);
		}
	}
	else if (pParam->pPos != nullptr)
	{
		pos.z = (pParam->bParent == true) ?
			pParam->pPos->z + m_pPosParent->z:
			pParam->pPos->z + m_pos.z;
	}

}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^����ړ��ʂ��擾
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetMoveFromParam(CONST PARAM * pParam, D3DXVECTOR3 & move)
{
	// �ϐ��錾
	float       fSpeed;		// ���x

	// ���x�̎擾
	GetSpeedFromParam(pParam, fSpeed);
	float fAddRotY = m_rot.y;
	float fSin = -sinf(fAddRotY);
	float fCos = -cosf(fAddRotY);
	float fSin2 = -sinf(fAddRotY + D3DX_PI *  0.5f);
	float fCos2 = -cosf(fAddRotY + D3DX_PI *  0.5f);

	move.x = pParam->rot.z * fSin + pParam->rot.x * fSin2 + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.z = pParam->rot.z * fCos + pParam->rot.x * fCos2 +  (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.y = pParam->rot.y + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	// ���K��
	CMylibrary::CreateUnitVector(&move, &move);

	move *= fSpeed;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^���瑬�x���擾
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetSpeedFromParam(CONST PARAM * pParam, float & fSpeed)
{
	fSpeed =
		(pParam->pSpeedRand != nullptr) ?
		(float)(rand() % pParam->pSpeedRand->nMax + pParam->pSpeedRand->nMin) :
		pParam->fSpeed;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^����ړ��ʂƕ����ɕ�����ʒu�̎擾
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetMoveAndPosAccordingDirFromParam(CONST PARAM * pParam, D3DXVECTOR3 & move, D3DXVECTOR3 & pos)
{
	// �ϐ��錾
	float fSpeed;		// ���x
	float fLength;		// ���_����̋���

	// ���x�̎擾
	GetSpeedFromParam(pParam, fSpeed);

	// ���_����̋������擾
	GetLengthFromParam(pParam, fLength);

	float fAddRotY = m_rot.y;
	float fSin = -sinf(fAddRotY);
	float fCos = -cosf(fAddRotY);
	float fSin2 = -sinf(fAddRotY + D3DX_PI *  0.5f);
	float fCos2 = -cosf(fAddRotY + D3DX_PI *  0.5f);

	move.x = pParam->rot.z * fSin + pParam->rot.x * fSin2 + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.z = pParam->rot.z * fCos + pParam->rot.x * fCos2 + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.y = pParam->rot.y + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	// ���K��
	CMylibrary::CreateUnitVector(&move, &move);

	// �ʒu���v�Z����
	if (pParam->bParent == true)
	{
		pos.x = move.x * fLength + m_pPosParent->x;
		pos.y = move.y * fLength + m_pPosParent->y;
		pos.z = move.z * fLength + m_pPosParent->z;
	}
	else
	{
		pos.x = move.x * fLength + m_pos.x;
		pos.y = move.y * fLength + m_pos.y;
		pos.z = move.z * fLength + m_pos.z;
	}


	// �ړ��ʂ��v�Z����
	move *= fSpeed;
}

//-------------------------------------------------------------------------------------------------------------
// �p�����[�^���猴�_����̋������擾
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetLengthFromParam(CONST PARAM * pParam, float & fLength)
{
	fLength =
		(pParam->pLengthRand != nullptr) ?
		(float)(rand() % pParam->pLengthRand->nMax + pParam->pLengthRand->nMin) :
		pParam->fLength;
}

//-------------------------------------------------------------------------------------------------------------
// �J���ł��邩�`�F�b�N
//-------------------------------------------------------------------------------------------------------------
bool C3DParticle::ReleaseCheck(void)
{
	// �ϐ��錾
	PARAM * pParam = &m_Offset[m_Name].pParam[0];	// �p�����|�C���^
	int nCntApea   = MYLIB_INT_UNSET;				// �o���J�E���g
	// �p�����[�^�����[�v
	for (int nCntParam = 0; nCntParam < m_Offset[m_Name].nNumParam; nCntParam++)
	{
		if (pParam[nCntParam].nFrame <= m_pFrame[nCntParam])
		{
			nCntApea++;
		}
	}
	if (m_Offset[m_Name].nNumParam <= nCntApea)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// �ʒu�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetPos(D3DXVECTOR3 * pPos)
{
	m_pPosParent = pPos;
	m_pos = *pPos;
}

//-------------------------------------------------------------------------------------------------------------
// �����̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetRot(D3DXVECTOR3 * pRot)
{
	m_pRotParent = pRot;
	m_rot = *pRot;
}

//-------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g���̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetName(CONST OFFSETNAME & name)
{
	m_Name = name;
}

//-------------------------------------------------------------------------------------------------------------
// �t���[���̏���
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::StandbyFrame(const int & nNumParam)
{
	// �t���[���̐���
	m_pFrame = new int[nNumParam];

	for (int nCntFrame = 0; nCntFrame <nNumParam; nCntFrame++)
	{
		m_pFrame[nCntFrame] = MYLIB_INT_UNSET;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �p�����\���̂̃R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
C3DParticle::PARAM::PARAM()
{
	nTimer       = MYLIB_INT_UNSET;							// �^�C�}�[
	nFrame       = MYLIB_INT_UNSET;							// �t���[����
	nNumber      = MYLIB_INT_UNSET;							// ��
	nType        = C3DEffect::TYPE::TYPE_NONE;				// ���
	nTexType     = C3DEffect::TEXTURETYPE::TEXTYPE_NONE;	// �e�N�X�`���̎��
	pPos         = nullptr;									// �ʒu�̃|�C���^
	pPosXRand    = nullptr;									// �ʒuX���W�����_���p�̃|�C���^
	pPosYRand    = nullptr;									// �ʒuY���W�����_���p�̃|�C���^
	pPosZRand    = nullptr;									// �ʒuY���W�����_���p�̃|�C���^
	fLength      = MYLIB_FLOAT_UNSET;						// ���_����̋���
	pLengthRand  = nullptr;									// ���_����̋��������_���p�̃|�C���^
	col          = MYLIB_D3DXCOR_UNSET;						// �F�̃|�C���^
	fRadius      = MYLIB_FLOAT_UNSET;						// ���a
	pRadiusRand  = nullptr;									// ���a�̃����_���p�̃|�C���^
	fRadiusValue = MYLIB_FLOAT_UNSET;						// ���a�̕ω���
	fAngle       = MYLIB_FLOAT_UNSET;						// ���ˊp�x
	pAngleRand   = nullptr;									// ���ˊp�x�̃����_���p�̃|�C���^
	fSpeed       = MYLIB_FLOAT_UNSET;						// ���x
	pSpeedRand   = nullptr;									// ���x�̃����_���p�̃|�C���^
	nLife        = MYLIB_INT_UNSET;							// ���C�t
	pLifeRand    = nullptr;									// ���C�t�̃����_���p�̃|�C���^
	fGravity     = MYLIB_FLOAT_UNSET;						// �d��
	rot          = MYLIB_VEC3_UNSET;						// ����
	bParent      = false;									// �e�t���O
	bDirEquaPos  = false;									// �����ƕ��ˈʒu�𓯂��ɂ���
}

//-------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g�\���̂̃R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
C3DParticle::OFFSET::OFFSET()
{
	int    nNumParam = MYLIB_INT_UNSET;	// �g���p�����[�^��
	PARAM* pParam    = nullptr;			// �p�����[�^�P�̂̈ʒu
}
