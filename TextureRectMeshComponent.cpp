#include "TextureRectMeshComponent.h"
#include "TextureMesh.h"
void CTextureRectMeshComponent::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition)
{
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	std::vector<CTextureMesh> pVertices;
	pVertices.push_back(CTextureMesh(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT2(fWidth, fHeight)));

	m_pd3dPositionBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &pVertices[0], sizeof(CTextureMesh) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = sizeof(CTextureMesh);
	m_d3dPositionBufferView.SizeInBytes = sizeof(CTextureMesh) * m_nVertices;
}
