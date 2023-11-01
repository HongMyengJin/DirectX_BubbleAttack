#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "ThirdPersonCamera.h"
#include "TerrainObject.h"
#include "SkyBoxObject.h"
#include "TextureRectObject.h"
#include "ParticleObject.h"
#include "RippleWaterObject.h"
class CStage : public CScene
{
public:
	CStage();
	~CStage();

	virtual void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	virtual void CreateGraphicsPipelineState(ID3D12Device* pd3dDevice);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	virtual bool ProcessInput(HWND hWnd);
	void AnimateObjects(float fTimeElapsed);
	void UpdateObjects(float fTimeElapsed);

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);

	void RenderParticle(ID3D12GraphicsCommandList* pd3dCommandList);
	void OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void Release();
protected:
	std::unique_ptr<CThirdPersonCamera>		m_pCamera;
	std::unique_ptr<CTerrainObject>			m_pTerrain;
	std::unique_ptr<CRippleWaterObject>		m_pTerrainWater;
	std::unique_ptr<CSkyBoxObject>			m_pSkyBoxObject;
	std::unique_ptr<CParticleObject>		m_pParticleObject;
	std::vector<std::shared_ptr<CTextureRectObject>> m_pTextureRectObjects;
	
};