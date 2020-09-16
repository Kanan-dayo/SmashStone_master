//=============================================================================
//
//	���C�u�������� [ kananlibrary.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "kananlibrary.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "light.h"
#include "game.h"
#include "ImGui/imgui.h"				// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"		// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"		// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �\���̒�`
//=============================================================================

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
#ifdef _DEBUG
bool CKananLibrary::m_WireFrame = false;
int CKananLibrary::m_nCulling = 0;
#endif

//=============================================================================
// �}�g���b�N�X�v�Z
//=============================================================================
void CKananLibrary::CalcMatrix(D3DXMATRIX * pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot)
{
	D3DXMATRIX	mtxRot, mtxTrans;			//�v�Z�p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pMtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);
}

//=============================================================================
// �e�̃}�g���b�N�X�v�Z
//=============================================================================
void CKananLibrary::CalcShadowMatrix(D3DXMATRIX & mtxShadow, D3DXVECTOR3 const & pos, D3DXVECTOR3 const & nor)
{
	D3DXPLANE planeField;
	D3DXVECTOR3 LightVec = CManager::GetRenderer()->GetGame()->GetLight()->GetLightVec(0);	// ���C�g�̎擾
	D3DXVECTOR4 ShadowVec = ZeroVector4;

	// �}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// ���Ƌt�����Ƀx�N�g����ݒ�
	ShadowVec = D3DXVECTOR4(-LightVec.x, -LightVec.y, -LightVec.z, 0);

	// ���ʂ̍��W�����߂�
	D3DXPlaneFromPointNormal(&planeField, &pos, &nor);

	// �e�̃}�g���b�N�X������
	D3DXMatrixShadow(&mtxShadow, &ShadowVec, &planeField);
}

//=============================================================================
// ���b�V�����璸�_���W�̍ő�ŏ����o��
//=============================================================================
MODEL_VTX CKananLibrary::OutputModelVtxColl(LPD3DXMESH mesh)
{
	int			nNumVertices = mesh->GetNumVertices();					// ���_���擾
	DWORD		sizeFVF = D3DXGetFVFVertexSize(mesh->GetFVF());		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	BYTE		*pVertexBuffer;	// ���_�o�b�t�@�̃|�C���^
	MODEL_VTX	outVtx;			// �o�͂��钸�_���

								// ���_�o�b�t�@�����b�N
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

	// ���_�����J��Ԃ�
	for (int nCntVtx = 0; nCntVtx < nNumVertices; nCntVtx++)
	{
		// �o�b�t�@���璸�_���W���擾
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// �S�Ă̒��_�����r���āA�ŏ��l�E�ő�l�𔲂��o��
		// �ŏ��l
		if (outVtx.VtxMin.x > vtx.x)
			outVtx.VtxMin.x = vtx.x;
		if (outVtx.VtxMin.y > vtx.y)
			outVtx.VtxMin.y = vtx.y;
		if (outVtx.VtxMin.z > vtx.z)
			outVtx.VtxMin.z = vtx.z;
		// �ő�l
		if (outVtx.VtxMax.x < vtx.x)
			outVtx.VtxMax.x = vtx.x;
		if (outVtx.VtxMax.y < vtx.y)
			outVtx.VtxMax.y = vtx.y;
		if (outVtx.VtxMax.z < vtx.z)
			outVtx.VtxMax.z = vtx.z;

		// �T�C�Y���|�C���^��i�߂�
		pVertexBuffer += sizeFVF;
	}

	// ���_�o�b�t�@���A�����b�N
	mesh->UnlockVertexBuffer();

	// �l��Ԃ�
	return outVtx;
}

//=============================================================================
// ���f�����̔j��
//=============================================================================
void CKananLibrary::ReleaseModelInfo(MODELINFO *pModelInfo)
{
	// �o�b�t�@�̊J��
	if (pModelInfo->matBuff)
	{
		pModelInfo->matBuff->Release();
		pModelInfo->matBuff = nullptr;
	}
	// ���b�V���̊J��
	if (pModelInfo->mesh)
	{
		pModelInfo->mesh->Release();
		pModelInfo->mesh = nullptr;
	}
	if (pModelInfo->pTexture)
	{
		//pModelInfo->pTexture->Release();
		pModelInfo->pTexture = nullptr;
	}

	// �}�e���A�����̏�����
	pModelInfo->matNum = 0;
}

