//==================================================================================================================
//
// ���b�V���t�B�[���h[meshField.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"
#include "Mylibrary.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CPlayer;

//==================================================================================================================
//
// ���b�V���t�B�[���h�N���X�̒�`
//
//==================================================================================================================
class CMeshField : public CScene
{
public:
	CMeshField(PRIORITY type);								// �R���X�g���N�^
	~CMeshField();											// �f�X�g���N�^
	void Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CMeshField *Create(const INTEGER2 & block, const D3DXVECTOR3 & size, const D3DXVECTOR3 & pos);// ��������
	static CMeshField *GetMeshField(void);					// ���b�V���t�B�[���h���擾
	static HRESULT Load(void);								// �e�N�X�`����񃍁[�h
	static void Unload(void);								// �e�N�X�`�����A�����[�h

	void SetPos(D3DXVECTOR3 pos);							// �ʒu�ݒ菈��
	void SetMove(D3DXVECTOR3 move);							// �ړ��ʐݒ菈��
	void SetNor(void);										// �@���̐ݒ�

	D3DXVECTOR3 GetPos(void);								// �ʒu�擾����
	D3DXVECTOR3 GetMove(void);								// �ړ��ʎ擾����

	float GetHeight(D3DXVECTOR3 &pos);						// ���b�V���̍����擾

protected:

private:
	bool GetNor(const D3DXVECTOR3 &pos,
				const D3DXVECTOR3 &VtxPos0, 
				const D3DXVECTOR3 &VtxPos1, 
				const D3DXVECTOR3 &VtxPos2);				// �@���̎擾

	static LPDIRECT3DTEXTURE9 m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	static CMeshField *m_pMeshField;						// ���b�V���t�B�[���h�̃|�C���^
	static CPlayer *m_pPlayer;								// �v���C���[�̏��|�C���^

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// �o�b�t�@
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;						// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	VERTEX_3D *m_pVtx;										// ���_�i�[
	D3DXMATRIX m_mtxWorld;									// ���[���h�}�g���b�N�X

	D3DXVECTOR3 m_pos;										// �ʒu
	D3DXVECTOR3 m_rot;										// ��]
	D3DXVECTOR3 m_move;										// �ړ�
	D3DXVECTOR3 m_size;										// �C�`�}�X������̑傫��
	D3DXVECTOR3 *m_aVecA;									// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[
	D3DXVECTOR3 *m_aVecB;									// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[
	D3DXVECTOR3 m_vectorA;									// A�x�N�g��
	D3DXVECTOR3 m_vectorB;									// B�x�N�g��
	D3DXVECTOR3 m_vectorC;									// C�x�N�g��

	INTEGER2 m_block;

	int m_nDepth;											// �c�̃u���b�N��
	int m_nWidth;											// ���̃u���b�N��
	int m_nNumVertex;										// �����_��
	int m_nNumIndex;										// ���C���f�b�N�X��
	int m_nNumPolygon;										// ���|���S����
	int nNumber;											// �O�p�`�̔z��̔ԍ�
	int StartBox;											// �n�܂锠
	int EndBox;												// ������锠
	int m_nCntAnim;											// �A�j���[�V�����̃J�E���^
	float fDivide;											// sin�̒��g������ϐ�
};
#endif