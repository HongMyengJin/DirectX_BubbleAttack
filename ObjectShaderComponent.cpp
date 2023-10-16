#include "ObjectShaderComponent.h"

void CObjectShaderComponent::Init()
{
}

void CObjectShaderComponent::Update(float fTimeElapsed)
{
}

//void CObjectShaderComponent::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
//{
//}
//
//void CObjectShaderComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
//{
//}

void CObjectShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

//void CObjectShaderComponent::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
//{
//}

D3D12_SHADER_BYTECODE CObjectShaderComponent::CreateVertexShader()
{
	D3DReadFileToBlob(L"VertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CObjectShaderComponent::CreatePixelShader()
{
	D3DReadFileToBlob(L"PixelShader.cso", m_pd3dPixelShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC CObjectShaderComponent::CreateInputLayout()
{
	UINT nInputElementDescs = 5;
	std::vector< D3D12_INPUT_ELEMENT_DESC> pd3dInputElementDescs;
	pd3dInputElementDescs.resize(nInputElementDescs);

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = &pd3dInputElementDescs[0];
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CObjectShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CObjectShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CObjectShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
}

//D3D12_RASTERIZER_DESC CObjectShaderComponent::CreateRasterizerState()
//{
//    return D3D12_RASTERIZER_DESC();
//}
//
//D3D12_BLEND_DESC CObjectShaderComponent::CreateBlendState()
//{
//    return D3D12_BLEND_DESC();
//}
//
//D3D12_DEPTH_STENCIL_DESC CObjectShaderComponent::CreateDepthStencilState()
//{
//    return D3D12_DEPTH_STENCIL_DESC();
//}
