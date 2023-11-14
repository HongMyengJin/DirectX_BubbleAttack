#pragma once
#include "stdafx.h"
#include "Component.h"
#include "TextureComponent.h"
#include "TextureLoader.h"
struct MaterialData
{
	XMFLOAT4															m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4															m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4															m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4															m_xmf4AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	UINT																m_nType = 0x00;

	float																m_fGlossiness = 0.0f;
	float																m_fSmoothness = 0.0f;
	float																m_fSpecularHighlight = 0.0f;
	float																m_fMetallic = 0.0f;
	float																m_fGlossyReflection = 0.0f;
};
class CMaterialValueComponent : public CComponent
{
public:
	CMaterialValueComponent(int nTextures, ResourceTextureType eTextureType);
	~CMaterialValueComponent() {};

	MaterialData& GetMaterialData() { return m_tMaterialData; };

	void SetMaterialData(MaterialData tMaterialData) { m_tMaterialData = tMaterialData;}
	bool LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, UINT nType, UINT nRootParameter, UINT iTextureIndex, FILE* pInFile, std::shared_ptr<CTextureLoader> pTextureLoader);
public:
	MaterialData														m_tMaterialData;
	std::vector<std::shared_ptr<CTextureComponent>>						m_Textures;
	UINT																m_nTexture;
};

class CMaterialsComponent : public CComponent
{
public:
	CMaterialsComponent() {};
	~CMaterialsComponent() {};

	virtual void Init(UINT nMaterials, UINT nTextureN, std::vector<ResourceTextureType> eTextureType);
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, ResourceTextureType nType, UINT nRootParameter, UINT iMaterialIndex, UINT iTextureIndex, wchar_t* pszFileName);
	void LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, FILE* pInFile, std::shared_ptr<CTextureLoader> pTextureLoader);
	void CreateBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT dxgiFormat, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, UINT iMaterialIndex, UINT nIndex);

	virtual void CreateShaderResourceView(ID3D12Device* pd3dDevice, CDescriptorHeap* pDescriptorHeap, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex, UINT nTextureN, UINT iMaterialIndex = 0);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int iIndex);


public:
	UINT													m_nMaterials = 0;
	std::vector<std::unique_ptr<CMaterialValueComponent>>	m_MaterialDatas;
};


