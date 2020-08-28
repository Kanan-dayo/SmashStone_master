//=============================================================================
//
// �I�u�W�F�N�g���� [ object.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "object.h"
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
#define COLOR_ALPHA		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))	// �����ȃJ���[
#endif

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject::CObject()
{
	// �v�f�̏�����
	m_bCollision	= false;
	m_pos			= ZeroVector3;
	m_posOld		= ZeroVector3;
	m_posBegin		= ZeroVector3;
	m_move			= ZeroVector3;
	m_rot			= ZeroVector3;
	m_rotBegin		= ZeroVector3;
	m_scale			= OneVector3;
	m_nColliderID	= -1;
#ifdef _DEBUG
	m_bRelease = false;
#endif
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// ������
//=============================================================================
void CObject::Init(void)
{
}

//=============================================================================
// �I��
//=============================================================================
void CObject::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CObject::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	// �ʒu�X�V
	m_pos += m_move;
}

//=============================================================================
// �`��
//=============================================================================
void CObject::Draw(void)
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

	// �X�P�[���̌v�Z
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

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
void CObject::DrawAlpha(void)
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

	// �X�P�[���̌v�Z
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

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
		pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
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
CObject * CObject::Create(void)
{
	// �������m��
	CObject *pObject = new CObject();

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
void CObject::SetObjInfo(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const D3DXVECTOR3 & scale, MODELINFO *pModelInfo, const int & type)
{
	m_pos = pos;
	m_posBegin = pos;
	m_rot = rot;
	m_rotBegin = rot;
	m_scale = scale;
	m_pModelInfo = *pModelInfo;
	m_nType = type;

	// �R���C�_�[�̐ݒ�
	SetCollider();
}

//=============================================================================
// �I�u�W�F�N�g�̓����蔻��
//=============================================================================
bool CObject::CollObject(D3DXVECTOR3 *pos, const D3DXVECTOR3 & posOld, D3DXVECTOR3 *move, const MODEL_VTX & modelVtx)
{
	bool bLand = false;
	MODEL_VTX	Vtx = CObjectManager::GetModelVtx(m_nType);

	if (pos->x + modelVtx.VtxMax.x > m_pos.x + Vtx.VtxMin.x && pos->x + modelVtx.VtxMin.x < m_pos.x + Vtx.VtxMax.x &&
		pos->y + modelVtx.VtxMax.y >= m_pos.y + Vtx.VtxMin.y && pos->y + modelVtx.VtxMin.y < m_pos.y + Vtx.VtxMax.y)
	{
		// �u���b�N�O���̔���
		if (pos->z + modelVtx.VtxMax.z >= m_pos.z + Vtx.VtxMin.z && posOld.z + modelVtx.VtxMax.z <= m_posOld.z + Vtx.VtxMin.z)
		{
			pos->z = m_pos.z + Vtx.VtxMin.z - modelVtx.VtxMax.z;
			move->z = 0.0f;
		}
		// �u���b�N�㕔�̔���
		if (pos->z + modelVtx.VtxMin.z <= m_pos.z + Vtx.VtxMax.z && posOld.z + modelVtx.VtxMin.z >= m_posOld.z + Vtx.VtxMax.z)
		{
			pos->z = m_pos.z + Vtx.VtxMax.z - modelVtx.VtxMin.z;
			move->z = 0.0f;
		}
	}

	if (pos->z + modelVtx.VtxMax.z > m_pos.z + Vtx.VtxMin.z && pos->z + modelVtx.VtxMin.z < m_pos.z + Vtx.VtxMax.z &&
		pos->y + modelVtx.VtxMax.y >= m_pos.y + Vtx.VtxMin.y && pos->y + modelVtx.VtxMin.y < m_pos.y + Vtx.VtxMax.y)
	{
		// �u���b�N�����̔���
		if (pos->x + modelVtx.VtxMax.x >= m_pos.x + Vtx.VtxMin.x && posOld.x + modelVtx.VtxMax.x <= m_posOld.x + Vtx.VtxMin.x)
		{
			pos->x = m_pos.x + Vtx.VtxMin.x - modelVtx.VtxMax.x;
			move->x = 0.0f;
		}
		// �u���b�N�E���̔���
		if (pos->x + modelVtx.VtxMin.x <= m_pos.x + Vtx.VtxMax.x && posOld.x + modelVtx.VtxMin.x >= m_posOld.x + Vtx.VtxMax.x)
		{
			pos->x = m_pos.x + Vtx.VtxMax.x - modelVtx.VtxMin.x;
			move->x = 0.0f;
		}
	}

	if (pos->x + modelVtx.VtxMax.x > m_pos.x + Vtx.VtxMin.x && pos->x + modelVtx.VtxMin.x < m_pos.x + Vtx.VtxMax.x &&
		pos->z + modelVtx.VtxMax.z > m_pos.z + Vtx.VtxMin.z && pos->z + modelVtx.VtxMin.z < m_pos.z + Vtx.VtxMax.z)
	{
		// �u���b�N�㕔�̔���
		if (pos->y + modelVtx.VtxMin.y <= m_pos.y + Vtx.VtxMax.y && posOld.y + modelVtx.VtxMin.y >= m_posOld.y + Vtx.VtxMax.y)
		{
			pos->y = m_pos.y + Vtx.VtxMax.y - modelVtx.VtxMin.y;
			move->y = 0.0f;
			// ��ɍڂ��Ă���
			bLand = true;
		}
		// �u���b�N�����̔���
		if (pos->y + modelVtx.VtxMax.y >= m_pos.y + Vtx.VtxMin.y && posOld.y + modelVtx.VtxMax.y <= m_posOld.y + Vtx.VtxMin.y)
		{
			pos->y = m_pos.y + Vtx.VtxMin.y - modelVtx.VtxMax.y;
			move->y = 0.0f;
		}
	}

	return bLand;
}

//=============================================================================
// �R���C�_�[�̐ݒ�
//=============================================================================
void CObject::SetCollider(void)
{
	// �ݒ肳��Ă���Ƃ�
	if (m_nColliderID != -1)
	{// �����𔲂���
		return;
	}
	// �R���C�_�[ID�̐ݒ�
	m_nColliderID = C3DBoxCollider::SetColliderInfo(&m_pos, NULL, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::TOP_OBJECT + m_nType);
}

#ifdef _DEBUG
//=============================================================================
// ImGui�̍X�V
//=============================================================================
void CObject::ShowObjectInfo(char cPrintText[16])
{
	// �c���[
	if (ImGui::TreeNode(cPrintText))
	{
		// �U�I�u�W�F�N�g�ȊO
		if (cPrintText != "FakeObject")
			ImGui::Text("type : %d", m_nType);

		ImGui::DragFloat3("pos", m_pos, 0.5f);
		ImGui::DragFloat3("rot", m_rot, 0.05f, -D3DX_PI, D3DX_PI);
		ImGui::DragFloat3("scale", m_scale, 0.05f, 0.0f, 100.0f);

		//ImGui::Checkbox("Collision", &m_bCollision);

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