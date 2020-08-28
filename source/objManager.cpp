//=============================================================================
//
// �I�u�W�F�N�g�}�l�[�W���[����  [ objManager.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "objManager.h"
#include "manager.h"
#include "renderer.h"
#include "Inputkeyboard.h"
#include "debugproc.h"
#include "mouse.h"
#include "camera.h"
#include "game.h"
#include <iostream>
#include "ImGui/imgui.h"				// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"		// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"		// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================
#define MODEL_PASS_CUT	(25)	// ���f���̃p�X��؂��鐔

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
char						CObjectManager::m_cModelFile[MAX_TEXT] =
{
	"data/OBJECT/object_model.txt",
};

char						CObjectManager::m_aFileName[STAGE_MAX][MAX_TEXT] = 
{ 
	"data/OBJECT/offset_stage1.txt",
	"data/OBJECT/offset_stage2.txt",
};

STAGETYPE					CObjectManager::m_stageType				= STAGE_1;
CObjectManager::STAGEINFO	CObjectManager::m_stageInfo[STAGE_MAX]	= {};
std::vector<OBJINFO>		CObjectManager::m_objInfo				= {};
int							CObjectManager::m_nNumTexture			= 0;
int							*CObjectManager::m_pModelIndex			= {};

#ifdef _DEBUG
int							CObjectManager::m_nFakeType = 0;
int							CObjectManager::m_stateMode = CObjectManager::MODE_GAME;
bool						CObjectManager::m_bObjUse = false;
bool						CObjectManager::m_bShowAnother = false;
std::string					*CObjectManager::m_pObjName = {};
#endif

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObjectManager::CObjectManager(PRIORITY type = CScene::PRIORITY_OBJECT) : CScene(type)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectManager::~CObjectManager()
{

}

//=============================================================================
// ������
//=============================================================================
void CObjectManager::Init()
{

}

//=============================================================================
// �I��
//=============================================================================
void CObjectManager::Uninit()
{
#ifdef _DEBUG
	if (m_pFakeObject)
	{
		m_pFakeObject->Uninit();
		delete m_pFakeObject;
		m_pFakeObject = nullptr;
	}

#endif

	// �T�C�Y���J��Ԃ� (size_t = unsigned, vector.size()�Ń���������Ԃ�)
	for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// �������폜
		delete m_stageInfo[m_stageType].pObject[nCnt];
	}
	// ���g���Ȃ���
	m_stageInfo[m_stageType].pObject.clear();
}

//=============================================================================
// �X�V
//=============================================================================
void CObjectManager::Update()
{
	// ���f�������J��Ԃ�
	for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// nullcheck
		if (m_stageInfo[m_stageType].pObject[nCnt])
			m_stageInfo[m_stageType].pObject[nCnt]->Update();
	}

#ifdef _DEBUG
	if (m_pFakeObject)
	{
		m_pFakeObject->Update();
	}
	// ImGui�̍X�V
	ShowObjectManagerInfo();
#endif
}

//=============================================================================
// �`��	
//=============================================================================
void CObjectManager::Draw()
{
	// ���f�������J��Ԃ�
	for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// nullcheck
		if (m_stageInfo[m_stageType].pObject[nCnt])
			m_stageInfo[m_stageType].pObject[nCnt]->Draw();
	}

#ifdef _DEBUG
	if (m_pFakeObject)
		m_pFakeObject->DrawAlpha();
#endif
}

