//*************************************************************************************************************
//
// �}�C���C�u����[Mylibrary.inl]
// Contents : Mylibrary inline functions
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CBMPFILE_INL
#define _CBMPFILE_INL

/*
// struct inline func
// �\���̂̃C�����C���֐�
*/

//----------------------------------------------------------------------------------------------------
// 3����float
//----------------------------------------------------------------------------------------------------
inline FLOAT3
FLOAT3::operator +(const FLOAT3 &rhs) const
{
	return FLOAT3(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline FLOAT3
FLOAT3::operator -(const FLOAT3 &rhs) const
{
	return FLOAT3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline FLOAT3
FLOAT3::operator -(void) const
{
	return FLOAT3(x * -1.0f, y * -1.0f, z * -1.0f);
}

inline FLOAT3
FLOAT3::operator *(const FLOAT3 &r) const
{
	return FLOAT3(x * r.x, y * r.y, z * r.z);
}

inline FLOAT3
FLOAT3::operator /(const FLOAT3 &r) const
{
	return FLOAT3(x / r.x, y / r.y, z / r.z);
}

inline FLOAT3
FLOAT3::operator *(float r) const
{
	return FLOAT3(x * r, y * r, z * r);
}

inline FLOAT3
FLOAT3::operator /(float r) const
{
	return FLOAT3(x / r, y / r, z / r);
}

inline float
FLOAT3::Dot(const FLOAT3 &r) const {
	return x * r.x + y * r.y + z * r.z;
}

inline FLOAT3
FLOAT3::Cross(const FLOAT3 &r) const {
	return FLOAT3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
}

inline float
FLOAT3::Length() const {
	return sqrtf(LengthSq());
}

inline float
FLOAT3::LengthSq() const {
	return x * x + y * y + z * z;
}

inline void
FLOAT3::Norm() {
	const float len = Length();
	if (len > 0.0f) {
		x /= len;
		y /= len;
		z /= len;
	}
}

inline FLOAT3
FLOAT3::GetNorm() const {
	const float len = Length();
	if (len > 0.0f) {
		return FLOAT3(x / len, y / len, z / len);
	}
	return FLOAT3(0.0f, 0.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------
// 3D�x�N�g��
//----------------------------------------------------------------------------------------------------
inline VEC3& VEC3::operator =(const FLOAT3 &r) {
	x = r.x;
	y = r.y;
	z = r.z;
	return *this;
}

inline void 
VEC3::norm(void) {
	const float len = Length();
	if (len > 0.0f) {
		x /= len;
		y /= len;
		z /= len;
	}
}

inline bool
VEC3::IsVertical(const VEC3 &r) const {
	float DotValue = Dot(r);
	return (-MYLIB_OX_EPSILON < DotValue && DotValue < MYLIB_OX_EPSILON);	// �덷�͈͓��Ȃ琂���Ɣ���
}

inline bool
VEC3::IsParallel(const VEC3 &r) const {
	float CrossValue = Cross(r).LengthSq();
	return (-MYLIB_OX_EPSILON < CrossValue && CrossValue < MYLIB_OX_EPSILON);	// �덷�͈͓��Ȃ畽�s�Ɣ���
}

inline bool
VEC3::IsSharpAngle(const VEC3 &r) const {
	return (Dot(r) >= 0.0f);
}

//----------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------
inline FLOAT3
LINE::GetPoint(float fCoffi) const
{
	return Point + fCoffi * Vec;
}

//----------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------
inline FLOAT3
SEGMENT::GetEndPoint() const
{
	return Point + Vec;
}

//----------------------------------------------------------------------------------------------------
// 3D�̐������
//----------------------------------------------------------------------------------------------------
/* * �R���X�g���N�^ */
inline
LINESEGMENT3D::LINESEGMENT3D(D3DXVECTOR3 &start, D3DXVECTOR3 &vector)
{
	this->start = start;
	this->vector = vector;
}

/* * �P�����Z�q + */
inline LINESEGMENT3D
LINESEGMENT3D::operator+(void) const
{
	return *this;
}

/* * �P�����Z�q - */
inline LINESEGMENT3D
LINESEGMENT3D::operator-(void) const
{
	return LINESEGMENT3D(-start, -vector);
}

/* * �񍀉��Z�q + */
inline LINESEGMENT3D
LINESEGMENT3D::operator + (CONST LINESEGMENT3D& rhs) const
{
	return LINESEGMENT3D(this->start + rhs.start, this->vector + rhs.vector);
}
/* * �񍀉��Z�q - */
inline LINESEGMENT3D
LINESEGMENT3D::operator - (CONST LINESEGMENT3D& rhs) const
{
	return LINESEGMENT3D(this->start - rhs.start, this->vector - rhs.vector);
}

/* * �񍀉��Z�q * */
inline LINESEGMENT3D
LINESEGMENT3D::operator * (CONST LINESEGMENT3D& rhs) const
{
	return
		LINESEGMENT3D(
			D3DXVECTOR3(
				this->start.x * rhs.start.x,
				this->start.y * rhs.start.y,
				this->start.z * rhs.start.z),
			D3DXVECTOR3(
				this->vector.x * rhs.vector.x,
				this->vector.y * rhs.vector.y,
				this->vector.z * rhs.vector.z));
}

/* * �񍀉��Z�q / */
inline LINESEGMENT3D
LINESEGMENT3D::operator / (CONST LINESEGMENT3D& rhs) const
{
	return
		LINESEGMENT3D(
			D3DXVECTOR3(
				this->start.x / rhs.start.x,
				this->start.y / rhs.start.y,
				this->start.z / rhs.start.z),
			D3DXVECTOR3(
				this->vector.x / rhs.vector.x,
				this->vector.y / rhs.vector.y,
				this->vector.z / rhs.vector.z));
}

/* * ��r���Z�q == */
inline bool 
LINESEGMENT3D::operator == (CONST LINESEGMENT3D& rhs) const
{
	return this->start == rhs.start && this->vector == this->vector;
}

/* * ��r���Z�q != */
inline bool
LINESEGMENT3D::operator != (CONST LINESEGMENT3D& rhs) const
{
	return this->start != rhs.start || this->vector != this->vector;
}

/* * ������Z�q += */
inline LINESEGMENT3D&
LINESEGMENT3D::operator += (CONST LINESEGMENT3D& rhs)
{
	this->start += rhs.start;
	this->vector += rhs.vector;
	return *this;
}

/* * ������Z�q -= */
inline LINESEGMENT3D&
LINESEGMENT3D::operator -= (CONST LINESEGMENT3D& rhs)
{
	this->start -= rhs.start;
	this->vector -= rhs.vector;
	return *this;
}

/* * ������Z�q *= */
inline LINESEGMENT3D&
LINESEGMENT3D::operator *= (CONST LINESEGMENT3D& rhs)
{
	this->start.x *= rhs.start.x;
	this->start.y *= rhs.start.y;
	this->start.z *= rhs.start.z;
	this->vector.x *= rhs.vector.x;
	this->vector.y *= rhs.vector.y;
	this->vector.z *= rhs.vector.z;

	return *this;
}

/* * ������Z�q /= */
inline LINESEGMENT3D&
LINESEGMENT3D::operator /= (CONST LINESEGMENT3D& rhs)
{
	this->start.x /= rhs.start.x;
	this->start.y /= rhs.start.y;
	this->start.z /= rhs.start.z;
	this->vector.x /= rhs.vector.x;
	this->vector.y /= rhs.vector.y;
	this->vector.z /= rhs.vector.z;

	return *this;
}

/*
// class inline func
// �N���X�̃C�����C���֐�
*/
//----------------------------------------------------------------------------------------------------
// class CInputkey ���̓L�[�N���X
//----------------------------------------------------------------------------------------------------

/* * ������ */
inline void
CInputkey::Init(void)
{
	this->m_nSize = 0;		// �T�C�Y
	this->m_pKeyTop = NULL;		// �L�[�̐擪�|�C���^
}

/* * �L�[�f�[�^�̎擾 */
inline void
CInputkey::KeyInit(void)
{
	this->m_pKeyTop->nData = 0;		// �f�[�^
	this->m_pKeyTop->nKey = 0;		// �L�[
	this->m_pKeyTop->pNext = NULL;	// �L�[�̐擪�|�C���^
}

/* * �L�[�f�[�^�̐��� */
inline void
CInputkey::KeyCreate(void)
{
	// ����
	this->m_pKeyTop = new INPUTKEYCELL;
	// �������s
	if (this->m_pKeyTop == NULL)
	{
		return;
	}
	// �L�[�̏�����
	this->KeyInit();
}

/* * �L�[�f�[�^���X�g�̊J�� */
inline void
CInputkey::KeyListRelease(void)
{
	// �ϐ��錾
	INPUTKEYCELL* pTemp;	// �ꎞ�ۑ��p
	INPUTKEYCELL* pSwap;	// �����p

							// �擪�|�C���^
	pTemp = this->m_pKeyTop;
	// ���݂��Ă��鎞
	if (pTemp != NULL)
	{
		// ���X�g�̊J��
		while (pTemp != NULL)
		{
			// �����p�ɐݒ�
			pSwap = pTemp->pNext;
			// �ۑ��p��j��
			delete pTemp;
			// �ۑ��p�ɐݒ�
			pTemp = pSwap;
		}
	}
	this->m_pKeyTop = NULL;
}

/* * �L�[�f�[�^�̑}�� */
inline void
CInputkey::KeyInsert(int nData, int nKey)
{
	// �ϐ��錾
	INPUTKEYCELL * pInsert;			// �}������|�C���^
									// ����
	pInsert = new INPUTKEYCELL;
	// �����Ɏ��s
	if (pInsert == NULL)
	{
		return;
	}
	// �f�[�^�̐ݒ�
	pInsert->nData = nData;
	// �L�[�̐ݒ�
	pInsert->nKey = nKey;
	// �}�����鎟�̃|�C���^�̐ݒ�
	pInsert->pNext = this->m_pKeyTop;
	// �擪�|�C���^�̐ݒ�
	this->m_pKeyTop = pInsert;
	// �T�C�Y���C���N�������g
	m_nSize++;
}

/* * �L�[�f�[�^�𔲂���� */
inline void
CInputkey::KeyPullout(int nKey)
{
	// �ϐ��錾
	INPUTKEYCELL *pTarget = NULL;	// �^�[�Q�b�g�p�|�C���^
	INPUTKEYCELL *pChain = NULL;	// �Ȃ��̃|�C���^

									// �^�[�Q�b�g�̐ݒ�
	pTarget = this->m_pKeyTop;
	// �^�[�Q�b�g�����Ȃ���
	if (pTarget == NULL)
	{
		return;
	}
	// �Ȃ��̃|�C���^�̐ݒ�
	pChain = pTarget->pNext;

	// �擪�𔲂����ꍇ
	if (pTarget->nKey == nKey)
	{
		// �T�C�Y���f�N�������g
		m_nSize--;
		// �Ȃ��̃|�C���^��擪�ɂ���
		this->m_pKeyTop = pChain;
		// �^�[�Q�b�g��j��
		delete pTarget;
		pTarget = NULL;
		return;
	}

	// �擪�ȊO�𔲂����ꍇ
	while (pTarget != NULL)
	{
		// �^�[�Q�b�g�̃L�[�ƔC�ӂ̃L�[�������Ƃ�
		if (pTarget->nKey == nKey)
		{
			// �T�C�Y���f�N�������g
			m_nSize--;
			// �Ȃ��̎��Ƀ^�[�Q�b�g�̎��̃|�C���^��ݒ�
			pChain->pNext = pTarget->pNext;
			// �^�[�Q�b�g�̔j��
			delete pTarget;
			pTarget = NULL;
			return;
		}
		// �Ȃ��Ƀ^�[�Q�b�g�|�C���^��ݒ�
		pChain = pTarget;
		// �^�[�Q�b�g�Ƀ^�[�Q�b�g�̎���ݒ�
		pTarget = pTarget->pNext;
	}
}

/* * �L�[�f�[�^�̎擾 */
inline int
CInputkey::GetKeyData(int nKey)
{
	// �ϐ��錾
	INPUTKEYCELL *pTarget = NULL;	// �^�[�Q�b�g�̃|�C���^
	INPUTKEYCELL *pChain = NULL;	// �Ȃ��̃|�C���^

									// �^�[�Q�b�g�̐ݒ�
	pTarget = this->m_pKeyTop;
	// �^�[�Q�b�g��NULL����Ȃ������[�v
	while (pTarget != NULL)
	{
		// �^�[�Q�b�g�̃L�[�ƔC�ӂ̃L�[�������Ƃ�
		if (pTarget->nKey == nKey)
		{// �f�[�^��Ԃ�
			return pTarget->nData;
		}
		// �^�[�Q�b�g���Ȃ��ɐݒ�
		pChain = pTarget;
		// �^�[�Q�b�g�Ƀ^�[�Q�b�g�Ɏ���ݒ�
		pTarget = pTarget->pNext;
	}
	return -1;
}

/* * �T�C�Y�̎擾 */
inline int
CInputkey::GetSize(void)
{
	return m_nSize;
}

//----------------------------------------------------------------------------------------------------
// class CString ������N���X
//----------------------------------------------------------------------------------------------------

/* * �R���X�g���N�^ */
inline
CString::CString()
{
	this->Init();
}

/* * ������ */
inline void
CString::Init(void)
{
	this->m_string = NULL;				// ������|�C���^
	this->m_nStringLength = MYLIB_INT_UNSET;	// ������̒���
}

/* * �J������ */
inline void
CString::release(void)
{
	// NULL�J������Ă��Ȃ���
	if (this->m_string != NULL)
	{
		// ������̔j��
		delete[this->m_nStringLength]this->m_string;
		// ������
		this->Init();
	}
}

/* * ������̐ݒ�i�ݒ肵�Ă����ꍇ�Đݒ肷��j */
inline void
CString::Set(STRING string)
{
	// �J������
	this->release();
	// ����
	if (this->m_string == NULL)
	{
		this->m_nStringLength = (int)strlen(string) + 1;
		this->m_string = new char[this->m_nStringLength];
		strcpy(this->m_string, string);
	}
}

/* * ������̎擾 */
inline STRING
CString::Get(void)
{
	return this->m_string;
}



#endif