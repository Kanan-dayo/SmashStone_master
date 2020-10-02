//=============================================================================
//
// ���f���L�����N�^�[����  [ modelCharacter.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "modelcharacter.h"
#include "modelparts.h"
#include "manager.h"
#include "renderer.h"
#include "Inputkeyboard.h"
#include "debugproc.h"
#include "sceneX.h"
#include "motion.h"
#include "kananlibrary.h"
#include "motion.h"
#include "ImGui/imgui.h"				// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"		// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"		// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
MODELCHARACTER	CModelCharacter::m_pModelCharacter[CHARACTER_MAX] = {};
char			CModelCharacter::m_aFileName[CHARACTER_MAX][64] =
{
	{ "data/OFFSET/offset_fokker.txt" },
	{ "data/OFFSET/offset_fokker_trans.txt" },
	{ "data/OFFSET/offset_niyasu.txt" },
	{ "data/OFFSET/offset_niyasu_trans.txt" },
	{ "data/OFFSET/offset_sanyasu.txt" },
	{ "data/OFFSET/offset_sanyasu_trans.txt" },
	{ "data/OFFSET/offset_yonyasu.txt" },
	{ "data/OFFSET/offset_yonyasu_trans.txt" }
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelCharacter::CModelCharacter()
{
	// ������
	m_pModelParts = nullptr;
	m_nAllFrame = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelCharacter::~CModelCharacter()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CModelCharacter::Init()
{
	// ���f������������
	m_pModelParts = new CModelParts[m_pModelCharacter[m_type].nNumParts];

	if (m_pModelParts)
	{
		for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
		{
			// ������
			m_pModelParts[nCnt].Init();
			// ���f�����ݒ�
			m_pModelParts[nCnt].BindModelInfo(&m_pModelCharacter[m_type].pModelInfo[nCnt]);
		}
		// �p�[�c�̃I�t�Z�b�g�擾
		LoadOffset(m_type);
	}

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CModelCharacter::Uninit()
{
	// nullcheck
	if (m_pModelParts)
	{
		// �j��
		delete[] m_pModelParts;
		m_pModelParts = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CModelCharacter::Update()
{
	if (m_pModelParts == nullptr)
	{
		return;
	}

	// ���f�������J��Ԃ�
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		D3DXVECTOR3 *pPos = m_pModelParts[nCnt].GetPos();
		D3DXVECTOR3 *pRot = m_pModelParts[nCnt].GetRot();

		// ���[�V�����p�t���[����0�̂Ƃ�
		if (m_nFrame == 0)
		{
			// ���f���̃C���f�b�N�X���擾
			int nIndex = m_pModelParts[nCnt].GetIndex();
			// ���[�V�����̃t���[�����̎擾
			float fFrame = (float)(CMotion::GetFrame((PARAM_TYPE)(m_type / 2) ,m_motion, m_nKey));

			// ��Ɨp�̕ϐ�
			D3DXVECTOR3 WorkPos = ZeroVector3;
			D3DXVECTOR3 WorkRot = ZeroVector3;

			// ���̈ʒu�Ƃ̍������Z�o
			/*if ((m_State == CMotionCharacter::MOTION_DEADLY ||
			m_State == CMotionCharacter::MOTION_ATTACK_SP ||
			m_State == CMotionCharacter::MOTION_ATTACK_4 ||
			m_State == CMotionCharacter::MOTION_ATTACK_3 ||
			m_State == CMotionCharacter::MOTION_ATTACK_2 ||
			m_State == CMotionCharacter::MOTION_ATTACK_1) &&
			CMotion::m_nCntKey != 0)
			{// �ړ����郂�[�V�������L�[��0�ȊO�̎�
			WorkPos =
			((CMotionCharacter::m_MotionInfo[m_MotionWeapon][CMotionCharacter::m_State].MotionKey[CMotion::m_nCntKey].pos[nCntParts] -
			CMotionCharacter::m_MotionInfo[m_MotionWeapon][CMotionCharacter::m_State].MotionKey[CMotion::m_nCntKey - 1].pos[nCntParts])
			+ CMotion::m_pOffsetPos[nCntParts])
			- *pModel[nCntParts].GetPosition();
			}*/
			{
				// �ړI�n�܂ł̈ړ��ʂ��v�Z
				WorkPos = (CMotion::GetPosDest((PARAM_TYPE)(m_type / 2), m_motion, m_nKey, nIndex) + *m_pModelParts[nCnt].GetOffsetPos()) - *pPos;
				// �ړ��ʂ��t���[�����Ŋ���
				m_pModelParts[nCnt].SetUpdatePos(WorkPos / fFrame);
			}

			// �ړI�n�܂ł̉�]�ʂ��v�Z
			WorkRot = (CMotion::GetRotDest((PARAM_TYPE)(m_type / 2), m_motion, m_nKey, nIndex) + *m_pModelParts[nCnt].GetOffsetRot()) - *pRot;
			// ��]�ʂ��t���[�����Ŋ���
			m_pModelParts[nCnt].SetUpdateRot(WorkRot / fFrame);
		}

		// �ЂƂ܂��A���肵����]�ʂ�ݒ�
		m_pModelParts[nCnt].SetRot(*pRot + *m_pModelParts[nCnt].GetUpdateRot());
		// ��]�ʂ��Ď擾
		D3DXVECTOR3 *fRot = m_pModelParts[nCnt].GetRot();
		// ��]����
		CKananLibrary::InterpolationRot(fRot);

		m_pModelParts[nCnt].SetRot(*fRot);
		m_pModelParts[nCnt].SetPos(*pPos + *m_pModelParts[nCnt].GetUpdatePos());

		// nullcheck
		if (m_pModelParts)
			// �X�V
			m_pModelParts[nCnt].Update();
	}

	// �t���[�����Z
	m_nFrame++;

	// �t���[�������l�܂ŗ�����
	if (m_nFrame >= CMotion::GetFrame((PARAM_TYPE)(m_type / 2), m_motion, m_nKey))
	{
		// �t���[��������
		m_nFrame = 0;

		// �L�[���Z
		m_nKey++;

		// �L�[�����l�܂ŗ�����
		if (m_nKey >= CMotion::GetNumKey((PARAM_TYPE)(m_type / 2), m_motion))
		{
			//�t���[���ƃL�[������
			ResetMotion();

			// ���[�v���Ȃ���
			if (!CMotion::GetLoop((PARAM_TYPE)(m_type / 2), m_motion))
			{
				// �j���[�g�����ɖ߂�
				m_motion = CMotion::PLAYER_NEUTRAL;
			}
		}

		// ���̃��[�V���������Z�b�g
		SetMotion(m_motion);
	}

#ifdef _DEBUG
	// ImGui�̍X�V
	ShowModelCharaInfo();
#endif
}

//=============================================================================
// �`��	
//=============================================================================
void CModelCharacter::Draw()
{
	// ���f�������J��Ԃ�
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		// nullcheck
		if (m_pModelParts)
			// �`��
			m_pModelParts[nCnt].Draw();
	}
}

//=============================================================================
// ���G�`��	
//=============================================================================
void CModelCharacter::DrawInvincible(int nInvincibleTime)
{
	// ���f�������J��Ԃ�
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		// nullcheck
		if (m_pModelParts)
			// �`��
			m_pModelParts[nCnt].DrawInvincible(nInvincibleTime);
	}
}

//=============================================================================
// ����
//=============================================================================
CModelCharacter *CModelCharacter::Create(CHARACTER_TYPE modeltype)
{
	// �������m��
	CModelCharacter *pModelCharacter = new CModelCharacter;

	// ���s
	if (!pModelCharacter)
		return nullptr;

	// �^�C�v�̐ݒ�
	pModelCharacter->m_type = modeltype;

	// ������
	pModelCharacter->Init();

	// �l��Ԃ�
	return pModelCharacter;
}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CModelCharacter::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �L�����N�^�[������
	for (int nCntModel = 0; nCntModel < CHARACTER_MAX; nCntModel++)
	{
		// ���f�����擾
		LoadFileName((CHARACTER_TYPE)nCntModel);

		// �u���b�N�R�����g
		char cComment[MAX_TEXT];
		sprintf(cComment, "�L�����N�^�[ %d �̃��f���ǂݍ��݊J�n", nCntModel);
		CKananLibrary::StartBlockComment(cComment);

		// �e�N�X�`��������
		for (int nCntTex = 0; nCntTex < m_pModelCharacter[nCntModel].nNumTexture; nCntTex++)
		{
			// �p�[�c������
			for (int nCntParts = 0; nCntParts < m_pModelCharacter[nCntModel].nNumParts; nCntParts++)
			{
				// �e�N�X�`������
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					&m_pModelCharacter[nCntModel].pModelInfo[nCntTex].cTextureName[0],
					&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].pTexture)))
				{
					// �ł��Ȃ���Ύ��s
					printf("�e�N�X�`���������s (�e�N�X�`�� %d)\n", nCntTex);
					m_pModelCharacter[nCntModel].pModelInfo[nCntParts].bTex = false;
				}
			}
		}

		// �p�[�c������
		for (int nCntParts = 0; nCntParts < m_pModelCharacter[nCntModel].nNumParts; nCntParts++)
		{
			// ���f������
			if (FAILED(D3DXLoadMeshFromX(&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].cModelName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].matBuff,
				NULL,
				&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].matNum,
				&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].mesh)))
			{
				// �ł��Ȃ���Ύ��s
				printf("���f���������s (�p�[�c %d)\n", nCntParts);
			}
		}

		// �u���b�N�R�����g
		sprintf(cComment, "�L�����N�^�[ %d �̃��f���ǂݍ��ݏI��", nCntModel);
		CKananLibrary::EndBlockComment(cComment);
	}

	// ����
	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CModelCharacter::Unload(void)
{
	// �L���������J��Ԃ�
	for (int nCntModel = 0; nCntModel < CHARACTER_MAX; nCntModel++)
	{
		// �����[�X���s��
		for (int nCnt = 0; nCnt < m_pModelCharacter[nCntModel].nNumParts; nCnt++)
			CKananLibrary::ReleaseModelInfo(&m_pModelCharacter[nCntModel].pModelInfo[nCnt]);

		// nullcheck
		if (m_pModelCharacter[nCntModel].pModelInfo)
		{
			// �j��
			delete[] m_pModelCharacter[nCntModel].pModelInfo;
			m_pModelCharacter[nCntModel].pModelInfo = nullptr;
		}
	}
}