//=============================================================================
// ����
//=============================================================================
CObjectManager *CObjectManager::Create(STAGETYPE type)
{
	// �������m��
	CObjectManager *pMana = new CObjectManager();

	// nullcheck
	if (!pMana)
	{
		// ���s
		std::cout << "�I�u�W�F�N�g�}�l�[�W���[�������s" << std::endl;
		return nullptr;
	}

	// ������
	pMana->Init();
	// �^�C�v��ۑ�
	pMana->m_stageType = type;
	// �X�e�[�W�z�u
	pMana->SetOffset();

	// ����
	return pMana;
}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CObjectManager::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���f�����擾
	LoadModel();

	// �I�t�Z�b�g�ǂݍ���
	LoadOffset();

	// �e�N�X�`������
	CreateTexture();

	// �I�u�W�F�N�g��񐶐�
	CreateObjInfo();

	// ����
	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CObjectManager::Unload(void)
{
	// nullcheck
	if (m_pModelIndex)
	{
		// �j��
		delete m_pModelIndex;
		m_pModelIndex = nullptr;
	}

#ifdef _DEBUG
	for (int nCnt = 0; nCnt < (int)m_objInfo.size(); nCnt++)
	{
		m_pObjName[nCnt].erase(m_pObjName[nCnt].begin(), m_pObjName[nCnt].end());
		m_pObjName[nCnt].clear();
	}

	if (m_pObjName)
	{
		delete[] m_pObjName;
		m_pObjName = nullptr;
	}
#endif

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// nullcheck
		if (m_stageInfo[nCnt].objOffset)
		{
			// �j��
			delete[] m_stageInfo[nCnt].objOffset;
			m_stageInfo[nCnt].objOffset = nullptr;
		}
	}
}

//=============================================================================
// ���f���z�u
//=============================================================================
HRESULT CObjectManager::SetOffset(void)
{
	for (int nCnt = 0; nCnt < m_stageInfo[m_stageType].nNumObject; nCnt++)
	{
		m_stageInfo[m_stageType].pObject.push_back(CObject::Create());
	}

	for (int nCnt = 0; nCnt < (int)m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// �I�u�W�F�N�g���̐ݒ�
		m_stageInfo[m_stageType].pObject[nCnt]->SetObjInfo(m_stageInfo[m_stageType].objOffset[nCnt].pos,
			m_stageInfo[m_stageType].objOffset[nCnt].rot,
			m_stageInfo[m_stageType].objOffset[nCnt].scale,
			&m_objInfo[m_stageInfo[m_stageType].objOffset[nCnt].nType].modelInfo,
			m_stageInfo[m_stageType].objOffset[nCnt].nType);
	}

	return S_OK;
}