//=============================================================================
// ��]�̕��
//=============================================================================
void CKananLibrary::InterpolationRot(D3DXVECTOR3 *rot)
{
	// 3.14�ȏ�
	if (rot->x > D3DX_PI)
	{
		rot->x -= D3DX_PI * 2;
	}
	// 3.14�ȉ�
	if (rot->x < -D3DX_PI)
	{
		rot->x += D3DX_PI * 2;
	}
	// 3.14�ȏ�
	if (rot->y > D3DX_PI)
	{
		rot->y -= D3DX_PI * 2;
	}
	// 3.14�ȉ�
	if (rot->y < -D3DX_PI)
	{
		rot->y += D3DX_PI * 2;
	}
	// 3.14�ȏ�
	if (rot->z > D3DX_PI)
	{
		rot->z -= D3DX_PI * 2;
	}
	// 3.14�ȉ�
	if (rot->z < -D3DX_PI)
	{
		rot->z += D3DX_PI * 2;
	}
}

//=============================================================================
// float�^�̉�]�̕��
//=============================================================================
void CKananLibrary::InterpolationFloat(float & Float)
{
	// 3.14�ȏ�
	if (Float > D3DX_PI)
	{
		Float -= D3DX_PI * 2;
	}
	// 3.14�ȉ�
	if (Float < -D3DX_PI)
	{
		Float += D3DX_PI * 2;
	}
}

//=============================================================================
// �������v�Z
//=============================================================================
float CKananLibrary::OutputSqrt(D3DXVECTOR3 difpos)
{
	float fSqrt = sqrt(difpos.x * difpos.x + difpos.y * difpos.y + difpos.z * difpos.z);
	return fSqrt;
}

//=============================================================================
// �����v�Z
//=============================================================================
float CKananLibrary::OutputDistance(D3DXVECTOR3 difpos)
{
	return difpos.x * difpos.x + difpos.y * difpos.y + difpos.z * difpos.z;
}

//=============================================================================
// �x�N�g���v�Z
//=============================================================================
D3DXVECTOR3 CKananLibrary::OutputVector(const D3DXVECTOR3 & start, const D3DXVECTOR3 & end)
{
	// �x�N�g����ۑ�����ϐ�
	D3DXVECTOR3 vector = end - start;
	// �x�N�g���𐳋K��
	D3DXVec3Normalize(&vector, &vector);
	// �x�N�g����Ԃ�
	return vector;
}

//=============================================================================
// ���s���Ȃ�ImGui�_�~�[
//=============================================================================
void CKananLibrary::SameLineDummy(const float & fX, const float & fY)
{
	// ���s����
	ImGui::SameLine();

	// �T�C�Y��ݒ肵�A�_�~�[�����
	ImVec2 size = ImVec2(fX, fY);
	ImGui::Dummy(size);

	// ���s����
	ImGui::SameLine();
}

//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CKananLibrary::CreateTexture(LPDIRECT3DTEXTURE9 * pTexture, char * cTextureName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �f�o�b�O�e�L�X�g�p
	char cText[MAX_TEXT];

	// �e�N�X�`������
	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice,
		cTextureName,
		pTexture)))
	{
		// ����
		sprintf(cText, "�e�N�X�`���𐶐� (%s)", cTextureName);
		printf("%s\n", &cText[0]);
		return S_OK;
	}

	// ���s
	sprintf(cText, "�e�N�X�`���̐������s (%s)", cTextureName);
	printf("%s\n", &cText[0]);
	return E_FAIL;
}

//=============================================================================
// �r���{�[�h������
//=============================================================================
void CKananLibrary::Billboard(D3DXMATRIX *mtxWorld, const D3DXMATRIX mtxView)
{
	mtxWorld->_11 = mtxView._11;
	mtxWorld->_12 = mtxView._21;
	mtxWorld->_13 = mtxView._31;
	mtxWorld->_21 = mtxView._12;
	mtxWorld->_22 = mtxView._22;
	mtxWorld->_23 = mtxView._32;
	mtxWorld->_31 = mtxView._13;
	mtxWorld->_32 = mtxView._23;
	mtxWorld->_33 = mtxView._33;
}

//=============================================================================
// �����̏���
//=============================================================================
void CKananLibrary::InertiaMove(D3DXVECTOR3 *move)
{
	move->x += (0 - move->x) * INERTIA;
	move->y += (0 - move->y) * INERTIA;
	move->z += (0 - move->z) * INERTIA;
}

void CKananLibrary::InertiaMoveXZ(D3DXVECTOR3 * move)
{
	move->x += (0 - move->x) * INERTIA;
	move->z += (0 - move->z) * INERTIA;
}

