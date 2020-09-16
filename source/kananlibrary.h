// ===================================================================
//
// ���C�u�������� [ kananlibrary.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _KANANLIBRARY_H_
#define _KANANLIBRARY_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "main.h"
#include <string>	// string�^�Ȃǂ̕�����̑���ɕK�v

// ===================================================================
// �}�N����`
// ===================================================================
// �x�N�g��
#define ZeroVector2		(D3DXVECTOR2(0.0f, 0.0f))
#define OneVector2		(D3DXVECTOR2(1.0f, 1.0f))
#define ZeroVector3		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define OneVector3		(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
#define OneVec2Zero		(D3DXVECTOR3(1.0f, 1.0f, 0.0f))
#define ZeroVector4		(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))

// �J���[
#define ZeroColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))		// ���F
#define AlphaColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))		// ����
#define WhiteColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// ��
#define BlackColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// ��

// �@��
#define AboveNormal		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))			// �^��̖@���x�N�g��

// �e�L�X�g
#define MAX_TEXT			(128)									// �e�L�X�g�̍ő吔
#define COMMENT_BLOCK		("#============================================================\n")	// �u���b�N�R�����g�̘g
#define COMMENT_NEW_LINE	("\n")									// ���s
#define COMMENT_BLOCK_LINE	("#\n")									// �R�����g���s
#define COMMENT_AUTHOR		("# Author : KANAN NAGANAWA\n")			// ���Җ�

// ����
#define INERTIA				(0.3f)								// ����
#define GRAVITY				(0.3f)								// �d��

// ����
#define ONE_SECOND_FPS		(60)								// ��b���̃t���[����

// ===================================================================
// �\���̒�`
// ===================================================================
/*typedef struct INTEGER2
{	// int�^�̍\����
	int x;
	int y;

	// �R���X�g���N�^
	INTEGER2() {};
	INTEGER2(int nX, int nY)
	{
		x = nX;
		y = nY;
	}
} INTEGER2;*/

/*typedef struct FLOAT2
{	// int�^�̍\����
	float x;
	float y;

	// �R���X�g���N�^
	FLOAT2() {};
	FLOAT2(float fX, float fY)
	{
		x = fX;
		y = fY;
	}
} FLOAT2;*/

typedef struct MODEL_VTX
{
	D3DXVECTOR3 VtxMax;	// ���_�̍ő�l
	D3DXVECTOR3 VtxMin;	// ���_�̍ŏ��l

						// �R���X�g���N�^
	MODEL_VTX() {};
	MODEL_VTX(D3DXVECTOR3 Max, D3DXVECTOR3 Min)
	{
		VtxMax = Max;
		VtxMin = Min;
	}
} MODEL_VTX;

typedef struct
{	// ���f�����̏��
	char			cModelName[MAX_TEXT];	// ���f���̃t�@�C����
	char			cTextureName[MAX_TEXT];	// �e�N�X�`���̃t�@�C����
	LPD3DXMESH		mesh;				// ���b�V��
	LPD3DXBUFFER	matBuff;			// ���_���
	DWORD			matNum;				// �}�e���A����
	LPDIRECT3DTEXTURE9 pTexture;		// �e�N�X�`��
	bool			bTex;				// �e�N�X�`�������邩
} MODELINFO;

typedef struct
{	// ���f���̏ڍׂ��i�[����\����
	MODELINFO modelInfo;	// ���f�����
	MODEL_VTX modelVtx;		// ���f�����_���
} OBJINFO;

typedef struct
{	// �L�����N�^�[���������f���̏��
	MODELINFO	*pModelInfo;	// ���f������}�e���A���Ȃǂ̃��f�����
	int			nNumParts;		// ���f���̃p�[�c��
	int			nNumTexture;	// �e�N�X�`����
} MODELCHARACTER;

typedef enum
{	// �ړ��p�̃L�[
	ONE_UP = DIK_W,			// �����
	ONE_DOWN = DIK_S,		// ������
	ONE_LEFT = DIK_A,		// ������
	ONE_RIGHT = DIK_D,		// �E����
	ONE_JUMP = DIK_SPACE,	// �W�����v����
	ONE_LIFT = DIK_B,		// �����グ����
	ONE_ATTACK = DIK_N,		// �U������
	ONE_SMASH = DIK_M		// �X�}�b�V������
} PLAYER_MOVE_KEY_ONE;

typedef enum
{	// �ړ��p�̃L�[
	TWO_UP	= DIK_UPARROW,			// �����
	TWO_DOWN = DIK_DOWNARROW,		// ������
	TWO_LEFT = DIK_LEFTARROW,		// ������
	TWO_RIGHT = DIK_RIGHTARROW,		// �E����
	TWO_JUMP = DIK_NUMPAD0,			// �W�����v����
	TWO_LIFT = DIK_NUMPAD1,			// �����グ����
	TWO_ATTACK = DIK_NUMPAD2,		// �U������
	TWO_SMASH = DIK_NUMPAD3			// �X�}�b�V������
} PLAYER_MOVE_KEY_TWO;

typedef enum
{	// �L�����N�^�[�̎��
	CHARACTER_NONE,			// �����Ȃ�
	CHARACTER_1YASU = 0,	// 1���X
	CHARACTER_1YASU_TRANS,	// 1���X�ϐg
	CHARACTER_2YASU,		// 2���X
	CHARACTER_2YASU_TRANS,	// 2���X�ϐg
	CHARACTER_3YASU,		// 3���X
	CHARACTER_3YASU_TRANS,	// 3���X�ϐg
	CHARACTER_4YASU,		// 4���X
	CHARACTER_4YASU_TRANS,	// 4���X�ϐg
	CHARACTER_MAX			// �L�����N�^�[�̍ő吔
} CHARACTER_TYPE;

