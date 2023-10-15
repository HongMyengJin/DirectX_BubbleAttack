#include "SceneManager.h"
#include "Stage.h"

void CSceneManager::ChangeSceneComponent(SceneType eSceneType, ID3D12Device* pd3dDevice)
{
	switch (eSceneType)
	{
	case SceneType::Stage1Type:
	{
		m_pCurrentScene = std::make_unique<CStage>();
		m_pCurrentScene->BuildObjects(pd3dDevice);
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
		m_pCurrentScene->UpdateObjects(fTimeElapsed);
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