//=============================================================================
// �d�͂̏���
//=============================================================================
void CKananLibrary::Gravity(float & fMoveY)
{
	fMoveY -= GRAVITY;
}

//=============================================================================
// int�^�̏���ݒ�
//=============================================================================
void CKananLibrary::IntegerUpperLimit(int * nValue, int upperLimit)
{
	if (*nValue > upperLimit)
	{
		*nValue = upperLimit;
	}
}

//=============================================================================
// int�^�̉����ݒ�
//=============================================================================
void CKananLibrary::IntegerLowerLimit(int *nValue, int lowerLimit)
{
	if (*nValue < lowerLimit)
	{
		*nValue = lowerLimit;
	}
}

//=============================================================================
// float�^�̉����ݒ�
//=============================================================================
bool CKananLibrary::FloatLowerLimit(float * fValue, const float & lowerLimit)
{
	bool bLimit = false;

	if (*fValue < lowerLimit)
	{
		*fValue = lowerLimit;
		bLimit = true;
	}
	return bLimit;
}

//=============================================================================
// ��������̐�������
//=============================================================================
bool CKananLibrary::LimitVector3(D3DXVECTOR3 &Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit)
{
	bool bLimit = false;

	if (Value.x > upperLimit.x)
	{
		Value.x = upperLimit.x;
		bLimit = true;
	}
	if (Value.x < lowerLimit.x)
	{
		Value.x = lowerLimit.x;
		bLimit = true;
	}
	if (Value.y > upperLimit.y)
	{
		Value.y = upperLimit.y;
	}
	if (Value.y < lowerLimit.y)
	{
		Value.y = lowerLimit.y;
	}
	if (Value.z > upperLimit.z)
	{
		Value.z = upperLimit.z;
		bLimit = true;
	}
	if (Value.z < lowerLimit.z)
	{
		Value.z = lowerLimit.z;
		bLimit = true;
	}

	return bLimit;
}

bool CKananLibrary::LimitVector2(D3DXVECTOR2 & Value, const D3DXVECTOR2 lowerLimit, const D3DXVECTOR2 upperLimit)
{
	bool bLimit = false;

	if (Value.x > upperLimit.x)
	{
		Value.x = upperLimit.x;
		bLimit = true;
	}
	if (Value.x < lowerLimit.x)
	{
		Value.x = lowerLimit.x;
		bLimit = true;
	}
	if (Value.y > upperLimit.y)
	{
		Value.y = upperLimit.y;
	}
	if (Value.y < lowerLimit.y)
	{
		Value.y = lowerLimit.y;
	}

	return bLimit;
}

//=============================================================================
// ��������̃��[�v����
//=============================================================================
void CKananLibrary::LimitLoopVector3(D3DXVECTOR3 * Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit)
{
	if (Value->x > upperLimit.x)
	{
		Value->x = lowerLimit.x;
	}
	if (Value->x < lowerLimit.x)
	{
		Value->x = upperLimit.x;
	}
	if (Value->y > upperLimit.y)
	{
		Value->y = lowerLimit.y;
	}
	if (Value->y < lowerLimit.y)
	{
		Value->y = upperLimit.y;
	}
	if (Value->z > upperLimit.z)
	{
		Value->z = lowerLimit.z;
	}
	if (Value->z < lowerLimit.z)
	{
		Value->z = upperLimit.z;
	}
}

//=============================================================================
// �t�H�O�̐���
//=============================================================================
void CKananLibrary::SetFog(const float fStartPos, const float fEndPos, const D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);						// �t�H�Oon
	pDevice->SetRenderState(D3DRS_FOGCOLOR, col);						// �J���[�ݒ�
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);			// ���_���[�h
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);			// �e�[�u�����[�h
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStartPos));		// �J�n�ʒu
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEndPos));			// �I���ʒu
}

//=============================================================================
// �u���b�N�R�����g�g�̕\��
//=============================================================================
void CKananLibrary::PrintBlockCommentFrame(void)
{
#ifdef _DEBUG
	std::cout << "==================================================" << std::endl;
#endif
}

//=============================================================================
// �u���b�N�R�����g�̊J�n
//=============================================================================
void CKananLibrary::StartBlockComment(const std::string & str)
{
#ifdef _DEBUG
	PrintBlockCommentFrame();
	std::cout << str << std::endl;
#endif
}

