#pragma once
#include "stdafx.h"
#include "Component.h"
#include "TextureComponent.h"

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
	CMaterialValueComponent(int nTextures);
	~CMaterialValueComponent() {};

	MaterialData& GetMaterialData() { return m_tMaterialData; };

	void SetMaterialData(MaterialData tMaterialData) { m_tMaterialData = tMaterialData;}
	bool LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, UINT nType, UINT nRootParameter, UINT iTextureIndex, FILE* pInFile);
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

	virtual void Init(UINT nMaterials, UINT nTextureN);
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, ResourceTextureType nType, UINT nRootParameter, UINT iMaterialIndex, UINT iTextureIndex, wchar_t* pszFileName);
	void LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, FILE* pInFile);
	virtual void CreateShaderResourceView(ID3D12Device* pd3dDevice, CDescriptorHeap* pDescriptorHeap, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex, UINT nTextureN);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int iIndex);


public:
	UINT													m_nMaterials = 0;
	std::vector<std::unique_ptr<CMaterialValueComponent>>	m_MaterialDatas;
};


