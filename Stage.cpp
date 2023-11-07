#include "Stage.h"
#include "RippleWaterObjectShaderComponent.h"
#include "TextureRectMeshShaderComponent.h"
#include "ParticleObjectShaderComponent.h"
#include "TerrainObjectShaderComponent.h"
#include "SkyBoxShaderComponent.h"
#include "ObjectShaderComponent.h"

#include "TextureRectObject.h"
#include "UIObjectShaderComponent.h"
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

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[2];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 7;
	pd3dDescriptorRanges[0].BaseShaderRegister = 6; //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 13; //t13: Buffer
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER pd3dRootParameters[7];

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

	pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[4].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[1]);
	pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[5].Descriptor.ShaderRegister = 12; //Framework Info
	pd3dRootParameters[5].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[6].Constants.Num32BitValues = 16; //Texture Animation (4x4) Matrix
	pd3dRootParameters[6].Constants.ShaderRegister = 5; //
	pd3dRootParameters[6].Constants.RegisterSpace = 0;
	pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


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

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
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

	m_pCamera = std::make_unique<CThirdPersonCamera>();
	m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	std::shared_ptr<CObjectShaderComponent> pObjectShaderComponent = std::make_shared<CObjectShaderComponent>();
	pObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());
	m_pPlayersGameObject = std::make_unique<CPlayerGameObject>();
	m_pPlayersGameObject->Init(XMFLOAT3(12.f, 10.f, 12.f));

	m_pPlayersGameObject->AddShaderComponent(pObjectShaderComponent);
	m_pPlayersGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/Player.bin");
	m_pPlayersGameObject->LoadPlayerFrameData();
	m_pPlayersGameObject->SetPosition(XMFLOAT3(300.f, 24.f, 300.f));


	std::shared_ptr<CTextureRectMeshShaderComponent> pTextureRectMeshShaderComponent = std::make_shared<CTextureRectMeshShaderComponent>();
	pTextureRectMeshShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

	for (int i = 0; i < 3; i++)
	{
		std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

		pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(15.f, 15.f), L"Image/Explode_8x8.dds");
		pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
		pTextureRectObject->SetAnimateLifeTime(1.f);
		pTextureRectObject->SetLoop(true);
		m_pTextureRectObjects.push_back(pTextureRectObject);
	}

	for (int i = 0; i < 3; i++)
	{
		std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

		pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(80.f, 80.f), L"Image/Explode_8x8.dds");
		pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
		pTextureRectObject->SetAnimateLifeTime(1.f);
		pTextureRectObject->SetLoop(false);
		pTextureRectObject->SetEnable(false);
		m_pEffectRectObjects.push_back(pTextureRectObject);
	}





	std::shared_ptr<CBombGameObject> pBombGameObject = std::make_shared<CBombGameObject>();
	pBombGameObject->Init(XMFLOAT3(20.f, 20.f, 20.f));
	pBombGameObject->AddShaderComponent(pObjectShaderComponent);
	pBombGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/PlayerBullet.bin");
	pBombGameObject->SetSpriteEffect(m_pTextureRectObjects[0], XMFLOAT3(45.f, 8.f, -12.f));
	m_pPlayersGameObject->LoadPlayerBombObject(pBombGameObject);
	
	for (int i = 0; i < 2; i++)
	{
		pBombGameObject = std::make_shared<CBombGameObject>();
		pBombGameObject->Init(XMFLOAT3(20.f, 20.f, 20.f));
		pBombGameObject->AddShaderComponent(pObjectShaderComponent);
		pBombGameObject->SetSpriteEffect(m_pTextureRectObjects[1 + i], XMFLOAT3(45.f, 8.f, -12.f));
		pBombGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/PlayerBullet.bin");

		std::shared_ptr<CMonsterGameObject> pMonsterMesh1 = std::make_shared<CMonsterGameObject>();
		pMonsterMesh1->Init(XMFLOAT3(0.f, 0.f, 0.f));
		pMonsterMesh1->AddShaderComponent(pObjectShaderComponent);
		pMonsterMesh1->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/SlimMonster.bin");

		std::shared_ptr<CMonsterGameObject> pMonsterGameObject = std::make_shared<CMonsterGameObject>();
		pMonsterGameObject->SetChild(NULL, pMonsterMesh1);
		pMonsterGameObject->Init(XMFLOAT3(13.f, 13.f, 13.f));
		pMonsterGameObject->AddShaderComponent(pObjectShaderComponent);
		pMonsterGameObject->SetPosition(XMFLOAT3(120.f + i * 65.f, 24.f, 200.f));
		pMonsterGameObject->LoadMonsterBombObject(pBombGameObject);
		//pMonsterGameObject->Rotate(0.f, -90.f, 0.f);
		m_pMonsterObjects.push_back(pMonsterGameObject);
	}
	
	//CGameObject* pObject = m_pGameObject->FindFrame("bobomb_Skeleton_8");

	//pObject->SetPosition(XMFLOAT3(0.f, 1000.f, 0.f));
	m_pLightObject = std::make_unique<CLight>();
	m_pLightObject->Init();
	m_pLightObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	XMFLOAT3 xmf3TerrainScale(8.0f, 2.0f, 8.0f);
	XMFLOAT3 xmf3WaterTerrainScale(10.f, 2.0f, 10.f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);

	std::shared_ptr<CTerrainObjectShaderComponent> pTerrainShaderComponent = std::make_shared<CTerrainObjectShaderComponent>();
	pTerrainShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	m_pTerrain = std::make_shared<CTerrainObject>();
	m_pTerrain->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), _T("Image/HeightMap.raw"), m_pd3dDescriptorHeap.get(), 257, 257, 17, 17, xmf3TerrainScale, xmf4Color);
	m_pTerrain->AddShaderComponent(pTerrainShaderComponent);
	m_pTerrain->SetPosition(XMFLOAT3(0.f, -200.f, 0.f));

	std::shared_ptr<CRippleWaterObjectShaderComponent> pTerrainWaterShaderComponent = std::make_shared<CRippleWaterObjectShaderComponent>();
	pTerrainWaterShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	m_pTerrainWater = std::make_unique<CRippleWaterObject>();
	m_pTerrainWater->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), 257, 257, 100, 100, xmf3WaterTerrainScale, xmf4Color);
	m_pTerrainWater->AddShaderComponent(pTerrainWaterShaderComponent);
	m_pTerrainWater->SetPosition(XMFLOAT3(-120.f + 0.f +(257 * 0.5f), 0.f, -80.f + (257 * 0.5f)));
	//m_pTerrainWater->SetPosition(XMFLOAT3((257 * xmf3TerrainScale.x * 0.5f), 155.0f - 250.f, +(257 * xmf3TerrainScale.z * 0.5f)));

	for (int i = 0; i < 5; i++)
	{
		XMFLOAT3 xmf3Position = XMFLOAT3(50.f + i * 80.f, 0.f, 680.f + -i * i * 40.f);
		std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

		pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(45.f, 45.f), L"Image/Tree01.dds");
		pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
		pTextureRectObject->SetAnimateLifeTime(1.f);
		pTextureRectObject->SetLoop(true);
		pTextureRectObject->SetPosition(XMFLOAT3(xmf3Position.x, m_pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) - 200.f + 22.f, xmf3Position.z));
		m_pTreesRectObjects.push_back(pTextureRectObject);
	}


	for (int i = 0; i < 5; i++)
	{
		XMFLOAT3 xmf3Position = XMFLOAT3(1200.f + i * 80.f, 0.f, 680.f + -i * i * 40.f);
		std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

		pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(45.f, 45.f), L"Image/Tree02.dds");
		pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
		pTextureRectObject->SetAnimateLifeTime(1.f);
		pTextureRectObject->SetLoop(true);
		pTextureRectObject->SetPosition(XMFLOAT3(xmf3Position.x, m_pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) - 200.f + 22.f, xmf3Position.z));
		m_pTreesRectObjects.push_back(pTextureRectObject);
	}

	std::shared_ptr<CSkyBoxShaderComponent> pSkyBoxShaderComponent = std::make_shared<CSkyBoxShaderComponent>();
	pSkyBoxShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	m_pSkyBoxObject = std::make_unique<CSkyBoxObject>();
	m_pSkyBoxObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get());
	m_pSkyBoxObject->AddShaderComponent(pSkyBoxShaderComponent);

	std::shared_ptr<CUIObjectShaderComponent> pUIShaderComponent = std::make_shared<CUIObjectShaderComponent>();
	pUIShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	for (int i = 0; i < 1; i++)
	{
		std::shared_ptr<CUIGameObject> pUIObject = std::make_shared<CUIGameObject>();

		pUIObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f), L"Image/ScoreFrame.dds");
		pUIObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.9f));
		pUIObject->SetSize(XMFLOAT2(500.f, 100.f));
		pUIObject->AddShaderComponent(pUIShaderComponent);
		m_pUIObjects.push_back(pUIObject);
	}
	for (int i = 0; i < 2; i++)
	{
		std::shared_ptr<CUINumberGameObject> pUIObject = std::make_shared<CUINumberGameObject>();

		pUIObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f), L"Image/NumberUI.dds");
		pUIObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.42f + FRAME_BUFFER_WIDTH * 0.13f * i, FRAME_BUFFER_HEIGHT * 0.9f));
		pUIObject->SetSize(XMFLOAT2(50.f, 50.f));
		pUIObject->AddShaderComponent(pUIShaderComponent);
		m_pUINumberObjects.push_back(pUIObject);
	}

	std::shared_ptr<CUIGameObject> pUIObject = std::make_shared<CUIGameObject>();

	pUIObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f), L"Image/Win.dds");
	pUIObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.9f));
	pUIObject->SetSize(XMFLOAT2(180.f, 180.f));
	pUIObject->AddShaderComponent(pUIShaderComponent);
	pUIObject->SetEnable(false);
	m_pUIObjects.push_back(pUIObject);

	pUIObject = std::make_shared<CUIGameObject>();

	pUIObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f), L"Image/Lose.dds");
	pUIObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.9f));
	pUIObject->SetSize(XMFLOAT2(180.f, 180.f));
	pUIObject->AddShaderComponent(pUIShaderComponent);
	pUIObject->SetEnable(false);
	m_pUIObjects.push_back(pUIObject);




	//std::shared_ptr<CParticleObjectShaderComponent> pParticleObjectShaderComponent = std::make_shared<CParticleObjectShaderComponent>();
	//pParticleObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

	//m_pParticleObject = std::make_unique<CParticleObject>();
	//m_pParticleObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 65.0f, 0.0f), 0.0f, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(8.0f, 8.0f), MAX_PARTICLES);
	//m_pParticleObject->AddShaderComponent(pParticleObjectShaderComponent);
}

