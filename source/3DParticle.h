//*************************************************************************************************************
//
// 3D�p�[�e�B�N������[3DParticle.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "3DEffect.h"


//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C3DParticle : public CScene
{
public:
	typedef enum
	{
		OFFSET_NONE = -1,	// ����
		STONEAPPEAR_R,		// �X�g�[���ԏo��
		STONENORMAL_R,		// �X�g�[���Ԓʏ�
		STONEDISAPPEA_R,	// �X�g�[���ԏ���
		STONEAPPEAR_G,		// �X�g�[���Ώo��
		STONENORMAL_G,		// �X�g�[���Βʏ�
		STONEDISAPPEA_G,	// �X�g�[���Ώ���
		STONEAPPEAR_B,		// �X�g�[���o��
		STONENORMAL_B,		// �X�g�[���ʏ�
		STONEDISAPPEA_B,	// �X�g�[������
		STONEFLASH,			// �X�g�[���̔��s
		HIT,				// ��������
		STRONGHIT,			// ������������
		SMASHATTACKSTART,	// �X�}�b�V���U���J�n
		SMASHATTACKHIT,		// �X�}�b�V���U�����������Ƃ�
		SMASHDASH,			// �X�}�b�V���_�b�V��
		HITREFLECTION,		// ���˕Ԃ����Ƃ�
		CHARGE_R,			// �`���[�W��
		CHARGE_G,			// �`���[�W��
		CHARGE_B,			// �`���[�W��
		STONEHAVE_R,		// �����Ă���X�g�[����
		STONEHAVE_G,		// �����Ă���X�g�[����
		STONEHAVE_B,		// �����Ă���X�g�[����

		OFFSET_MAX			// �ő�
	}OFFSETNAME;

	typedef enum
	{
		BITS_NONE = -1,
		BILLBOARD,		// �r���{�[�h�t���O
		TARGET,			// �ڕW�_�L���t���O
		LIFE_AUTO,		// ���C�t�����t���O
		ALPHA_AUTO,		// �A���t�@�l�����t���O
		RADIUS_AUTO,	// ���a�ω��l�����t���O
		MOVE_AUTO,		// �ړ��ʎ����t���O
		DIR_REVERSE,	// �ړ������t�t���O
		PARENT,			// �e�t���O
		CLING_PARENT,	// �e�ɂ������t���O
		MOVE_CHANGE,	// �ړ��ʕω��t���O
		DIREQUAPOS,		// �����ƕ��ˈʒu�𓯂��ɂ���
		BITS_MAX		// �ő吔
	} BIT_INDEX;

	typedef struct PARAM
	{
		PARAM();							// �R���X�g���N�^
		int				nTimer;				// �^�C�}�[
		int				nFrame;				// �t���[����
		int				nNumber;			// ��
		int				nType;				// ���
		int				nTexType;			// �e�N�X�`���̎��
		D3DXVECTOR3*	pPos;				// �ʒu�̃|�C���^
		RANGE*			pPosXRand;			// �ʒuX���W�����_���p�̃|�C���^
		RANGE*			pPosYRand;			// �ʒuY���W�����_���p�̃|�C���^
		RANGE*			pPosZRand;			// �ʒuY���W�����_���p�̃|�C���^
		float			fLength;			// ���_����̋���
		RANGE*			pLengthRand;		// ���_����̋��������_���p�̃|�C���^
		D3DXCOLOR		col;				// �F
		float			fRadius;			// ���a
		RANGE*			pRadiusRand;		// ���a�̃����_���p�̃|�C���^
		float			fRadiusValue;		// ���a�̕ω���
		float			fAngle;				// ���ˊp�x
		RANGE*			pAngleRand;			// ���ˊp�x�̃����_���p�̃|�C���^
		float			fSpeed;				// ���x
		RANGE*			pSpeedRand;			// ���x�̃����_���p�̃|�C���^
		int				nLife;				// ���C�t
		RANGE*			pLifeRand;			// ���C�t�̃����_���p�̃|�C���^
		float			fGravity;			// �d��
		D3DXVECTOR3		rot;				// ����
		D3DXVECTOR3		Target;				// �ڕW�n�_
		float			fInertiaCoeffi;		// �����W��
		D3DXCOLOR		colValue;			// �F�̕ω��l
		RANGE*			pColRRand;			// �F��R�����_���p�̃|�C���^
		RANGE*			pColGRand;			// �F��G�����_���p�̃|�C���^
		RANGE*			pColBRand;			// �F��B�����_���p�̃|�C���^
		RANGE*			pColARand;			// �F��A�����_���p�̃|�C���^
		float			fScalCol;			// �F�̏k���l
		UBITS_12		Flag;				// �t���O

		bool			bParent;			// �e�t���O
		bool			bDirEquaPos;		// �����ƕ��ˈʒu�𓯂��ɂ���

	}PARAM;

	typedef struct OFFSET
	{
		OFFSET();					// �R���X�g���N�^
		int          nNumParam;		// �g���p�����[�^��
		PARAM*       pParam;		// �p�����[�^�P�̂̈ʒu
	} OFFSET;

	C3DParticle() : CScene(CScene::PRIORITY_EFFECT) {}
	~C3DParticle() {}

	static void Load(void);																				// �ǂݍ���
	static void Unload(void);																			// �J��
	static void ReleaseParam(OFFSET *pOffset);															// �p�����[�^�̊J��
	static void ReleaseParamMemb(PARAM * pParam);														// �p�����[�^�����o�̊J��

	static void LoadFromFile(CONST_STRING pFileName, const int &nCntOffSet);							// �t�@�C������ǂݍ���
	static void LoadParamFromFile(FILE *pFile, const int &nCntOffSet, const int &nCntParam);			// �t�@�C������p�����[�^��ǂݍ���

	void Init(void);																					// ����������
	void Uninit(void);																					// �I������
	void Update(void);																					// �X�V����
	void Draw(void);																					// �`�揈��

	void SetEffectFromParam(PARAM * pParam);															// �p�����[�^����G�t�F�N�g��ݒ�

	static C3DParticle* Set(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, CONST OFFSETNAME & name);			// �ݒ�

private:
	void GetRadiuFromParam(CONST PARAM * pParam, float &fRadius);										// �p�����[�^���甼�a���擾
	void GetLifeFromParam(CONST PARAM * pParam, int &nLife);											// �p�����[�^���烉�C�t���擾
	D3DXVECTOR3 * GetParentFromParam(CONST PARAM * pParam);												// �p�����[�^����e���擾
	void GetPosFromParam(CONST PARAM * pParam, D3DXVECTOR3 &pos);										// �p�����[�^����ʒu���擾
	void GetMoveFromParam(CONST PARAM * pParam, D3DXVECTOR3 &move);										// �p�����[�^����ړ��ʂ��擾
	void GetSpeedFromParam(CONST PARAM * pParam, float &fSpeed);										// �p�����[�^���瑬�x���擾
	void GetMoveAndPosAccordingDirFromParam(CONST PARAM * pParam, D3DXVECTOR3 &move, D3DXVECTOR3 &pos);	// �p�����[�^����ړ��ʂƕ����ɕ�����ʒu�̎擾
	void GetLengthFromParam(CONST PARAM * pParam, float &fLength);										// �p�����[�^���猴�_����̋������擾
	bool ReleaseCheck(void);																			// �J���ł��邩�`�F�b�N����
	void SetPos(D3DXVECTOR3* pPos);																		// �ʒu�̐ݒ�
	void SetRot(D3DXVECTOR3* pRot);																		// �����̐ݒ�
	void SetName(CONST OFFSETNAME& name);																// �I�t�Z�b�g���̐ݒ�
	void StandbyFrame(const int& nNumParam);															// �t���[���|�C���^�̏���
	static OFFSET m_Offset[OFFSET_MAX];																	// �I�t�Z�b�g���
	D3DXVECTOR3* m_pPosParent;																			// �e�̈ʒu
	D3DXVECTOR3  m_pos;																					// �ʒu
	D3DXVECTOR3* m_pRotParent;																			// �e�̌���
	D3DXVECTOR3  m_rot;																					// ����
	OFFSETNAME   m_Name;																				// �I�t�Z�b�g��
	int          m_nTimer;																				// �^�C�}�[�J�E���g
	int*         m_pFrame;																				// �t���[���|�C���^
};










#endif