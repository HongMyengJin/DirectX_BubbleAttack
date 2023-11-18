#include "TextureComponent.h"

CTextureComponent::CTextureComponent(UINT nTextureN)
{
	m_nTextureN = nTextureN;
	if (m_nTextureN <= 0)
		return;


	m_ppd3dTextureUploadBuffers.resize(m_nTextureN);
	m_ppd3dTextures.resize(m_nTextureN);

	m_stTextureName.resize(m_nTextureN);
	m_pd3dSrvGpuDescriptorHandles.resize(m_nTextureN);
	m_pd3dSamplerGpuDescriptorHandles.resize(m_nTextureN);

	m_pnResourceTypes.resize(m_nTextureN);
	m_pdxgiBufferFormats.resize(m_nTextureN);
	m_pnBufferElements.resize(m_nTextureN);
	m_pnBufferStrides.resize(m_nTextureN);

}

void CTextureComponent::Init(ResourceTextureType nTextureType, int nSamplers, int nRootParameters)
{
	m_iTextureType = nTextureType;
	m_nRootParameters = nRootParameters;
	m_pnRootParameterIndices.resize(m_nRootParameters);

	m_nSamplers = nSamplers;
}

void CTextureComponent::Update(float fTimeElapsed, void* pData, void* pData2)
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

void CTextureComponent::CreateShaderResourceView(ID3D12Device* pd3dDevice, CDescriptorHeap* pDescriptorHeap, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex, UINT nTextureN)
{
	pDescriptorHeap->m_d3dSrvCPUDescriptorNextHandle.ptr += (::gnCbvSrvDescriptorIncrementSize * nDescriptorHeapIndex);
	pDescriptorHeap->m_d3dSrvGPUDescriptorNextHandle.ptr += (::gnCbvSrvDescriptorIncrementSize * nDescriptorHeapIndex);

	for (int i = 0; i < nTextureN; i++)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = GetShaderResourceViewDesc(i);
		pd3dDevice->CreateShaderResourceView(m_ppd3dTextures[i].Get(), &d3dShaderResourceViewDesc, pDescriptorHeap->m_d3dSrvCPUDescriptorNextHandle);
		pDescriptorHeap->m_d3dSrvCPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		SetGpuDescriptorHandle(i, pDescriptorHeap->m_d3dSrvGPUDescriptorNextHandle);
		pDescriptorHeap->m_d3dSrvGPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
	}
	for (int i = 0; i < m_nRootParameters; i++) SetRootParameterIndex(i, nRootParameterStartIndex + i);
}

void CTextureComponent::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	m_pd3dSamplerGpuDescriptorHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void CTextureComponent::SetRootParameterIndex(int nIndex, UINT nRootParameterIndex)
{
	m_pnRootParameterIndices[nIndex] = nRootParameterIndex;
}

void CTextureComponent::SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	m_pd3dSrvGpuDescriptorHandles[nIndex] = d3dSrvGpuDescriptorHandle;
}

void CTextureComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_nRootParameters == m_nTextureN)
	{
		for (int i = 0; i < m_nRootParameters; i++)
		{
			if (m_pd3dSrvGpuDescriptorHandles[i].ptr && (m_pnRootParameterIndices[i] != -1)) 
				pd3dCommandList->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[i], m_pd3dSrvGpuDescriptorHandles[i]);
		}
	}
	else
	{
		if (m_pd3dSrvGpuDescriptorHandles[0].ptr) pd3dCommandList->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[0], m_pd3dSrvGpuDescriptorHandles[0]);
	}
}

void CTextureComponent::LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, m_ppd3dTextureUploadBuffers[nIndex].GetAddressOf(), D3D12_RESOURCE_STATE_GENERIC_READ/*D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE*/);
}

void CTextureComponent::CreateBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT dxgiFormat, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = (UINT)ResourceTextureType::ResourceTexture2DBUFFER;
	m_pdxgiBufferFormats[nIndex] = dxgiFormat;
	m_pnBufferElements[nIndex] = nElements;
	m_pnBufferStrides[nIndex] = nStride;
	m_ppd3dTextures[nIndex] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pData, nElements * nStride, d3dHeapType, d3dResourceStates, &m_ppd3dTextureUploadBuffers[nIndex]);
}

void CTextureComponent::CreateTexture(ID3D12Device* pd3dDevice, UINT nWidth, UINT nHeight, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue, UINT nResourceType, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTexture2DResource(pd3dDevice, nWidth, nHeight, 1, 0, dxgiFormat, d3dResourceFlags, d3dResourceStates, pd3dClearValue);
}

D3D12_SHADER_RESOURCE_VIEW_DESC CTextureComponent::GetShaderResourceViewDesc(int nIndex)
{
	ID3D12Resource* pShaderResource = m_ppd3dTextures[nIndex].Get();
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	switch (m_iTextureType)
	{
	case ResourceTextureType::ResourceTexture2D:
	case ResourceTextureType::ResourceTexture2D_ARRAY:
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		d3dShaderResourceViewDesc.Texture2D.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		break;
	case ResourceTextureType::ResourceTexture2DARRAY:
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
		break;
	case ResourceTextureType::ResourceTextureCUBE:
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		d3dShaderResourceViewDesc.TextureCube.MipLevels = 1;
		d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		break;
	case ResourceTextureType::ResourceTexture2DBUFFER:
		d3dShaderResourceViewDesc.Format = m_pdxgiBufferFormats[nIndex];
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
		d3dShaderResourceViewDesc.Buffer.NumElements = m_pnBufferElements[nIndex];
		d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
		d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		break;
	case ResourceTextureType::ResourceTextureTypeEnd:
		break;
	default:
		break;
	}	

	return	d3dShaderResourceViewDesc;
}

Microsoft::WRL::ComPtr<ID3D12Resource> CTextureComponent::GetTextureResource(UINT iIndex)
{
	return m_ppd3dTextures[iIndex];
}

std::array<TCHAR, 64> CTextureComponent::GetTextureName(UINT iIndex)
{
	return m_stTextureName[iIndex];
}

void CTextureComponent::SetTextureResource(UINT iIndex, Microsoft::WRL::ComPtr<ID3D12Resource> pTextureResource)
{
	m_ppd3dTextures[iIndex] = pTextureResource;
}

