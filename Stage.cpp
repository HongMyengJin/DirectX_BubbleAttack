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
#include "TerrainTessellationShaderComponent.h"

CStage::CStage()
{
}

CStage::~CStage()
{

}

void CStage::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
    // 루트 시그니쳐 생성

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[7];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 6; //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 7; //t7: gtxtAlbedoTexture
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 8; //t8: gtxtAlbedoTexture
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[3].NumDescriptors = 1;
	pd3dDescriptorRanges[3].BaseShaderRegister = 9; //t9: gtxtAlbedoTexture
	pd3dDescriptorRanges[3].RegisterSpace = 0;
	pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[4].NumDescriptors = 1;
	pd3dDescriptorRanges[4].BaseShaderRegister = 10; //t10: gtxtAlbedoTexture
	pd3dDescriptorRanges[4].RegisterSpace = 0;
	pd3dDescriptorRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[5].NumDescriptors = 1;
	pd3dDescriptorRanges[5].BaseShaderRegister = 13; //t13: Buffer
	pd3dDescriptorRanges[5].RegisterSpace = 0;
	pd3dDescriptorRanges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[6].NumDescriptors = MAX_DEPTH_TEXTURES;
	pd3dDescriptorRanges[6].BaseShaderRegister = 17; //t17: gtxtDepthTexture
	pd3dDescriptorRanges[6].RegisterSpace = 0;
	pd3dDescriptorRanges[6].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER pd3dRootParameters[13];

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

	pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[5].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[2]);
	pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[6].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[3]);
	pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[7].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[4]);
	pd3dRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[8].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[8].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[5]);
	pd3dRootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[9].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[9].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[6]);
	pd3dRootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[10].Descriptor.ShaderRegister = 12; //Framework Info
	pd3dRootParameters[10].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[11].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[11].Constants.Num32BitValues = 16; //Texture Animation (4x4) Matrix
	pd3dRootParameters[11].Constants.ShaderRegister = 5; //
	pd3dRootParameters[11].Constants.RegisterSpace = 0;
	pd3dRootParameters[11].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[12].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[12].Descriptor.ShaderRegister = 3; 
	pd3dRootParameters[12].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[12].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;



	D3D12_STATIC_SAMPLER_DESC pd3dSamplerDescs[3];

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

	pd3dSamplerDescs[2].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	pd3dSamplerDescs[2].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pd3dSamplerDescs[2].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pd3dSamplerDescs[2].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pd3dSamplerDescs[2].MipLODBias = 0.0f;
	pd3dSamplerDescs[2].MaxAnisotropy = 1;
	pd3dSamplerDescs[2].ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //D3D12_COMPARISON_FUNC_LESS
	pd3dSamplerDescs[2].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE; // D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	pd3dSamplerDescs[2].MinLOD = 0;
	pd3dSamplerDescs[2].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[2].ShaderRegister = 2;
	pd3dSamplerDescs[2].RegisterSpace = 0;
	pd3dSamplerDescs[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

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

void CStage::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
    CreateGraphicsRootSignature(pd3dDevice);

	m_pd3dDescriptorHeap = std::make_unique<CDescriptorHeap>();
	m_pd3dDescriptorHeap->CreateCbcSrvDescriptorHeap(pd3dDevice, 0, 70);

	m_pTextureLoader = std::make_shared<CTextureLoader>();

	m_pCamera = std::make_unique<CThirdPersonCamera>();
	m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	std::shared_ptr<CObjectShaderComponent> pObjectShaderComponent = std::make_shared<CObjectShaderComponent>();
	pObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPlayersGameObject = std::make_shared<CPlayerGameObject>();
	m_pPlayersGameObject->Init(XMFLOAT3(0.f, 0.f, 0.f));

	m_pPlayersGameObject->AddShaderComponent(pObjectShaderComponent);
	m_pPlayersGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/Penguin.bin", m_pTextureLoader);
	m_pPlayersGameObject->LoadPlayerFrameData();
	m_pPlayersGameObject->SetPosition(XMFLOAT3(0.f, 5.f, 130.f));


	std::shared_ptr<CTextureRectMeshShaderComponent> pTextureRectMeshShaderComponent = std::make_shared<CTextureRectMeshShaderComponent>();
	pTextureRectMeshShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

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
	pBombGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/PlayerBullet.bin", m_pTextureLoader);
	pBombGameObject->SetSpriteEffect(m_pTextureRectObjects[0], XMFLOAT3(45.f, 8.f, -12.f));
	m_pPlayersGameObject->LoadPlayerBombObject(pBombGameObject);
	
	for (int i = 0; i < 1; i++)
	{
		pBombGameObject = std::make_shared<CBombGameObject>();
		pBombGameObject->Init(XMFLOAT3(20.f, 20.f, 20.f));
		pBombGameObject->AddShaderComponent(pObjectShaderComponent);
		pBombGameObject->SetSpriteEffect(m_pTextureRectObjects[1 + i], XMFLOAT3(45.f, 8.f, -12.f));
		pBombGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/PlayerBullet.bin", m_pTextureLoader);

		std::shared_ptr<CMonsterGameObject> pMonsterMesh1 = std::make_shared<CMonsterGameObject>();
		pMonsterMesh1->Init(XMFLOAT3(0.f, 0.f, 0.f));
		pMonsterMesh1->AddShaderComponent(pObjectShaderComponent);
		pMonsterMesh1->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/BP_Mega_Ice_Golem.bin", m_pTextureLoader);

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
	m_pLightObject = std::make_shared<CLight>();
	m_pLightObject->Init();
	m_pLightObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//XMFLOAT3 xmf3TerrainScale(4.0f, 0.4f, 4.0f);
	XMFLOAT3 xmf3WaterTerrainScale(10.f, 2.0f, 10.f);

	XMFLOAT3 xmf3Scale(16.0f, 3.0f, 16.0f);

	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);

	std::shared_ptr<CTerrainObjectShaderComponent> pTerrainShaderComponent = std::make_shared<CTerrainObjectShaderComponent>();
	pTerrainShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);

	m_pTerrain = std::make_shared<CTerrainObject>();
	m_pTerrain->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), _T("Image/HeightMap.raw"), m_pd3dDescriptorHeap.get(), 257, 257, 17, 17, xmf3Scale, xmf4Color);
	m_pTerrain->AddShaderComponent(pTerrainShaderComponent);
	m_pTerrain->SetPosition(XMFLOAT3(-400.f, 0.f, -400.f));

	std::shared_ptr<CRippleWaterObjectShaderComponent> pTerrainWaterShaderComponent = std::make_shared<CRippleWaterObjectShaderComponent>();
	pTerrainWaterShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);

	m_pTerrainWater = std::make_unique<CRippleWaterObject>();
	m_pTerrainWater->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), 257, 257, 100, 100, xmf3WaterTerrainScale, xmf4Color);
	m_pTerrainWater->AddShaderComponent(pTerrainWaterShaderComponent);
	m_pTerrainWater->SetPosition(XMFLOAT3(-120.f + 0.f +(257 * 0.5f), 0.f, -80.f + (257 * 0.5f)));
	//m_pTerrainWater->SetPosition(XMFLOAT3((257 * xmf3TerrainScale.x * 0.5f), 155.0f - 250.f, +(257 * xmf3TerrainScale.z * 0.5f)));

	//for (int i = 0; i < 5; i++)
	//{
	//	XMFLOAT3 xmf3Position = XMFLOAT3(50.f + i * 80.f, 0.f, 680.f + -i * i * 40.f);
	//	std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

	//	pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(45.f, 45.f), L"Image/Tree01.dds");
	//	pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
	//	pTextureRectObject->SetAnimateLifeTime(1.f);
	//	pTextureRectObject->SetLoop(true);
	//	pTextureRectObject->SetPosition(XMFLOAT3(xmf3Position.x, m_pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) - 200.f + 22.f, xmf3Position.z));
	//	m_pTreesRectObjects.push_back(pTextureRectObject);
	//}


	//for (int i = 0; i < 5; i++)
	//{
	//	XMFLOAT3 xmf3Position = XMFLOAT3(1200.f + i * 80.f, 0.f, 680.f + -i * i * 40.f);
	//	std::shared_ptr<CTextureRectObject> pTextureRectObject = std::make_shared<CTextureRectObject>();

	//	pTextureRectObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(45.f, 45.f), L"Image/Tree02.dds");
	//	pTextureRectObject->AddShaderComponent(pTextureRectMeshShaderComponent);
	//	pTextureRectObject->SetAnimateLifeTime(1.f);
	//	pTextureRectObject->SetLoop(true);
	//	pTextureRectObject->SetPosition(XMFLOAT3(xmf3Position.x, m_pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) - 200.f + 22.f, xmf3Position.z));
	//	m_pTreesRectObjects.push_back(pTextureRectObject);
	//}

	std::shared_ptr<CSkyBoxShaderComponent> pSkyBoxShaderComponent = std::make_shared<CSkyBoxShaderComponent>();
	pSkyBoxShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);

	m_pSkyBoxObject = std::make_unique<CSkyBoxObject>();
	m_pSkyBoxObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get());
	m_pSkyBoxObject->AddShaderComponent(pSkyBoxShaderComponent);

	std::shared_ptr<CUIObjectShaderComponent> pUIShaderComponent = std::make_shared<CUIObjectShaderComponent>();
	pUIShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);

	for (int i = 0; i < 1; i++)
	{
		std::shared_ptr<CUIGameObject> pUIObject = std::make_shared<CUIGameObject>();

		pUIObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f), L"Image/IceScoreFrame2.dds");
		pUIObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.9f));
		pUIObject->SetSize(XMFLOAT2(500.f, 180.f));
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


	//pUIObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get(), XMFLOAT2(10.f, 10.f), L"Image/SlideBar.dds");
	//pUIObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.9f));
	//pUIObject->SetSize(XMFLOAT2(180.f, 180.f));
	//pUIObject->AddShaderComponent(pUIShaderComponent);
	//pUIObject->SetEnable(true);
	//m_pUIObjects.push_back(pUIObject);


	std::shared_ptr<CVelocityGaugeUIGameObject> pUIVelocityObject = std::make_shared<CVelocityGaugeUIGameObject>();
	pUIVelocityObject->Init(pd3dDevice, pd3dCommandList, m_pd3dDescriptorHeap.get());
	pUIVelocityObject->AddShaderComponent(pUIShaderComponent);
	pUIVelocityObject->SetEnable(true);
	m_pUIObjects.push_back(pUIVelocityObject);

	//std::shared_ptr<CParticleObjectShaderComponent> pParticleObjectShaderComponent = std::make_shared<CParticleObjectShaderComponent>();
	//pParticleObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

	//m_pParticleObject = std::make_unique<CParticleObject>();
	//m_pParticleObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 65.0f, 0.0f), 0.0f, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(8.0f, 8.0f), MAX_PARTICLES);
	//m_pParticleObject->AddShaderComponent(pParticleObjectShaderComponent);

	//DXGI_FORMAT	pdxgiRtvFormats[1] = { DXGI_FORMAT_D32_FLOAT };

	//m_pDepthRenderShader = std::make_shared<CDepthRenderShaderComponent>();
	//m_pDepthRenderShader->Init(m_pLightObject);
	//m_pDepthRenderShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//m_pDepthRenderShader->BuildObjects(pd3dDevice, pd3dCommandList, NULL);


	//m_pDepthRenderShader->AddGameObject(m_pPlayersGameObject);
	//m_pDepthRenderShader->AddGameObject(m_pMonsterObjects[0]);
	////m_pDepthRenderShader->AddGameObject(m_pTerrain);

	//m_pShadowShader = std::make_shared<CShadowMapShaderComponent>();
	//m_pShadowShader->Init();
	//m_pShadowShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);
	//m_pShadowShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pDepthRenderShader->GetDepthFromLightMaterialComponent(), m_pd3dDescriptorHeap.get());


	//m_pShadowShader->AddGameObject(m_pTerrain);



	//std::shared_ptr<CDynamicCubeMappingShaderComponent> pDynamicCubeMappingShaderComponent = std::make_shared<CDynamicCubeMappingShaderComponent>();
	//pDynamicCubeMappingShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);

	//m_pDynamicCubeMappingGameObject = std::make_shared<CDynamicCubeMappingGameObject>();
	//m_pDynamicCubeMappingGameObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), 256, XMFLOAT3(XMFLOAT3(45.f, 45.f, 45.f)));
	//m_pDynamicCubeMappingGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/BP_Mini_Ice_Bear_C.bin", m_pTextureLoader);
	//m_pDynamicCubeMappingGameObject->SetScale(XMFLOAT3(15.f, 15.f, 15.f));
	//m_pDynamicCubeMappingGameObject->SetPosition(XMFLOAT3(0.f, 15.f, 0.f));
	//m_pDynamicCubeMappingGameObject->AddShaderComponent(pDynamicCubeMappingShaderComponent);

	//_T("Image/HeightMap.raw"), m_pd3dDescriptorHeap.get(), 257, 257, 17, 17, xmf3TerrainScale, xmf4Color

	std::shared_ptr<CTerrainTessellationShaderComponent> pTerrainTessellationShaderComponent = std::make_shared<CTerrainTessellationShaderComponent>();
	pTerrainTessellationShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH);
	m_pTessellationTerrainObject = std::make_shared<CTessellationTerrainObject>();
	m_pTessellationTerrainObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), _T("Image/HeightMap2.raw"), m_pd3dDescriptorHeap.get(), 257, 257, 13, 13, xmf3Scale, xmf4Color);
	m_pTessellationTerrainObject->AddShaderComponent(pTerrainTessellationShaderComponent);
		//pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Image/HeightMap.raw"), 257, 257, 13, 13, xmf3Scale, xmf4Color
}

