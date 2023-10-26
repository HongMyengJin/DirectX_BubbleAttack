#include "Stage.h"
#include "ObjectShaderComponent.h"
#include "TerrainObjectShaderComponent.h"
#include "TerrainWaterShaderComponent.h"
#include "SkyBoxShaderComponent.h"
#include "TextureRectObject.h"
#include "TextureRectMeshShaderComponent.h"
#include "Camera.h"


CStage::CStage()
{
}

CStage::~CStage()
{

}

void CStage::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
    // 루트 시그니쳐 생성

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[1];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 7;
	pd3dDescriptorRanges[0].BaseShaderRegister = 6; //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER pd3dRootParameters[4];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 33;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[3].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[0]);
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_STATIC_SAMPLER_DESC pd3dSamplerDescs[2];

	pd3dSamplerDescs[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pd3dSamplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].MipLODBias = 0;
	pd3dSamplerDescs[0].MaxAnisotropy = 1;
	pd3dSamplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pd3dSamplerDescs[0].MinLOD = 0;
	pd3dSamplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[0].ShaderRegister = 0;
	pd3dSamplerDescs[0].RegisterSpace = 0;
	pd3dSamplerDescs[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dSamplerDescs[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pd3dSamplerDescs[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].MipLODBias = 0;
	pd3dSamplerDescs[1].MaxAnisotropy = 1;
	pd3dSamplerDescs[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pd3dSamplerDescs[1].MinLOD = 0;
	pd3dSamplerDescs[1].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[1].ShaderRegister = 1;
	pd3dSamplerDescs[1].RegisterSpace = 0;
	pd3dSamplerDescs[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(pd3dSamplerDescs);
	d3dRootSignatureDesc.pStaticSamplers = pd3dSamplerDescs;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	Microsoft::WRL::ComPtr<ID3DBlob> pd3dSignatureBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> pd3dErrorBlob;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, pd3dSignatureBlob.GetAddressOf(), pd3dErrorBlob.GetAddressOf());
	HRESULT hResult = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)m_pd3dGraphicsRootsignature.GetAddressOf());

}


void CStage::CreateGraphicsPipelineState(ID3D12Device* pd3dDevice)
{
    // 정점 쉐이더, 픽셀 쉐이더 생성
    Microsoft::WRL::ComPtr<ID3DBlob> pd3dVertexShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> pd3dPixelShaderBlob;

    D3DReadFileToBlob(L"VertexShader.cso", pd3dVertexShaderBlob.GetAddressOf());
    D3DReadFileToBlob(L"PixelShader.cso", pd3dPixelShaderBlob.GetAddressOf());

    // 레스터라이저 상태 설정
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

    // 블렌드 상태 설정
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

    // 그래픽 파이프라인 상태 설정
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

void CStage::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
    CreateGraphicsRootSignature(pd3dDevice);

	m_pd3dDescriptorHeap = std::make_unique<CDescriptorHeap>();
	m_pd3dDescriptorHeap->CreateCbcSrvDescriptorHeap(pd3dDevice, 0, 100);

	m_pCamera = std::make_unique<CCamera>();
	m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	std::shared_ptr<CObjectShaderComponent> pObjectShaderComponent = std::make_shared<CObjectShaderComponent>();
	pObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());
	/*m_pGameObject = std::make_unique<CGameObject>();
	m_pGameObject->Init();

	m_pGameObject->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
	m_pGameObject->AddShaderComponent(pObjectShaderComponent);
	m_pGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/Weapon_PlasmaRain.bin");*/

	m_pLightObject = std::make_unique<CLight>();
	m_pLightObject->Init();
	m_pLightObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	XMFLOAT3 xmf3Scale(8.0f, 2.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);

	std::shared_ptr<CTerrainObjectShaderComponent> pTerrainShaderComponent = std::make_shared<CTerrainObjectShaderComponent>();
	pTerrainShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	m_pTerrain = std::make_unique<CTerrainObject>();
	m_pTerrain->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), _T("Image/HeightMap.raw"), m_pd3dDescriptorHeap.get(), 257, 257, 257, 257, xmf3Scale, xmf4Color);
	m_pTerrain->AddShaderComponent(pTerrainShaderComponent);
	m_pTerrain->SetPosition(XMFLOAT3(0.f, -250.f, 0.f));

	std::shared_ptr<CTerrainWaterShaderComponent> pTerrainWaterShaderComponent = std::make_shared<CTerrainWaterShaderComponent>();
	pTerrainWaterShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	m_pTerrainWater = std::make_unique<CTerrainWater>();
	m_pTerrainWater->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), 257 * xmf3Scale.x, 257 * xmf3Scale.z);
	m_pTerrainWater->AddShaderComponent(pTerrainWaterShaderComponent);
	m_pTerrainWater->SetPosition(XMFLOAT3(0.f, 155.0f - 250.f, 0.f));
	//m_pTerrainWater->SetPosition(XMFLOAT3((257 * xmf3Scale.x * 0.5f), 155.0f - 250.f, +(257 * xmf3Scale.z * 0.5f)));

	std::shared_ptr<CSkyBoxShaderComponent> pSkyBoxShaderComponent = std::make_shared<CSkyBoxShaderComponent>();
	pSkyBoxShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	m_pSkyBoxObject = std::make_unique<CSkyBoxObject>();
	m_pSkyBoxObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get());
	m_pSkyBoxObject->AddShaderComponent(pSkyBoxShaderComponent);

	std::shared_ptr<CTextureRectMeshShaderComponent> pTextureRectMeshShaderComponent = std::make_shared<CTextureRectMeshShaderComponent>();
	pTextureRectMeshShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

	for (int i = 0; i < 30; i++)
	{
		std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

		pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f));
		pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
		pTextureRectObject->SetPosition(XMFLOAT3(i * 20.f - 30 * 0.5f * 20.f - 100.f, 0.f, i * 20.f - 30 * 0.5f * 20.f));
		pTextureRectObject->SetAnimateLifeTime(0.5f + 0.1f * i);
		m_pTextureRectObjects.push_back(pTextureRectObject);
	}

	
}

