//==================================================================================================================
//
// �_�̕`�� [bar.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS

//==================================================================================================================
//	�C���N���[�h�t�@�C��
//==================================================================================================================
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "manager.h"
#include "bar.h"
#include "debugProc.h"

//==================================================================================================================
//	�ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9 CBar::m_pTexture[BARTYPE_MAX] = {};		// �e�N�X�`�����
char *CBar::m_apFileName[BARTYPE_MAX] =						// �ǂݍ��ރ��f���̃\�[�X��
{
	{ "data/TEXTURE/�}1.png" },			// 0�Ԗڂ̃t���[��
	{ "data/TEXTURE/HPbar.png" },		// 0�Ԗڂ̗̑̓o�[
	{ "data/TEXTURE/�}1.png" },			// 1�Ԗڂ̃t���[��
	{ "data/TEXTURE/HPbar.png" },		// 1�Ԗڂ̗̑̓o�[
	{ "data/TEXTURE/�}1.png" },			// 2�Ԗڂ̃t���[��
	{ "data/TEXTURE/HPbar.png" },		// 2�Ԗڂ̗̑̓o�[
	{ "data/TEXTURE/�}1.png" },			// 3�Ԗڂ̃t���[��
	{ "data/TEXTURE/HPbar.png" },		// 3�Ԗڂ̗̑̓o�[
	{ "data/TEXTURE/�}1.png" },			// 4�Ԗڂ̃t���[��
	{ "data/TEXTURE/HPbar.png" },		// 4�Ԗڂ̗̑̓o�[
};

//==================================================================================================================
//	�R���X�g���N�^
//==================================================================================================================
CBar::CBar()
{

}

//==================================================================================================================
//	�f�X�g���N�^
//==================================================================================================================
CBar::~CBar(void)
{

}

//==================================================================================================================
//	����������
//==================================================================================================================
void CBar::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// �f�o�C�X���擾����

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TEXTURE_BAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
}

//==================================================================================================================
//	�I������
//==================================================================================================================
void CBar::Uninit(void)
{
	// ���_�o�b�t�@������Ƃ�
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();	// ���_�o�b�t�@�̊J��
		m_pVtxBuff = NULL;		// NULL�ɂ���
	}
}

//==================================================================================================================
//	�X�V����
//==================================================================================================================
void CBar::Update(void)
{

}

//==================================================================================================================
//	�`�揈��
//==================================================================================================================
void CBar::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// �f�o�C�X���擾����

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCntTex = 0; nCntTex < TEXTURE_BAR; nCntTex++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[nCntTex]);

		// �|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//==================================================================================================================
// �I�u�W�F�N�g�̐���
//==================================================================================================================
CBar *CBar::Create(void)
{
	CBar *pBar = NULL;			// �ϐ�������

	pBar = new CBar;			// �������𓮓I�Ɋm��

	pBar->Init();				// ����������

	return pBar;				// �l��Ԃ�
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CBar::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// �f�o�C�X���擾����

	//==============================�e�N�X�`���̓ǂݍ���==============================//
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < BARTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
	}

	// �l��Ԃ�
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CBar::Unload(void)
{
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCntTex = 0; nCntTex < TEXTURE_BAR; nCntTex++)
	{
		// �e�N�X�`��������Ƃ�
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();		// �e�N�X�`���̊J��
			m_pTexture[nCntTex] = NULL;			// NULL�ɂ���
		}
	}
}

//==================================================================================================================
// ���_�̐ݒ�
//==================================================================================================================
void CBar::SetVertexBar(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx += index * 4;	// ���_��4�����Z

	// ���_���W�̐ݒ�(�E���Őݒ肷��)
	m_pVtx[0].pos.x = pos.x - fWidth / 2;
	m_pVtx[0].pos.y = pos.y - fHeight;
	m_pVtx[0].pos.z = 0.0f;

	m_pVtx[1].pos.x = pos.x + fWidth / 2;
	m_pVtx[1].pos.y = pos.y - fHeight;
	m_pVtx[1].pos.z = 0.0f;

	m_pVtx[2].pos.x = pos.x - fWidth / 2;
	m_pVtx[2].pos.y = pos.y;
	m_pVtx[2].pos.z = 0.0f;

	m_pVtx[3].pos.x = pos.x + fWidth / 2;
	m_pVtx[3].pos.y = pos.y;
	m_pVtx[3].pos.z = 0.0f;

	// �������W(1.0�ŌŒ�)
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;

	// �F�̐ݒ�
	m_pVtx[0].col = col;
	m_pVtx[1].col = col;
	m_pVtx[2].col = col;
	m_pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

}

//==================================================================================================================
// �o�[��]����
//==================================================================================================================
void CBar::RotBar(int index, D3DXVECTOR3 pos, float fAngle, D3DXVECTOR3 diff)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx += index * 4;					// ���_��4�����Z

	// ���S�_����̈ʒu
	D3DXVECTOR3 originPos0 = m_pVtx[0].pos - pos;
	D3DXVECTOR3 originPos1 = m_pVtx[1].pos - pos;
	D3DXVECTOR3 originPos2 = m_pVtx[2].pos - pos;
	D3DXVECTOR3 originPos3 = m_pVtx[3].pos - pos;

	// ��]�̒��S�_�̍�
	D3DXVECTOR3 diffPos0 = originPos0 - diff;
	D3DXVECTOR3 diffPos1 = originPos1 - diff;
	D3DXVECTOR3 diffPos2 = originPos2 - diff;
	D3DXVECTOR3 diffPos3 = originPos3 - diff;

	// �ړ����W�̐ݒ�
	m_pVtx[0].pos.x = diffPos0.x * cosf(fAngle) - diffPos0.y * sinf(fAngle) + pos.x + diff.x;
	m_pVtx[0].pos.y = diffPos0.x * sinf(fAngle) + diffPos0.y * cosf(fAngle) + pos.y + diff.y;
	m_pVtx[0].pos.z = 0.0f;

	m_pVtx[1].pos.x = diffPos1.x * cosf(fAngle) - diffPos1.y * sinf(fAngle) + pos.x + diff.x;
	m_pVtx[1].pos.y = diffPos1.x * sinf(fAngle) + diffPos1.y * cosf(fAngle) + pos.y + diff.y;
	m_pVtx[1].pos.z = 0.0f;

	m_pVtx[2].pos.x = diffPos2.x * cosf(fAngle) - diffPos2.y * sinf(fAngle) + pos.x + diff.x;
	m_pVtx[2].pos.y = diffPos2.x * sinf(fAngle) + diffPos2.y * cosf(fAngle) + pos.y + diff.y;
	m_pVtx[2].pos.z = 0.0f;

	m_pVtx[3].pos.x = diffPos3.x * cosf(fAngle) - diffPos3.y * sinf(fAngle) + pos.x + diff.x;
	m_pVtx[3].pos.y = diffPos3.x * sinf(fAngle) + diffPos3.y * cosf(fAngle) + pos.y + diff.y;
	m_pVtx[3].pos.z = 0.0f;

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//==================================================================================================================
// �o�[�F�ݒ菈��
//==================================================================================================================
void CBar::SetColBar(int index, D3DXCOLOR col)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx += index * 4;					// ���_��4�����Z

	// ���_�J���[
	m_pVtx[0].col = col;
	m_pVtx[1].col = col;
	m_pVtx[2].col = col;
	m_pVtx[3].col = col;

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();

}
