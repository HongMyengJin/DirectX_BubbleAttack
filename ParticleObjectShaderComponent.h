#pragma once
#include "ShaderComponent.h"
class CParticleObjectShaderComponent : public CShaderComponent
{
public:
	CParticleObjectShaderComponent() {};
	~CParticleObjectShaderComponent() {};

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	void CreateStreamOutputShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	void CreateDrawShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	virtual void Init();
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual D3D12_SHADER_BYTECODE CreateStreamOutputVertexShader(); // SteamOutputVertexShaders
	virtual D3D12_SHADER_BYTECODE CreateDrawVertexShader();  // DrawVertexShaders

	virtual D3D12_SHADER_BYTECODE CreateDrawPixelShader(); // PixelShader

	virtual D3D12_SHADER_BYTECODE CreateStreamOutputGeometryShader(); // StreamOutputGeometry
	virtual D3D12_SHADER_BYTECODE CreateDrawGeometryShader(); // DrawGeometryShader
	virtual D3D12_BLEND_DESC CreateBlendState();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_STREAM_OUTPUT_DESC CreateStreamOutputLayout();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

protected:
	std::vector<Microsoft::WRL::ComPtr<ID3DBlob>>				m_pd3dVertexShaderBlob;
	std::vector<Microsoft::WRL::ComPtr<ID3DBlob>>				m_pd3dPixelShaderBlob;
	std::vector<Microsoft::WRL::ComPtr<ID3DBlob>>				m_pd3dGeometryShaderBlob;

};

