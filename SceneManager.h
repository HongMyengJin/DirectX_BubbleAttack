#pragma once
#include "Scene.h"
class CSceneManager
{
public:
	CSceneManager() {};
	~CSceneManager();
public:
	void MakeScenes(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void ChangeSceneComponent(SceneType eSceneType); // SceneType을 넘기면 해당 씬이 만들어짐
	void UpdateCurrentScene(float fTimeElapsed);
	void PreRenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);
	void PostCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);

	void ProcessInputCurrentScene(HWND hWnd, float fTimeElapsed);

	void Release();
private:
	std::shared_ptr<CScene> m_pCurrentScene; // 현재 씬 컴포넌트
	std::vector<std::shared_ptr<CScene>> m_pScenes;
};