#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "ThirdPersonCamera.h"
#include "TerrainObject.h"
#include "SkyBoxObject.h"
#include "TextureRectObject.h"
#include "ParticleObject.h"
#include "RippleWaterObject.h"
#include "UIGameObject.h"
#include "MonsterGameObject.h"
#include "VelocityGaugeUIGameObject.h"
#include "TextureLoader.h"
#include "ShadowMapShaderComponent.h"
#include "DepthRenderShaderComponent.h"
#include "DynamicCubeMappingGameObject.h"
#include "DynamicCubeMappingShaderComponent.h"
#include "ObjectShaderComponent.h"

class CMirrorStage : public CScene
{
public:
	CMirrorStage() {} ;
	~CMirrorStage() {};

	virtual void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	virtual bool ProcessInput(HWND hWnd, float fTimeElapsed);
	void AnimateObjects(float fTimeElapsed);
	void UpdateObjects(float fTimeElapsed);

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);

	void RenderParticle(ID3D12GraphicsCommandList* pd3dCommandList);
	void OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	void CollisionCheck();
	virtual void Release();

private:
	std::unique_ptr<CThirdPersonCamera>						m_pCamera;
	std::unique_ptr<CParticleObject>						m_pParticleObject;
	std::vector<std::shared_ptr<CTextureRectObject>>		m_pTextureRectObjects;
	std::vector<std::shared_ptr<CTextureRectObject>>		m_pEffectRectObjects;
	std::vector<std::shared_ptr<CMonsterGameObject>>		m_pMonsterObjects;
	std::vector<std::shared_ptr<CUINumberGameObject>>		m_pUINumberObjects;
	std::vector<std::shared_ptr<CUIGameObject>>				m_pUIObjects;
	std::shared_ptr<CTextureLoader>							m_pTextureLoader;

	std::shared_ptr<CShadowMapShaderComponent>				m_pShadowShader;
	std::shared_ptr<CDepthRenderShaderComponent>			m_pDepthRenderShader;

	std::shared_ptr<CDynamicCubeMappingGameObject>			m_pDynamicCubeMappingGameObject;
	UINT													m_iPlayerAttack = 0;
	UINT													m_iMonsterAttack = 0;
	bool													m_bResult = false;

	UCHAR													pPreKeysBuffer[256];

};

