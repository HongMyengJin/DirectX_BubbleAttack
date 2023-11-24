#pragma once
#include "Scene.h"
class CSceneManager
{
public:
	CSceneManager() {};
	~CSceneManager();
public:
	void MakeScenes(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void ChangeSceneComponent(SceneType eSceneType); // SceneType�� �ѱ�� �ش� ���� �������
	void UpdateCurrentScene(float fTimeElapsed);
	void PreRenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);
	void PostCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);

	void ProcessInputCurrentScene(HWND hWnd, float fTimeElapsed);

	void Release();
private:
	std::shared_ptr<CScene> m_pCurrentScene; // ���� �� ������Ʈ
	std::vector<std::shared_ptr<CScene>> m_pScenes;
};