#include "Stage.h"
#include "RippleWaterObjectShaderComponent.h"
#include "TextureRectMeshShaderComponent.h"
#include "ParticleObjectShaderComponent.h"
#include "TerrainObjectShaderComponent.h"
#include "SkyBoxShaderComponent.h"
#include "ObjectShaderComponent.h"

#include "TextureRectObject.h"






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
	m_pPlayersGameObject->Init();

	m_pPlayersGameObject->AddShaderComponent(pObjectShaderComponent);
	m_pPlayersGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/ThirdPersonMap.bin");
	m_pPlayersGameObject->LoadPlayerFrameData();
	m_pPlayersGameObject->SetScale(XMFLOAT3(1.f, 1.f, 1.f));
	m_pPlayersGameObject->SetPosition(XMFLOAT3(200.f, 24.f, 200.f));

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

	m_pTerrain = std::make_unique<CTerrainObject>();
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

	std::shared_ptr<CSkyBoxShaderComponent> pSkyBoxShaderComponent = std::make_shared<CSkyBoxShaderComponent>();
	pSkyBoxShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get());

	m_pSkyBoxObject = std::make_unique<CSkyBoxObject>();
	m_pSkyBoxObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get());
	m_pSkyBoxObject->AddShaderComponent(pSkyBoxShaderComponent);

	std::shared_ptr<CParticleObjectShaderComponent> pParticleObjectShaderComponent = std::make_shared<CParticleObjectShaderComponent>();
	pParticleObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

	//m_pParticleObject = std::make_unique<CParticleObject>();
	//m_pParticleObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 65.0f, 0.0f), 0.0f, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(8.0f, 8.0f), MAX_PARTICLES);
	//m_pParticleObject->AddShaderComponent(pParticleObjectShaderComponent);

	std::shared_ptr<CTextureRectMeshShaderComponent> pTextureRectMeshShaderComponent = std::make_shared<CTextureRectMeshShaderComponent>();
	pTextureRectMeshShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

	for (int i = 0; i < 10; i++)
	{
		std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

		pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f));
		pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
		pTextureRectObject->SetPosition(XMFLOAT3(i * 20.f - 30 * 0.5f * 20.f - 100.f, 0.f, i * 20.f - 30 * 0.5f * 20.f));
		pTextureRectObject->SetAnimateLifeTime(0.2f + 0.1f * i);
		m_pTextureRectObjects.push_back(pTextureRectObject);
	}


	
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

		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				if (pKeysBuffer[VK_RBUTTON] & 0xF0)
					m_pPlayersGameObject->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_pPlayersGameObject->Rotate(cyDelta, cxDelta, 0.0f);
			}
			if (dwDirection) m_pPlayersGameObject->Move(dwDirection, 0.7f);
		}
		return true;
	}

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
		m_pCamera->Update(m_pPlayersGameObject.get(), m_pPlayersGameObject->GetPosition(), fTimeElapsed);
	

	if (m_pLightObject)
	{
		m_pLightObject->SetPosition(1, m_pPlayersGameObject->GetPosition());
		m_pLightObject->SetOffsetPosition(1, XMFLOAT3(0.f, 100.f, 0.f));
		m_pLightObject->Update(fTimeElapsed, NULL);
	}
	if (m_pPlayersGameObject)
	{
		m_pPlayersGameObject->UpdateFrame(fTimeElapsed);
		m_pPlayersGameObject->Update(fTimeElapsed, nullptr);
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
	}
	if (m_pSkyBoxObject)
	{
		m_pSkyBoxObject->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}

	if (m_pPlayersGameObject)
		m_pPlayersGameObject->PrepareRender(pd3dCommandList);
	if (m_pPlayersGameObject)
	{
		float xmfOffsetY = 10.f;
		XMFLOAT3 xmfPosition = m_pPlayersGameObject->GetPosition();
		m_pPlayersGameObject->SetPosition(XMFLOAT3(xmfPosition.x, m_pTerrain->GetHeight(xmfPosition.x, xmfPosition.z) - 200.f, xmfPosition.z));
		m_pPlayersGameObject->Render(pd3dCommandList, m_pCamera.get(), nullptr);
	}

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
	RenderParticle(pd3dCommandList);
}

void CStage::RenderParticle(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pParticleObject)
		m_pParticleObject->Render(pd3dCommandList, m_pCamera.get(), NULL);
}

void CStage::OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pParticleObject)
		m_pParticleObject->OnPostRender();
}

void CStage::Release()
{
	if(m_pCamera)
		m_pCamera->Release();
	if (m_pLightObject)
		m_pLightObject->Release();
}
