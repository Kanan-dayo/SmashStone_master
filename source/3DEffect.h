//*************************************************************************************************************
//
// 3D�G�t�F�N�g����[3DEffect.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _3DEFFECT_H_
#define _3DEFFECT_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"
#include "scene.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define _3DEFFE_USEQUANTITY 1000		// �g�p����G�t�F�N�g��
#define _3DEFFE_USEVERTEX   4			// �g�p���钸�_��

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C3DEffect :public CScene
{
public:
	// �񋓌^��`
	typedef enum TYPE
	{
		TYPE_NONE = -1,		// ����
		NORMAL,				// �ʏ�
		CHAR,				// ����
		TYPE_MAX			// �ő�
	}TYPE;

	typedef enum TEXTURETYPE
	{
		TEXTYPE_NONE = -1,		// ����
		DEFAULT_1,				// �f�t�H���g
		DEFAULT_2,				// �f�t�H���g
		SPARKLE1,				// �L���L��1
		SPARKLE2,				// �L���L��2
		SPARKLE3,				// �L���L��3
		CHAR_�[,				// �����[
		CHAR_�L,				// �����L
		CHAR_�M,				// �����M
		CHAR_�O,				// �����O
		CHAR_�Q,				// �����Q
		CHAR_�S,				// �����S
		CHAR_�K,				// �����K
		CHAR_�Y,				// �����Y
		CHAR_�^,				// �����^
		CHAR_�_,				// �����_
		CHAR_�c,				// �����c
		CHAR_�h,				// �����h
		CHAR_��,				// ������
		CHAR_��,				// ������
		TEXTYPE_MAX			// �ő�
	}TEXTURETYPE;

	typedef struct VIBRATION_OPTION
	{
		VIBRATION_OPTION() {}		// �R���X�g���N�^
		~VIBRATION_OPTION() {}		// �f�X�g���N�^
		
		int   nMaxDiviDir;			// �����_���̕����ő啪����
		float fScalDirValue;		// �������̏k���l
		int   nMaxDist;				// �����_���̍ő啪����
		float fScalDistValue;		// �������̏k���l

	}VIBRATION_OPTION;

	typedef struct VIBRATION
	{
		VIBRATION() :VecDir(MYLIB_3DVECTOR_ZERO), fDist(MYLIB_FLOAT_UNSET), bRandDist(false),Pos(MYLIB_3DVECTOR_ZERO){}		// �R���X�g���N�^
		~VIBRATION() {}		// �f�X�g���N�^

		D3DXVECTOR3 VecDir;				// �����x�N�g��
		float       fDist;				// ����
		D3DXVECTOR3 Pos;				// �ʒu
		bool        bRandDist;			// ���������_�����t���O
		void        Update(void);		// �X�V����
		void        GetVecDir(void);	// �����x�N�g���̎擾
		void        GetDist(void);		// �����̎擾
		void        CalPos(void);		// �ʒu�̌v�Z
	}VIBRATION;

	typedef struct SETINGPARAM
	{
		SETINGPARAM() :
			bBillBoard(false),
			type(MYLIB_INT_UNSET),
			Origin(MYLIB_3DVECTOR_ZERO),
			nTexType(MYLIB_INT_UNSET),
			pos(MYLIB_3DVECTOR_ZERO),
			pParent(nullptr),
			move(MYLIB_3DVECTOR_ZERO),
			col(MYLIB_D3DXCOR_UNSET),
			fRadius(MYLIB_FLOAT_UNSET),
			nLife(MYLIB_INT_UNSET),
			fGravity(MYLIB_FLOAT_UNSET),
			fRadiusValue(MYLIB_FLOAT_UNSET) {}

		bool        bBillBoard;		// �r���{�[�h�t���O
		int         type;			// �G�t�F�N�g�̎��
		int         nTexType;		// �e�N�X�`���̎��
		D3DXVECTOR3 pos;			// �ʒu
		D3DXVECTOR3 *pParent;		// �e�̈ʒu
		D3DXVECTOR3 Origin;			// ���_
		D3DXVECTOR3 move;			// �ړ���
		D3DXCOLOR   col;			// �F
		float       fRadius;		// ���a
		int         nLife;			// ���C�t
		float       fGravity;		// �d��
		float       fRadiusValue;	// ���a�̕ω��l
	} SETINGPARAM;

	// �\���̒�`
	typedef struct PARAMETER
	{
		bool        bUse;			// �g�p�t���O
		bool        bDisp;			// �`��t���O
		bool        bBillBoard;		// �r���{�[�h�t���O
		TRANSFORM   Trans;			// �g�����X���
		D3DXVECTOR3 *pParent;		// �e�̈ʒu
		D3DXVECTOR3 Origin;			// ���_
		D3DXVECTOR3 move;			// �ړ���
		D3DXCOLOR   col;			// �F
		float       fRadius;		// ���a
		float       fAngle;			// �p�x
		float       fRadiusValue;	// ���a�̕ω��l
		float       fAlphaValue;	// �A���t�@�l�̕ω��l
		int         nLife;			// ��������
		int         nTexType;		// �e�N�X�`���̎��
		float       fGravity;		// �d�͏���
		TYPE        type;			// ���
		VIBRATION   Vibrat;			// �U��
	}PARAMETER;

	C3DEffect() : CScene(PRIORITY_EFFECT) {};	// �R���X�g���N�^
	~C3DEffect() {};							// �f�X�g���N�^

	static void Load(void);						// �ǂݍ���
	static void Unload(void);					// �J��
	void Make(void);							// �쐬
	void Init(void);							// ������
	void Uninit(void);							// �I��
	void Update(void);							// �X�V
	void Draw(void);							// �`��

	static void Set(SETINGPARAM &Seting);		// �ʒu�̐ݒ�
private:
	/* �����o�֐� */
	static void CreateTexture(CONST_STRING SetingStr,const int &nCntInfo, LPDIRECT3DDEVICE9 pDevice);	// �e�N�X�`���̍쐬
	HRESULT     MakeVertex(LPDIRECT3DDEVICE9 pDevice);													// ���_���̍쐬
	static void SetVartexSize(VERTEX_3D *pVtx, PARAMETER &Effect);										// ���_�T�C�Y�̐ݒ�
	static void SetVetexColor(VERTEX_3D *pVtx, PARAMETER &Effect);										// ���_�J���[�̐ݒ�

	/* �����o�ϐ� */
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// �o�b�t�@���
	static LPDIRECT3DTEXTURE9      m_pTexInfo[TEXTYPE_MAX];				// �e�N�X�`�����
	static const float             m_cfBaseAngle;						// ��{�p�x
	static int                     m_nNumTextureMax;					// �e�N�X�`���̍ő吔
	static PARAMETER               m_EffectPram[_3DEFFE_USEQUANTITY];	// �G�t�F�N�g�̃p�����[�^
	static VIBRATION_OPTION        m_VibrationOption;					// �U���̃I�v�V����

};

#endif