bool CStage::ProcessInput()
{
    return false;
}

void CStage::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < m_pTextureRectObjects.size(); i++)
	{
		if (m_pTextureRectObjects[i])
		{
			m_pTextureRectObjects[i]->Animate(fTimeElapsed);
		}
	}
}

void CStage::UpdateObjects(float fTimeElapsed)
{
	if (m_pCamera)
		m_pCamera->Update(fTimeElapsed);
	if (m_pGameObject)
	{
		m_pGameObject->SetPosition(XMFLOAT3(0.f, 0.f, 20.f));
		m_pGameObject->Update(fTimeElapsed, nullptr);
	}
}

void CStage::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	////////// 삼각형 그리기 //////////
	// 
    //// 그래픽 루트 시그니쳐를 설정
    pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootsignature.Get());
	pd3dCommandList->SetDescriptorHeaps(1, m_pd3dDescriptorHeap->m_pd3dCbvSrvDescriptorHeap.GetAddressOf());

	if (m_pCamera)
		m_pCamera->UpdateShaderVariables(pd3dCommandList);
	if (m_pLightObject)
		m_pLightObject->UpdateShaderVariables(pd3dCommandList);


}

void CStage::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
    PrepareRender(pd3dCommandList);

	if (m_pSkyBoxObject)
		m_pSkyBoxObject->PrepareRender(pd3dCommandList);

	if (m_pSkyBoxObject)
		m_pSkyBoxObject->Render(pd3dCommandList, m_pCamera.get(), nullptr);

	if (m_pGameObject)
		m_pGameObject->PrepareRender(pd3dCommandList);
	if (m_pGameObject)
		m_pGameObject->Render(pd3dCommandList, m_pCamera.get(), nullptr);

	if (m_pTerrain)
		m_pTerrain->PrepareRender(pd3dCommandList);

	if(m_pTerrain)
		m_pTerrain->Render(pd3dCommandList, m_pCamera.get(), nullptr);

	if (m_pTerrainWater)
		m_pTerrainWater->PrepareRender(pd3dCommandList);

	if (m_pTerrainWater)
		m_pTerrainWater->Render(pd3dCommandList, m_pCamera.get(), nullptr);

	for (int i = 0; i < m_pTextureRectObjects.size(); i++)
	{
		if (m_pTextureRectObjects[i])
		{
			m_pTextureRectObjects[i]->PrepareRender(pd3dCommandList);
			m_pTextureRectObjects[i]->Render(pd3dCommandList, m_pCamera.get(), nullptr);
		}
	}
	
}

void CStage::Release()
{
	if(m_pCamera)
		m_pCamera->Release();
	if (m_pLightObject)
		m_pLightObject->Release();
}