bool CStage::ProcessInput(HWND hWnd)
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	if (GetKeyboardState(pKeysBuffer))
	{
		DWORD dwDirection = 0;
		if (pKeysBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeysBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeysBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeysBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeysBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeysBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;

		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		if (GetCapture() == hWnd)
		{
			SetCursor(NULL);
			GetCursorPos(&ptCursorPos);
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}

		if (pKeysBuffer[VK_SPACE] & 0xF0) // 점프
		{
			// Terrain 높이 체크
			m_pPlayersGameObject->MoveBomb(0.001f, m_pPlayersGameObject->GetLookVector());
		}
		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				if (pKeysBuffer[VK_RBUTTON] & 0xF0)
					m_pPlayersGameObject->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_pPlayersGameObject->Rotate(cyDelta, cxDelta, 0.0f);
			}
			if (dwDirection)
			{
				m_pPlayersGameObject->Move(dwDirection, 1.5f);
				m_pPlayersGameObject->SetboolMove(true);
			}
			else
			{
				m_pPlayersGameObject->SetboolMove(false);
			}
		}
		return true;
	}

	return false;
}

void CStage::AnimateObjects(float fTimeElapsed)
{

	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		if (m_pMonsterObjects[i])
		{
			float xmfOffsetY = 10.f;
			XMFLOAT3 xmfPosition = m_pMonsterObjects[i]->GetPosition();
			m_pMonsterObjects[i]->SetPosition(XMFLOAT3(xmfPosition.x, m_pTerrain->GetHeight(xmfPosition.x, xmfPosition.z) - 200.f, xmfPosition.z));
			m_pMonsterObjects[i]->Update(fTimeElapsed, m_pPlayersGameObject->GetPosition(), m_pTerrain);
		}
	}
}

