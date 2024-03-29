#pragma once
#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"
#include "Light.h"
#include "DescriptorHeap.h"
#include "PlayerGameObject.h"
#include "Camera.h"
class CScene
{
public:
	CScene();
	~CScene();

	virtual void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice) = 0;

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	virtual bool ProcessInput(HWND hWnd, float fTimeElapsed);
	virtual void AnimateObjects(float fTimeElapsed) = 0;
	virtual void UpdateObjects(float fTimeElapsed) = 0;

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL) = 0;
	virtual void OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList) = 0;

	virtual void Release();
	void SetChangeScene(bool bChangeScene);
	bool GetChangeScene();
protected:
	std::unique_ptr<CDescriptorHeap>				m_pd3dDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>		m_pd3dGraphicsRootsignature; // 루트 시그니쳐의 인터페이스 포인터
	Microsoft::WRL::ComPtr<ID3D12PipelineState>		m_pd3dPipelineState; // 파이프라인 상태를 나타내는 인터페이스 포인터

	std::shared_ptr<CPlayerGameObject>				m_pPlayersGameObject;
	std::shared_ptr<CLight>							m_pLightObject;
	bool											m_bChangeScene = false;
};