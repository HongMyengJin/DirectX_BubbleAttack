#pragma once
#include "GameObject.h"
#include "Scene.h"
class CMirrorObject : public CGameObject
{
public:
	CMirrorObject() {}
	~CMirrorObject() {}

public:
	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap);
	void OnScenePreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage);
	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage);
private:
	std::shared_ptr<CCamera>	m_pCameras; // 거울 방향 카메라

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_pd3dRtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_pd3dDsvDescriptorHeap;

	Microsoft::WRL::ComPtr<ID3D12Resource>			m_pd3dDepthStencilBuffer;

	D3D12_CPU_DESCRIPTOR_HANDLE						m_pd3dRtvCPUDescriptorHandles;
	D3D12_CPU_DESCRIPTOR_HANDLE						m_d3dDsvDescriptorCPUHandle;
};