void CStage::UpdateObjects(float fTimeElapsed)
{
	if (m_pCamera)
		m_pCamera->Update(m_pPlayersGameObject.get(), m_pPlayersGameObject->GetPosition(), fTimeElapsed);
	

	if (m_pLightObject)
	{
		m_pLightObject->SetPosition(1, m_pPlayersGameObject->GetPosition());
		m_pLightObject->SetOffsetPosition(1, XMFLOAT3(0.f, 150.f, 0.f));
		m_pLightObject->Update(fTimeElapsed, NULL);
	}
	if (m_pPlayersGameObject)
	{
		m_pPlayersGameObject->UpdateFrame(fTimeElapsed);
		m_pPlayersGameObject->Update(fTimeElapsed, nullptr, m_pTerrain);
	}
	for (int i = 0; i < m_pUINumberObjects.size(); i++)
	{
		m_pUINumberObjects[i]->Animate(fTimeElapsed);
	}
	
	for (int i = 0; i < m_pUIObjects.size(); i++)
	{
		m_pUIObjects[i]->Animate(fTimeElapsed);
	}

	for (int i = 0; i < m_pEffectRectObjects.size(); i++)
	{
		m_pEffectRectObjects[i]->AnimateUV(fTimeElapsed);
	}
	CollisionCheck(); // 충돌 체크
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

	if (m_pTerrainWater)
		m_pTerrainWater->UpdateShaderVariables(pd3dCommandList);

}

