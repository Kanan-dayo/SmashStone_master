//*************************************************************************************************************
//
// 3D�G�t�F�N�g����[3DEffect.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "3DEffect.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define _3DEFFE_FILENAME "data/TEXT/Effect/Texture/EffectTexture.txt"
#define _3DEFFE_OPNEMODE "r"

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9     C3DEffect::m_pVtxBuff                        = nullptr;						// �o�b�t�@���
LPDIRECT3DTEXTURE9          C3DEffect::m_pTexInfo[C3DEffect::TEXTYPE_MAX]= MYLIB_INITSTRUCT_WITHCONST;	// �e�N�X�`�����
const float                 C3DEffect::m_cfBaseAngle                     = D3DX_PI * 0.25f;				// ��{�p�x
int                         C3DEffect::m_nNumTextureMax                  = MYLIB_INT_UNSET;				// �e�N�X�`���̍ő吔
C3DEffect::PARAMETER        C3DEffect::m_EffectPram[_3DEFFE_USEQUANTITY] = MYLIB_INITSTRUCT_WITHCONST;	// �G�t�F�N�g�̃p�����[�^
C3DEffect::VIBRATION_OPTION C3DEffect::m_VibrationOption                 = MYLIB_INITSTRUCT_WITHCONST;	// �U���̃I�v�V����

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Load(void)
{
#ifdef _DEBUG
	DWORD start = timeGetTime();			// �v���X�^�[�g����
#endif // _DEBUG

	// �ϐ��錾
	FILE *pFile;						// �t�@�C���|�C���^
	char aRead[MYLIB_STRINGSIZE];		// �ǂݍ��ݗp
	char aComp[MYLIB_STRINGSIZE];		// ��r�p
	char aWork[MYLIB_STRINGSIZE];		// ��Ɨp
	int  nCntInfo;						// ���J�E���g
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̎擾
	VIBRATION_OPTION SetingOption;		// �ݒ�p�I�v�V����

	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	aWork[0] = MYLIB_CHAR_UNSET;
	nCntInfo = MYLIB_INT_UNSET;
	pDevice = CManager::GetRenderer()->GetDevice();
	SetingOption = MYLIB_INITSTRUCT_WITHCONST;

	// �t�@�C�����J��
	if ((pFile = fopen(_3DEFFE_FILENAME, _3DEFFE_OPNEMODE)) == NULL)
	{// ���s������
		return;
	}

	// �G���h�X�N���v�g
	while (strcmp(aComp,"END_SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%s", &aComp);

		// �t�@�C����
		if (sscanf(aRead, "FILENAME = %s", aWork) == 1)
		{
			// �e�N�X�`���̍쐬
			CreateTexture(aWork, nCntInfo, pDevice);
			// ���J�E���g��i�߂�
			nCntInfo++;
		}
		// �����̍ő吔
		else if (sscanf(aRead, "MAXDIST = %d", &SetingOption.nMaxDist) == 1)
		{
			m_VibrationOption.nMaxDist = SetingOption.nMaxDist;
		}
		// �����̏k���l
		else if (sscanf(aRead, "SCALEDIST = %f", &SetingOption.fScalDistValue) == 1)
		{
			m_VibrationOption.fScalDistValue = SetingOption.fScalDistValue;
		}
		// �����̍ő�l
		else if (sscanf(aRead, "MAXDIVIDIR = %d", &SetingOption.nMaxDiviDir) == 1)
		{
			m_VibrationOption.nMaxDiviDir = SetingOption.nMaxDiviDir;
		}
		// �����̏k���l
		else if (sscanf(aRead, "SCALDIR = %f", &SetingOption.fScalDirValue) == 1)
		{
			m_VibrationOption.fScalDirValue = SetingOption.fScalDirValue;
		}

		// ���J�E���g���e�N�X�`���^�C�v�ȏ�ɂȂ����Ƃ�
		if (nCntInfo >= TEXTYPE_MAX)
		{// �����𔲂���
			break;
		}
	}

	fclose(pFile);
#ifdef _DEBUG
	DWORD end = timeGetTime();			// �v���X�^�[�g����

	cout << "\nC3DEffect::Load �e�N�X�`�����̓ǂݍ��ݏI��\n";
	cout << "C3DEffect::Load �e�N�X�`�����̓ǂݍ��� �������x = " << (end - start) << "�@[" << (end - start) * 0.001f << "sec.]\n";
#endif // _DEBUG

};

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Unload(void)
{
	// �e�N�X�`���^�C�v�̍ő吔���[�v
	for (int nCntTex = 0; nCntTex < TEXTYPE_MAX; nCntTex++)
	{// �|�C���^��null��������
		if (m_pTexInfo[nCntTex] != nullptr)
		{// �e�N�X�`�����̊J������
			m_pTexInfo[nCntTex]->Release();
			m_pTexInfo[nCntTex] = nullptr;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �쐬
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Make(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	MakeVertex(pDevice);

	// ������
	Init();
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Init(void)
{
	// �p�����[�^�|�C���^
	PARAMETER* pEffePram = &m_EffectPram[0];

	for (int nCntPrame = 0; nCntPrame < _3DEFFE_USEQUANTITY; nCntPrame++)
	{
		pEffePram[nCntPrame].bUse             = false;								// �g�p�t���O
		pEffePram[nCntPrame].bDisp            = true;								// �`��t���O
		pEffePram[nCntPrame].bBillBoard       = true;								// �r���{�[�h�t���O
		pEffePram[nCntPrame].Trans            = TRANSFORM();						// �g�����X���
		pEffePram[nCntPrame].pParent          = nullptr;							// �e�̈ʒu
		pEffePram[nCntPrame].move             = MYLIB_VEC3_UNSET;					// �ړ���
		pEffePram[nCntPrame].col              = MYLIB_D3DXCOR_UNSET;				// �F
		pEffePram[nCntPrame].fRadius          = MYLIB_FLOAT_UNSET;					// ���a
		pEffePram[nCntPrame].fAngle           = D3DX_PI * 0.5f;						// �p�x
		pEffePram[nCntPrame].fRadiusValue     = MYLIB_FLOAT_UNSET;					// ���a�̕ω��l
		pEffePram[nCntPrame].fAlphaValue      = MYLIB_FLOAT_UNSET;					// �A���t�@�l�̕ω��l
		pEffePram[nCntPrame].nLife            = MYLIB_INT_UNSET;					// ��������
		pEffePram[nCntPrame].nTexType         = MYLIB_INT_UNSET;					// �e�N�X�`���̎��
		pEffePram[nCntPrame].fGravity         = MYLIB_FLOAT_UNSET;					// �d�͏���
		pEffePram[nCntPrame].type             = TYPE_NONE;							// ���
		pEffePram[nCntPrame].Vibrat           = VIBRATION();						// �U��

		
		if (pEffePram[nCntPrame].Vibrat.bRandDist == false)
		{
			pEffePram[nCntPrame].Vibrat.fDist = m_VibrationOption.nMaxDist * m_VibrationOption.fScalDistValue;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Update(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;				// ���_���̃|�C���^
	PARAMETER* pEffePram;			// �p�����[�^�|�C���^
	pEffePram = &m_EffectPram[0];	// �|�C���^�̏�����

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrame = 0; nCntPrame < _3DEFFE_USEQUANTITY; nCntPrame++, pVtx += _3DEFFE_USEVERTEX)
	{
		// �g�p����Ă��Ȃ���
		if (pEffePram[nCntPrame].bUse == false)
		{// �������X�L�b�v
			continue;
		}
		// �G�t�F�N�g�̃��C�t��0�ȉ��ɂȂ����珈���𔲂���
		if (pEffePram[nCntPrame].nLife <= MYLIB_INT_UNSET)
		{// �g�p�t���O���I�t�ɂ���
			pEffePram[nCntPrame].bUse = false;
			// �������X�L�b�v
			continue;
		}
		//�@���C�t�����炷
		pEffePram[nCntPrame].nLife--;

		// �d�͂�������
		pEffePram[nCntPrame].move.y += pEffePram[nCntPrame].fGravity;

		// ��������
		CMylibrary::SlowingMove(&pEffePram[nCntPrame].move, 0.2f);

		// �ړ��ʂ����Z
		pEffePram[nCntPrame].Trans.pos += pEffePram[nCntPrame].move;

		// �����^�C�v�̎�
		if (pEffePram[nCntPrame].type == TYPE::CHAR)
		{
			// �U���̍X�V����
			pEffePram[nCntPrame].Vibrat.Update();
#ifdef _DEBUG
			CDebugProc::Print("�G�t�F�N�g�U������\n");
#endif
		}

		// ���a��ω�������
		pEffePram[nCntPrame].fRadius += pEffePram[nCntPrame].fRadiusValue;

		// ���a��0.0f�ȉ��̎�
		if (pEffePram[nCntPrame].fRadius <= 0.0f)
		{// ���a��0.0f�ɌŒ�
			pEffePram[nCntPrame].fRadius = 0.0f;
			// �g�p�t���O���I�t�ɂ���
			pEffePram[nCntPrame].bUse = false;
			// ���_�T�C�Y�̐ݒ�
			SetVartexSize(pVtx, pEffePram[nCntPrame]);
		}
		// ���_�T�C�Y�̐ݒ�
		SetVartexSize(pVtx, pEffePram[nCntPrame]);

		// �A���t�@�l��ω�������
		pEffePram[nCntPrame].col.a -= pEffePram[nCntPrame].fAlphaValue;

		// �A���t�@�l��0.0f�ȉ��̎�
		if (pEffePram[nCntPrame].col.a <= 0.0f)
		{// �A���t�@�l��0.0f�ɌŒ�
			pEffePram[nCntPrame].col.a = 0.0f;
			// �g�p�t���O���I�t�ɂ���
			pEffePram[nCntPrame].bUse = false;
			// ���_�J���[�̐ݒ�
			SetVetexColor(pVtx, pEffePram[nCntPrame]);
		}
		// ���_�J���[�̐ݒ�
		SetVetexColor(pVtx, pEffePram[nCntPrame]);
	}

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxTrans, mtxRot, mtxParent;					// �v�Z�p�}�g���b�N�X
	PARAMETER* pEffePram;							// �p�����[�^�|�C���^

	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	pEffePram = &m_EffectPram[0];					// �|�C���^�̏�����

	// ���C�e�B���O���[�h����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�o�b�t�@�@�L���@����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// ����(�����)���J�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntPrame = 0; nCntPrame < _3DEFFE_USEQUANTITY; nCntPrame++)
	{
		// �g�p����Ă��Ȃ����܂��́A�`��t���O�������Ă��Ȃ���
		if (pEffePram[nCntPrame].bUse == false ||
			pEffePram[nCntPrame].bDisp == false)
		{// �������X�L�b�v
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pEffePram[nCntPrame].Trans.mtxWorld);
		
		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR)
		{
			// �ʏ�u�����h
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR &&
			pEffePram[nCntPrame].bBillBoard == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxParent);
			// �r���{�[�h�̐ݒ�
			CMylibrary::SetBillboard(pDevice, &mtxParent);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans
				, pEffePram[nCntPrame].Origin.x
				, pEffePram[nCntPrame].Origin.y
				, pEffePram[nCntPrame].Origin.z);

			D3DXMatrixMultiply(&mtxParent
				, &mtxParent
				, &mtxTrans);
		}
		// �r���{�[�h�t���O�������Ă�����
		else if (pEffePram[nCntPrame].bBillBoard == true)
			{// �r���{�[�h�̐ݒ�
				CMylibrary::SetBillboard(pDevice, &pEffePram[nCntPrame].Trans.mtxWorld);
			}

		if (pEffePram[nCntPrame].bBillBoard == false)
		{
			// �ʒu�𔽉f
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				pEffePram[nCntPrame].Trans.rot.y,
				pEffePram[nCntPrame].Trans.rot.x,
				pEffePram[nCntPrame].Trans.rot.z);

			D3DXMatrixMultiply(
				&pEffePram[nCntPrame].Trans.mtxWorld,
				&pEffePram[nCntPrame].Trans.mtxWorld,
				&mtxRot);
		}

		// �ʒu�𔽉f
		D3DXMatrixTranslation(
			&mtxTrans,
			pEffePram[nCntPrame].Trans.pos.x,
			pEffePram[nCntPrame].Trans.pos.y,
			pEffePram[nCntPrame].Trans.pos.z);

		D3DXMatrixMultiply(
			&pEffePram[nCntPrame].Trans.mtxWorld,
			&pEffePram[nCntPrame].Trans.mtxWorld,
			&mtxTrans);

		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR)
		{
			// �}�g���b�N�X�̂���Ɗ|����
			D3DXMatrixMultiply(&pEffePram[nCntPrame].Trans.mtxWorld
				, &pEffePram[nCntPrame].Trans.mtxWorld
				, &mtxParent);
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pEffePram[nCntPrame].Trans.mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexInfo[pEffePram[nCntPrame].nTexType]);
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPrame * _3DEFFE_USEVERTEX, 2);

		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR)
		{
			// �����_�[�X�e�[�g(���Z��������)
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
	}

	//�A���t�@�e�X�g�߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Z�o�b�t�@�@�L���@����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// ����(�����)���J�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-------------------------------------------------------------------------------------------------------------
// �ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Set(SETINGPARAM & Seting)
{
	// �ϐ��錾
	PARAMETER* pEffePram;							// �p�����[�^�|�C���^
	pEffePram = &m_EffectPram[0];					// �|�C���^�̏�����

	// �ő吔���[�v
	for (int nCntEffect = 0; nCntEffect < _3DEFFE_USEQUANTITY; nCntEffect++)
	{
		// �g�p�t���O�I���̌̂̎�
		if (pEffePram[nCntEffect].bUse == true)
		{// �������X�L�b�v
			continue;
		}
		// �ϐ���`
		VERTEX_3D *pVtx;		// ���_�|�C���^
		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_�|�C���^�̍X�V
		pVtx += nCntEffect * 4;

		// �e�̈ʒu�̐ݒ�
		pEffePram[nCntEffect].pParent = Seting.pParent;
		// �`��t���O�𗧂Ă�
		pEffePram[nCntEffect].bDisp = true;
		// �r���{�[�h�̐ݒ�
		pEffePram[nCntEffect].bBillBoard = Seting.bBillBoard;
		if (pEffePram[nCntEffect].bBillBoard == false)
		{
			pEffePram[nCntEffect].Trans.rot.y = atan2f(Seting.move.x, Seting.move.z);
			pEffePram[nCntEffect].Trans.rot.x = atan2f(Seting.move.y, Seting.move.z);
			pEffePram[nCntEffect].Trans.rot.z = atan2f(Seting.move.y, Seting.move.x);
		}

		// �d�͂̐ݒ�
		pEffePram[nCntEffect].fGravity = Seting.fGravity;
		// ���C�t�̐ݒ�
		pEffePram[nCntEffect].nLife = Seting.nLife;
		// �e�N�X�`����ނ̐ݒ�
		pEffePram[nCntEffect].nTexType = Seting.nTexType;
		// �G�t�F�N�g��ނ̐ݒ�
		pEffePram[nCntEffect].type = (C3DEffect::TYPE)Seting.type;
		// �ړ��ʂ̐ݒ�
		pEffePram[nCntEffect].move = Seting.move;
		// ���a�̐ݒ�
		pEffePram[nCntEffect].fRadius = Seting.fRadius;
		// �ʒu�̐ݒ�
		pEffePram[nCntEffect].Trans.pos = Seting.pos;
		// �F�̐ݒ�
		pEffePram[nCntEffect].col = Seting.col;
		// ���_�̐ݒ�
		pEffePram[nCntEffect].Origin = Seting.Origin;

		if (pEffePram[nCntEffect].type == TYPE::CHAR)
		{
			pEffePram[nCntEffect].Vibrat.bRandDist = true;
			// ���a�ω��l�̐ݒ�
			pEffePram[nCntEffect].fRadiusValue = 0.0f;
			// �A���t�@�ω��l�̐ݒ�
			pEffePram[nCntEffect].fAlphaValue = 0.0f;
		}
		else
		{
			pEffePram[nCntEffect].Vibrat.bRandDist = false;
			// �A���t�@�ω��l�̐ݒ�
			pEffePram[nCntEffect].fAlphaValue = pEffePram[nCntEffect].col.a / pEffePram[nCntEffect].nLife;

			// ���a�ω��l�̐ݒ�
			//pEffePram[nCntEffect].fRadiusValue = -(pEffePram[nCntEffect].fRadius / pEffePram[nCntEffect].nLife);
			pEffePram[nCntEffect].fRadiusValue = Seting.fRadiusValue;
		}
		SetVartexSize(pVtx, pEffePram[nCntEffect]);
		SetVetexColor(pVtx, pEffePram[nCntEffect]);

		// �g�p�t���O���I��
		pEffePram[nCntEffect].bUse = true;
		//���_�f�[�^���A�����b�N
		m_pVtxBuff->Unlock();
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �e�N�X�`���̍쐬
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::CreateTexture(CONST_STRING SetingStr, const int &nCntInfo, LPDIRECT3DDEVICE9 pDevice)
{
	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(pDevice, SetingStr, &m_pTexInfo[nCntInfo]) != D3D_OK)
	{
#ifdef _DEBUG
		MessageBox(NULL, "�e�N�X�`���̐����Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
		cout << nCntInfo << "�ڂ�3D�G�t�F�N�g�̃e�N�X�`���̐����Ɏ��s���܂����B";
#endif
	}
}

//-------------------------------------------------------------------------------------------------------------
// ���_�̍쐬
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * _3DEFFE_USEVERTEX * _3DEFFE_USEQUANTITY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < _3DEFFE_USEQUANTITY; nCntEffect++, pVtx += _3DEFFE_USEVERTEX)
	{
		pVtx[0].pos = MYLIB_3DVECTOR_ZERO;
		pVtx[1].pos = MYLIB_3DVECTOR_ZERO;
		pVtx[2].pos = MYLIB_3DVECTOR_ZERO;
		pVtx[3].pos = MYLIB_3DVECTOR_ZERO;
		// �@���x�N�g��
		pVtx[0].nor = MYLIB_VEC3_ZONE;
		pVtx[1].nor = MYLIB_VEC3_ZONE;
		pVtx[2].nor = MYLIB_VEC3_ZONE;
		pVtx[3].nor = MYLIB_VEC3_ZONE;
		// ���_�J���[
		pVtx[0].col = MYLIB_D3DXCOR_UNSET;
		pVtx[1].col = MYLIB_D3DXCOR_UNSET;
		pVtx[2].col = MYLIB_D3DXCOR_UNSET;
		pVtx[3].col = MYLIB_D3DXCOR_UNSET;
		// texture���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;

}