//=============================================================================
// ���f���ǂݍ���
//=============================================================================
HRESULT CObjectManager::LoadModel(void)
{
	// �ϐ��錾
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";
	int nNumType = 0;

	// �t�@�C�����J��
	pFile = fopen(&m_cModelFile[0], "r");

	CKananLibrary::StartBlockComment("�I�u�W�F�N�g�t�@�C���̓ǂݍ��݊J�n�B");

	// nullcheck
	if (!pFile)
	{
		// �u���b�N�R�����g
		CKananLibrary::EndBlockComment("�t�@�C�����J���܂���ł����B");
		// ���s
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
				continue;
			// ���f���ԍ���������
			else if (strcmp(cHeadText, "NUM_TYPE") == 0)
			{
				// ���f���^�C�v���ǂݍ���
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumType);
				printf("�ǂݍ��񂾃I�u�W�F�N�g�^�C�v�� %d\n", nNumType);

				OBJINFO objInfo;
				int nCntModel = 0;
				while (nCntModel != nNumType)
				{
					// ��s�ǂݍ���
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);

					// �t�@�C�����ǂݍ���
					if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{
						// ���f�����擾
						if (SUCCEEDED(sscanf(cReadText, "%s %s %s",
							&cDieText,
							&cDieText,
							&objInfo.modelInfo.cModelName[0])))
						{
							// �ǂݍ��񂾃t�@�C������\��
							printf("���f�� %s ��ǂݍ���\n", &objInfo.modelInfo.cModelName[0]);
							// �e�N�X�`������
							objInfo.modelInfo.bTex = false;
							// �I�u�W�F�N�g����ۑ�
							m_objInfo.push_back(objInfo);
							// ���f���������Z
							nCntModel++;
						}
					}
				}
			}
			// �e�N�X�`���ԍ���������
			else if (strcmp(cHeadText, "NUM_TEXTURE") == 0)
			{
				// �e�N�X�`�����ǂݍ���
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_nNumTexture);
				printf("�ǂݍ��񂾃e�N�X�`���� %d\n", m_nNumTexture);

				if (m_nNumTexture > 0)
					m_pModelIndex = new int[m_nNumTexture];	// �e�N�X�`�������蓖�Ă郂�f���ԍ�

				int nCntTex = 0;
				// �ǂݍ��񂾃e�N�X�`�������ݒ萔�ɂȂ�܂ŌJ��Ԃ�
				while (nCntTex != m_nNumTexture)
				{
					// ��s�ǂݍ���
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);

					// �t�@�C�����ǂݍ���
					if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
					{
						int nModel = 0;
						char cFileName[MAX_TEXT];
						// ���f���ԍ��擾
						if (SUCCEEDED(sscanf(cReadText, "%s %s %s : %s %s %d",
							&cDieText,		// TEX_FILE
							&cDieText,		// =
							&cFileName[0],	// �e�N�X�`����
							&cDieText,		// MODEL
							&cDieText,		// =
							&nModel)))		// ���f���ԍ�
						{
							// �e�N�X�`������ۑ�
							strcpy(m_objInfo[nModel].modelInfo.cTextureName, cFileName);
							m_objInfo[nModel].modelInfo.bTex = true;
							// ���f���ԍ���ۑ�
							m_pModelIndex[nCntTex] = nModel;
							// �ǂݍ��񂾃t�@�C������\��
							printf("�e�N�X�`�� %s ��ǂݍ���\n", &m_objInfo[nModel].modelInfo.cTextureName[0]);
							// �e�N�X�`���������Z
							nCntTex++;
						}
					}
				}
			}
		}
	}

	// �t�@�C�������
	fclose(pFile);

	CKananLibrary::EndBlockComment("�t�@�C���̓ǂݍ��ݏI������");

	// ����
	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�t�@�C���ǂݍ���
//=============================================================================
HRESULT CObjectManager::LoadOffset(void)
{
	// �ϐ��錾
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";
	int nNumType = 0;

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// �ǂݍ��񂾃��f�����i�[�p
		int nModel = 0;
		int nCntStonePos = 0;

		// �t�@�C�����J��
		pFile = fopen(&m_aFileName[nCnt][0], "r");

		CKananLibrary::StartBlockComment("�X�e�[�W�t�@�C���̓ǂݍ��݊J�n�B");

		// nullcheck
		if (!pFile)
		{
			// �u���b�N�R�����g
			CKananLibrary::EndBlockComment("�t�@�C�����J���܂���ł����B");
			// ���s
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
					continue;
				// �I�u�W�F�N�g��
				if (strcmp(cHeadText, "NUM_OBJECT") == 0)
				{
					// ���f�����̎擾
					sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_stageInfo[nCnt].nNumObject);
					// �I�t�Z�b�g�̃������m��
					if (!m_stageInfo[nCnt].objOffset)
						m_stageInfo[nCnt].objOffset = new OBJECT_OFFSET[m_stageInfo[nCnt].nNumObject];
				}
				// �Z�b�g��������
				else if (strcmp(cHeadText, "OBJECTSET") == 0)
				{
					// �G���h�Z�b�g������܂ŌJ��Ԃ�
					while (strcmp(cHeadText, "END_OBJECTSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);
						// ���
						if (strcmp(cHeadText, "TYPE") == 0)
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_stageInfo[nCnt].objOffset[nModel].nType);
						// �ʒu
						if (strcmp(cHeadText, "POS") == 0)
							sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText,
								&m_stageInfo[nCnt].objOffset[nModel].pos.x,
								&m_stageInfo[nCnt].objOffset[nModel].pos.y,
								&m_stageInfo[nCnt].objOffset[nModel].pos.z);
						// ��]
						if (strcmp(cHeadText, "ROT") == 0)
							sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText,
								&m_stageInfo[nCnt].objOffset[nModel].rot.x,
								&m_stageInfo[nCnt].objOffset[nModel].rot.y,
								&m_stageInfo[nCnt].objOffset[nModel].rot.z);
						// �g�嗦
						if (strcmp(cHeadText, "SCALE") == 0)
							sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText,
								&m_stageInfo[nCnt].objOffset[nModel].scale.x,
								&m_stageInfo[nCnt].objOffset[nModel].scale.y,
								&m_stageInfo[nCnt].objOffset[nModel].scale.z);
					}

					// ���f���������Z
					nModel++;
				}
				else if (strcmp(cHeadText, "SET_STONEPOS") == 0)
				{
					// �G���h�Z�b�g������܂ŌJ��Ԃ�
					while (strcmp(cHeadText, "END_STONEPOS") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);
						// �ʒu
						if (strcmp(cHeadText, "POS") == 0)
							sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText,
								&m_stageInfo[nCnt].stonePos[nCntStonePos].x,
								&m_stageInfo[nCnt].stonePos[nCntStonePos].y,
								&m_stageInfo[nCnt].stonePos[nCntStonePos].z);

						// �ʒu�������Z
						nCntStonePos++;
					}
				}
				
				//CPolygonCollider::LoadPolyPos(nCnt, cHeadText, cReadText, pFile);
			}
		}

		// �t�@�C�������
		fclose(pFile);

		// �I�u�W�F�N�g������\��
		printf("�ǂݍ��񂾃I�u�W�F�N�g���� %d\n", m_stageInfo[nCnt].nNumObject);

		CKananLibrary::EndBlockComment("�t�@�C���̓ǂݍ��ݏI������");

	}
	// ����
	return S_OK;
}

