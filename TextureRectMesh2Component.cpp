#include "TextureRectMesh2Component.h"
#include "TextureMesh.h"

void CTextureRectMesh2Component::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition, bool Front)
{
	m_nVertices = 6;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	std::vector<CTextureMesh> pVertices;
	float fx = (fWidth * 0.5f)/* + fxPosition*/, fy = (fHeight * 0.5f) /*+ fyPosition*/, fz = (fDepth * 0.5f) /*+ fzPosition*/;

	//if (fWidth == 0.0f)
	//{
	//	if (fxPosition > 0.0f)
	//	{
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(1.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(0.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.0f, 0.0f)));
	//	}
	//	else
	//	{
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(1.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(0.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.0f, 0.0f)));
	//	}
	//}
	//else if (fHeight == 0.0f)
	//{
	//	if (fyPosition > 0.0f)
	//	{
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 0.0f)));
	//	}
	//	else
	//	{
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 0.0f)));
	//	}
	//}
	//else if (fDepth == 0.0f)
	//{
	//	if (fzPosition > 0.0f)
	//	{
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(1.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(0.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
	//	}
	//	else
	//	{
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(1.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(0.0f, 0.0f)));
	//		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
	//	}
	//}
	if (Front)
	{
		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(1.0f, 1.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(0.0f, 0.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
	}
	else
	{
		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(1.0f, 1.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(0.0f, 0.0f)));
		pVertices.push_back(CTextureMesh(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f)));
	}

	m_pd3dPositionBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &pVertices[0], sizeof(CTextureMesh) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = sizeof(CTextureMesh);
	m_d3dPositionBufferView.SizeInBytes = sizeof(CTextureMesh) * m_nVertices;
}
