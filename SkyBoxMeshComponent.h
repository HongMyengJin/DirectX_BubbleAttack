#pragma once
#include "Mesh.h"
class CSkyBoxMeshComponent : public CMeshComponent
{
public:

	CSkyBoxMeshComponent() {};
	~CSkyBoxMeshComponent() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth);

};

