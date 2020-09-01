//==================================================================================================================
//
// HP�N���X[hitpoint.h]
// Author:Seiya Takahashi
//
//==================================================================================================================
#ifndef _HITPOINT_H
#define _HITPOINT_H

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define MAX_BAR 5			// �o�[�̍ő�{��

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CBar;
class CScene;
class CPlayer;

//==================================================================================================================
//�N���X�̒�`
//==================================================================================================================
class CHitPoint : public CScene
{
public:

	CHitPoint(PRIORITY type);			// �R���X�g���N�^
	~CHitPoint();						// �f�X�g���N�^
	void Init(void);					// ����������
	void Uninit(void);					// �I������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	static CHitPoint *Create(int nNumber, float fMaxHP);// ��������

	void SetPos(D3DXVECTOR3 pos);		// �ʒu�ݒ菈��
	void SetnPlayerNum(int nPlayerNum);	// �v���C���[�ԍ��ݒ菈��

	D3DXVECTOR3 GetPos(void);			// �ʒu�擾����

protected:

private:

	void Bar0Update(float NowHP);	// �o�[0�̍X�V����
	void Bar1Update(float NowHP);	// �o�[1�̍X�V����
	void Bar2Update(float NowHP);	// �o�[2�̍X�V����
	void Bar3Update(float NowHP);	// �o�[3�̍X�V����
	void Bar4Update(float NowHP);	// �o�[4�̍X�V����

	static CPlayer *m_pPlayer;		// �v���C���[�̏��|�C���^
	static int m_nInitCnt;			// �������p�J�E���^

	CBar *m_pBar;					// �o�[�̏��|�C���^

	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_rot0;				// ��]
	D3DXVECTOR3 m_rot1;				// ��]
	D3DXVECTOR3 m_rot2;				// ��]
	D3DXVECTOR3 m_rot3;				// ��]

	int m_nCntCol0;			// ���l�����p�J�E���^
	int m_nCntCol1;			// ���l�����p�J�E���^
	int m_nCntCol2;			// ���l�����p�J�E���^
	int m_nCntCol3;			// ���l�����p�J�E���^
	int m_nCntPos0;			// �ʒu�ύX�p�J�E���^
	int m_nCntPos1;			// �ʒu�ύX�p�J�E���^
	int m_nCntPos2;			// �ʒu�ύX�p�J�E���^
	int m_nCntPos3;			// �ʒu�ύX�p�J�E���^
	int m_nPlayerNum;		// �v���C���[�̔ԍ�

	float m_fMaxHP;			// �ő�HP
	float m_fNowHP;			// ���݂�HP
	float m_fHeight;		// �c�̒���

	bool m_bBar[MAX_BAR];	// �o�[�����邩�ǂ���
	bool m_bDisplay;		// UI��\�������邩�ǂ���
};

#endif