void CStage::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
    PrepareRender(pd3dCommandList);

	if (m_pSkyBoxObject)
	{
		m_pSkyBoxObject->SetPosition(m_pCamera->GetPosition());
		m_pSkyBoxObject->PrepareRender(pd3dCommandList);
		m_pSkyBoxObject->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}

	if (m_pPlayersGameObject)
	{
		float xmfOffsetY = 10.f;
		XMFLOAT3 xmfPosition = m_pPlayersGameObject->GetPosition();
		m_pPlayersGameObject->SetPosition(XMFLOAT3(xmfPosition.x, m_pTerrain->GetHeight(xmfPosition.x, xmfPosition.z) - 200.f, xmfPosition.z));
		m_pPlayersGameObject->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}


	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		if (m_pMonsterObjects[i])
		{
			m_pMonsterObjects[i]->PrepareRender(pd3dCommandList);
			m_pMonsterObjects[i]->Render(pd3dCommandList, m_pCamera.get(), nullptr);
		}
	}
	

	if (m_pTerrain)
	{
		m_pTerrain->PrepareRender(pd3dCommandList);
		m_pTerrain->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}

	if (m_pTerrainWater)
	{
		m_pTerrainWater->PrepareRender(pd3dCommandList);
		m_pTerrainWater->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}

	for (int i = 0; i < m_pEffectRectObjects.size(); i++)
	{
		m_pEffectRectObjects[i]->PrepareRender(pd3dCommandList);
		m_pEffectRectObjects[i]->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}

	for (int i = 0; i < m_pTreesRectObjects.size(); i++)
	{
		m_pTreesRectObjects[i]->PrepareRender(pd3dCommandList);
		m_pTreesRectObjects[i]->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}


	for (int i = 0; i < m_pUIObjects.size(); i++)
	{
		m_pUIObjects[i]->PrepareRender(pd3dCommandList);
		m_pUIObjects[i]->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}


	for (int i = 0; i < m_pUINumberObjects.size(); i++)
	{
		m_pUINumberObjects[i]->PrepareRender(pd3dCommandList);
		m_pUINumberObjects[i]->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}

}

