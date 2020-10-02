//=============================================================================
//
// ���f���̃p�[�c����  [ modelParts.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "modelparts.h"
#include "renderer.h"
#include "manager.h"
#include "Inputkeyboard.h"
#include "ImGui/imgui.h"
#include "debugproc.h"
#include "player.h"
#include "sceneX.h"
#include "kananlibrary.h"

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelParts::CModelParts()
{
	//������
	m_nIndex = 0;
	m_nParent = 0;
	m_pos = ZeroVector3;
	m_rot = ZeroVector3;
	m_OffsetPos = ZeroVector3;
	m_OffsetRot = ZeroVector3;
	m_UpdatePos = ZeroVector3;
	m_UpdateRot = ZeroVector3;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelParts::~CModelParts()
{

}

//=============================================================================
// ������
//=============================================================================
void CModelParts::Init(void)
{

}

//=============================================================================
// �I��
//=============================================================================
void CModelParts::Uninit(void)
{

}

//=============================================================================
//�X�V
//=============================================================================
void CModelParts::Update(void)
{
	
}

//=============================================================================
// �`��
//=============================================================================
void CModelParts::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL	*pMat;
	D3DMATERIAL9	matDef;

	// ���[���h�}�g���b�N�X�̌v�Z
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// �e�̏��������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_mtxParent);

	// �e�N�X�`���̐ݒ�
	if (m_pModelInfo->bTex)
		pDevice->SetTexture(0, m_pModelInfo->pTexture);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	// ���_�o�b�t�@�̌^���X�g���[���ɓ`����
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		if (pMat[nCntMat].MatD3D.Diffuse.a != 1.0f)
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �`��
		m_pModelInfo->mesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ���G�`��
//=============================================================================
void CModelParts::DrawInvincible(int nInvincibleTime)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL	*pMat;
	D3DMATERIAL9	matDef;

	// ���[���h�}�g���b�N�X�̌v�Z
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// �e�̏��������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_mtxParent);

	// �e�N�X�`���̐ݒ�
	if (m_pModelInfo->bTex)
		pDevice->SetTexture(0, m_pModelInfo->pTexture);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@�̌^���X�g���[���ɓ`����
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		// �_��
		if (nInvincibleTime % 3 == 0)
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
		else if (nInvincibleTime % 3 == 1)
			pMat[nCntMat].MatD3D.Diffuse.a = 0.8f;
		else if (nInvincibleTime % 3 == 2)
			pMat[nCntMat].MatD3D.Diffuse.a = 0.6f;
		else if (nInvincibleTime % 3 == 3)
			pMat[nCntMat].MatD3D.Diffuse.a = 0.4f;
		else if (nInvincibleTime % 3 == 4)
			pMat[nCntMat].MatD3D.Diffuse.a = 0.2f;

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
void CModelParts::DrawShadow(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef, matBlack;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	matBlack = matDef;

	// �F�̐ݒ�
	matBlack.Diffuse = BlackColor;

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

//=============================================================================
// �����ȏ��Z�b�g (�e�N�X�`���t��)
//=============================================================================
void CModelParts::SetPartsTexInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture)
{
	m_OffsetPos = pos;
	m_OffsetRot = rot;
	m_pos = pos;
	m_rot = rot;
	m_nParent = nParent;
	m_nIndex = nIndex;
	m_pTexture = pTexture;
}

//=============================================================================
// �����ȏ��Z�b�g
//=============================================================================
void CModelParts::SetPartsInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_OffsetPos = pos;
	m_OffsetRot = rot;
	m_pos = pos;
	m_rot = rot;
	m_nParent = nParent;
	m_nIndex = nIndex;
}