//=============================================================================
// �L�����N�^�[�̃}�g���b�N�X�ݒ�
//=============================================================================
void CModelCharacter::SetCharacterMtx(D3DXMATRIX *mtx)
{
	// ���f��������
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		if (m_pModelParts)
		{
			// �e�����Ȃ���
			if (m_pModelParts[nCnt].GetParent() == -1)
				// �匳�̃}�g���b�N�X�ݒ�
				m_pModelParts[nCnt].SetMtxParent(mtx);
			else
				// �e�p�[�c�̃}�g���b�N�X�ݒ�
				m_pModelParts[nCnt].SetMtxParent(m_pModelParts[m_pModelParts[nCnt].GetParent()].GetMtx());
		}
	}
}

//=============================================================================
// ���[�V�����Z�b�g
//=============================================================================
void CModelCharacter::SetMotion(CMotion::MOTION_TYPE motiontype)
{
	// ���[�V�������Ⴆ�΃��[�V�������Z�b�g
	if (m_motion != motiontype)
		ResetMotion();

	// ���[�V������ݒ�
	m_motion = motiontype;

	// ���[�V�����̑��t���[�����i�[����
	m_nAllFrame = 0;

	// �L�[�����J��Ԃ�
	for (int nCnt = 0; nCnt < CMotion::GetNumKey((PARAM_TYPE)(m_type / 2), m_motion); nCnt++)
	{
		// �t���[���������Z����
		m_nAllFrame += CMotion::GetFrame((PARAM_TYPE)(m_type / 2), m_motion, nCnt);
	}
}

