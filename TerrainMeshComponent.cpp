#include "TerrainMeshComponent.h"

void CTerrainMeshComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
	if (!m_pHeightMapGridMeshs.empty())
	{
		for (int i = 0; i < m_pHeightMapGridMeshs.size(); i++)
		{
			if (m_pHeightMapGridMeshs[i]) m_pHeightMapGridMeshs[i]->Render(pd3dCommandList);
		}
	}
}

void CTerrainMeshComponent::SetMesh(int nIndex, std::shared_ptr<CHeightMapGridMesh> pMesh)
{
	if (!m_pHeightMapGridMeshs.empty())
	{
		//if (m_pHeightMapGridMeshs[nIndex]) m_pHeightMapGridMeshs[nIndex]->Release();
		m_pHeightMapGridMeshs[nIndex] = pMesh;
	}
}

void CTerrainMeshComponent::ResizeMesh(int nSize)
{
	m_pHeightMapGridMeshs.resize(nSize);
}
