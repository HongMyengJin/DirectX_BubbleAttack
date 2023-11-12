#include "Light.h"

void LightDatas::Init()
{
	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	m_pLightsDatas[0].m_bEnable = true;
	m_pLightsDatas[0].m_nType = DIRECTIONAL_LIGHT;
	m_pLightsDatas[0].m_xmf4Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_pLightsDatas[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLightsDatas[0].m_xmf4Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.f);
	m_pLightsDatas[0].m_xmf3Direction = XMFLOAT3(0.f, -0.9f, 0.0f);
	m_pLightsDatas[1].m_bEnable = false;
	m_pLightsDatas[1].m_nType = SPOT_LIGHT;
	m_pLightsDatas[1].m_fRange = 300.f;
	m_pLightsDatas[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f);
	m_pLightsDatas[1].m_xmf4Diffuse = XMFLOAT4(0.1f, 0.1f, 0.f, 1.f);
	m_pLightsDatas[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLightsDatas[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLightsDatas[1].m_xmf3Direction = XMFLOAT3(0.f, -1.f, 0.f);
	m_pLightsDatas[1].m_xmf3Attenuation = XMFLOAT3(0.00015f, 0.00015f, 0.00015f);
	m_pLightsDatas[1].m_fFalloff = 10.0f;
	m_pLightsDatas[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLightsDatas[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
}


void CLight::Init()
{
	m_vOffsetPositions.resize(2);
	m_vPositions.resize(2);
	m_pLightData = std::make_shared<LightDatas>();
	m_pLightData->Init();

	m_pcbMappedLights = std::make_shared<LightDatas>();
	m_pcbMappedLights->Init();
}

void CLight::Animate(float fTimeElapsed)
{
}

void CLight::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	
	
	for (int i = 0; i < 2; i++)
		XMStoreFloat3(&m_pLightData->m_pLightsDatas[i].m_xmf3Position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_vPositions[i]), DirectX::XMLoadFloat3(&m_vOffsetPositions[i])));

}

void CLight::SetPosition(UINT Iindex, XMFLOAT3 Position)
{
	m_vPositions[Iindex] = Position;
}

void CLight::SetOffsetPosition(UINT Iindex, XMFLOAT3 Offset)
{
	m_vOffsetPositions[Iindex] = Offset;
}

void CLight::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CLight::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{

}

void CLight::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LightDatas) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);;
	m_pd3dcbLights->Map(0, NULL, (void**)&m_pcbMappedLights);
}

void CLight::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	::memcpy(&m_pcbMappedLights->m_pLightsDatas[0], &m_pLightData->m_pLightsDatas[0], sizeof(LightData) * MAX_LIGHT);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_pLightData->m_xmf4GlobalAmbient, sizeof(XMFLOAT4));

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights
}

void CLight::Release()
{
	m_pd3dcbLights.Reset();
}