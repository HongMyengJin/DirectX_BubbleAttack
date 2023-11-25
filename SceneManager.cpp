#include "SceneManager.h"
#include "Stage.h"
#include "MirrorStage.h"
CSceneManager::~CSceneManager()
{
	Release();
}
void CSceneManager::MakeScenes(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pScenes.resize(UINT(SceneType::SceneTypeEnd));
	// Main Stage
	std::shared_ptr<CScene> pScene = std::make_shared<CStage>();
	pScene->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pScenes[UINT(SceneType::Stage1Type)] = pScene;

	//MirrorStage
	pScene = std::make_shared<CMirrorStage>();
	pScene->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pScenes[UINT(SceneType::MirrorStageType)] = pScene;

}
void CSceneManager::ChangeSceneComponent(SceneType eSceneType)
{
	switch (eSceneType)
	{
	case SceneType::Stage1Type:
	{
		//Release();
		m_pCurrentScene = m_pScenes[UINT(SceneType::Stage1Type)];

		break;
	}
	case SceneType::MirrorStageType:
	{
		//Release();
		m_pCurrentScene = m_pScenes[UINT(SceneType::MirrorStageType)];
		break;
	}
	case SceneType::SceneTypeEnd:
		break;
	default:
		break;
	}
}

void CSceneManager::UpdateCurrentScene(float fTimeElapsed)
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->AnimateObjects(fTimeElapsed);
		m_pCurrentScene->UpdateObjects(fTimeElapsed);
		if (m_pCurrentScene->GetChangeScene())
		{
			m_pCurrentScene->SetChangeScene(false);
			ChangeSceneComponent(SceneType::MirrorStageType);
		}

	}
}

void CSceneManager::PreRenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList)
{

	if (m_pCurrentScene)
		m_pCurrentScene->PrepareRender(pd3dCommandList);
}

void CSceneManager::RenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList)
{

	if (m_pCurrentScene)
		m_pCurrentScene->Render(pd3dCommandList);
}

void CSceneManager::PostCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pCurrentScene)
		m_pCurrentScene->OnPostRender(pd3dCommandList);
}

void CSceneManager::ProcessInputCurrentScene(HWND hWnd, float fTimeElapsed)
{
	if (m_pCurrentScene)
		m_pCurrentScene->ProcessInput(hWnd, fTimeElapsed);
}

void CSceneManager::Release()
{
	for (int i = 0; i < m_pScenes.size(); i++)
	{
		m_pScenes[i]->Release();
	}
}
