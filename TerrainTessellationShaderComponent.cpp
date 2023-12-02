#include "TerrainTessellationShaderComponent.h"

void CTerrainTessellationShaderComponent::Init()
{
}

void CTerrainTessellationShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreateVertexShader()
{
	D3DReadFileToBlob(L"CubeMappingObjectVertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreatePixelShader()
{
	D3DReadFileToBlob(L"CubeMappingObjectPixelShader.cso", m_pd3dPixelShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreateDomainShader()
{
	D3DReadFileToBlob(L"TerrainTessellationObjectDomainShader.cso", m_pd3dDomainShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dDomainShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dDomainShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreateHullShader()
{
	D3DReadFileToBlob(L"TerrainTessellationObjectHullShader.cso", m_pd3dHullShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dHullShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dHullShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC CTerrainTessellationShaderComponent::CreateInputLayout()
{
	UINT nInputElementDescs = 4;

	m_d3dPdInputElementDescs.resize(nInputElementDescs);

	m_d3dPdInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = &m_d3dPdInputElementDescs[0];
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_RASTERIZER_DESC CTerrainTessellationShaderComponent::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

void CTerrainTessellationShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainTessellationShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainTessellationShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
}
