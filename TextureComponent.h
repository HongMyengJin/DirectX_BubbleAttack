#pragma once
#include "stdafx.h"
#include "Component.h"
#include "ShaderComponent.h"
#include "DescriptorHeap.h"
class CTextureComponent : public CComponent
{
public:
	CTextureComponent(UINT nTextureN);
	~CTextureComponent() {};

	virtual void Init(ResourceTextureType nTextureType, int nSamplers, int nRootParameters);
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PrepareRender(void* pContext = nullptr);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr);
	virtual void PostRender(void* pContext = nullptr);

	void CreateShaderResourceView(ID3D12Device* pd3dDevice, CDescriptorHeap* pDescriptorHeap, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex, UINT nTextureN);
	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);
	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);
	void CreateBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT dxgiFormat, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, UINT nIndex);
	void CreateTexture(ID3D12Device* pd3dDevice, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue, UINT nResourceType, UINT nIndex);
	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);

	Microsoft::WRL::ComPtr<ID3D12Resource>	GetTextureResource(UINT iIndex);
	std::array<TCHAR, 64>					GetTextureName(UINT iIndex);

	void SetTextureResource(UINT iIndex, Microsoft::WRL::ComPtr<ID3D12Resource> pTextureResource);

	std::vector<std::array<TCHAR, 64>>						m_stTextureName;
private:
	UINT													m_nTextureN = 0;
	//std::vector<std::string>								m_stTextureName;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>		m_ppd3dTextures;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>		m_ppd3dTextureUploadBuffers;

	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>				m_pd3dSrvGpuDescriptorHandles;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>				m_pd3dSamplerGpuDescriptorHandles;

	std::unique_ptr<CShaderComponent>						m_pShaderComponent;

	std::vector<UINT>										m_pnResourceTypes;
	std::vector<DXGI_FORMAT>								m_pdxgiBufferFormats;
	std::vector<int>										m_pnBufferElements;

	std::vector<int>										m_pnBufferStrides;

	int														m_nRootParameters = 0;
	std::vector<int>										m_pnRootParameterIndices;

	int														m_nSamplers = 0;

	ResourceTextureType										m_iTextureType = ResourceTextureType::ResourceTextureTypeEnd;
};

