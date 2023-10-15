#pragma once
#include "stdafx.h"
#include "Scene.h"
class CStage : public CScene
{
public:
	CStage();
	~CStage();

	virtual void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	virtual void CreateGraphicsPipelineState(ID3D12Device* pd3dDevice);

	void BuildObjects(ID3D12Device* pd3dDevice);

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void UpdateObjects(float fTimeElapsed);

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);
};