void CStage::RenderParticle(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CStage::OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CStage::CollisionCheck()
{
	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		m_pMonsterObjects[i]->SetCollision(false);
		m_pMonsterObjects[i]->GetBombGameObject()->SetCollision(false);
	}
	m_pPlayersGameObject->SetCollision(false);
	m_pPlayersGameObject->GetBombGameObject()->SetCollision(false);


	if (m_pMonsterObjects[0]->CollisionCheck(m_pMonsterObjects[1])
		
		)
	{
		if (!(m_pMonsterObjects[0]->GetCollision() || m_pMonsterObjects[1]->GetCollision())) // 둘 다 충돌이 없었음
		{
			m_pMonsterObjects[0]->SetCollision(true);
			m_pMonsterObjects[0]->SetPosition(m_pMonsterObjects[0]->GetPrePosition());
		}


		
	}

	// Player - Monster
	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		if (m_pPlayersGameObject->CollisionCheck(m_pMonsterObjects[i])) // 충돌
		{
			// 플레이어, 몬스터 충돌 처리
			m_pPlayersGameObject->SetCollision(true);
			m_pMonsterObjects[i]->SetCollision(true);

			m_pPlayersGameObject->SetPosition(m_pPlayersGameObject->GetPrePosition());

		}
	}

	// 플레이어 - 불렛 충돌
	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		if (m_pMonsterObjects[i]->GetBombGameObject()->GetBombState() == BombSTATE::BombBurn && 
			m_pPlayersGameObject->CollisionCheck(m_pMonsterObjects[i]->GetBombGameObject())) // 충돌
		{
			// 플레이어 - 불렛 충돌 처리
			m_pPlayersGameObject->SetCollision(true);
			m_pMonsterObjects[i]->GetBombGameObject()->SetCollision(true);
			for (int j = 0; j < m_pEffectRectObjects.size(); j++)
			{
				if (!m_pEffectRectObjects[j]->GetEnable())
				{
					m_pMonsterObjects[i]->GetBombGameObject()->SetBombState(BombSTATE::BombWait);
					m_pEffectRectObjects[j]->SetEnable(true);
					XMFLOAT3 xmf3Positions = m_pPlayersGameObject->GetPosition();
					xmf3Positions.y += 15.f;
					m_pEffectRectObjects[j]->SetWorldPosition(xmf3Positions);
					if(!m_bResult)
						m_iMonsterAttack++;
					break;
				}
			}
		}
	}

	// 몬스터 - 불렛 충돌
	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		if (m_pPlayersGameObject->GetBombGameObject()->GetBombState() == BombSTATE::BombBurn &&
			m_pMonsterObjects[i]->CollisionCheck(m_pPlayersGameObject->GetBombGameObject())) // 충돌
		{
			// 몬스터 - 플레이어 불렛 충돌

			m_pMonsterObjects[i]->SetCollision(true);
			m_pPlayersGameObject->GetBombGameObject()->SetCollision(true);
			
			for (int j = 0; j < m_pEffectRectObjects.size(); j++)
			{
				if (!m_pEffectRectObjects[j]->GetEnable())
				{
					m_pPlayersGameObject->GetBombGameObject()->SetBombState(BombSTATE::BombWait);
					m_pEffectRectObjects[j]->SetEnable(true);
					XMFLOAT3 xmf3Positions = m_pMonsterObjects[i]->GetPosition();
					xmf3Positions.y += 15.f;
					m_pEffectRectObjects[j]->SetWorldPosition(xmf3Positions);
					if (!m_bResult)
						m_iPlayerAttack++;
					break;
				}
			}
		}
	}
	if (!m_bResult)
	{
		if (m_iPlayerAttack >= 10)
		{
			m_pUIObjects[1]->SetEnable(true);
			m_bResult = true;
		}
		else if (m_iMonsterAttack >= 10)
		{
			m_pUIObjects[2]->SetEnable(true);
			m_bResult = true;
		}
	}
	m_pUINumberObjects[0]->UpdateNumber(m_iPlayerAttack);
	m_pUINumberObjects[1]->UpdateNumber(m_iMonsterAttack);
}

void CStage::Release()
{
	if(m_pCamera)
		m_pCamera->Release();
	if (m_pLightObject)
		m_pLightObject->Release();
}