bool CStage::ProcessInput(HWND hWnd, float fTimeElapsed)
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

		if (pKeysBuffer[VK_SPACE] & 0xF0) // 불렛 높이 지정
		{
			m_pPlayersGameObject->Acceleration(fTimeElapsed);
			dynamic_cast<CVelocityGaugeUIGameObject*>(m_pUIObjects[3].get())->SetBarValue(MAX_VELOCITY, m_pPlayersGameObject->GetBombGameObject()->GetVelocity().y);
		}
		else
		{
			if (pPreKeysBuffer[VK_SPACE] & 0xF0) // 이전에 점프
			{
				// Terrain 높이 체크
				m_pPlayersGameObject->MoveBomb(0.001f, m_pPlayersGameObject->GetLookVector());
				dynamic_cast<CVelocityGaugeUIGameObject*>(m_pUIObjects[3].get())->SetBarValue(MAX_VELOCITY, 0.f);
			}
		}


		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				if (pKeysBuffer[VK_RBUTTON] & 0xF0)
					m_pPlayersGameObject->Rotate(cyDelta * 0.3f, 0.0f, -cxDelta * 0.3f);
				else
					m_pPlayersGameObject->Rotate(cyDelta * 0.3f, cxDelta * 0.3f, 0.0f);
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

	}


	memcpy(pPreKeysBuffer, pKeysBuffer, sizeof(UCHAR) * 256);

	return true;
}