typedef enum
{	// �p�����[�^�[�̃^�C�v
	PARAM_1YASU = 0,	// 1���X
	PARAM_2YASU,		// 2���X
	PARAM_3YASU,		// 3���X
	PARAM_4YASU,		// 4���X
	PARAM_MAX			// �ő吔
} PARAM_TYPE;

typedef enum
{	// �v���C���[��
	PLAYER_NONE,	// ���Ȃ�
	PLAYER_ONE = 0,	// 1P
	PLAYER_TWO,		// 2P
	PLAYER_MAX		// �ő吔
} NUM_PLAYER;

typedef enum
{	// �X�e�[�W�̎��
	STAGE_1 = 0,	// �X�e�[�W1
	STAGE_2,		// �X�e�[�W2
	STAGE_MAX		// �ő吔
} STAGETYPE;

// ===================================================================
// �N���X��`
// ===================================================================
class CInputKeyboard;
class CInputGamepad;

class CKananLibrary
{
public:
	CKananLibrary() {};
	~CKananLibrary() {};

	static void CalcMatrix(D3DXMATRIX *pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot);				// �}�g���b�N�X�v�Z
	static void CalcShadowMatrix(D3DXMATRIX & mtxShadow, D3DXVECTOR3 const & pos, D3DXVECTOR3 const & nor);	// �e�̃}�g���b�N�X�v�Z

	static MODEL_VTX OutputModelVtxColl(LPD3DXMESH mesh);												// ���f���̃��b�V�����璸�_���W�̍ő�E�ŏ����o��

	static void ReleaseModelInfo(MODELINFO *pModelInfo);												// ���f�����̔j��
	static void Billboard(D3DXMATRIX *mtxWorld, const D3DXMATRIX mtxView);								// �r���{�[�h������
	static void InterpolationRot(D3DXVECTOR3 *rot);														// ��]�̕��
	static void InterpolationFloat(float & Float);														// float�^�̕��
	static void InertiaMove(D3DXVECTOR3 *move);															// �����̏���
	static void InertiaMoveXZ(D3DXVECTOR3 *move);															// �����̏���
	static void Gravity(float & fMoveY);																	// �d�͂̏���
	static void IntegerUpperLimit(int *nValue, int upperLimit);											// int�^�̏��
	static void IntegerLowerLimit(int *nValue, int lowerLimit);											// int�^�̉���
	static bool FloatLowerLimit(float *fValue, const float &lowerLimit);								// float�^�̉���
	static bool LimitVector3(D3DXVECTOR3 &Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit);		// ��������̐�������
	static bool LimitVector2(D3DXVECTOR2 &Value, const D3DXVECTOR2 lowerLimit, const D3DXVECTOR2 upperLimit);		// ��������̐�������
	static void LimitLoopVector3(D3DXVECTOR3 *Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit);	// ��������̃��[�v����
	static void SetFog(const float fStartPos, const float fEndPos, const D3DXCOLOR col);							// �t�H�O�̐���
	static void LimitColor(D3DXCOLOR &col, const D3DXCOLOR lowerLimit, const D3DXCOLOR upperLimit);		// �J���[�̉������

	static void CreateModelInfo(MODELINFO *pModelInfo);													// ���f���̐���

	static void PrintBlockCommentFrame(void);															// �u���b�N�R�����g�g�̕\��
	static void StartBlockComment(const std::string &str);												// �u���b�N�R�����g�̊J�n
	static void EndBlockComment(const std::string &str);												// �u���b�N�R�����g�̏I��

	static HRESULT Up(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// �����
	static HRESULT Down(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// ������
	static HRESULT Left(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// ������
	static HRESULT Right(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// �E����
	static HRESULT Decide(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// �������
	static HRESULT Return(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// �߂����
	static HRESULT Pause(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// �|�[�Y����
	static HRESULT GetMoveByKeyboard(CInputKeyboard *pKey, int nPlayer);	// �L�[�{�[�h�ňړ����͂��Ă��邩�擾
	static HRESULT GetMoveByGamepad(CInputGamepad *pGamepad);					// �Q�[���p�b�h�ő��E���Ă��邩�擾

	static int DecideRandomValue(int nMaxValue, bool *bSelect); // �I�����̒����烉���_���̒l��Ԃ�

	static float OutputSqrt(D3DXVECTOR3 difpos);				// �������v�Z
	static float OutputDistance(D3DXVECTOR3 difpos);			// �����v�Z
	static D3DXVECTOR3 OutputVector(const D3DXVECTOR3 & start, const D3DXVECTOR3 & end);	// �x�N�g���v�Z

	static void SameLineDummy(const float & fX, const float & fY);

	static HRESULT CreateTexture(LPDIRECT3DTEXTURE9 *pTexture, char *cTextureName);

#ifdef _DEBUG
	static void ShowDebugInfo(void);					// ImGui�̍X�V
	static void ShowOffsetInfo(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move);					// ���W�Ȃǂ̏��
	static bool GetWire(void) { return m_WireFrame; }	// ���C���[�t���[���`��̎擾
#endif

private:

#ifdef _DEBUG
	static bool m_WireFrame;			// ���C���[�t���[���`��̐؂�ւ��p
	static int m_nCulling;				// �J�����O�̐؂�ւ��p
#endif

};
#endif