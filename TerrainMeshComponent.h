#pragma once
#include "Mesh.h"
#include "HeightMapGridMesh.h"
class CTerrainMeshComponent : public CMeshComponent
{
public:
	CTerrainMeshComponent() : CMeshComponent() {};
	~CTerrainMeshComponent() {};

public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr); // UINT nSubset

	void SetMesh(int nIndex, std::shared_ptr<CGridMeshComponent> pMesh);
	void ResizeMesh(int nSize);

protected:
	std::vector<std::shared_ptr<CGridMeshComponent>> m_pHeightMapGridMeshs;

};