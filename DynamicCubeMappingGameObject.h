#pragma once
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "MaterialComponent.h"

class CDynamicCubeMappingGameObject : public CGameObject
{
public:
	CDynamicCubeMappingGameObject() {};
	~CDynamicCubeMappingGameObject() {};
	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, UINT nCubeMapSize, XMFLOAT3 xmf3Extent);
	void OnScenePreRender(ID3D12GraphicsCommandList* pd3dCommandList,  CScene* pStage);
	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage);
private:
	std::shared_ptr<CCamera>	m_pCameras[6];

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_pd3dRtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_pd3dDsvDescriptorHeap;

	Microsoft::WRL::ComPtr<ID3D12Resource>			m_pd3dDepthStencilBuffer;
	//std::shared_ptr<CMaterialsComponent>			m_pRenderTargetMaterialComponent;

	D3D12_CPU_DESCRIPTOR_HANDLE						m_pd3dRtvCPUDescriptorHandles[6];
	D3D12_CPU_DESCRIPTOR_HANDLE						m_d3dDsvDescriptorCPUHandle;

};

