#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
    // ��Ʈ �ñ״��� ����
    D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
    ZeroMemory(&d3dRootSignatureDesc, sizeof(d3dRootSignatureDesc));
    d3dRootSignatureDesc.NumParameters = 0;
    d3dRootSignatureDesc.pParameters = NULL;
    d3dRootSignatureDesc.NumStaticSamplers = 0;
    d3dRootSignatureDesc.pStaticSamplers = NULL;
    d3dRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> pd3dSignatureBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> pd3dErrorBlob;
    D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, pd3dSignatureBlob.GetAddressOf(), pd3dErrorBlob.GetAddressOf());
    pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)m_pd3dGraphicsRootsignature.GetAddressOf());

}

void CScene::CreateGraphicsPipelineState(ID3D12Device* pd3dDevice)
{
    // ���� ���̴�, �ȼ� ���̴� ����
    Microsoft::WRL::ComPtr<ID3DBlob> pd3dVertexShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> pd3dPixelShaderBlob;

    D3DReadFileToBlob(L"VertexShader.cso", pd3dVertexShaderBlob.GetAddressOf());
    D3DReadFileToBlob(L"PixelShader.cso", pd3dPixelShaderBlob.GetAddressOf());

    // �����Ͷ����� ���� ����
    D3D12_RASTERIZER_DESC d3dRasterizerDesc;
    ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
    d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    d3dRasterizerDesc.FrontCounterClockwise = FALSE;
    d3dRasterizerDesc.DepthBias = 0;
    d3dRasterizerDesc.DepthBiasClamp = 0.f;
    d3dRasterizerDesc.SlopeScaledDepthBias = 0.f;
    d3dRasterizerDesc.DepthClipEnable = TRUE;
    d3dRasterizerDesc.MultisampleEnable = FALSE;
    d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
    d3dRasterizerDesc.ForcedSampleCount = 0;
    d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // ���� ���� ����
    D3D12_BLEND_DESC d3dBlendDesc;
    ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
    d3dBlendDesc.AlphaToCoverageEnable = FALSE;
    d3dBlendDesc.IndependentBlendEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
    d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
    d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // �׷��� ���������� ���� ����
    D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
    ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    d3dPipelineStateDesc.pRootSignature = m_pd3dGraphicsRootsignature.Get();
    d3dPipelineStateDesc.VS.pShaderBytecode = pd3dVertexShaderBlob->GetBufferPointer();
    d3dPipelineStateDesc.VS.BytecodeLength = pd3dVertexShaderBlob->GetBufferSize();
    d3dPipelineStateDesc.PS.pShaderBytecode = pd3dPixelShaderBlob->GetBufferPointer();
    d3dPipelineStateDesc.PS.BytecodeLength = pd3dPixelShaderBlob->GetBufferSize();
    d3dPipelineStateDesc.RasterizerState = d3dRasterizerDesc;
    d3dPipelineStateDesc.BlendState = d3dBlendDesc;
    d3dPipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
    d3dPipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
    d3dPipelineStateDesc.InputLayout.pInputElementDescs = NULL;
    d3dPipelineStateDesc.InputLayout.NumElements = 0;
    d3dPipelineStateDesc.SampleMask = UINT_MAX;
    d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    d3dPipelineStateDesc.NumRenderTargets = 1;
    d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    d3dPipelineStateDesc.SampleDesc.Count = 1;
    d3dPipelineStateDesc.SampleDesc.Quality = 0;
    pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)m_pd3dPipelineState.GetAddressOf());
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice)
{
    CreateGraphicsRootSignature(pd3dDevice);
    CreateGraphicsPipelineState(pd3dDevice);
}

bool CScene::ProcessInput()
{
    return false;
}

void CScene::AnimateObjects(float fTimeElapsed)
{
}

void CScene::UpdateObjects(float fTimeElapsed)
{
}

void CScene::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
    // �׷��� ��Ʈ �ñ״��ĸ� ����
    pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootsignature.Get());

    // ���������� ���� ����
    pd3dCommandList->SetPipelineState(m_pd3dPipelineState.Get());

    // ������Ƽ�� ��������(�ﰢ�� ����Ʈ) ����
    pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
    PrepareRender(pd3dCommandList);

    pd3dCommandList->DrawInstanced(3, 1, 0, 0);
}
