//=============================================================================
//
// ���[�V��������  [ motion.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "Inputkeyboard.h"
#include "debugproc.h"
#include "modelcharacter.h"
#include "ImGui/imgui.h"				// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"		// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"		// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
CMotion::MOTION_INFO CMotion::m_pMotionInfo[MAX_CHARACTER_TYPE][CMotion::MOTION_MAX] = {};
char CMotion::m_apFileName[MAX_CHARACTER_TYPE][CMotion::MOTION_MAX][MAX_TEXT] =
{
	{{ "data/MOTION/fokker/motion_neutral.txt" },
	{ "data/MOTION/fokker/motion_run.txt" },
	{ "data/MOTION/fokker/motion_jump.txt" },
	{ "data/MOTION/fokker/motion_fall.txt" },
	{ "data/MOTION/fokker/motion_lift.txt" },
	{ "data/MOTION/fokker/motion_lift_neutral.txt" },
	{ "data/MOTION/fokker/motion_lift_walk.txt" },
	{ "data/MOTION/fokker/motion_throw.txt" },
	{ "data/MOTION/fokker/motion_attack_0.txt" },
	{ "data/MOTION/fokker/motion_attack_1.txt" },
	{ "data/MOTION/fokker/motion_attack_2.txt" },
	{ "data/MOTION/fokker/motion_attack_3.txt" },
	{ "data/MOTION/fokker/motion_smashCharge.txt" },
	{ "data/MOTION/fokker/motion_smash.txt" },
	{ "data/MOTION/fokker/motion_daunted.txt" },
	{ "data/MOTION/fokker/motion_blowAway.txt" },
	{ "data/MOTION/fokker/motion_down.txt" },
	{ "data/MOTION/fokker/motion_get_up.txt" },
	{ "data/MOTION/fokker/motion_get_up_acrobatic.txt" } },

	{ { "data/MOTION/niyasu/motion_neutral.txt" },
	{ "data/MOTION/niyasu/motion_run.txt" },
	{ "data/MOTION/niyasu/motion_jump.txt" },
	{ "data/MOTION/niyasu/motion_fall.txt" },
	{ "data/MOTION/niyasu/motion_lift.txt" },
	{ "data/MOTION/niyasu/motion_lift_neutral.txt" },
	{ "data/MOTION/niyasu/motion_lift_walk.txt" },
	{ "data/MOTION/niyasu/motion_throw.txt" },
	{ "data/MOTION/niyasu/motion_attack_0.txt" },
	{ "data/MOTION/niyasu/motion_attack_1.txt" },
	{ "data/MOTION/niyasu/motion_attack_2.txt" },
	{ "data/MOTION/niyasu/motion_attack_3.txt" },
	{ "data/MOTION/niyasu/motion_smashCharge.txt" },
	{ "data/MOTION/niyasu/motion_smash.txt" },
	{ "data/MOTION/niyasu/motion_daunted.txt" },
	{ "data/MOTION/niyasu/motion_blowAway.txt" },
	{ "data/MOTION/niyasu/motion_down.txt" },
	{ "data/MOTION/niyasu/motion_get_up.txt" },
	{ "data/MOTION/niyasu/motion_get_up_acrobatic.txt" } },

	{ { "data/MOTION/sanyasu/motion_neutral.txt" },
	{ "data/MOTION/sanyasu/motion_run.txt" },
	{ "data/MOTION/sanyasu/motion_jump.txt" },
	{ "data/MOTION/sanyasu/motion_fall.txt" },
	{ "data/MOTION/sanyasu/motion_lift.txt" },
	{ "data/MOTION/sanyasu/motion_lift_neutral.txt" },
	{ "data/MOTION/sanyasu/motion_lift_walk.txt" },
	{ "data/MOTION/sanyasu/motion_throw.txt" },
	{ "data/MOTION/sanyasu/motion_attack_0.txt" },
	{ "data/MOTION/sanyasu/motion_attack_1.txt" },
	{ "data/MOTION/sanyasu/motion_attack_2.txt" },
	{ "data/MOTION/sanyasu/motion_attack_3.txt" },
	{ "data/MOTION/sanyasu/motion_smashCharge.txt" },
	{ "data/MOTION/sanyasu/motion_smash.txt" },
	{ "data/MOTION/sanyasu/motion_daunted.txt" },
	{ "data/MOTION/sanyasu/motion_blowAway.txt" },
	{ "data/MOTION/sanyasu/motion_down.txt" },
	{ "data/MOTION/sanyasu/motion_get_up.txt" },
	{ "data/MOTION/sanyasu/motion_get_up_acrobatic.txt" } },

	{ { "data/MOTION/yonyasu/motion_neutral.txt" },
	{ "data/MOTION/yonyasu/motion_run.txt" },
	{ "data/MOTION/yonyasu/motion_jump.txt" },
	{ "data/MOTION/yonyasu/motion_fall.txt" },
	{ "data/MOTION/yonyasu/motion_lift.txt" },
	{ "data/MOTION/yonyasu/motion_lift_neutral.txt" },
	{ "data/MOTION/yonyasu/motion_lift_walk.txt" },
	{ "data/MOTION/yonyasu/motion_throw.txt" },
	{ "data/MOTION/yonyasu/motion_attack_0.txt" },
	{ "data/MOTION/yonyasu/motion_attack_1.txt" },
	{ "data/MOTION/yonyasu/motion_attack_2.txt" },
	{ "data/MOTION/yonyasu/motion_attack_3.txt" },
	{ "data/MOTION/yonyasu/motion_smashCharge.txt" },
	{ "data/MOTION/yonyasu/motion_smash.txt" },
	{ "data/MOTION/yonyasu/motion_daunted.txt" },
	{ "data/MOTION/yonyasu/motion_blowAway.txt" },
	{ "data/MOTION/yonyasu/motion_down.txt" },
	{ "data/MOTION/yonyasu/motion_get_up.txt" },
	{ "data/MOTION/yonyasu/motion_get_up_acrobatic.txt" } }
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMotion::CMotion()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// ���[�V�����̃��[�h
//=============================================================================
HRESULT CMotion::Load()
{
	// �u���b�N�R�����g
	CKananLibrary::StartBlockComment("���[�V�����t�@�C���̓ǂݍ��݊J�n");

	for (int charaType = 0; charaType < MAX_CHARACTER_TYPE; charaType++)
	{
		char cText[32];
		sprintf(cText, "[%d]�Ԗڂ̃L�����N�^�[", charaType);
		std::cout << cText << std::endl;

		for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
		{
			// ���f���ǂݍ���
			LoadMotion((PARAM_TYPE)charaType, (CMotion::MOTION_TYPE)nCnt);
		}
	}

	// �u���b�N�R�����g
	CKananLibrary::EndBlockComment("���[�V�����t�@�C���̓ǂݍ��ݏI��");

	// ����
	return S_OK;
}

//=============================================================================
// ���[�V�����̃A�����[�h
//=============================================================================
void CMotion::UnLoad()
{
	for (int charaType = 0; charaType < MAX_CHARACTER_TYPE; charaType++)
	{
		// ���[�V��������
		for (int nCntMotion = 0; nCntMotion < MOTION_MAX; nCntMotion++)
		{
			// �L�[����
			for (int nCntKeyInfo = 0; nCntKeyInfo < m_pMotionInfo[charaType][nCntMotion].nNumKey; nCntKeyInfo++)
			{
				// nullcheck
				if (m_pMotionInfo[charaType][nCntMotion].pKeyInfo[nCntKeyInfo].pKey)
				{
					// ���݂̃L�[�̃������J��
					delete[] m_pMotionInfo[charaType][nCntMotion].pKeyInfo[nCntKeyInfo].pKey;
					m_pMotionInfo[charaType][nCntMotion].pKeyInfo[nCntKeyInfo].pKey = nullptr;
				}
			}
			// nullcheck
			if (m_pMotionInfo[charaType][nCntMotion].pKeyInfo)
			{
				// ���݂̃L�[�C���t�H�̃������J��
				delete[] m_pMotionInfo[charaType][nCntMotion].pKeyInfo;
				m_pMotionInfo[charaType][nCntMotion].pKeyInfo = nullptr;
			}
		}
	}
}

//=============================================================================
// �e�L�X�g�t�@�C�����烂�[�V�������[�h
//=============================================================================
HRESULT CMotion::LoadMotion(PARAM_TYPE charaType, MOTION_TYPE motiontype)
{
	// �ϐ��錾
	FILE *pFile;
	char cReadText[MAX_TEXT] = {};
	char cHeadText[MAX_TEXT] = {};
	char cDieText[MAX_TEXT] = {};
	int nLoop = 0;
	int nCntkeyInfo = 0;
	int nCntKey = 0;
	bool bInfo = false;	// �Ȃ����L�[����������̂ŁA���̕ϐ��ň��ɂ���

	// �t�@�C�����J��
	pFile = fopen(m_apFileName[charaType][motiontype], "r");

	// nullcheck
	if (!pFile)
	{
		// �t�@�C���ǂݍ��ݎ��s
		std::cout << m_apFileName[charaType][motiontype] << " �̓ǂݍ��ݎ��s [�~]" << std::endl;
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
			// ���[�V�����Z�b�g
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// �G���h���[�V�����Z�b�g������܂ŌJ��Ԃ�
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// ���s
					if (strcmp(cHeadText, "\n") == 0)
					{
					}
					// ���[�v���邩
					else if (strcmp(cHeadText, "LOOP") == 0)
					{
						// ��xint�^�Ŋi�[����
						sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nLoop);

						// 1��0��bool�Ƃ��Ĕ��f����
						m_pMotionInfo[charaType][motiontype].bLoop = nLoop ? true : false;
					}
					// �L�[��
					else if (strcmp(cHeadText, "NUM_KEY") == 0 && !bInfo)
					{
						sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfo[charaType][motiontype].nNumKey);

						// �L�[�����������m��
						m_pMotionInfo[charaType][motiontype].pKeyInfo = new KEY_INFO[m_pMotionInfo[charaType][motiontype].nNumKey];

						// �L�[�Z�b�g���s���̂ŁA�L�[���J�E���g������������
						nCntkeyInfo = 0;

						bInfo = true;
					}
					// �L�[�Z�b�g
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// �p�[�c�����̃L�[��錾
						int nKey = CModelCharacter::GetPartsNum((CHARACTER_TYPE)(charaType * 2));

						// �L�[�����������m��
						m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].pKey = new KEY[nKey];

						// �L�[�J�E���g������������
						nCntKey = 0;

						// �G���h�L�[�Z�b�g������܂ŌJ��Ԃ�
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);
							// �t���[����
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].nFrame);
							}
							// �U��
							if (strcmp(cHeadText, "ATTACK_ON") == 0)
							{
								// �U���L�[��ۑ�
								m_pMotionInfo[charaType][motiontype].nAttackKey = nCntkeyInfo;
							}
							// �U��
							if (strcmp(cHeadText, "ATTACK_INDEX") == 0)
							{
								int nAttackIndex = -1;
								if (sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nAttackIndex) == 3)
									m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].nAttackIndex.push_back(nAttackIndex);
							}
							// �L�[
							if (strcmp(cHeadText, "KEY") == 0)
							{
								// �G���h�L�[������܂ŌJ��Ԃ�
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);
									// �ʒu
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].posDest.x,
											&m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].posDest.y, &m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].posDest.z);
									}
									// ��]
									if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].rotDest.x,
											&m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].rotDest.y, &m_pMotionInfo[charaType][motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].rotDest.z);
									}
									// �I��
									if (strcmp(cHeadText, "END_KEY") == 0)
									{
										// �L�[���Z
										nCntKey++;
									}
								}
							}
							// �I��
							if (strcmp(cHeadText, "END_KEYSET") == 0)
							{
								// �L�[�����Z
								nCntkeyInfo++;
								bInfo = false;
							}
						}
					}
				}
			}
		}
	}
	// �t�@�C�������
	fclose(pFile);

	// �f�o�b�O�R�����g�\��
	std::cout << m_apFileName[charaType][motiontype] << " �̓ǂݍ��݊���" << std::endl;

	return S_OK;
}