//=============================================================================
// �e�N�X�`������
//=============================================================================
void CObjectManager::CreateTexture()
{
	// �e�N�X�`��������
	for (int nCnt = 0; nCnt < m_nNumTexture; nCnt++)
	{
		// �e�N�X�`���𐶐�
		CKananLibrary::CreateTexture(&m_objInfo[m_pModelIndex[nCnt]].modelInfo.pTexture,
			m_objInfo[m_pModelIndex[nCnt]].modelInfo.cTextureName);
	}
}

//=============================================================================
// �I�u�W�F�N�g��񐶐�
//=============================================================================
void CObjectManager::CreateObjInfo()
{
#ifdef _DEBUG
	// �I�u�W�F�N�g���̃������m��
	m_pObjName = new std::string[(int)m_objInfo.size()];
#endif

	// ��ސ�����
	for (size_t nCnt = 0; nCnt < m_objInfo.size(); nCnt++)
	{
		// ���f�����̐���
		CKananLibrary::CreateModelInfo(&m_objInfo[nCnt].modelInfo);

		// ���f���̒��_���W�̍ő�E�ŏ������߂�
		m_objInfo[nCnt].modelVtx =
			CKananLibrary::OutputModelVtxColl(m_objInfo[nCnt].modelInfo.mesh);

#ifdef _DEBUG
		// ���f���̃p�X���擾
		std::string modelName = m_objInfo[nCnt].modelInfo.cModelName;
		// ���f�����̂ݕۑ�
		m_pObjName[nCnt] = modelName.substr(MODEL_PASS_CUT);
#endif
	}
}

