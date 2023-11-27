#pragma once
#include "Mesh.h"
class CTextureRectMesh2Component : public CMeshComponent
{
public:
	CTextureRectMesh2Component() {};
	~CTextureRectMesh2Component() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f, float fxPosition = 0.0f, float fyPosition = 0.0f, float fzPosition = 0.0f, bool Front = false);
};

