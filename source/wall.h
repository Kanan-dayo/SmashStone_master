//*************************************************************************************************************
//
// �ǂ̏���[wall.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _WALL_H_
#define _WALL_H_

#define _CRT_SECURE_NO_WARNINGS

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Mylibrary.h"
#include "kananlibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define WALL_MAX	(4)		// �ǂ̍ő吔

#ifdef _DEBUG
//#define WALL_DEBUG
#endif

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CWall : public CScene
{
public:

	/* �񋓌^��` */
	// �ǂ̃^�C�v
	enum WALLTEX
	{
		WALLTEX_NONE = -1,	// ����
		WALLTEX_FIELD,		// �L��
		WALLTEX_MAX			// �ő吔
	};

	enum SETINGPOS
	{
		SETINGPOS_NONE =-1,	// ����
		SETINGPOS_POSIX,	// ����X�̈ʒu
		SETINGPOS_POSIZ,	// ����Z�̈ʒu
		SETINGPOS_NEGX,		// ����X�̈ʒu
		SETINGPOS_NEGZ,		// ����Z�̈ʒu
		SETINGPOS_MAX		// �ő吔
	};
	
	/* �\���̒�` */
	// �ǂ̒P�̏��
	typedef struct
	{
		TRANSFORM   trans;	// �g�����X���
		D3DXVECTOR3 nor;	// �ʂ̖@�����
		D3DXVECTOR3 size;	// �傫��
		D3DXPLANE   plane;	// ����
	} SINGLEINFO;

	/* �����o�֐� */
	// �R���X�g���N�^
	inline CWall() : CScene(CScene::PRIORITY_FIELD) {}
	// �f�X�g���N�^
	inline ~CWall() {}

	// �ǂݍ���
	static HRESULT Load(void);
	// �J��
	static void Unload(void);
	// ����
	static CWall* Create(WALLTEX enmWallTex);

	// �Փ˔���
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor,bool bReflection);

	// ��_�����߂�
	static bool GetIntersection(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pOut_Intersect, CONST SINGLEINFO* pSingleInfo);

	// ������
	void Init(void);
	// �I��
	void Uninit(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);
	// �e�N�X�`���^�C�v�̐ݒ�
	void BindTexType(WALLTEX enmTexType);
private:
	/* �����o�֐� */
	// ���_���̍쐬
	HRESULT MakeVertex(void);
	/* �����o�ϐ� */
	static LPDIRECT3DTEXTURE9 m_pAllTexture[WALLTEX_MAX];		// �S�Ẵe�N�X�`�����
	static D3DXVECTOR2        m_Length;							// ���S����̋���
	static D3DXVECTOR3        m_CenterPos;						// ���S�ʒu
	static float              m_fHeight;						// ����
	static char				  m_cFileName[STAGE_MAX][MYLIB_STRINGSIZE];	// �t�@�C����
	LPDIRECT3DTEXTURE9        m_pTexture;						// �e�N�X�`�����̃|�C���^
	LPDIRECT3DVERTEXBUFFER9   m_pVtexBuff;						// ���_�o�b�t�@�̃|�C���^
	SINGLEINFO                m_SingleInfo[WALL_MAX];			// �ǒP�̂̏��
};



#endif