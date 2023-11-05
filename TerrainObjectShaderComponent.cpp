#include "TerrainObjectShaderComponent.h"

void CTerrainObjectShaderComponent::Init()
{
}

void CTerrainObjectShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CTerrainObjectShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

D3D12_SHADER_BYTECODE CTerrainObjectShaderComponent::CreateVertexShader()
{
	D3DReadFileToBlob(L"TerrainObjectVertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainObjectShaderComponent::CreatePixelShader()
{
	D3DReadFileToBlob(L"TerrainObjectPixelShader.cso", m_pd3dPixelShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC CTerrainObjectShaderComponent::CreateInputLayout()
{
	UINT nInputElementDescs = 5;

	m_d3dPdInputElementDescs.resize(nInputElementDescs);

	m_d3dPdInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[4] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = &m_d3dPdInputElementDescs[0];
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}
void CTerrainObjectShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainObjectShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainObjectShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
}