void CStage::AnimateObjects(float fTimeElapsed)
{

	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		if (m_pMonsterObjects[i])
		{
			float xmfOffsetY = 0.f;
			XMFLOAT3 xmfPosition = m_pMonsterObjects[i]->GetPosition();
			m_pMonsterObjects[i]->SetPosition(XMFLOAT3(xmfPosition.x, m_pTerrain->GetHeight(xmfPosition.x + 400.f, xmfPosition.z + 400.f) + xmfOffsetY, xmfPosition.z));
			m_pMonsterObjects[i]->Update(fTimeElapsed, m_pPlayersGameObject->GetPosition(), m_pTerrain);
		}
	}
}

void CStage::UpdateObjects(float fTimeElapsed)
{

	if (m_pCamera)
		m_pCamera->Update(m_pPlayersGameObject.get(), m_pPlayersGameObject->GetPosition(), fTimeElapsed);
	
	if(m_pDynamicCubeMappingGameObject)
		m_pDynamicCubeMappingGameObject->Update(fTimeElapsed, NULL);

	if (m_pLightObject)
		m_pLightObject->Update(fTimeElapsed, NULL);

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
	//// 그래픽 루트 시그니쳐를 설정
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootsignature.Get());
	pd3dCommandList->SetDescriptorHeaps(1, m_pd3dDescriptorHeap->m_pd3dCbvSrvDescriptorHeap.GetAddressOf());

	if (m_pLightObject)
		m_pLightObject->UpdateShaderVariables(pd3dCommandList);

	//m_pDepthRenderShader->PreRender(pd3dCommandList);

	if(m_pDynamicCubeMappingGameObject)
		m_pDynamicCubeMappingGameObject->OnPreRender(pd3dCommandList, this);


}

