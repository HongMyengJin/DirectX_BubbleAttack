#include "MirrorStage.h"
#include "MirrorObjectShaderComponent.h"
#include "TerrainObjectShaderComponent.h"
#include "SkyBoxShaderComponent.h"
void CMirrorStage::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
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


void CMirrorStage::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	CreateGraphicsRootSignature(pd3dDevice);

	m_pd3dDescriptorHeap = std::make_unique<CDescriptorHeap>();
	m_pd3dDescriptorHeap->CreateCbcSrvDescriptorHeap(pd3dDevice, 0, 300);

	m_pTextureLoader = std::make_shared<CTextureLoader>();

	m_pCamera = std::make_unique<CThirdPersonCamera>();
	m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//// 그래픽 루트 시그니쳐를 설정
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootsignature.Get());
	pd3dCommandList->SetDescriptorHeaps(1, m_pd3dDescriptorHeap->m_pd3dCbvSrvDescriptorHeap.GetAddressOf());

	m_pLightObject = std::make_shared<CLight>();
	m_pLightObject->Init();
	m_pLightObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	XMFLOAT3 xmf3TerrainScale(4.0f, 0.4f, 4.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);

	std::shared_ptr<CTerrainObjectShaderComponent> pTerrainShaderComponent = std::make_shared<CTerrainObjectShaderComponent>();
	pTerrainShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);

	m_pTerrain = std::make_shared<CTerrainObject>();
	m_pTerrain->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), _T("Image/HeightMap.raw"), m_pd3dDescriptorHeap.get(), 257, 257, 17, 17, xmf3TerrainScale, xmf4Color);
	m_pTerrain->AddShaderComponent(pTerrainShaderComponent);
	m_pTerrain->SetPosition(XMFLOAT3(-400.f, -50.f, -400.f));

	std::shared_ptr<CObjectShaderComponent> pObjectShaderComponent = std::make_shared<CObjectShaderComponent>();
	pObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pPlayersGameObject = std::make_shared<CPlayerGameObject>();
	m_pPlayersGameObject->Init(XMFLOAT3(0.f, 0.f, 0.f));

	m_pPlayersGameObject->AddShaderComponent(pObjectShaderComponent);
	m_pPlayersGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get(), "Model/Penguin.bin", m_pTextureLoader);
	m_pPlayersGameObject->LoadPlayerFrameData();
	m_pPlayersGameObject->SetPosition(XMFLOAT3(0.f, 5.f, 130.f));

	std::shared_ptr<CSkyBoxShaderComponent> pSkyBoxShaderComponent = std::make_shared<CSkyBoxShaderComponent>();
	pSkyBoxShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);

	m_pSkyBoxObject = std::make_unique<CSkyBoxObject>();
	m_pSkyBoxObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get());
	m_pSkyBoxObject->AddShaderComponent(pSkyBoxShaderComponent);

	std::shared_ptr<CMirrorObjectShaderComponent> pMirrorObjectShaderComponent = std::make_shared<CMirrorObjectShaderComponent>();
	pMirrorObjectShaderComponent->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	m_pMirrorObject = std::make_shared<CMirrorObject>();
	m_pMirrorObject->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), m_pd3dDescriptorHeap.get());
	m_pMirrorObject->SetPosition(XMFLOAT3(0.f, 30.f, 0.f));
	m_pMirrorObject->AddShaderComponent(pMirrorObjectShaderComponent);
}

bool CMirrorStage::ProcessInput(HWND hWnd, float fTimeElapsed)
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
		}
		else
		{
			if (pPreKeysBuffer[VK_SPACE] & 0xF0) // 이전에 점프
			{
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
				m_pPlayersGameObject->Move(dwDirection, 5.f);
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

void CMirrorStage::AnimateObjects(float fTimeElapsed)
{
}

void CMirrorStage::UpdateObjects(float fTimeElapsed)
{
	if (m_pCamera)
		m_pCamera->Update(m_pPlayersGameObject.get(), m_pPlayersGameObject->GetPosition(), fTimeElapsed);

	if (m_pMirrorObject)
		m_pMirrorObject->Update(fTimeElapsed, NULL);
}

void CMirrorStage::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	//// 그래픽 루트 시그니쳐를 설정
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootsignature.Get());
	pd3dCommandList->SetDescriptorHeaps(1, m_pd3dDescriptorHeap->m_pd3dCbvSrvDescriptorHeap.GetAddressOf());

	if (m_pLightObject)
		m_pLightObject->UpdateShaderVariables(pd3dCommandList);

	if (m_pMirrorObject)
		m_pMirrorObject->OnPreRender(pd3dCommandList, this);

}

void CMirrorStage::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	if (!m_bFirst)
	{
		m_bFirst = true;
		return;
	}
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
	if (m_pPlayersGameObject)
	{
		float xmfOffsetY = 0.f;
		XMFLOAT3 xmfPosition = m_pPlayersGameObject->GetPosition();
		m_pPlayersGameObject->CGameObject::PrepareRender(pd3dCommandList);
		m_pPlayersGameObject->Render(pd3dCommandList, pCameraData, nullptr);
	}

	if (m_pTerrain)
	{
		m_pTerrain->PrepareRender(pd3dCommandList);
		m_pTerrain->Render(pd3dCommandList, pCameraData, nullptr);
	}

	if (pCameraData == m_pCamera.get())
	{
		if (m_pMirrorObject)
		{
			m_pMirrorObject->PrepareRender(pd3dCommandList);
			m_pMirrorObject->Render(pd3dCommandList, pCameraData, nullptr);
		}
	}

}

void CMirrorStage::RenderParticle(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMirrorStage::OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMirrorStage::CollisionCheck()
{
}

void CMirrorStage::Release()
{
	if (m_pCamera)
		m_pCamera->Release();
	if (m_pLightObject)
		m_pLightObject->Release();
}
