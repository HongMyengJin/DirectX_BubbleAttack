#pragma once
#include "Mesh.h"
class CUIMesh : public CMeshComponent
{
public:
	CUIMesh() {};
	~CUIMesh() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
};

