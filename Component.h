#pragma once
#include "stdafx.h"
class CComponent
{
public:
	CComponent() {};
	~CComponent() {};
public:
	virtual void Init() {};
	virtual void Update(float fTimeElapsed, void* pData, void* pData2) {};

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) {};
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr) {};
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList) {};

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList) {};
};

