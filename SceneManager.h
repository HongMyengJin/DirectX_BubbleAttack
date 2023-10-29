#pragma once
#include "Scene.h"
class CSceneManager
{
public:
	CSceneManager() {};
	~CSceneManager() {};
public:
	void ChangeSceneComponent(SceneType eSceneType, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList); // SceneType�� �ѱ�� �ش� ���� �������
	void UpdateCurrentScene(float fTimeElapsed);
	void PreRenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);
	void PostCurrentScene(ID3D12GraphicsCommandList* pd3dCommandList);

	void ProcessInputCurrentScene(HWND hWnd);

	void Release();
private:
	std::unique_ptr<CScene> m_pCurrentScene; // ���� �� ������Ʈ

};