//-------------------------------------------------------------------------------------------------------------
// ���_�T�C�Y�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVartexSize(VERTEX_3D * pVtx, PARAMETER & Effect)
{
	// ���_�̐ݒ�
	pVtx[0].pos.x = Effect.Vibrat.Pos.x + -sinf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
	pVtx[0].pos.y = Effect.Vibrat.Pos.y + -cosf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
	pVtx[1].pos.x = Effect.Vibrat.Pos.x + -sinf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[1].pos.y = Effect.Vibrat.Pos.y + cosf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[2].pos.x = Effect.Vibrat.Pos.x + sinf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[2].pos.y = Effect.Vibrat.Pos.y + -cosf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[3].pos.x = Effect.Vibrat.Pos.x + sinf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
	pVtx[3].pos.y = Effect.Vibrat.Pos.y + cosf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// ���_�J���[�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVetexColor(VERTEX_3D * pVtx, PARAMETER & Effect)
{
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = Effect.col;
}

//-------------------------------------------------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::Update(void)
{
	// �����x�N�g���̎擾
	this->GetVecDir();
	// �����̎擾
	this->GetDist();
	// �ʒu�̌v�Z
	this->CalPos();
}

//-------------------------------------------------------------------------------------------------------------
// �����x�N�g���̎擾
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::GetVecDir(void)
{
	// ���E��
	int nOffsetting = m_VibrationOption.nMaxDiviDir * 2;

	this->VecDir.x = ((rand() % nOffsetting) - m_VibrationOption.nMaxDiviDir)*m_VibrationOption.fScalDirValue;
	this->VecDir.y = ((rand() % nOffsetting) - m_VibrationOption.nMaxDiviDir)*m_VibrationOption.fScalDirValue;

	// ���K������
	CMylibrary::CreateUnitVector(&VecDir, &VecDir);
}

//-------------------------------------------------------------------------------------------------------------
// �����̎擾
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::GetDist(void)
{
	if (bRandDist == true)
	{
		this->fDist = (rand() % m_VibrationOption.nMaxDist) * m_VibrationOption.fScalDistValue;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �ʒu�̌v�Z
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::CalPos(void)
{
	this->Pos = this->VecDir * this->fDist;
}
