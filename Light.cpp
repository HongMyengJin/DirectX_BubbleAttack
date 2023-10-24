#include "Light.h"

void LightDatas::Init()
{
	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	m_pLightsDatas[0].m_bEnable = true;
	m_pLightsDatas[0].m_nType = DIRECTIONAL_LIGHT;
	m_pLightsDatas[0].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLightsDatas[0].m_xmf4Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_pLightsDatas[0].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLightsDatas[0].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pLightsDatas[1].m_bEnable = true;
	m_pLightsDatas[1].m_nType = SPOT_LIGHT;
	m_pLightsDatas[1].m_fRange = 500.0f;
	m_pLightsDatas[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLightsDatas[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLightsDatas[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLightsDatas[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLightsDatas[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLightsDatas[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLightsDatas[1].m_fFalloff = 8.0f;
	m_pLightsDatas[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLightsDatas[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
}


void CLight::Init()
{
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
}

void CLight::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CLight::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
}

void CLight::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LightDatas) + 255) & ~255); //256�� ���
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