//=============================================================================
// �I�t�Z�b�g�ǂݍ���
//=============================================================================
HRESULT CModelCharacter::LoadOffset(CHARACTER_TYPE type)
{
	// �ϐ��錾
	FILE *pFile;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";
	int nCntParts = 0;

	// �i�[�p
	D3DXVECTOR3 pos = ZeroVector3;
	D3DXVECTOR3 rot = ZeroVector3;
	int nParent = 0;

	// �t�@�C�����J��
	pFile = fopen(&m_aFileName[type][0], "r");

	// nullcheck
	if (pFile)
	{
		// �X�N���v�g������܂ŌJ��Ԃ�
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}
		// �X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂ŌJ��Ԃ�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);
				// ���s
				if (strcmp(cHeadText, "\n") == 0)
				{
				}
				// �L�����N�^�[�Z�b�g��������
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					// �G���h�L�����N�^�[�Z�b�g������܂ŌJ��Ԃ�
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);
						// ���s
						if (strcmp(cHeadText, "\n") == 0)
						{
						}
						// �p�[�c�Z�b�g��������
						else if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							// �G���h�p�[�c�Z�b�g������܂ŌJ��Ԃ�
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								int nBeWeapon = 0;
								float fRadius = 0.0f;
								//	�y�A�����g
								if (strcmp(cHeadText, "PARENT") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nParent);
								}
								//	���킩�ǂ���
								if (strcmp(cHeadText, "WEAPON") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nBeWeapon);
								}
								//	�����蔻��̔��a
								if (strcmp(cHeadText, "RADIUS") == 0)
								{
									sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &fRadius);
								}
								// �ʒu
								if (strcmp(cHeadText, "POS") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &pos.x, &pos.y, &pos.z);
								}
								// ��]
								if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &rot.x, &rot.y, &rot.z);
								}
								if (strcmp(cHeadText, "END_PARTSSET") == 0)
								{
									if (m_pModelCharacter[type].pModelInfo[nCntParts].bTex)
									{
										// �e�N�X�`���t���̃p�[�c���i�[
										m_pModelParts[nCntParts].SetPartsTexInfo(nCntParts, nParent, pos, rot, m_pModelCharacter[type].pModelInfo[nCntParts].pTexture);
									}
									else
									{
										// �e�N�X�`���Ȃ��̃p�[�c���i�[
										m_pModelParts[nCntParts].SetPartsInfo(nCntParts, nParent, pos, rot);
									}

									if (nBeWeapon == 1)
									{

									}

									// �p�[�c�����Z
									nCntParts++;
								}
							}
						}
					}
				}
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}

	// �t�@�C�����J���Ȃ�������
	else
	{
		printf("�I�t�Z�b�g�t�@�C�����J���܂���ł����B\n");
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// ���f�����E���f�����ǂݍ���
//=============================================================================
HRESULT CModelCharacter::LoadFileName(CHARACTER_TYPE type)
{
	// �ϐ��錾
	FILE *pFile;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";
	int nNumModel = 0;
	int nNumTexture = 0;

	// �t�@�C�����J��
	pFile = fopen(&m_aFileName[type][0], "r");

	CKananLibrary::StartBlockComment("���f���t�@�C���ǂݍ��݊J�n");

	// nullcheck
	if (!pFile)
	{
		// �t�@�C���ǂݍ��ݎ��s
		CKananLibrary::EndBlockComment("���f���t�@�C�����J���܂���ł���");
		return E_FAIL;
	}

	// �X�N���v�g������܂ŌJ��Ԃ�
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);
	}
	// �X�N���v�g��������
	if (strcmp(cHeadText, "SCRIPT") == 0)
	{
		// �G���h�X�N���v�g������܂ŌJ��Ԃ�
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
			// ���s
			if (strcmp(cHeadText, "\n") == 0)
			{
			}
			// ���f���ԍ���������
			else if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumModel);
				m_pModelCharacter[type].pModelInfo = new MODELINFO[nNumModel];
				m_pModelCharacter[type].nNumParts = nNumModel;
				printf("�ǂݍ��񂾃p�[�c�� : %d\n", nNumModel);

				// �ǂݍ��񂾃��f����
				int nCntModel = 0;
				// ���f�������J��Ԃ�
				while (nCntModel < nNumModel)
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// �t�@�C�����ǂݍ���
					if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{
						// ���f�����擾
						sscanf(cReadText, "%s %s %s", &cDieText, &cDieText, m_pModelCharacter[type].pModelInfo[nCntModel].cModelName);
						printf("�t�@�C�� %s ���擾\n", m_pModelCharacter[type].pModelInfo[nCntModel].cModelName);
						m_pModelCharacter[type].pModelInfo[nCntModel].bTex = false;

						// ���f���J�E���g���Z
						nCntModel++;
					}
				}
			}
			// �e�N�X�`���ԍ���������
			else if (strcmp(cHeadText, "NUM_TEXTURE") == 0)
			{
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumTexture);
				m_pModelCharacter[type].nNumTexture = nNumTexture;

				// �ǂݍ��񂾃e�N�X�`����
				int nCntTexture = 0;
				// ������
				sprintf(m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName, "");

				// �e�N�X�`�������J��Ԃ�
				while (nCntTexture < nNumTexture)
				{
					// ������
					m_pModelCharacter[type].pModelInfo[nCntTexture].bTex = false;
					sprintf(m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName, "");

					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// �t�@�C�����ǂݍ���
					if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
					{
						// �e�N�X�`�����擾
						sscanf(cReadText, "%s %s %s", &cDieText, &cDieText, m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName);
						printf("�t�@�C�� %s ���擾\n", m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName);
						m_pModelCharacter[type].pModelInfo[nCntTexture].bTex = true;

						// �e�N�X�`���J�E���g���Z
						nCntTexture++;
					}
				}
			}
		}
	}

	// �t�@�C�������
	fclose(pFile);

	CKananLibrary::EndBlockComment("���f���t�@�C���ǂݍ��ݏI��");

	return S_OK;
}

