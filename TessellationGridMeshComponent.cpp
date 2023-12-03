#include "TessellationGridMeshComponent.h"
#include "HeightMapGridMesh.h"

void CTessellationGridMeshComponent::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, void* pContext, XMINT2 xmi2ImageScale)
{
	m_nStride = sizeof(CDiffused2TexturedVertex);
	m_nOffset = 0;
	m_nSlot = 0;

	m_nWidth = nWidth;
	m_nLength = nLength;

	m_xmf3Scale = xmf3Scale;

	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
	m_nVertices = 25;

	std::vector<CDiffused2TexturedVertex> pVertices;
	pVertices.resize(m_nVertices);

	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	float fHeight = 0.0f, fMinHeight = +FLT_MAX, fMaxHeight = -FLT_MAX;

	int nIncrease = 3; //(Block Size == 9) ? 2, (Block Size == 13) ? 3
	for (int i = 0, z = (zStart + nLength - 1); z >= zStart; z -= nIncrease)
	{
		for (int x = xStart; x < (xStart + nWidth); x += nIncrease, i++)
		{
			float xPosition = x * m_xmf3Scale.x, zPosition = z * m_xmf3Scale.z;
			fHeight = pHeightMapImage->GetTerrainTessellationHeight(xPosition, zPosition, m_xmf3Scale);
			pVertices[i].m_xmf3Position = XMFLOAT3(xPosition, fHeight, zPosition);
			pVertices[i].m_xmf4Diffuse = Vector4::Add(OnGetColor(int(x), int(z), pContext), xmf4Color);
			pVertices[i].m_xmf2TexCoord0 = XMFLOAT2(float(x) / float(cxHeightMap - 1), float(czHeightMap - 1 - z) / float(czHeightMap - 1));
			pVertices[i].m_xmf2TexCoord1 = XMFLOAT2(float(x) / float(m_xmf3Scale.x * 0.5f), float(z) / float(m_xmf3Scale.z * 0.5f));
			if (fHeight < fMinHeight) fMinHeight = fHeight;
			if (fHeight > fMaxHeight) fMaxHeight = fHeight;
		}
	}

	m_pd3dPositionBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &pVertices[0], m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = m_nStride;
	m_d3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;


	/*m_nIndice.resize(((nWidth * 2) * (nLength - 1)) + ((nLength - 1) - 1));

	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			for (int x = 0; x < nWidth; x++)
			{
				if ((x == 0) && (z > 0)) m_nIndice[j++] = (UINT)(x + (z * nWidth));
				m_nIndice[j++] = (UINT)(x + (z * nWidth));
				m_nIndice[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			for (int x = nWidth - 1; x >= 0; x--)
			{
				if (x == (nWidth - 1)) m_nIndice[j++] = (UINT)(x + (z * nWidth));
				m_nIndice[j++] = (UINT)(x + (z * nWidth));
				m_nIndice[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
	}

	m_ppd3dSubSetIndexBuffers.resize(1);
	m_ppd3dSubSetIndexUploadBuffers.resize(1);
	m_pd3dSubSetIndexBufferViews.resize(1);

	m_ppd3dSubSetIndexBuffers[0] = CreateBufferResource(pd3dDevice, pd3dCommandList, &m_nIndice[0], sizeof(UINT) * m_nIndice.size(), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_ppd3dSubSetIndexUploadBuffers[0]);

	m_pd3dSubSetIndexBufferViews[0].BufferLocation = m_ppd3dSubSetIndexBuffers[0]->GetGPUVirtualAddress();
	m_pd3dSubSetIndexBufferViews[0].Format = DXGI_FORMAT_R32_UINT;
	m_pd3dSubSetIndexBufferViews[0].SizeInBytes = sizeof(UINT) * m_nIndice.size();*/

}

void CTessellationGridMeshComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dPositionBufferView);
	if (!m_ppd3dSubSetIndexBuffers.empty())
	{
		pd3dCommandList->IASetIndexBuffer(&m_pd3dSubSetIndexBufferViews[0]);
		pd3dCommandList->DrawIndexedInstanced(m_nIndice.size(), 1, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}
