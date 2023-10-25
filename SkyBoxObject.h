#pragma once
#include "GameObject.h"
class CSkyBoxObject : public CGameObject
{
public:
	CSkyBoxObject() {};
	~CSkyBoxObject() {};
	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap);

};

