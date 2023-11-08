#include "SceneManager.h"
#include "Stage.h"

void CSceneManager::ChangeSceneComponent(SceneType eSceneType, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	switch (eSceneType)
	{
	case SceneType::Stage1Type:
	{
		Release();
		m_pCurrentScene = std::make_unique<CStage>();
		m_pCurrentScene->BuildObjects(pd3dDevice, pd3dCommandList);
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
	{
		m_pCurrentScene->Render(pd3dCommandList);
	}
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
	if (m_pCurrentScene)
		m_pCurrentScene->Release();
}
