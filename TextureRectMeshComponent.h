#pragma once
#include "Mesh.h"
class CTextureRectMeshComponent : public CMeshComponent
{
public:
	CTextureRectMeshComponent() {};
	~CTextureRectMeshComponent() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f, float fxPosition = 0.0f, float fyPosition = 0.0f, float fzPosition = 0.0f);
};

