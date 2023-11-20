#include "DepthRenderShaderComponent.h"
#include "Camera.h"

void CDepthRenderShaderComponent::Init(std::shared_ptr<CLight> pLights)
{
	m_pLights = pLights;
	m_pToLightSpaces = std::make_shared<TOLIGHTSPACES>();

	XMFLOAT4X4 xmf4x4ToTexture = { 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f };
	m_xmProjectionToTexture = XMLoadFloat4x4(&xmf4x4ToTexture);

   
}

void CDepthRenderShaderComponent::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext)
{
    m_pDepthFromLightMaterialComponent = std::make_shared<CMaterialsComponent>();

    D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
    ::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
    d3dDescriptorHeapDesc.NumDescriptors = MAX_DEPTH_TEXTURES;
    d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    d3dDescriptorHeapDesc.NodeMask = 0;
    HRESULT hResult = pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)m_pd3dRtvDescriptorHeap.GetAddressOf());


    std::vector<ResourceTextureType> m_vTextureType;
    m_vTextureType.resize(1);
    m_vTextureType[0] = ResourceTextureType::ResourceTexture2D_ARRAY;

    m_pDepthFromLightMaterialComponent->Init(1, MAX_DEPTH_TEXTURES, m_vTextureType);


    D3D12_CLEAR_VALUE d3dClearValue = { DXGI_FORMAT_R32_FLOAT, { 1.0f, 1.0f, 1.0f, 1.0f } };
    for (UINT i = 0; i < MAX_DEPTH_TEXTURES; i++) 
        m_pDepthFromLightMaterialComponent->CreateTexture(pd3dDevice, _DEPTH_BUFFER_WIDTH, _DEPTH_BUFFER_HEIGHT, DXGI_FORMAT_R32_FLOAT, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON, &d3dClearValue, (UINT)ResourceTextureType::ResourceTexture2D, 0, i);

    D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
    d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
    d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;
    d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R32_FLOAT;

    D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    for (UINT i = 0; i < MAX_DEPTH_TEXTURES; i++)
    {
        ID3D12Resource* pd3dTextureResource = m_pDepthFromLightMaterialComponent->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(i).Get();
        pd3dDevice->CreateRenderTargetView(pd3dTextureResource, &d3dRenderTargetViewDesc, d3dRtvCPUDescriptorHandle);
        m_pd3dRtvCPUDescriptorHandles[i] = d3dRtvCPUDescriptorHandle;
        d3dRtvCPUDescriptorHandle.ptr += ::gnRtvDescriptorIncrementSize;
    }

    d3dDescriptorHeapDesc.NumDescriptors = 1;
    d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    hResult = pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);

    D3D12_RESOURCE_DESC d3dResourceDesc;
    d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    d3dResourceDesc.Alignment = 0;
    d3dResourceDesc.Width = _DEPTH_BUFFER_WIDTH;
    d3dResourceDesc.Height = _DEPTH_BUFFER_HEIGHT;
    d3dResourceDesc.DepthOrArraySize = 1;
    d3dResourceDesc.MipLevels = 1;
    d3dResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
    d3dResourceDesc.SampleDesc.Count = 1;
    d3dResourceDesc.SampleDesc.Quality = 0;
    d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_HEAP_PROPERTIES d3dHeapProperties;
    ::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
    d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    d3dHeapProperties.CreationNodeMask = 1;
    d3dHeapProperties.VisibleNodeMask = 1;

    d3dClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    d3dClearValue.DepthStencil.Depth = 1.0f;
    d3dClearValue.DepthStencil.Stencil = 0;

    pd3dDevice->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void**)&m_pd3dDepthBuffer);

    D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
    ::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
    d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

    m_d3dDsvDescriptorCPUHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    pd3dDevice->CreateDepthStencilView(m_pd3dDepthBuffer.Get(), &d3dDepthStencilViewDesc, m_d3dDsvDescriptorCPUHandle);

    for (int i = 0; i < MAX_DEPTH_TEXTURES; i++)
    {
        m_pDepthRenderCameras[i] = std::make_shared<CCamera>();
        m_pDepthRenderCameras[i]->SetViewport(0, 0, _DEPTH_BUFFER_WIDTH, _DEPTH_BUFFER_HEIGHT, 0.0f, 1.0f);
        m_pDepthRenderCameras[i]->SetScissorRect(0, 0, _DEPTH_BUFFER_WIDTH, _DEPTH_BUFFER_HEIGHT);
        m_pDepthRenderCameras[i]->CreateShaderVariables(pd3dDevice, pd3dCommandList);
    }

    CreateShaderVariable(pd3dDevice, pd3dCommandList);
}

void CDepthRenderShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CDepthRenderShaderComponent::PreRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
    for (int j = 0; j < MAX_LIGHT; j++)
    {
        LightData stLightData = m_pLights->GetLightData(j);
        if (stLightData.m_bEnable)
        {
            XMFLOAT3 xmf3Position = stLightData.m_xmf3Position;
            XMFLOAT3 xmf3Look = stLightData.m_xmf3Direction;
            XMFLOAT3 xmf3Up = XMFLOAT3(0.0f, +1.0f, 0.0f);

            XMMATRIX xmmtxLightView = XMMatrixLookToLH(XMLoadFloat3(&xmf3Position), XMLoadFloat3(&xmf3Look), XMLoadFloat3(&xmf3Up));

            float fNearPlaneDistance = 10.0f, fFarPlaneDistance = stLightData.m_fRange;

            XMMATRIX xmmtxProjection;
            if (stLightData.m_nType == DIRECTIONAL_LIGHT)
            {
                float fWidth = _PLANE_WIDTH, fHeight = _PLANE_HEIGHT;
                fNearPlaneDistance = 0.f, fFarPlaneDistance = 1500.0f;
                xmmtxProjection = XMMatrixOrthographicLH(fWidth, fHeight, fNearPlaneDistance, fFarPlaneDistance);
            }
            else if (stLightData.m_nType == SPOT_LIGHT)
            {
                float fFovAngle = 60.0f; // m_pLights->m_pLights[j].m_fPhi = cos(60.0f);
                float fAspectRatio = float(_DEPTH_BUFFER_WIDTH) / float(_DEPTH_BUFFER_HEIGHT);
                xmmtxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fFovAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
            }
            else if (stLightData.m_nType == POINT_LIGHT)
            {
                //ShadowMap[6]
            }

            m_pDepthRenderCameras[j]->SetPosition(xmf3Position);
            XMStoreFloat4x4(&m_pDepthRenderCameras[j]->m_xmf4x4View, xmmtxLightView);
            XMStoreFloat4x4(&m_pDepthRenderCameras[j]->m_xmf4x4Projection, xmmtxProjection);

            XMMATRIX xmmtxToTexture = XMMatrixTranspose(xmmtxLightView * xmmtxProjection * m_xmProjectionToTexture);
            XMStoreFloat4x4(&m_pToLightSpaces->m_pToLightSpaces[j].m_xmf4x4ToTexture, xmmtxToTexture);

            m_pToLightSpaces->m_pToLightSpaces[j].m_xmf4Position = XMFLOAT4(xmf3Position.x, xmf3Position.y, xmf3Position.z, 1.0f);

            ID3D12Resource* pd3dTextureResource = m_pDepthFromLightMaterialComponent->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(j).Get();
            ::SynchronizeResourceTransition(pd3dCommandList, pd3dTextureResource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);

            float pfClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            pd3dCommandList->ClearRenderTargetView(m_pd3dRtvCPUDescriptorHandles[j], pfClearColor, 0, NULL);
            pd3dCommandList->ClearDepthStencilView(m_d3dDsvDescriptorCPUHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);
            pd3dCommandList->OMSetRenderTargets(1, &m_pd3dRtvCPUDescriptorHandles[j], TRUE, &m_d3dDsvDescriptorCPUHandle);
            //m_pDepthRenderCameras[j]->RegenerateViewMatrix();
            Render(pd3dCommandList, m_pDepthRenderCameras[j].get());

            ::SynchronizeResourceTransition(pd3dCommandList, pd3dTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);
        }
        else
        {
            m_pToLightSpaces->m_pToLightSpaces[j].m_xmf4Position.w = 0.0f;
        }
    }
}

void CDepthRenderShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CDepthRenderShaderComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
    PrepareRender(pd3dCommandList);

    pCamera->SetViewportsAndScissorRects(pd3dCommandList);
    pCamera->UpdateShaderVariables(pd3dCommandList);

    UpdateShaderVariable(pd3dCommandList);
     for (int i = 0; i < m_vGameObjects.size(); i++)
    {
        m_vGameObjects[i]->Render(pd3dCommandList, pCamera, NULL);
    }
}

D3D12_SHADER_BYTECODE CDepthRenderShaderComponent::CreateVertexShader()
{
    D3DReadFileToBlob(L"ObjectVertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

    return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CDepthRenderShaderComponent::CreatePixelShader()
{
    D3DReadFileToBlob(L"DepthWritePixelShader.cso", m_pd3dPixelShaderBlob.GetAddressOf());

    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

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

void CDepthRenderShaderComponent::CreateShaderVariable(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
    UINT ncbDepthElementBytes;

    ncbDepthElementBytes = ((sizeof(TOLIGHTSPACES) + 255) & ~255); //256ÀÇ ¹è¼ö
    m_pd3dcbToLightSpaces = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbDepthElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

    m_pd3dcbToLightSpaces->Map(0, NULL, (void**)&m_pcbMappedToLightSpaces);
}

void CDepthRenderShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList)
{
    ::memcpy(m_pcbMappedToLightSpaces, m_pToLightSpaces.get(), sizeof(TOLIGHTSPACES));

    D3D12_GPU_VIRTUAL_ADDRESS d3dcbToLightGpuVirtualAddress = m_pd3dcbToLightSpaces->GetGPUVirtualAddress();
    pd3dCommandList->SetGraphicsRootConstantBufferView(12, d3dcbToLightGpuVirtualAddress); //ToLight
}

D3D12_RASTERIZER_DESC CDepthRenderShaderComponent::CreateRasterizerState()
{
    D3D12_RASTERIZER_DESC d3dRasterizerDesc;
    ::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
    d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    d3dRasterizerDesc.FrontCounterClockwise = FALSE;
#ifdef _WITH_RASTERIZER_DEPTH_BIAS
    d3dRasterizerDesc.DepthBias = 100000;
#endif
    d3dRasterizerDesc.DepthBiasClamp = 0.f;
    d3dRasterizerDesc.SlopeScaledDepthBias = 1.f;
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
    d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //D3D12_COMPARISON_FUNC_LESS_EQUAL
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

std::shared_ptr<CMaterialsComponent>& CDepthRenderShaderComponent::GetDepthFromLightMaterialComponent()
{
    return m_pDepthFromLightMaterialComponent;
}

void CDepthRenderShaderComponent::AddGameObject(std::shared_ptr<CGameObject> pGameObject)
{
    m_vGameObjects.push_back(pGameObject);
}
