#include "DepthRenderShaderComponent.h"

void CDepthRenderShaderComponent::Init(std::shared_ptr<CLight> pLights)
{
	m_pLights = pLights;
	m_pToLightSpaces = std::shared_ptr<TOLIGHTSPACES>();

	XMFLOAT4X4 xmf4x4ToTexture = { 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f };
	m_xmProjectionToTexture = XMLoadFloat4x4(&xmf4x4ToTexture);
}

void CDepthRenderShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CDepthRenderShaderComponent::PreRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CDepthRenderShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

D3D12_SHADER_BYTECODE CDepthRenderShaderComponent::CreateVertexShader()
{
    D3DReadFileToBlob(L"ObjectPixelShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CDepthRenderShaderComponent::CreatePixelShader()
{
    D3DReadFileToBlob(L"DepthWritePixelShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC CDepthRenderShaderComponent::CreateInputLayout()
{
    UINT nInputElementDescs = 5;

    m_d3dPdInputElementDescs.resize(nInputElementDescs);

    m_d3dPdInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    m_d3dPdInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    m_d3dPdInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    m_d3dPdInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    m_d3dPdInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    d3dInputLayoutDesc.pInputElementDescs = &m_d3dPdInputElementDescs[0];
    d3dInputLayoutDesc.NumElements = nInputElementDescs;

    return(d3dInputLayoutDesc);
}

void CDepthRenderShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
    UINT ncbDepthElementBytes;

    ncbDepthElementBytes = ((sizeof(TOLIGHTSPACES) + 255) & ~255); //256ÀÇ ¹è¼ö
    m_pd3dcbToLightSpaces = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbDepthElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

    m_pd3dcbToLightSpaces->Map(0, NULL, (void**)&m_pcbMappedToLightSpaces);
}

void CDepthRenderShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
    ::memcpy(m_pcbMappedToLightSpaces, m_pToLightSpaces.get(), sizeof(TOLIGHTSPACES));

    D3D12_GPU_VIRTUAL_ADDRESS d3dcbToLightGpuVirtualAddress = m_pd3dcbToLightSpaces->GetGPUVirtualAddress();
    pd3dCommandList->SetGraphicsRootConstantBufferView(6, d3dcbToLightGpuVirtualAddress); //ToLight
}

D3D12_RASTERIZER_DESC CDepthRenderShaderComponent::CreateRasterizerState()
{
    D3D12_RASTERIZER_DESC d3dRasterizerDesc;
    ::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
    d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    d3dRasterizerDesc.FrontCounterClockwise = FALSE;
#ifdef _WITH_RASTERIZER_DEPTH_BIAS
    d3dRasterizerDesc.DepthBias = 250000;
#endif
    d3dRasterizerDesc.DepthBiasClamp = 0.0f;
    d3dRasterizerDesc.SlopeScaledDepthBias = 1.0f;
    d3dRasterizerDesc.DepthClipEnable = TRUE;
    d3dRasterizerDesc.MultisampleEnable = FALSE;
    d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
    d3dRasterizerDesc.ForcedSampleCount = 0;
    d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    return(d3dRasterizerDesc);
}

D3D12_DEPTH_STENCIL_DESC CDepthRenderShaderComponent::CreateDepthStencilState()
{
    D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
    ::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
    d3dDepthStencilDesc.DepthEnable = TRUE;
    d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS; //D3D12_COMPARISON_FUNC_LESS_EQUAL
    d3dDepthStencilDesc.StencilEnable = FALSE;
    d3dDepthStencilDesc.StencilReadMask = 0x00;
    d3dDepthStencilDesc.StencilWriteMask = 0x00;
    d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
    d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

    return(d3dDepthStencilDesc);
}
