#include "MirrorObjectShaderComponent.h"

void CMirrorObjectShaderComponent::Init()
{
}

void CMirrorObjectShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}


void CMirrorObjectShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

D3D12_SHADER_BYTECODE CMirrorObjectShaderComponent::CreateVertexShader()
{
    D3DReadFileToBlob(L"TextureObjectVertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf()); 

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CMirrorObjectShaderComponent::CreatePixelShader()
{
    D3DReadFileToBlob(L"TextureObjectPixelShader.cso", m_pd3dPixelShaderBlob.GetAddressOf());

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

//D3D12_SHADER_BYTECODE CMirrorObjectShaderComponent::CreateGeometryShader()
//{
//    D3DReadFileToBlob(L"MiirorObjectGeometryShader.cso", m_pd3dGeometrylShaderBlob.GetAddressOf());
//
//    D3D12_SHADER_BYTECODE d3dShaderByteCode;
//    d3dShaderByteCode.BytecodeLength = m_pd3dGeometrylShaderBlob->GetBufferSize();
//    d3dShaderByteCode.pShaderBytecode = m_pd3dGeometrylShaderBlob->GetBufferPointer();
//
//    return d3dShaderByteCode;
//}

D3D12_INPUT_LAYOUT_DESC CMirrorObjectShaderComponent::CreateInputLayout()
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

void CMirrorObjectShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMirrorObjectShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMirrorObjectShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
}

D3D12_RASTERIZER_DESC CMirrorObjectShaderComponent::CreateRasterizerState()
{
    D3D12_RASTERIZER_DESC d3dRasterizerDesc;
    ::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
    d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    //	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
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

D3D12_BLEND_DESC CMirrorObjectShaderComponent::CreateBlendState()
{
    D3D12_BLEND_DESC d3dBlendDesc;
    ::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
    d3dBlendDesc.AlphaToCoverageEnable = TRUE;
    d3dBlendDesc.IndependentBlendEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
    d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    return(d3dBlendDesc);
}


