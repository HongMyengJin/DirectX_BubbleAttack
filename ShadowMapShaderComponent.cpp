#include "ShadowMapShaderComponent.h"

void CShadowMapShaderComponent::Init()
{
}

void CShadowMapShaderComponent::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, std::shared_ptr<CMaterialsComponent> pDepthFromLightMaterialComponent, CDescriptorHeap* pDescriptorHeap)
{
    m_pDepthFromLightMaterialComponent = pDepthFromLightMaterialComponent;
    m_pDepthFromLightMaterialComponent->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 9, 2, 0); // 수정 필요
}


void CShadowMapShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CShadowMapShaderComponent::PreRender(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CShadowMapShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CShadowMapShaderComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
    PrepareRender(pd3dCommandList);
    UpdateShaderVariables(pd3dCommandList);
    for (int i = 0; i < m_vGameObjects.size(); i++)
    {
        m_vGameObjects[i]->Render(pd3dCommandList, pCamera, NULL);
    }
}


D3D12_SHADER_BYTECODE CShadowMapShaderComponent::CreateVertexShader()
{
    D3DReadFileToBlob(L"ShadowMapObjectVertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CShadowMapShaderComponent::CreatePixelShader()
{
    D3DReadFileToBlob(L"ShadowMapObjectPixelShader.cso", m_pd3dPixelShaderBlob.GetAddressOf());

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC CShadowMapShaderComponent::CreateInputLayout()
{
    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    UINT nInputElementDescs = 2;
    m_d3dPdInputElementDescs.resize(nInputElementDescs);

    m_d3dPdInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    m_d3dPdInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

    d3dInputLayoutDesc.pInputElementDescs = &m_d3dPdInputElementDescs[0];
    d3dInputLayoutDesc.NumElements = nInputElementDescs;

    return(d3dInputLayoutDesc);
}

void CShadowMapShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CShadowMapShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
    m_pDepthFromLightMaterialComponent->UpdateShaderVariable(pd3dCommandList, 0);
}

D3D12_DEPTH_STENCIL_DESC CShadowMapShaderComponent::CreateDepthStencilState()
{
    D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
    ::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
    d3dDepthStencilDesc.DepthEnable = TRUE;
    d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
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

void CShadowMapShaderComponent::AddGameObject(std::shared_ptr<CGameObject> pGameObject)
{
    m_vGameObjects.push_back(pGameObject);
}
