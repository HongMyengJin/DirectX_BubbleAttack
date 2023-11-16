#pragma once
#include "ShaderComponent.h"
#include "TextureComponent.h"
#include "Light.h"

struct TOOBJECTSPACEINFO
{
	XMFLOAT4X4						m_xmf4x4ToTexture;
	XMFLOAT4						m_xmf4Position;
};

struct TOLIGHTSPACES
{
	TOOBJECTSPACEINFO				m_pToLightSpaces[MAX_LIGHT];
};
class CDepthRenderShaderComponent : public CShaderComponent
{
public:
	CDepthRenderShaderComponent() {};
	~CDepthRenderShaderComponent() {};

	virtual void Init(std::shared_ptr<CLight> pLights);
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PreRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(); // VertexShader
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(); // PixelShader
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

protected:

	std::shared_ptr<CTextureComponent>	m_pDepthFromLightTextureComponent;
	std::shared_ptr<CCamera>	m_pDepthRenderCameras[MAX_DEPTH_TEXTURES];

	XMMATRIX					m_xmProjectionToTexture;

	Microsoft::WRL::ComPtr<ID3D12Resource>	m_pd3dDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pd3dcbToLightSpaces;

	TOLIGHTSPACES*									m_pcbMappedToLightSpaces;
	std::shared_ptr<TOLIGHTSPACES>					m_pToLightSpaces;

	std::shared_ptr<CLight>	m_pLights;

};

