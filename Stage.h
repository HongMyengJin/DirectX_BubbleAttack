#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "Camera.h"
#include "TerrainObject.h"
class CStage : public CScene
{
public:
	CStage();
	~CStage();

	virtual void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	virtual void CreateGraphicsPipelineState(ID3D12Device* pd3dDevice);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void UpdateObjects(float fTimeElapsed);

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Release();
protected:
	std::unique_ptr<CCamera>				m_pCamera;
	std::unique_ptr<CTerrainObject>			m_pTerrain;
	
};