//=============================================================================
// �u���b�N�R�����g�̏I��
//=============================================================================
void CKananLibrary::EndBlockComment(const std::string & str)
{
#ifdef _DEBUG
	std::cout << str << std::endl;
	PrintBlockCommentFrame();
#endif
}

//=============================================================================
// ��������̏���
//=============================================================================
void CKananLibrary::LimitColor(D3DXCOLOR & col, const D3DXCOLOR lowerLimit, const D3DXCOLOR upperLimit)
{
	if (col.r > upperLimit.r)
	{
		col.r = upperLimit.r;
	}
	if (col.r < lowerLimit.r)
	{
		col.r = lowerLimit.r;
	}
	if (col.g > upperLimit.g)
	{
		col.g = upperLimit.g;
	}
	if (col.g < lowerLimit.g)
	{
		col.g = lowerLimit.g;
	}
	if (col.b > upperLimit.b)
	{
		col.b = upperLimit.b;
	}
	if (col.b < lowerLimit.b)
	{
		col.b = lowerLimit.b;
	}
	if (col.a > upperLimit.a)
	{
		col.a = upperLimit.a;
	}
	if (col.a < lowerLimit.a)
	{
		col.a = lowerLimit.a;
	}
}

//=============================================================================
// ���f������
//=============================================================================
void CKananLibrary::CreateModelInfo(MODELINFO * pModelInfo)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���f������
	D3DXLoadMeshFromX(pModelInfo->cModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModelInfo->matBuff,
		NULL,
		&pModelInfo->matNum,
		&pModelInfo->mesh);
}

//=============================================================================
// �����
//=============================================================================
HRESULT CKananLibrary::Up(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetKeyboardTrigger(DIK_W))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetbConnect())
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX <= 1.0f && fValueZ > 0.0f)
			return S_OK;

		// �L�[����
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_UP))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CKananLibrary::Down(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetKeyboardTrigger(DIK_S))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetbConnect())
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX >= -1.0f && fValueZ < 0.0f)
			return S_OK;

		// �L�[����
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_DOWN))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CKananLibrary::Left(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetKeyboardTrigger(DIK_A))
		return TRUE;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetbConnect())
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX < 0 && fValueZ >= -1.0f)
			return TRUE;

		// �L�[����
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_LEFT))
			return TRUE;
	}

	// ���͖���
	return FALSE;
}

//=============================================================================
// �E����
//=============================================================================
HRESULT CKananLibrary::Right(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetKeyboardTrigger(DIK_D))
		return TRUE;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetbConnect())
	{
		// ���X�e�B�b�N�擾
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// �X�e�B�b�N����
		if (fValueX < 0 && fValueZ <= 1.0f)
			return TRUE;

		// �L�[����
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_RIGHT))
			return TRUE;
	}

	// ���͖���
	return FALSE;
}

//=============================================================================
// �������
//=============================================================================
HRESULT CKananLibrary::Decide(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetKeyboardTrigger(DIK_RETURN))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetbConnect())
	{
		// �X�e�B�b�N����
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// �߂����
//=============================================================================
HRESULT CKananLibrary::Return(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetKeyboardTrigger(DIK_BACKSPACE))
		return S_OK;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetbConnect())
	{
		// �X�e�B�b�N����
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_B))
			return S_OK;
	}

	// ���͖���
	return E_FAIL;
}

//=============================================================================
// �|�[�Y����
//=============================================================================
HRESULT CKananLibrary::Pause(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// �L�[�{�[�h����
	if (Key && Key->GetKeyboardTrigger(DIK_P))
		return TRUE;

	// �Q�[���p�b�h�ڑ��m�F
	if (pGamepad->GetbConnect())
	{
		// �X�e�B�b�N����
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_START))
			return TRUE;
	}

	// ���͖���
	return FALSE;
}

//=============================================================================
// �L�[�{�[�h�ňړ����͂��Ă��邩�̊m�F
//=============================================================================
HRESULT CKananLibrary::GetMoveByKeyboard(CInputKeyboard * pKey, int nPlayer)
{
	// ���͂���Ă��Ȃ�
	if (	(nPlayer == PLAYER_ONE && 
		(!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_UP)	 &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_DOWN)	 &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_RIGHT) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_LEFT) &&
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_ONE::ONE_JUMP) && 
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_ONE::ONE_ATTACK) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_SMASH))) ||
			(nPlayer == PLAYER_TWO &&
		(!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_UP) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_DOWN) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_RIGHT) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_LEFT) &&
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_TWO::TWO_JUMP) && 
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_TWO::TWO_ATTACK) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_SMASH))))
	{
		// �ړ����Ă��Ȃ�
		return E_FAIL;
	}

	// �ړ����Ă���
	return S_OK;
}