#ifdef _DEBUG
//=============================================================================
// ImGui�̍X�V
//=============================================================================
void CObjectManager::ShowObjectManagerInfo(void)
{
	// �E�B���h�E��\�����邩
	ImGui::Checkbox("show object window", &m_bShowAnother);

	// �\������
	if (!m_bShowAnother)
	{
		return;
	}

	// ImGui�̍X�V�J�n
	ImGui::Begin("object window", &m_bShowAnother);

	ImGui::Text("stage %d", m_stageType);

	// �J�����̎擾
	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();

	// �Z�[�u�{�^��
	if (ImGui::Button("save object offset"))
	{
		// �Z�[�u������
		if (SaveOffset() == S_OK)
		{
			// �}�E�X�̎擾
			CMouse *pMouse = CManager::GetMouse();
			// �E�B���h�E�n���h���̎擾
			HWND hWnd = pMouse->GetWnd();
			// �m�F�̃��b�Z�[�W
			MessageBox(hWnd, "�I�u�W�F�N�g�t�@�C�����Z�[�u���܂���", "�m�F", MB_OK);
		}
	}

	// �Q�[�����[�h�ɐ؂�ւ�
	if (ImGui::RadioButton("GAME", &m_stateMode, MODE_GAME))
	{
		if (m_bObjUse)
		{
			delete m_pFakeObject;
			m_pFakeObject = nullptr;
			m_bObjUse = false;
		}
	}
	// ���s���Ȃ�
	ImGui::SameLine();
	// �f�o�b�O���[�h�ɐ؂�ւ�
	if (ImGui::RadioButton("DEBUG", &m_stateMode, MODE_DEBUG))
	{
		// �U�I�u�W�F���g���Ă��Ȃ�
		if (!m_bObjUse)
		{
			// �������Ďg�p
			m_pFakeObject = new CObject();
			m_bObjUse = true;
		}

		// �U�I�u�W�F�����݂���
		if (m_pFakeObject)
		{
			// �f�t�H���g�^�C�v�Ƃ��ď�����
			m_pFakeObject->Init();

			// �������f����ݒ�
			m_pFakeObject->SetObjInfo(*pCamera->GetPosR(), 
				ZeroVector3, 
				OneVector3, 
				&m_objInfo[0].modelInfo, 
				0);
		}
	}

	if (m_stateMode == MODE_GAME)
	{
		// ���[�h�̏ڍׂ�\��
		ImGui::Text("This mode displays the details of the object.");
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
	else if (m_stateMode == MODE_DEBUG)
	{
		// ���[�h�̏ڍׂ�\��
		ImGui::Text("This mode allows you to place objects.");

		m_pFakeObject->ShowObjectInfo("FakeObject");

		// �I�u�W�F�^�C�v���ς������
		if (ImGui::InputInt("ObjectType", &m_nFakeType))
		{
			// �^�C�v�������Ȃ��悤
			if (m_nFakeType < 0)
				m_nFakeType = 0;
			else if (m_nFakeType >= (int)m_objInfo.size())
				m_nFakeType = (int)m_objInfo.size() - 1;

			// ���f�����i�[
			m_pFakeObject->SetObjInfo(*m_pFakeObject->GetPos(), 
				*m_pFakeObject->GetRot(), 
				*m_pFakeObject->GetScale(), 
				&m_objInfo[m_nFakeType].modelInfo,
				m_nFakeType);
		}

		// �I�u�W�F�N�g�̐���
		if (ImGui::Button("Create"))
		{
			// �������m��
			m_stageInfo[m_stageType].pObject.push_back(CObject::Create());
			// �������E���ݒ�
			m_stageInfo[m_stageType].pObject[(int)m_stageInfo[m_stageType].pObject.size() - 1]->Init();
			// ���f�����i�[
			m_stageInfo[m_stageType].pObject[(int)(m_stageInfo[m_stageType].pObject.size() - 1)]->SetObjInfo(*m_pFakeObject->GetPos(),
				*m_pFakeObject->GetRot(), 
				*m_pFakeObject->GetScale(), 
				&m_objInfo[m_nFakeType].modelInfo,
				m_nFakeType);
		}
	}

	// ���f�������J��Ԃ�
	for (int nCnt = (int)m_stageInfo[m_stageType].pObject.size() - 1; nCnt > -1; nCnt--)
	{
		// nullcheck
		if (m_stageInfo[m_stageType].pObject[nCnt])
		{
			// �p�X��char�ɃL���X�g
			char *cObjName = new char[m_pObjName[m_stageInfo[m_stageType].pObject[nCnt]->GetType()].size() + 1];
			// ������𕡐�
			strcpy(cObjName, m_pObjName[m_stageInfo[m_stageType].pObject[nCnt]->GetType()].c_str());
			// �c���[���̐ݒ�
			char cText[32] = {};
			sprintf(cText, "Object : %d (%s)", nCnt, cObjName);
			// ImGui�̍X�V
			m_stageInfo[m_stageType].pObject[nCnt]->ShowObjectInfo(cText);
			// ��������j��
			delete cObjName;
		}

		// �����[�X���L��
		if (m_stageInfo[m_stageType].pObject[nCnt]->GetRelease())
		{
			m_stageInfo[m_stageType].pObject[nCnt]->Uninit();
			delete m_stageInfo[m_stageType].pObject[nCnt];
			// �w�肵���ԍ��̃I�u�W�F�N�g���폜 : vector.erase( vector�̎n�܂� + �w�肵���ԍ� )
			m_stageInfo[m_stageType].pObject.erase(m_stageInfo[m_stageType].pObject.begin() + nCnt);
		}
	}

	// ����{�^��
	if (ImGui::Button("close this window"))
		m_bShowAnother = false;

	// ImGui�̍X�V�I��
	ImGui::End();
}

//=============================================================================
// �I�u�W�F�N�g���̃Z�[�u
//=============================================================================
HRESULT CObjectManager::SaveOffset(void)
{
	// �ϐ��錾
	FILE *pFile;

	// �e�L�X�g�t�@�C�������₷������R�����g
	char cEqual[2] = "=";

	// �ϐ����p�Ə������ݗp�̕ϐ�
	char cHeadText[MAX_TEXT];
	char cWriteText[MAX_TEXT];

	// �ʒu�E��]�̎擾�p
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// �t�@�C�����J��
	pFile = fopen(&m_aFileName[m_stageType][0], "w");

	// nullcheck
	if (pFile)
	{
		fputs(COMMENT_BLOCK, pFile);														// #=====================================================
		fputs(COMMENT_BLOCK_LINE, pFile);													// #

		strcpy(cWriteText, "# �I�u�W�F�N�g�̃I�t�Z�b�g\n");
		fputs(cWriteText, pFile);															// # �I�u�W�F�N�g�̃I�t�Z�b�g

		strcpy(cWriteText, COMMENT_AUTHOR);
		fputs(cWriteText, pFile);															// # Author : KANAN NAGANAWA
		fputs(COMMENT_BLOCK_LINE, pFile);													// #
		fputs(COMMENT_BLOCK, pFile);														// #=====================================================

		strcpy(cHeadText, "SCRIPT");
		fputs(cHeadText, pFile);															// SCRIPT
		fputs(COMMENT_NEW_LINE, pFile);														// \n
		fputs(COMMENT_NEW_LINE, pFile);														// \n

		// �X�N���v�g�������瑱����
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			fputs(COMMENT_BLOCK, pFile);													// #=====================================================
			strcpy(cWriteText, "# �I�u�W�F�N�g����\n");
			fputs(cWriteText, pFile);														// # �I�u�W�F�N�g����
			fputs(COMMENT_BLOCK, pFile);													// #=====================================================

			strcpy(cHeadText, "NUM_OBJECT");
			sprintf(cWriteText, "%s %s %d\n",
				&cHeadText,
				&cEqual,
				(int)m_stageInfo[m_stageType].pObject.size());
			fputs(cWriteText, pFile);														// NUM_OBJECT = m_nNumObject
			fputs(COMMENT_NEW_LINE, pFile);													// \n
			fputs(COMMENT_BLOCK, pFile);													// #=====================================================

			strcpy(cWriteText, "# �I�u�W�F�N�g���\n");
			fputs(cWriteText, pFile);														// # �I�u�W�F�N�g���
			fputs(COMMENT_BLOCK, pFile);													// #=====================================================

			for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
			{
				strcpy(cWriteText, "OBJECTSET\n");
				fputs(cWriteText, pFile);													// OBJECTSET

				strcpy(cHeadText, "TYPE");
				sprintf(cWriteText, "	%s %s %d",
					&cHeadText,
					&cEqual,
					(int)m_stageInfo[m_stageType].pObject[nCnt]->GetType());
				fputs(cWriteText, pFile);													//	TYPE = GetType()
				fputs(COMMENT_NEW_LINE, pFile);												//	\n

				strcpy(cHeadText, "POS");
				sprintf(cWriteText, "	%s %s %.3f %.3f %.3f",
					&cHeadText,
					&cEqual,
					m_stageInfo[m_stageType].pObject[nCnt]->GetPos()->x,
					m_stageInfo[m_stageType].pObject[nCnt]->GetPos()->y,
					m_stageInfo[m_stageType].pObject[nCnt]->GetPos()->z);
				fputs(cWriteText, pFile);											//	POS = GetPos()
				fputs(COMMENT_NEW_LINE, pFile);											//	\n

				strcpy(cHeadText, "ROT");
				sprintf(cWriteText, "	%s %s %.3f %.3f %.3f",
					&cHeadText,
					&cEqual,
					m_stageInfo[m_stageType].pObject[nCnt]->GetRot()->x,
					m_stageInfo[m_stageType].pObject[nCnt]->GetRot()->y,
					m_stageInfo[m_stageType].pObject[nCnt]->GetRot()->z);
				fputs(cWriteText, pFile);											//	ROT = GetRot()
				fputs(COMMENT_NEW_LINE, pFile);											//	\n
				
				strcpy(cHeadText, "SCALE");
				sprintf(cWriteText, "	%s %s %.3f %.3f %.3f",
					&cHeadText,
					&cEqual,
					m_stageInfo[m_stageType].pObject[nCnt]->GetScale()->x,
					m_stageInfo[m_stageType].pObject[nCnt]->GetScale()->y,
					m_stageInfo[m_stageType].pObject[nCnt]->GetScale()->z);
				fputs(cWriteText, pFile);											//	SCALE = GetScale()
				fputs(COMMENT_NEW_LINE, pFile);											//	\n

				strcpy(cWriteText, "END_OBJECTSET\n");
				fputs(cWriteText, pFile);													// END_OBJECTSET
				fputs(COMMENT_NEW_LINE, pFile);												// \n
			}
			fputs(COMMENT_NEW_LINE, pFile);													// \n

			strcpy(cWriteText, "SET_STONEPOS\n");
			fputs(cWriteText, pFile);														// SET_STONEPOS
			for (size_t nCnt = 0; nCnt < 5; nCnt++)
			{
				strcpy(cHeadText, "POS");
				sprintf(cWriteText, "	%s %s %.3f %.3f %.3f\n",
					&cHeadText,
					&cEqual,
					m_stageInfo[m_stageType].stonePos[nCnt].x,
					m_stageInfo[m_stageType].stonePos[nCnt].y,
					m_stageInfo[m_stageType].stonePos[nCnt].z);
				fputs(cWriteText, pFile);													//	POS = pos
			}
			strcpy(cWriteText, "END_STONEPOS\n");
			fputs(cWriteText, pFile);														// END_STONEPOS

			fputs(COMMENT_NEW_LINE, pFile);													// \n

			//CPolygonCollider::Save(m_stageType, pFile);

			strcpy(cWriteText, "END_SCRIPT\n");
			fputs(cWriteText, pFile);														// END_SCRIPT
		}
		// �t�@�C�������
		fclose(pFile);
	}

	// �t�@�C�����J���Ȃ�������
	else
	{
		// �R���\�[���ɕ\��
		printf("�I�u�W�F�N�g�t�@�C�����J���܂���ł����B\n");
		// ���s
		return E_FAIL;
	}

	// ����
	return S_OK;
}

#endif