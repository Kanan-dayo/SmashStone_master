//==================================================================================================================
//
// X�t�@�C���̕`��[sceneX.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "manager.h"
#include "sceneX.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "debugProc.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define COLOR_SHADOW		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	// �e�̃J���[

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CSceneX::CSceneX()
{
}

CSceneX::CSceneX(PRIORITY type) : CScene(type)
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CSceneX::~CSceneX()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CSceneX::Init(void)
{
	// ������
	m_pos		= ZeroVector3;	// �ʒu
	m_move		= ZeroVector3;	// �ړ���
	m_size		= OneVector3;	// �傫��
	m_rot		= ZeroVector3;	// ��]
	m_vecAxis	= ZeroVector3;	// ��]��
	m_fValueRot = 0.0f;			// ��]�p�i��]�ʁj
	m_bParent	= false;		// �e����
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CSceneX::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CSceneX::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	// ��]������������␳����
	CKananLibrary::InterpolationFloat(m_fValueRot);

	// �ʒu�X�V
	m_pos += m_move;
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CSceneX::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();								// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();							// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;										// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL *pMat;															// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;														// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mtxShadow;														// �V���h�E�}�g���b�N�X
	D3DXPLANE planeField;														// ���ʕϐ�
	D3DXVECTOR4 vecLight;														// ���C�g�̕����x�N�g��
	D3DXVECTOR3 pos, normal;													// �e�̈ʒu�A�@��

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	// �L�����N�^�[�̉e�`��
	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �`��
		m_pModelInfo->mesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pModelInfo->pTexture);

	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

}

//==================================================================================================================
// ���b�V���̕`��
//==================================================================================================================
void CSceneX::DrawMesh(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̌v�Z
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	//�e�̏��������
	if (m_bParent)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_mtxParent);

	// �e�N�X�`���̐ݒ�
	if (m_pModelInfo->bTex)
		pDevice->SetTexture(0, m_pModelInfo->pTexture);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �`��
		m_pModelInfo->mesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}


//=============================================================================
// �e�̕`��
//=============================================================================
void CSceneX::DrawShadow(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef, matBlack;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	matBlack = matDef;

	// �F�̐ݒ�
	matBlack.Diffuse = COLOR_SHADOW;

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	// ========================�X�e���V��============================
	// [�s�N�Z���P�ʂŎQ�ƒl��ݒ肵�A								]
	// [���̒l�Ə������r���ăs�N�Z���̕`����s�������f�����@	]
	// ==============================================================

	// �Q�ƒl	: D3DRS_STENCILPASS						(�X�e���V���e�X�g�EZ�e�X�g�𗼕��������Ă����� +1)
	// ����		: D3DRS_STENCILREF, D3DRS_STENCILFUNC	(�Q�ƒl��0�̂݃X�e���V���e�X�g���i)
	// ����āA�Q�ƒl = 0 ���ƕ`�悷��					(�X�e���V���e�X�g�����i���Ă���s�N�Z���̂݁A�`�悳���)

	// �X�e���V���̒l��0�ɂ���
	//pDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);
	//
	//pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);				// �X�e���V���e�X�g�L��
	//pDevice->SetRenderState(D3DRS_STENCILREF, 0);					// �X�e���V���̏����̒l
	//pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);		// (EQUAL : �����Ɠ���)

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		// �C���N�������g
		//pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matBlack);

		// �`��
		m_pModelInfo->mesh->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �X�e���V���e�X�g����
	//pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==================================================================================================================
// ��`�̓����蔻��
//==================================================================================================================
bool CSceneX::SetCollisionBox(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 posOld)
{
	bool bHit = false;

	// ���f���̉����͈͓̔��ɂ���Ƃ�
	if (m_pos.x - size.x < posOld.x && m_pos.x + size.x > posOld.x)
	{
		// �����烂�f���̂Ԃ�����
		if (m_pos.z - size.z > posOld.z && m_pos.z - size.z <= pos.z)
		{
			// �����������Ƃɂ���
			bHit = true;
		}
	}

	return bHit;
}

//==================================================================================================================
// ��������
//==================================================================================================================
CSceneX *CSceneX::Create(void)
{
	// �V�[�����I�Ɋm��
	CSceneX *pSceneX = new CSceneX(CScene::PRIORITY_FIELD);

	// �V�[��������
	pSceneX->Init();

	// �l��Ԃ�
	return pSceneX;
}