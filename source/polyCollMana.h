//=============================================================================
//
// �|���S���R���C�_�[�}�l�[�W���[����  [ polyCollMana.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _POLYCOLLMANA_H_
#define _POLYCOLLMANA_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"
#include "kananlibrary.h"
#include <vector>

//=============================================================================
// �N���X��`
//=============================================================================
class CPolygonCollider;

class CPolyCollMana : public CScene
{
public:
	CPolyCollMana() : CScene(PRIORITY_COLLISION) {}	// �R���X�g���N�^
	~CPolyCollMana() {}								// �f�X�g���N�^

	void Init();									// ������
	void Uninit();									// �I��
	void Update();									// �X�V
	void Draw();									// �`��
	static CPolyCollMana *Create(void);	// ����
	
	CPolygonCollider *GetPolyColl(int nIndex) { return m_pPolyColl[nIndex]; }

private:
	std::vector<CPolygonCollider*> m_pPolyColl;		// �|���S���R���C�_�[

};
#endif