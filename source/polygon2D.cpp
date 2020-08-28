//=============================================================================
//
// �V�[�����g��Ȃ�2D�|���S������ [ polygon2D.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "polygon2D.h"
#include "renderer.h"
#include "manager.h"
#include "kananlibrary.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPolygon2D::CPolygon2D()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPolygon2D::~CPolygon2D()
{
}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT CPolygon2D::Init()
{
	m_pos		= ZeroVector3;
	m_rot		= ZeroVector3;
	m_size		= ZeroVector3;
	m_col		= WhiteColor;
	m_fAngle	= 0.0f;
	m_fLength	= 0.0f;
	m_bShow		= true;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = ZeroVector3;
	pVtx[1].pos = ZeroVector3;
	pVtx[2].pos = ZeroVector3;
	pVtx[3].pos = ZeroVector3;

	//���_�̓������W
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�̐F
	pVtx[0].col = WhiteColor;
	pVtx[1].col = WhiteColor;
	pVtx[2].col = WhiteColor;
	pVtx[3].col = WhiteColor;

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
	
	// ����
	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void CPolygon2D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �|���S���̍X�V
//=============================================================================
void CPolygon2D::Update(void)
{
	// ���_���W�̐ݒ�
	MakeVertexPos();
}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void CPolygon2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	if (m_bShow)
		// �`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
}

//=============================================================================
// ���j�[�N�|�C���^�Ő���
//=============================================================================
CPolygon2D *CPolygon2D::Create(void)
{
	// �������m��
	CPolygon2D *pPolygon = new CPolygon2D;
	// ������
	pPolygon->Init();
	// �l��Ԃ�
	return pPolygon;
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CPolygon2D::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	// �|���S����]�p�̑Ίp�����Z�o
	m_fAngle = atan2f(m_size.x, m_size.y);
	m_fLength = (float)sqrt(m_size.x * m_size.x + m_size.y * m_size.y) / 2;
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CPolygon2D::SetCol(const D3DXCOLOR & col)
{
	m_col = col;

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���(���_���W)�̍X�V
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void CPolygon2D::SetAnim(const D3DXVECTOR2 & UV, const D3DXVECTOR2 & size)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(UV.x, UV.y);
	pVtx[1].tex = D3DXVECTOR2(UV.x + size.x, UV.y);
	pVtx[2].tex = D3DXVECTOR2(UV.x, UV.y + size.y);
	pVtx[3].tex = D3DXVECTOR2(UV.x + size.x, UV.y + size.y);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void CPolygon2D::MakeVertexPos(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_posStart)
	{
	case POSSTART_TOP_LEFT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f,		0.0f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x, 0.0f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f,		m_size.y, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
		break;
	case POSSTART_TOP_CENTRAL:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	0.0f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, m_size.y, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	m_size.y, 0.0f);
		break;
	case POSSTART_TOP_RIGHT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x,	0.0f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(0.0f,			0.0f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x,	m_size.y, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(0.0f,			m_size.y, 0.0f);
		break;
	case POSSTART_CENTRAL_LEFT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f,		-m_size.y * 0.5f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x, -m_size.y * 0.5f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f,		m_size.y * 0.5f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x, m_size.y * 0.5f, 0.0f);
		break;
	case POSSTART_CENTRAL_CENTRAL:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f,-m_size.y * 0.5f,0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f,m_size.y * 0.5f,0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);
		break;
	case POSSTART_CENTRAL_RIGHT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x,	-m_size.y * 0.5f,0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(0.0f,			-m_size.y * 0.5f,0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x,	m_size.y * 0.5f,0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(0.0f,			m_size.y * 0.5f,0.0f);
		break;
	case POSSTART_BOTTOM_LEFT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f,			-m_size.y, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x,		-m_size.y, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f,			0.0f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x,		0.0f, 0.0f);
		break;
	case POSSTART_BOTTOM_CENTRAL:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	0.0f, 0.0f);
		break;
	case POSSTART_BOTTOM_RIGHT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x,	-m_size.y, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(0.0f,			-m_size.y, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x,	0.0f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(0.0f,			0.0f, 0.0f);
		break;
	}

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}