//=============================================================================
// �Q�[���p�b�h�ňړ����͂��Ă��邩�̊m�F
//=============================================================================
HRESULT CKananLibrary::GetMoveByGamepad(CInputGamepad * pGamepad)
{
	// ���͂���Ă��Ȃ�
	if ((!pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A) &&
		!pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_B) &&
		!pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_X)))
	{
		// ���삵�Ă��Ȃ�
		return E_FAIL;
	}

	// ���삵�Ă���
	return S_OK;
}

//=============================================================================
// �I�����̒����烉���_���Ȓl��Ԃ�
//=============================================================================
int CKananLibrary::DecideRandomValue(int nMaxSelectType, bool * bSelect, bool compType)
{
	// �I�����̐�
	int RandRange = 0;

	// �I�����̐��𑝂₷
	for (int nCnt = 0; nCnt < nMaxSelectType; nCnt++)
	{
		// ���̑I��������̏ꍇ
		if (compType)
		{
			if (bSelect[nCnt])
				RandRange++;
		}
		// ���̑I��������̏ꍇ
		else
			if (!bSelect[nCnt])
				RandRange++;
	}

	// �I�����̐������������m��
	int *RandType = new int[RandRange];
	// ������
	for (int nCnt = 0; nCnt < RandRange; nCnt++)
	{
		RandType[nCnt] = 0;
	}
	// �ԍ��J�E���^
	int nCntRand = 0;

	// ��������Ă��Ȃ��ϐ��̔ԍ���z��ɕۑ�
	for (int nCnt = 0; nCnt < nMaxSelectType; nCnt++)
	{
		// ���̑I��������̏ꍇ
		if (compType)
		{
			if (bSelect[nCnt])
			{
				RandType[nCntRand] = nCnt;
				nCntRand++;
			}
		}
		// ���̑I��������̏ꍇ
		else
		{
			if (!bSelect[nCnt])
			{
				RandType[nCntRand] = nCnt;
				nCntRand++;
			}
		}
	}

	// �o�͗p
	int outValue = RandType[rand() % RandRange];
	// �|�C���^�̔j��
	delete[] RandType;

	// �l��Ԃ�
	return outValue;
}

#ifdef _DEBUG
//=============================================================================
// ImGui�̍X�V
//=============================================================================
void CKananLibrary::ShowDebugInfo(void)
{
	// �c���[�̃w�b�_�[
	if (ImGui::CollapsingHeader("DebugInfo"))
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// FPS�̕\��
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//// Scene2D�����̕\��
		//ImGui::Text("NumScene2D : %d", CScene2D::GetNumScene2D());
		//// Scene2D�����̕\��
		//ImGui::Text("NumScene3D : %d", CScene3D::GetNumScene3D());
		//// Scene2D�����̕\��
		//ImGui::Text("NumSceneX  : %d", CSceneX::GetNumSceneX());

		// ���C���[�t���[��
		if (ImGui::Checkbox("WireFrame", &m_WireFrame))
		{
			// ? A : B (�������Z�q : �^(A)�U(B)�ǂ��炩�����Ă͂܂��Ă���΁A��������s����)
			m_WireFrame ?
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME) :	//���C���[
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			//�ʏ�
		}

		// �S�̂̃J�����O
		if (ImGui::TreeNode("Culling"))
		{
			// �J�����O
			if (ImGui::RadioButton("CCW", &m_nCulling, 0))
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// ����(�����)���J�����O����
			// ���s���Ȃ�
			ImGui::SameLine();
			if (ImGui::RadioButton("CW", &m_nCulling, 1))
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);		// �\��(�E���)���J�����O����
			// ���s���Ȃ�
			ImGui::SameLine();
			if (ImGui::RadioButton("NONE", &m_nCulling, 2))
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// ���ʃJ�����O

			// �c���[�̍Ō�ɕK������
			ImGui::TreePop();
		}
	}
}
//=============================================================================
// ���W�Ȃǂ̏��
//=============================================================================
void CKananLibrary::ShowOffsetInfo(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move)
{
	ImGui::Text("pos (%.4f, %.4f, %.4f)", pos.x, pos.y, pos.z);
	ImGui::Text("rot (%.4f, %.4f, %.4f)", rot.x, rot.y, rot.z);
	ImGui::Text("move (%.4f, %.4f, %.4f)", move.x, move.y, move.z);
}
#endif