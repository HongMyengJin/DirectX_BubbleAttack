#pragma once
#include "stdafx.h"
#include "Component.h"
class CShaderComponent : public CComponent
{
public:
	CShaderComponent() {};
	~CShaderComponent() {};

	virtual void Init() {};
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader() { return D3D12_SHADER_BYTECODE(); }; // VertexShader
	virtual D3D12_SHADER_BYTECODE CreatePixelShader() { return D3D12_SHADER_BYTECODE(); }; // PixelShader
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout() { return D3D12_INPUT_LAYOUT_DESC(); };

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) {};
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World) {};

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

protected:
	Microsoft::WRL::ComPtr<ID3DBlob>									m_pd3dVertexShaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob>									m_pd3dPixelShaderBlob;

	int																	m_nPipelineStates = 1;
	int																	m_iCurrentPipelineState = 0;
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>>			m_ppd3dPipelineStates;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC									m_d3dPipelineStateDesc;

	std::vector<D3D12_INPUT_ELEMENT_DESC>								m_d3dPdInputElementDescs;
};

