//=============================================================================
//
// ���Ă�I�u�W�F�N�g���� [ objectLift.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "objectLift.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "objManager.h"
#include "3DBoxCollider.h"

#include "ImGui/imgui.h"				// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"		// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"		// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================
#ifdef _DEBUG
#define COLOR_ALPHA		(0.5f)	// �����ȃJ���[
#endif

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObjectLift::CObjectLift()
{
	// �v�f�̏�����
	m_pos			= ZeroVector3;
	m_posOld		= ZeroVector3;
	m_posBegin		= ZeroVector3;
	m_move			= ZeroVector3;
	m_rot			= ZeroVector3;
	m_rotBegin		= ZeroVector3;
	m_nColliderID	= -1;

#ifdef _DEBUG
	m_bRelease = false;
#endif
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectLift::~CObjectLift()
{

}

//=============================================================================
// ������
//=============================================================================
void CObjectLift::Init(void)
{
}

//=============================================================================
// �I��
//=============================================================================
void CObjectLift::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CObjectLift::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	// �ʒu�X�V
	m_pos += m_move;
}

//=============================================================================
// �`��
//=============================================================================
void CObjectLift::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �}�e���A���i�[�p
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	// �X�P�[���ҏW�p
	D3DXMATRIX mtxScale;

	// ���[���h�}�g���b�N�X�̌v�Z
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetTexture(0, NULL);

	// �e�N�X�`���̐ݒ�
	if (m_pModelInfo.bTex)
		pDevice->SetTexture(0, m_pModelInfo.pTexture);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelInfo.matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo.matNum; nCntMat++)
	{
		// �������ɂ���
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		// �`��
		m_pModelInfo.mesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

#ifdef _DEBUG
//=============================================================================
// �����`��
//=============================================================================
void CObjectLift::DrawAlpha(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �}�e���A���i�[�p
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	// �X�P�[���ҏW�p
	D3DXMATRIX mtxScale;

	// ���[���h�}�g���b�N�X�̌v�Z
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetTexture(0, NULL);

	// �e�N�X�`���̐ݒ�
	if (m_pModelInfo.bTex)
		pDevice->SetTexture(0, m_pModelInfo.pTexture);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelInfo.matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo.matNum; nCntMat++)
	{
		// �������ɂ���
		pMat[nCntMat].MatD3D.Diffuse.a = COLOR_ALPHA;
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		// �`��
		m_pModelInfo.mesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}
#endif

//=============================================================================
// ����
//=============================================================================
CObjectLift * CObjectLift::Create(void)
{
	// �������m��
	CObjectLift *pObject = new CObjectLift();

	// nullcheck
	if (!pObject)
		// ���s
		return nullptr;

	// ������
	pObject->Init();

	// �l��Ԃ�
	return pObject;
}

//=============================================================================
// �I�u�W�F�N�g�̏��ݒ�
//=============================================================================
void CObjectLift::SetObjInfo(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, MODELINFO *pModelInfo, const int & type)
{
	m_pos			= pos;
	m_posBegin		= pos;
	m_rot			= rot;
	m_rotBegin		= rot;
	m_pModelInfo	= *pModelInfo;
	m_nType			= type;

	// �R���C�_�[�̐ݒ�
	SetCollider();
}

//=============================================================================
// �R���C�_�[�̐ݒ�
//=============================================================================
void CObjectLift::SetCollider(void)
{
	// �ݒ肳��Ă���Ƃ�
	if (m_nColliderID != -1)
	{// �����𔲂���
		return;
	}
	// �R���C�_�[ID�̐ݒ�
	m_nColliderID = C3DBoxCollider::SetColliderInfo(&m_pos, NULL, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::TOP_OBJECT + CObjectManager::GetObjTypeAll() + m_nType);
}

#ifdef _DEBUG
//=============================================================================
// ImGui�̍X�V
//=============================================================================
void CObjectLift::ShowObjectInfo(char cPrintText[16])
{
	// �c���[
	if (ImGui::TreeNode(cPrintText))
	{
		// �U�I�u�W�F�N�g�ȊO
		if (cPrintText != "FakeObject")
			ImGui::Text("type : %d", m_nType);

		if (ImGui::DragFloat3("pos", m_pos, 0.5f))
		{
			m_posBegin = m_pos;
		}
		ImGui::DragFloat3("rot", m_rot, 0.05f, -D3DX_PI, D3DX_PI);

		// �U�I�u�W�F�N�g�ȊO
		if (cPrintText != "FakeObject")
		{
			// ���s�����ɊԊu��������
			ImGui::Dummy(ImVec2(200.0f, 0.0f));
			ImGui::SameLine();

			// �폜
			if (ImGui::Button("delete"))
				// �����[�X����
				m_bRelease = true;
		}

		// �c���[�̍Ō�ɕK������
		ImGui::TreePop();
	}
}
#endif