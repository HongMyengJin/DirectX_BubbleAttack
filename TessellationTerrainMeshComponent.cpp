#include "TessellationTerrainMeshComponent.h"

void CTessellationTerrainMeshComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
	if (!m_pHeightMapGridMeshs.empty())
	{
		for (int i = 0; i < m_pHeightMapGridMeshs.size(); i++)
		{
			if (m_pHeightMapGridMeshs[i]) m_pHeightMapGridMeshs[i]->Render(pd3dCommandList);
		}
	}
}

void CTessellationTerrainMeshComponent::SetMesh(int nIndex, std::shared_ptr<CTessellationGridMeshComponent> pMesh)
{
	if (!m_pHeightMapGridMeshs.empty())
	{
		m_pHeightMapGridMeshs[nIndex] = pMesh;
	}
}

void CTessellationTerrainMeshComponent::ResizeMesh(int nSize)
{
	m_pHeightMapGridMeshs.resize(nSize);
}