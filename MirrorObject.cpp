#include "MirrorObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "TextureRectMesh2Component.h"
void CMirrorObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, MirrorData eMirrorData)
{
	UINT MirrorWidthSize = 500;
	UINT MirrorHeightSize = 500;
	m_pComponents.resize(4);

	//																																											Front
	static XMFLOAT3 pxmf3LookAts[6] = { XMFLOAT3(+250.f, 45.0f, 0.0f), XMFLOAT3(-250.f, 45.0f, 0.0f), XMFLOAT3(0.0f, +250.f, 0.0f), XMFLOAT3(0.0f, -250.f, 0.0f), XMFLOAT3(0.0f, 45.0f, +250.f), XMFLOAT3(0.0f, 45.0f, -250.f) }; // 가운데가 아닌 조금 위에서 그려지도록
	static XMFLOAT3 pxmf3Ups[6] = { XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, +1.0f), XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, +1.0f, 0.0f) };

	m_xmf3LookAt = pxmf3LookAts[UINT(eMirrorData)];
	m_xmf3Up = pxmf3Ups[UINT(eMirrorData)];

	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();
	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CTextureRectMesh2Component>();

	switch (eMirrorData)
	{
	case MirrorData::Mirror_Light:
	case MirrorData::Mirror_Left:
		dynamic_cast<CTextureRectMesh2Component*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Init(pd3dDevice, pd3dCommandList, MirrorWidthSize, MirrorHeightSize, 20.f);
		break;
	case MirrorData::Mirror_Up:
	case MirrorData::Mirror_Down:
		dynamic_cast<CTextureRectMesh2Component*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Init(pd3dDevice, pd3dCommandList, MirrorWidthSize, MirrorHeightSize, 20.f);
		break;
	case MirrorData::Mirror_Front:
		dynamic_cast<CTextureRectMesh2Component*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Init(pd3dDevice, pd3dCommandList, MirrorWidthSize, MirrorHeightSize, 20.f, 0.f, 0.f, 0.f, true);
		break;
	case MirrorData::Mirror_Back:
		dynamic_cast<CTextureRectMesh2Component*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Init(pd3dDevice, pd3dCommandList, MirrorWidthSize, MirrorHeightSize, 20.f);
		break;
	}


	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(1);
	m_vTextureType[0] = ResourceTextureType::ResourceTexture2D;
	D3D12_CLEAR_VALUE d3dRtvClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM, { 0.0f, 0.0f, 0.0f, 1.0f } };

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 1, m_vTextureType);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateTexture(pd3dDevice, MirrorWidthSize, MirrorHeightSize, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &d3dRtvClearValue, UINT(ResourceTextureType::ResourceTexture2D), 0, 0);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 9, 1); // 수정 필요

	//
	// 뎁스 스텐실
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HRESULT hResult = pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// 렌더 타겟
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	hResult = pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)m_pd3dRtvDescriptorHeap.GetAddressOf());

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// 뎁스 스텐실 버퍼
	D3D12_CLEAR_VALUE d3dDsbClearValue = { DXGI_FORMAT_D24_UNORM_S8_UINT, { 1.0f, 0 } };
	m_pd3dDepthStencilBuffer = ::CreateTexture2DResource(pd3dDevice, MirrorWidthSize, MirrorHeightSize, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dDsbClearValue);

	pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer.Get(), NULL, m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());


	D3D12_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	d3dRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
	d3dRTVDesc.Texture2DArray.MipSlice = 0;
	d3dRTVDesc.Texture2DArray.PlaneSlice = 0;
	d3dRTVDesc.Texture2DArray.ArraySize = 1;

	m_pd3dRtvCPUDescriptorHandles = d3dRtvCPUDescriptorHandle;
	d3dRTVDesc.Texture2DArray.FirstArraySlice = 0; 
	pd3dDevice->CreateRenderTargetView(dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(0).Get(), &d3dRTVDesc, m_pd3dRtvCPUDescriptorHandles);
	d3dRtvCPUDescriptorHandle.ptr += ::gnRtvDescriptorIncrementSize;
	
	// 카메라 생성(한 방향으로 사진 찍기)

	m_pCameras = std::make_shared<CCamera>();
	m_pCameras->SetViewport(0, 0, MirrorWidthSize, MirrorHeightSize, 0.0f, 1.0f);
	m_pCameras->SetScissorRect(0, 0, MirrorWidthSize, MirrorHeightSize);
	m_pCameras->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_pCameras->GenerateProjectionMatrix(0.1f, 5000.0f, 1.0f/*Aspect Ratio*/, 140.f/*FOV*/);


	m_eMirrorType = eMirrorData;

}

void CMirrorObject::OnScenePreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage)
{
	float pfClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	::SynchronizeResourceTransition(pd3dCommandList, dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(0).Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position.y += -55.f;

	switch (m_eMirrorType)
	{
	case MirrorData::Mirror_Left:
		xmf3Position.x -= 88.f;
		break;
	case MirrorData::Mirror_Light:
		xmf3Position.x += 88.f;
		break;
	case MirrorData::Mirror_Up:
		break;
	case MirrorData::Mirror_Down:
		break;
	case MirrorData::Mirror_Front:
		xmf3Position.z -= 88.f;
		break;
	case MirrorData::Mirror_Back:
		xmf3Position.z += 88.f;
		break;
	case MirrorData::MirrorData_End:
		break;
	default:
		break;
	}
	
	m_pCameras->SetPosition(xmf3Position);
	m_pCameras->GenerateViewMatrix(xmf3Position, Vector3::Add(xmf3Position, m_xmf3LookAt), m_xmf3Up); // z 축방향으로 앞에 위치

	pd3dCommandList->ClearRenderTargetView(m_pd3dRtvCPUDescriptorHandles, pfClearColor, 0, NULL);
	pd3dCommandList->ClearDepthStencilView(m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	pd3dCommandList->OMSetRenderTargets(1, &m_pd3dRtvCPUDescriptorHandles, TRUE, &m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	if (pStage)
		pStage->Render(pd3dCommandList, m_pCameras.get());

	::SynchronizeResourceTransition(pd3dCommandList, dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(0).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
}

void CMirrorObject::OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage)
{
	OnScenePreRender(pd3dCommandList, pStage);
}
