#include "TextureComponent.h"

CTextureComponent::CTextureComponent(UINT nTextureN, UINT nTextureType, int nSamplers, int nRootParameters)
{
	m_nTextureN = nTextureN;
	if (m_nTextureN <= 0)
		return;

	m_ppd3dTextureUploadBuffers.resize(m_nTextureN);
	m_ppd3dTextures.resize(m_nTextureN);

	m_stTextureName.resize(m_nTextureN);
	//m_pd3dSrvGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nTextureN];

	m_pnResourceTypes.resize(m_nTextureN);
	m_pdxgiBufferFormats.resize(m_nTextureN);
	m_pnBufferElements.resize(m_nTextureN);

	m_nRootParameters = nRootParameters;
	m_pnRootParameterIndices.resize(m_nRootParameters);

	m_nSamplers = nSamplers;

	//if(m_nSamplers > 0)
	//	m_pd3dSamplerGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nSamplers];
}

void CTextureComponent::Init()
{
}

void CTextureComponent::Update(float fTimeElapsed)
{
}

void CTextureComponent::PrepareRender(void* pContext)
{
}

void CTextureComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
}

void CTextureComponent::PostRender(void* pContext)
{
}

void CTextureComponent::LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, m_ppd3dTextureUploadBuffers[nIndex].GetAddressOf(), D3D12_RESOURCE_STATE_GENERIC_READ/*D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE*/);
}