void CStage::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CCamera* pCameraData = pCamera;
	if (!pCameraData)
		pCameraData = m_pCamera.get();
	if (pCameraData)
	{
		pCameraData->SetViewportsAndScissorRects(pd3dCommandList);
		pCameraData->UpdateShaderVariables(pd3dCommandList);
	}


	if (m_pSkyBoxObject)
	{
		m_pSkyBoxObject->SetPosition(pCameraData->GetPosition());
		m_pSkyBoxObject->PrepareRender(pd3dCommandList);
		m_pSkyBoxObject->Render(pd3dCommandList, pCameraData, nullptr);
	}

	if (m_pTessellationTerrainObject)
	{
		m_pTessellationTerrainObject->PrepareRender(pd3dCommandList);
		m_pTessellationTerrainObject->Render(pd3dCommandList, pCameraData, nullptr);
	}

	if (m_pPlayersGameObject)
	{
		float xmfOffsetY = 0.f;
		XMFLOAT3 xmfPosition = m_pPlayersGameObject->GetPosition();
		m_pPlayersGameObject->SetPosition(XMFLOAT3(xmfPosition.x, m_pTerrain->GetHeight(xmfPosition.x + 400.f, xmfPosition.z + 400.f) + xmfOffsetY, xmfPosition.z));
		m_pPlayersGameObject->CGameObject::PrepareRender(pd3dCommandList);
		m_pPlayersGameObject->Render(pd3dCommandList, pCameraData, nullptr);
	}


	for (int i = 0; i < m_pMonsterObjects.size(); i++)
	{
		if (m_pMonsterObjects[i])
		{
			m_pMonsterObjects[i]->PrepareRender(pd3dCommandList);
			m_pMonsterObjects[i]->Render(pd3dCommandList, pCameraData, nullptr);
		}
	}
	
	for (int i = 0; i < m_pEffectRectObjects.size(); i++)
	{
		m_pEffectRectObjects[i]->PrepareRender(pd3dCommandList);
		m_pEffectRectObjects[i]->Render(pd3dCommandList, pCameraData, nullptr);
	}



	//for (int i = 0; i < m_pUIObjects.size(); i++)
	//{
	//	m_pUIObjects[i]->PrepareRender(pd3dCommandList);
	//	m_pUIObjects[i]->Render(pd3dCommandList, pCameraData, nullptr);
	//}


	//for (int i = 0; i < m_pUINumberObjects.size(); i++)
	//{
	//	m_pUINumberObjects[i]->PrepareRender(pd3dCommandList);
	//	m_pUINumberObjects[i]->Render(pd3dCommandList, pCameraData, nullptr);
	//}

	//m_pDepthRenderShader->UpdateShaderVariable(pd3dCommandList);
	//m_pShadowShader->Render(pd3dCommandList, pCameraData, nullptr);

	//if (m_pDynamicCubeMappingGameObject)
	//{
	//	m_pDynamicCubeMappingGameObject->PrepareRender(pd3dCommandList);
	//	m_pDynamicCubeMappingGameObject->Render(pd3dCommandList, pCameraData, nullptr);
	//}
	//if (!pCamera)
	//{
	//	if (m_pCamera)
	//	{
	//		m_pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	//		m_pCamera->UpdateShaderVariables(pd3dCommandList);
	//	}

	//	//m_pDepthRenderShader->UpdateShaderVariable(pd3dCommandList);
	//	//m_pShadowShader->Render(pd3dCommandList, m_pCamera.get(), nullptr);

	//	if (m_pCamera)
	//	{
	//		m_pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	//		m_pCamera->UpdateShaderVariables(pd3dCommandList);
	//	}
	//}
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


	//if (m_pMonsterObjects[0]->CollisionCheck(m_pMonsterObjects[1])
	//	
	//	)
	//{
	//	if (!(m_pMonsterObjects[0]->GetCollision() || m_pMonsterObjects[1]->GetCollision())) // 둘 다 충돌이 없었음
	//	{
	//		m_pMonsterObjects[0]->SetCollision(true);
	//		m_pMonsterObjects[0]->SetPosition(m_pMonsterObjects[0]->GetPrePosition());
	//	}


	//	
	//}

	//if (m_pPlayersGameObject->CollisionCheck(m_pDynamicCubeMappingGameObject)) // 충돌
	//{
	//	// 씬 전환
	//	m_bChangeScene = true;
	//}

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
