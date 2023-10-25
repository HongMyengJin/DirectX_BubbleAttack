#pragma once
#include "ShaderComponent.h"
class CSkyBoxShaderComponent : public CShaderComponent
{
public:
	CSkyBoxShaderComponent() {};
	~CSkyBoxShaderComponent() {};

	virtual void Init();
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(); // VertexShader
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(); // PixelShader
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

};

