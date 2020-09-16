//=============================================================================
//
// �L�����N�^�[�p�����[�^�[����  [ charaParam.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "charaParam.h"
#include "kananlibrary.h"
#include "player.h"
#include "game.h"
#include "mouse.h"
#include "ImGui/imgui.h"				// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"		// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"		// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================	
#define SPEED_IMGUI_DRAGFLOAT	(0.1f)	// DragFloat�̃X�s�[�h

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
CCharaParam::PLAYER_PARAM	CCharaParam::m_playerParam[PARAM_MAX] = {};
#ifdef _DEBUG
bool						CCharaParam::m_bShowWindow = false;
#endif

char						CCharaParam::m_aFileName[PARAM_MAX][64] =
{
	"data/PARAMETER/parameter_fokker.txt",
	"data/PARAMETER/parameter_niyasu.txt",
	"data/PARAMETER/parameter_sanyasu.txt",
	"data/PARAMETER/parameter_yonyasu.txt"
};


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCharaParam::CCharaParam() {}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharaParam::~CCharaParam() {}

//=============================================================================
// �L�����N�^�[���Ƃ̃p�����[�^�[���[�h
//=============================================================================
HRESULT CCharaParam::Load(void)
{
	// �ϐ��錾
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";

	CKananLibrary::StartBlockComment("�p�����[�^�[�t�@�C���̓ǂݍ��݊J�n");

	for (int type = 0; type < PARAM_MAX; type++)
	{
		// �U���ǂݍ��݂̃J�E���^
		int nCntAttack = 0;

		// �L��������\��
		printf("�L�����N�^�[ %d ��ǂݍ��� (%s)\n", type, m_aFileName[type]);
		// �t�@�C�����J��
		pFile = fopen(m_aFileName[type], "r");

		// nullcheck
		if (!pFile)
		{
			// ���s
			printf("�t�@�C�����J���܂���ł���\n");
			continue;
		}

		// �X�N���v�g������܂ŌJ��Ԃ�
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// ��s���ǂݍ���
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		// �G���h�X�N���v�g������܂ŌJ��Ԃ�
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// ��s���ǂݍ���
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);

			// ���s
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// �ő�HP
			else if (strcmp(cHeadText, "MAX_LIFE") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].fMaxLife);
			// ���鑬�x
			else if (strcmp(cHeadText, "RUN_SPEED") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].moveParam.fRunSpeed);
			// �W�����v��
			else if (strcmp(cHeadText, "JUMP_POWER") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].moveParam.fJumpPower);
			// �U����
			if (strcmp(cHeadText, "ATTACK_POWER") == 0)
				sscanf(cReadText, "%s %s %f",&cDieText, &cDieText, &m_playerParam[type].fAttackPower);
			// �h���
			if (strcmp(cHeadText, "DEFENSE_POWER") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].fDefensePower);
			// ���[�V�����ݒ�
			else if (strcmp(cHeadText, "SET_MOTIONPARAM") == 0)
			{
				// �G���h�X�N���v�g������܂ŌJ��Ԃ�
				while (strcmp(cHeadText, "END_MOTIONPARAM") != 0)
				{
					// ��s���ǂݍ���
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// �U���̃L�����Z���t���[��
					if (strcmp(cHeadText, "CANCEL_FRAME") == 0)
						sscanf(cReadText, "%s %s %d %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].CancelFrame.start,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].CancelFrame.end);
					// �_���[�W
					if (strcmp(cHeadText, "DAMAGE") == 0)
						sscanf(cReadText, "%s %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].nDamage);
					//���݃t���[��
					if (strcmp(cHeadText, "DAUNTED") == 0)
						sscanf(cReadText, "%s %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].nDaunted);
					// ������ѕ�
					if (strcmp(cHeadText, "BLOWAWAY_TYPE") == 0)
						sscanf(cReadText, "%s %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].blowType);
				}
				// �U���̉��Z
				nCntAttack++;
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}

	// ����
	CKananLibrary::EndBlockComment("�p�����[�^�[�t�@�C���̓ǂݍ��ݏI��");
	return S_OK;
}

//=============================================================================
// �L�����N�^�[���Ƃ̃p�����[�^�[�Z�[�u
//=============================================================================
HRESULT CCharaParam::Save(const PARAM_TYPE type)
{
	// �ϐ��錾
	FILE *pFile;

	// �e�L�X�g�t�@�C�������₷������R�����g
	char cEqual[2] = "=";

	// �ϐ����p�Ə������ݗp�̕ϐ�
	char cHeadText[MAX_TEXT];
	char cWriteText[MAX_TEXT];

	char cMotionName[ATTACK_MAX][64] =
	{
		"�ʏ�U�� 1",
		"�ʏ�U�� 2",
		"�ʏ�U�� 3 ",
		"�ʏ�U�� 4 ",
		"�󒆃p���`",
		"�󒆃L�b�N",
		"�X�}�b�V��",
	};

	// �ʒu�E��]�̎擾�p
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// �R���\�[���ɕ\��
	CKananLibrary::StartBlockComment("�L�����N�^�[�̃p�����[�^�[�Z�[�u�J�n");

	// �t�@�C�����J��
	pFile = fopen(m_aFileName[type], "w");

	// nullcheck
	if (!pFile)
	{
		// �R���\�[���ɕ\��
		CKananLibrary::EndBlockComment("�t�@�C�����J���܂���ł���");
		// ���s
		return E_FAIL;
	}

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs("# �L�����N�^�[�p�����[�^�[\n", pFile);									// # �L�����N�^�[�p�����[�^�[
	fputs(COMMENT_AUTHOR, pFile);													// # Author : KANAN NAGANAWA
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("SCRIPT	# �����Ȃ��� ", pFile);											// SCRIPT
	fputs(COMMENT_NEW_LINE, pFile);													// \n
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("# ��{�X�e�[�^�X\n", pFile);												// # ��{�X�e�[�^�X
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================

	strcpy(cHeadText, "MAX_LIFE");
	sprintf(cWriteText, "%s      %s %.5f # �ő�HP\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].fMaxLife);
	fputs(cWriteText, pFile);														// MAX_LIFE = fMaxLife

	strcpy(cHeadText, "RUN_SPEED");
	sprintf(cWriteText, "%s     %s %.5f # �ړ����x\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].moveParam.fRunSpeed);
	fputs(cWriteText, pFile);														// RUN_SPEED = fRunSpeed

	strcpy(cHeadText, "JUMP_POWER");
	sprintf(cWriteText, "%s    %s %.5f # �W�����v��\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].moveParam.fJumpPower);
	fputs(cWriteText, pFile);														// JUMP_POWER = fJumpPower

	strcpy(cHeadText, "ATTACK_POWER");
	sprintf(cWriteText, "%s  %s %.5f # �U����\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].fAttackPower);
	fputs(cWriteText, pFile);														// ATTACK_POWER = fAttackPower

	strcpy(cHeadText, "DEFENSE_POWER");
	sprintf(cWriteText, "%s %s %.5f # �h���\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].fDefensePower);
	fputs(cWriteText, pFile);														// DEFENSE_POWER = fDefensePower

	fputs(COMMENT_NEW_LINE, pFile);													// \n

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("# ���[�V�������Ƃ̃p�����[�^�[\n", pFile);								// # ���[�V�������Ƃ̃p�����[�^�[
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================

	// �U�������J��Ԃ�
	for (int nCnt = 0; nCnt < ATTACK_MAX; nCnt++)
	{
		sprintf(cWriteText, "SET_MOTIONPARAM	# %s\n",
			cMotionName[nCnt]);
		fputs(cWriteText, pFile);													//	SET_MOTIONPARAM  # ���[�V������

		strcpy(cHeadText, "CANCEL_FRAME");
		sprintf(cWriteText, "	%s %s %d - %d\n",
			&cHeadText,
			&cEqual,
			m_playerParam[type].motionParam[nCnt].CancelFrame.start,
			m_playerParam[type].motionParam[nCnt].CancelFrame.end);
		fputs(cWriteText, pFile);													//		CANCEL_FRAME = start - end

		sprintf(cWriteText, "	DAMAGE %s %d\n",
			&cEqual,
			m_playerParam[type].motionParam[nCnt].nDamage);
		fputs(cWriteText, pFile);													//		DAMAGE = nDamage

		sprintf(cWriteText, "	DAUNTED %s %d\n",
			&cEqual,
			m_playerParam[type].motionParam[nCnt].nDaunted);
		fputs(cWriteText, pFile);													//		DAUNTED = nDaunted

		strcpy(cHeadText, "BLOWAWAY_TYPE");
		sprintf(cWriteText, "	%s %s %d\n",
			&cHeadText,
			&cEqual,
			(int)m_playerParam[type].motionParam[nCnt].blowType);
		fputs(cWriteText, pFile);													//		BLOWAWAY_TYPE = blowType
		fputs("END_MOTIONPARAM\n", pFile);											//	END_MOTIONPARAM
		fputs(COMMENT_NEW_LINE, pFile);												// \n
	}
	fputs("END_SCRIPT	# �����Ȃ��� \n", pFile);									// END_SCRIPT

	CKananLibrary::EndBlockComment("�L�����N�^�[�̃p�����[�^�[�Z�[�u�I��");
	// �t�@�C�������
	fclose(pFile);

	// ����
	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// ImGui�̍X�V
//=============================================================================
void CCharaParam::UpdateImGui(void)
{
	// �E�B���h�E��\�����邩
	ImGui::Checkbox("show charaParam window", &m_bShowWindow);

	// nullcheck
	if (!m_bShowWindow)
		// �\�����Ȃ�
		return;

	// ImGui�̍X�V�J�n
	ImGui::Begin("charaParam window", &m_bShowWindow);

	PARAM_TYPE charaType[MAX_PLAYER];
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		charaType[nCnt] = (PARAM_TYPE)(CManager::GetRenderer()->GetGame()->GetPlayer(nCnt)->GetCharaType() / 2);
	}

	char cName[PARAM_MAX][16] = 
	{
		"1 yasu",
		"2 yasu",
		"3 yasu",
		"4 yasu"
	};

	// �^�u�p�̐���\��
	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// �v���C���[1�Ɠ����L����
			if (nCnt != 0 && charaType[nCnt] == charaType[0])
				// �������Ȃ�
				continue;

			// �L�����p�����[�^�[�̏ڍ�
			ShowCharaParam(cName[charaType[nCnt]], charaType[nCnt]);
		}
		// �^�u�̏I���ɕK������
		ImGui::EndTabBar();
	}

	// ����{�^��
	if (ImGui::Button("close this window"))
		m_bShowWindow = false;

	// ImGui�̍X�V�I��
	ImGui::End();
}

//=============================================================================
// �L�����p�����[�^�[�̏ڍ�
//=============================================================================
void CCharaParam::ShowCharaParam(const char cName[16], PARAM_TYPE type)
{
	// �v���C���[1�̏��
	if (ImGui::BeginTabItem(cName))
	{
		// ���[�V�������̕\�L��ݒ�
		char cMotionName[ATTACK_MAX][64] =
		{
			"0_Normal",
			"1_Normal",
			"2_Normal",
			"3_Normal",
			"AirPunch",
			"AirKick",
			"Smash",
		};

		// ��{���
		ImGui::DragFloat("MaxLife", &m_playerParam[type].fMaxLife, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("RunSpeed", &m_playerParam[type].moveParam.fRunSpeed, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("JumpPower", &m_playerParam[type].moveParam.fJumpPower, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("AttackPower", &m_playerParam[type].fAttackPower, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("DefensePower", &m_playerParam[type].fDefensePower, SPEED_IMGUI_DRAGFLOAT);
		ImGui::Dummy(ImVec2(0.0f, 2.0f));


		// �^�u�p�̐���\��
		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			for (int nCnt = 0; nCnt < ATTACK_MAX; nCnt++)
			{
				// �^�u��
				if (ImGui::BeginTabItem(cMotionName[nCnt]))
				{
					// ���ږ�
					ImGui::InputInt2("CancelFrame", &m_playerParam[type].motionParam[nCnt].CancelFrame.start);
					ImGui::InputInt("Damage", &m_playerParam[type].motionParam[nCnt].nDamage);
					ImGui::InputInt("Daunted", &m_playerParam[type].motionParam[nCnt].nDaunted);
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					// �^�u�̏I���ɕK������
					ImGui::EndTabItem();
				}
			}
			// �^�u�̏I���ɕK������
			ImGui::EndTabBar();
		}

		if (ImGui::Button("Save"))
		{
			// �Z�[�u
			if (Save(type) == S_OK)
			{
				// �}�E�X�̎擾
				CMouse *pMouse = CManager::GetMouse();
				// �E�B���h�E�n���h���̎擾
				HWND hWnd = pMouse->GetWnd();
				// �m�F�̃��b�Z�[�W
				MessageBox(hWnd, "�p�����[�^�[�t�@�C�����Z�[�u���܂���", "�m�F", MB_OK);
			}
		}

		// �^�u�̏I���ɕK������
		ImGui::EndTabItem();
	}
}

#endif