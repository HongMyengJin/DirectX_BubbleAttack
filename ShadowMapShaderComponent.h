#pragma once
#include "ShaderComponent.h"
#include "MaterialComponent.h"
class CShadowMapShaderComponent : public CShaderComponent
{
public:

	CShadowMapShaderComponent() {};
	~CShadowMapShaderComponent() {};

	virtual void Init();
	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, std::shared_ptr<CMaterialsComponent> pDepthFromLightMaterialComponent, CDescriptorHeap* pDescriptorHeap);
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PreRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(); // VertexShader
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(); // PixelShader
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	void AddGameObject(std::shared_ptr<CGameObject> pGameObject);

private:
	std::shared_ptr<CMaterialsComponent>			m_pDepthFromLightMaterialComponent;
	std::vector<std::shared_ptr<CGameObject>>		m_vGameObjects;
};

