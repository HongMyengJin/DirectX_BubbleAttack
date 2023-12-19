#pragma once
#include "GameObject.h"
#include "Scene.h"

enum class MirrorData
{
	Mirror_Left,
	Mirror_Light,
	Mirror_Up,
	Mirror_Down,
	Mirror_Front,
	Mirror_Back,
	MirrorData_End
};
class CMirrorObject : public CGameObject
{
public:
	CMirrorObject() {}
	~CMirrorObject() {}

public:
	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, MirrorData eMirrorData);
	void OnScenePreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage);
	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage);
private:
	std::shared_ptr<CCamera>	m_pCameras; // 거울 방향 카메라
	
	XMFLOAT3					m_xmf3LookAt;
	XMFLOAT3					m_xmf3Up;
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_pd3dRtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_pd3dDsvDescriptorHeap;

	Microsoft::WRL::ComPtr<ID3D12Resource>			m_pd3dDepthStencilBuffer;

	D3D12_CPU_DESCRIPTOR_HANDLE						m_pd3dRtvCPUDescriptorHandles;
	D3D12_CPU_DESCRIPTOR_HANDLE						m_d3dDsvDescriptorCPUHandle;


	MirrorData										m_eMirrorType;
};


