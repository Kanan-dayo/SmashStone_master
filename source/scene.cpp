//==================================================================================================================
//
// �`�悷����̂��Ǘ�����[scene.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "scene.h"
#include "renderer.h"

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
CScene *CScene::m_pTop[PRIORITY_MAX] = {};		// �擪�I�u�W�F�N�g�̃|�C���^
CScene *CScene::m_pCur[PRIORITY_MAX] = {};		// ����(�Ō��)�̃I�u�W�F�N�g�̃|�C���^

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CScene::CScene(PRIORITY type)
{
	// �擪�I�u�W�F�N�g��������
	if (m_pTop[type] == NULL)
	{
		m_pPrev = NULL;					// �O�̃I�u�W�F�N�g�̃|�C���^
		m_pTop[type] = this;			// �擪�I�u�W�F�N�g�̃|�C���^������ɂ���
		m_pCur[type] = this;
		m_pNext = NULL;
	}
	else
	{
		// �Ō���̗v�f��ݒ肷��
		if (m_pCur[type] != NULL)
		{
			m_pCur[type]->m_pNext = this;
			this->m_pPrev = m_pCur[type];
			m_pCur[type] = this;
		}
	}

	m_nPriority = type;
	m_bDeth = false;					// ���S�t���O�͗����Ă��Ȃ���Ԃɂ���
}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CScene::~CScene()
{

}

//==================================================================================================================
// �S�Ă̍X�V
//==================================================================================================================
void CScene::UpdateAll(void)
{
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		// �X�V
		if (m_pTop[nCnt])
		{
			CScene *pScene = m_pTop[nCnt];
			while (pScene)
			{
				CScene *pNextScene = pScene->m_pNext;
				pScene->Update();
				pScene = pNextScene;
			}
		}

		// �J��������s��
		if (m_pTop[nCnt])
		{
			CScene *pScene = m_pTop[nCnt];
			while (pScene)
			{
				CScene *pNextScene = pScene->m_pNext;
				pScene->Deleate(nCnt);
				pScene = pNextScene;
			}
		}
	}
}

//==================================================================================================================
// �S�Ă̕`��
//==================================================================================================================
void CScene::DrawAll(void)
{
	// �`�揇�Ԃ�for��
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		CScene *pScene = m_pTop[nCnt];				// �ϐ�����菉��������
		while (pScene)								// pScene��NULL�ɂȂ�܂ŃG���h���X
		{
			CScene *pSceneNext = pScene->m_pNext;	// ���̏��i�[	
			pScene->Draw();							// ���̏��̕`�揈��
			pScene = pSceneNext;					// pScene�Ɏ��̏����
		}
	}
}

//==================================================================================================================
// ���ׂĔj��
//==================================================================================================================
void CScene::ReleaseAll(void)
{
	// �`�揇�Ԃ�for��
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		// nullcheck
		if (m_pTop[nCnt])
		{
			// pScene����ɂȂ�܂ŌJ��Ԃ�
			CScene *pScene = m_pTop[nCnt];

			// nullcheck
			while (pScene)
			{
				// ���̃|�C���^��ۑ�
				CScene *pSceneNext = pScene->m_pNext;

				// ���̃|�C���^�������[�X
				pScene->Release();

				// ���̃|�C���^��j��
				pScene->Deleate(nCnt);

				// ���̃|�C���^�����̃|�C���^�ɏ㏑������
				pScene = pSceneNext;
			}
		}
	}
}

//==================================================================================================================
// ���S�t���O�𗧂Ă�
//==================================================================================================================
void CScene::Release(void)
{
	m_bDeth = true;				// ���S�t���O�𗧂Ă�
}

//==================================================================================================================
// �擾
//==================================================================================================================
CScene *CScene::GetScene(PRIORITY nPriority, int nCntScene)
{
	CScene *pScene = m_pTop[nPriority];				// �ϐ�����菉��������

	// �擪����nCntScene�����̃I�u�W�F�N�g�Ƀ|�C���^��n��
	for (int nCnt = 0; nCnt < nCntScene; nCnt++)
	{
		CScene *pSceneNext = pScene->m_pNext;		// ���̏��i�[	
		pScene = pSceneNext;						// pScene�Ɏ��̏����
	}

	return pScene;									// �l��Ԃ�
}

//==================================================================================================================
// �j��
//==================================================================================================================
void CScene::Deleate(int type)
{
	// ���S�t���O�������Ă���Ƃ�
	if (m_bDeth)
	{
		this->Uninit();

		// �폜���ɔz��Ɍ��������ł���̂ŁA�������ւ���
		if (this == m_pTop[type] && this == m_pCur[type])
		{
			m_pTop[type] = m_pCur[type] = nullptr;
		}
		else if (this == m_pTop[type])
		{
			m_pTop[type] = m_pTop[type]->m_pNext;
			m_pTop[type]->m_pPrev = nullptr;
		}
		else if (this == m_pCur[type])
		{
			m_pCur[type] = m_pCur[type]->m_pPrev;
			m_pCur[type]->m_pNext = nullptr;
		}
		else
		{
			m_pPrev->m_pNext = this->m_pNext;
			m_pNext->m_pPrev = this->m_pPrev;
		}
		delete this;
	}
}
