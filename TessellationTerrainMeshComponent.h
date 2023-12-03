#pragma once
#include "Mesh.h"
#include "TessellationGridMeshComponent.h"
class CTessellationTerrainMeshComponent : public CMeshComponent
{
public:
	CTessellationTerrainMeshComponent() {};
	~CTessellationTerrainMeshComponent() {};

public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr); // UINT nSubset

	void SetMesh(int nIndex, std::shared_ptr<CTessellationGridMeshComponent> pMesh);
	void ResizeMesh(int nSize);

protected:
	std::vector<std::shared_ptr<CTessellationGridMeshComponent>> m_pHeightMapGridMeshs;
};