//=============================================================================
// ���[�V������񃊃Z�b�g
//=============================================================================
void CModelCharacter::ResetMotion()
{
	m_nFrame = 0;
	m_nKey = 0;
}

//=============================================================================
// ���f���̍ăo�C���h
//=============================================================================
void CModelCharacter::ModelRebind(CHARACTER_TYPE type)
{
	m_type = type;
	for (int nCnt = 0; nCnt < m_pModelCharacter[type].nNumParts; nCnt++)
	{
		m_pModelParts[nCnt].BindModelInfo(&m_pModelCharacter[type].pModelInfo[nCnt]);
	}
}

#define ConvertNorModel(type) ((PARAM_TYPE)(type / 2))

//=============================================================================
// ���݂̃L�[���U����Ԃ��ǂ���
//=============================================================================
bool CModelCharacter::AttackKeyCondition(void)
{
	return (m_nKey == CMotion::GetAttackKey(ConvertNorModel(m_type), m_motion));
}

//=============================================================================
// �U�����Ă���p�[�c�C���f�b�N�X���擾����
//=============================================================================
int * CModelCharacter::GetAttackPartsIndex(void)
{
	return CMotion::GetnAttackIndex(ConvertNorModel(m_type),m_motion, m_nKey);
}

//=============================================================================
// �U�����Ă���p�[�c�C���f�b�N�X�̃T�C�Y�̎擾
//=============================================================================
int CModelCharacter::GetAttackPartsIndexSize(void)
{
	return CMotion::GetnAttackIndexSize(ConvertNorModel(m_type), m_motion, m_nKey);
}

#ifdef _DEBUG
//=============================================================================
// ImGui�̍X�V
//=============================================================================
void CModelCharacter::ShowModelCharaInfo(void)
{
	// ���݂̃t���[������\��
	/*ImGui::Text("nFrame : %d / %d", m_nFrame, CMotion::GetFrame(m_motion, m_nKey));
	// ���݂̃L�[����\��
	ImGui::Text("nKey : %d / %d", m_nKey, CMotion::GetNumKey(m_motion));*/
}
#endif