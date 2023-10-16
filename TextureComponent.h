#pragma once
#include "stdafx.h"
#include "Component.h"
#include "ShaderComponent.h"
class CTextureComponent : public CComponent
{
public:
	CTextureComponent(UINT nTextureN, UINT nTextureType, int nSamplers, int nRootParameters);
	~CTextureComponent() {};

	virtual void Init();
	virtual void Update(float fTimeElapsed);

	virtual void PrepareRender(void* pContext = nullptr);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr);
	virtual void PostRender(void* pContext = nullptr);

	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);

	std::array<TCHAR, 64>									m_stTextureName;
private:
	UINT													m_nTextureN = 0;
	UINT													m_nTextureType;
	//std::vector<std::string>								m_stTextureName;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>		m_ppd3dTextures;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>		m_ppd3dTextureUploadBuffers;

	Microsoft::WRL::ComPtr<ID3D12Resource>					m_pd3dSrvGpuDescriptorHandles;
	Microsoft::WRL::ComPtr<ID3D12Resource>					m_pd3dSamplerGpuDescriptorHandles;

	std::unique_ptr<CShaderComponent>						m_pShaderComponent;

	std::vector<UINT>										m_pnResourceTypes;
	std::vector<DXGI_FORMAT>								m_pdxgiBufferFormats;
	std::vector<int>										m_pnBufferElements;

	int														m_nRootParameters = 0;
	std::vector<int>										m_pnRootParameterIndices;

	int														m_nSamplers = 0;

};

