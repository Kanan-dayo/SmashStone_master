//==================================================================================================================
//
// �Q�[������[game.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _GAME_H
#define _GAME_H

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "Mylibrary.h"
#include "kananlibrary.h"
#include "stone.h"
#include <memory>	// �X�}�[�g�|�C���^�̎g�p�ɕK�v
#include "PolygonCollider.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define STONE_POS (5)
#define POLYCOLLI_USE_TYPE	4

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CMeshField;
class CCamera;
class CLight;
class CLogo;
class CModel;
class CPause;
class CMeshSphere;
class CPlayer;
class CTime;
class CWall;
class CObjectManager;
class CPolygonCollider;
class CUIKO;
class CUI_GameStart;
class CUI_GameResult;
class CRenderer;
class CUI;

//==================================================================================================================
//
// �Q�[���N���X�̒�`
//
//==================================================================================================================
class CGame
{
public:
	// ==========================================================
	// �Q�[���̏��
	// ==========================================================
	typedef enum
	{
		GAMESTATE_NONE = 0,		// �������Ă��Ȃ����
		GAMESTATE_BEFORE,		// �Q�[���̑O
		GAMESTATE_NORMAL,		// �ʏ���
		GAMESTATE_PAUSE,		// �|�[�Y���
		GAMESTATE_START_OVER,	// ���߂����蒼��
		GAMESTATE_BREAK,		// �Q�[�����f
		GAMESTATE_KO,			// KO���o
		GAMESTATE_KO_AFTER,		// KO��
		GAMESTATE_NEXTROUND,	// ���̃��E���h��
		GAMESTATE_RESULT,		// ���U���g
		GAMESTATE_END,			// �I��
		GAMESTATE_MAX			// �ő�
	} GAMESTATE;				// �Q�[���̏��

	CGame();									// �R���X�g���N�^
	~CGame();									// �f�X�g���N�^
	void Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��
	static CGame *Create(void);					// ��������

	static void SetPlayerType(const int nPlayer, const int type)
		{ m_nPlayerType[nPlayer] = type * 2; }									// �v���C���[�^�C�v��ۑ�
	static void SetStageType(const int type)	{ m_nStageType = type; }		// �X�e�[�W�^�C�v�̐ݒ�
	static void SetGameState(GAMESTATE state)	{ m_gameState = state; }		// �Q�[���̏�Ԑݒ�
	static void RemoveNumStone(int nIndexPos)	{ m_nNumStone--; m_bSetPos[nIndexPos] = false; }			// �X�g�[���̐������Z
	static void RemoveTypeStone(int nType)		{ m_bGetType[nType] = false; }			// �X�g�[���̎擾�𖳌�
	static void SetNumStone(const int &nStone)	{ m_nNumStone = nStone; }		// �X�g�[���̐��̐ݒ�
	static void SetNextMode(const int nextMode);								// ���̃��[�h�ֈڍs

	static GAMESTATE GetGameState(void)			{ return m_gameState; }			// �Q�[���̏�Ԏ擾
	static CPlayer *GetPlayer(int nPlayer)		{ return m_pPlayer[nPlayer]; }	// �L�����N�^�[�̏��擾����
	static CCamera *GetCamera(void)				{ return m_pCamera; }			// �J�����̎擾
	static CLight *GetLight(void)				{ return m_pLight; }			// ���C�g�̎擾
	static CMeshField *GetMeshField(void)		{ return m_pMeshField; }		// ���b�V���t�B�[���h�̎擾
	static int GetNumStone(void)				{ return m_nNumStone; }			// �X�g�[���̐����擾
	static CWall *GetWall(void)					{ return m_pWall; }				// �ǂ̎擾
	static CObjectManager *GetObjMana(void)		{ return m_pObjMana; }			// �I�u�W�F�N�g�}�l�[�W���[���擾
	static CPolygonCollider* GetpolyColly(int nIndex) { return m_pPolyColli[nIndex]; }
	static int GetRound(void)					{ return m_nRound; }			// ���݂̃��E���h���擾
	static int GetAllRound(void)				{ return m_nRoundAll; }			// �S���E���h���̎擾
	static NUM_PLAYER GetLosePlayer(void)		{ return m_losePlayer; }		// �������v���C���[�̎擾
	static void AppearStone(void);				// �ǂ�����ł��Ăяo����X�g�[���o��
	static bool GetbGetType(int nType)			{ return m_bGetType[nType]; }	// �擾���ꂽ�^�C�v�̎擾
	static int GetStageType(void)				{ return m_nStageType; }

protected:

private:
	void GameBefore(void);							// �Q�[���̑O�̍X�V
	void GameNormal(void);							// �ʏ�̍X�V
	void GamePause(void);							// �|�[�Y�̍X�V
	void GameKO(void);								// KO�̍X�V
	void GameKOAfter(void);							// KO�̌�̍X�V
	void SwitchPause(void);							// �|�[�Y�̐؂�ւ�
	void NextRound(void);							// ���̃��E���h��
	void GameResult(void);							// �Q�[���̃��U���g
	void DecideCreateStone(void);					// �X�g�[���𐶐����邩���߂�
	static GAMESTATE m_gameState;					// �Q�[�����
	static int m_nPlayerType[MAX_PLAYER];			// �L�����N�^�[�Z���N�g���̃^�C�v��ۑ�
	static CPlayer *m_pPlayer[MAX_PLAYER];			// �v���C���[�̔z��|�C���^
	static CMeshField *m_pMeshField;				// ���b�V���t�B�[���h�̏��|�C���^
	static CCamera *m_pCamera;						// �J�����̏��|�C���^
	static CLight *m_pLight;						// ���C�g�̏��|�C���^
	static CLogo *m_pLogo;							// ���S�̏��|�C���^
	static CPause *m_pPause;						// �|�[�Y�̏��|�C���^
	static CMeshSphere *m_pMeshSphere;				// ���b�V�����̏��|�C���^
	static CTime *m_pTime;							// �^�C���̏��|�C���^
	static CWall *m_pWall;							// �ǂ̃|�C���^
	static CUI *m_pUI;								// UI�|�C���^
	static int m_nCounterGameState;					// �Q�[���̏�ԊǗ��J�E���^�[
	static int m_nNumStone;							// ���������X�g�[���̐�
	static int m_nCntDecide;						// �X�g�[�������̃^�C�~���O�����߂�J�E���^
	static bool m_bSetPos[STONE_POS];				// �X�g�[������������Ă��邩
	static bool m_bGetType[CStone::STONE_ID_MAX];		// �X�g�[�����擾����Ă��邩
	static CObjectManager *m_pObjMana;				// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	static CPolygonCollider* m_pPolyColli[CPolygonCollider::POLYCOLLI_MAX];		// �|���S���R���C�_�[�̃|�C���^
	static CUIKO *m_pUIKO;							// KO�̃|�C���^
	static CUI_GameStart *m_pUIGameStart;			// �Q�[���J�n����UI�̃|�C���^
	static CUI_GameResult *m_pUIGameResult;			// �Q�[�����U���g��UI�̃|�C���^
	static NUM_PLAYER m_winPlayer;					// ���������v���C���[
	static NUM_PLAYER m_losePlayer;					// �������v���C���[
	INTEGER2 m_roundPoint;							// ���E���h�̃|�C���g��
	static int m_nRound;							// ���݂̃��E���h
	static int m_nRoundAll;							// �S���E���h��
	static int m_nStageType;						// �X�e�[�W�̃^�C�v
	int m_nCntAny;									// ���[�h���̗l�X�ȏ󋵂Ŏg��(��Ɏ��ԊǗ�)
};
#endif
