//==================================================================================================================
//
// bar�N���X[bar.h]
// Author:Seiya Takahashi
//
//==================================================================================================================
#ifndef _BAR_H
#define _BAR_H

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define TEXTURE_BAR 10												// �e�N�X�`���̍ő吔
#define MAX_WIDTH 20.0f												// ���̍ő�T�C�Y
#define MAX_WIDTH_FREAM 35.0f										// �t���[���̉��T�C�Y
#define MAX_HEIGHT 80.0f											// �c�̍ő�T�C�Y
#define MAX_HEIGHT_FREAME 160.0f									// �t���[���̏c�T�C�Y

//==================================================================================================================
// �N���X�̒�`
//==================================================================================================================
class CBar
{
public:

	//=============================================================================
	// �񋓌^��`
	//=============================================================================
	typedef enum
	{
		BARTYPE_FREAME0 = 0,	// 0�Ԗڂ̃t���[��
		BARTYPE_HP0,			// 0�Ԗڂ̗̑̓o�[
		BARTYPE_FREAME1,		// 1�Ԗڂ̃t���[��
		BARTYPE_HP1,			// 1�Ԗڂ̗̑̓o�[
		BARTYPE_FREMAE2,		// 2�Ԗڂ̃t���[��
		BARTYPE_HP2,			// 2�Ԗڂ̗̑̓o�[
		BARTYPE_FREAME3,		// 3�Ԗڂ̃t���[��
		BARTYPE_HP3,			// 3�Ԗڂ̗̑̓o�[
		BARTYPE_FREAME4,		// 4�Ԗڂ̃t���[��
		BARTYPE_HP4,			// 4�Ԗڂ̗̑̓o�[
		BARTYPE_MAX				// �ő吔
	} BARTYPE;					// Bar�^�C�v

	CBar();								// �R���X�g���N�^
	~CBar();							// �f�X�g���N�^
	void Init(void);					// ����������
	void Uninit(void);					// �I������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	static CBar *Create(void);			// ��������
	static HRESULT Load(void);			// �e�N�X�`���Ǎ�����
	static void Unload(void);			// �e�N�X�`���j������

	void SetVertexBar(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);	// ���_�ݒ菈��
	void RotBar(int index, D3DXVECTOR3 pos, float fAngle, D3DXVECTOR3 diff);					// �o�[��]����
	void SetColBar(int index, D3DXCOLOR col);													// �o�[�F�ݒ菈��

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[BARTYPE_MAX];	// �e�N�X�`�����
	static char *m_apFileName[BARTYPE_MAX];				// Bar�̃t�@�C����

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@
	VERTEX_2D *m_pVtx;									// ���_�i�[
};
#endif
