#include "TerrainWaterShaderComponent.h"

void CTerrainWaterShaderComponent::Init()
{
}

void CTerrainWaterShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CTerrainWaterShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

D3D12_SHADER_BYTECODE CTerrainWaterShaderComponent::CreateVertexShader()
{
	D3DReadFileToBlob(L"TerrainWaterObjectVertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainWaterShaderComponent::CreatePixelShader()
{
	D3DReadFileToBlob(L"TerrainWaterObjectPixelShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC CTerrainWaterShaderComponent::CreateInputLayout()
{
	UINT nInputElementDescs = 2;

	m_d3dPdInputElementDescs.resize(nInputElementDescs);

	m_d3dPdInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = &m_d3dPdInputElementDescs[0];
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CTerrainWaterShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainWaterShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainWaterShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
}
