#pragma once

#define TerrainTessellationTexture 0
#define TerrainTessellationWire 1

#include "ShaderComponent.h"
class CTerrainTessellationShaderComponent : public CShaderComponent
{
	public:
		CTerrainTessellationShaderComponent() {};
		~CTerrainTessellationShaderComponent() {};

		virtual void Init();
		virtual void Update(float fTimeElapsed, void* pData, void* pData2);

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(); // VertexShader
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(); // PixelShader
		virtual D3D12_SHADER_BYTECODE CreatePixel2Shader(); // PixelShader
		virtual D3D12_SHADER_BYTECODE CreateDomainShader(); // DomainShader
		virtual D3D12_SHADER_BYTECODE CreateHullShader(); // HullShader
		virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
		virtual D3D12_RASTERIZER_DESC CreateRasterizerState();

		virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, DXGI_FORMAT pdxgiRtvFormats, DXGI_FORMAT dxgiDsvFormat, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
		virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
		virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);

private:
	bool m_bWireRender = false;
};

