#include "DynamicCubeMappingGameObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
void CDynamicCubeMappingGameObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, UINT nCubeMapSize)
{

	m_pComponents.resize(4);

	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();
	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CMeshComponent>();

	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(1);
	m_vTextureType[0] = ResourceTextureType::ResourceTextureCUBE;
	D3D12_CLEAR_VALUE d3dRtvClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM, { 0.0f, 0.0f, 0.0f, 1.0f } };

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 1, m_vTextureType);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateTexture(pd3dDevice, nCubeMapSize, nCubeMapSize, 6, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &d3dRtvClearValue, UINT(ResourceTextureType::ResourceTextureCUBE), 0, 0);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 9, 1); // 수정 필요
	//m_pRenderTargetMaterialComponent = std::make_shared<CMaterialsComponent>();
	//m_pRenderTargetMaterialComponent->Init(1, 1, m_vTextureType);

	//m_pRenderTargetMaterialComponent->CreateTexture(pd3dDevice, nCubeMapSize, nCubeMapSize, 6, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &d3dRtvClearValue, UINT(ResourceTextureType::ResourceTextureCUBE), 0, 0);
	//m_pRenderTargetMaterialComponent->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 9, 1); // 수정 필요
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
	d3dDescriptorHeapDesc.NumDescriptors = 6;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	hResult = pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)m_pd3dRtvDescriptorHeap.GetAddressOf());

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// 뎁스 스텐실 버퍼
	D3D12_CLEAR_VALUE d3dDsbClearValue = { DXGI_FORMAT_D24_UNORM_S8_UINT, { 1.0f, 0 } };
	m_pd3dDepthStencilBuffer = ::CreateTexture2DResource(pd3dDevice, nCubeMapSize, nCubeMapSize, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dDsbClearValue);

	pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer.Get(), NULL, m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());


	D3D12_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	d3dRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
	d3dRTVDesc.Texture2DArray.MipSlice = 0;
	d3dRTVDesc.Texture2DArray.PlaneSlice = 0;
	d3dRTVDesc.Texture2DArray.ArraySize = 1;

	for (int j = 0; j < 6; j++)
	{
		m_pd3dRtvCPUDescriptorHandles[j] = d3dRtvCPUDescriptorHandle;
		d3dRTVDesc.Texture2DArray.FirstArraySlice = j; //i-번째 렌더 타겟 뷰는 텍스쳐 큐브의 i-번째 버퍼에서 시작
		pd3dDevice->CreateRenderTargetView(dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(0).Get(), &d3dRTVDesc, m_pd3dRtvCPUDescriptorHandles[j]);
		d3dRtvCPUDescriptorHandle.ptr += ::gnRtvDescriptorIncrementSize;
	}


	// 카메라 생성(직육면체 6면 방향을 사진 찍기)
	for (int i = 0; i < 6; i++)
	{
		m_pCameras[i] = std::make_shared<CCamera>();
		m_pCameras[i]->SetViewport(0, 0, nCubeMapSize, nCubeMapSize, 0.0f, 1.0f);
		m_pCameras[i]->SetScissorRect(0, 0, nCubeMapSize, nCubeMapSize);
		m_pCameras[i]->CreateShaderVariables(pd3dDevice, pd3dCommandList);
		m_pCameras[i]->GenerateProjectionMatrix(0.1f, 5000.0f, 1.0f/*Aspect Ratio*/, 90.0f/*FOV*/);
	}
}

void CDynamicCubeMappingGameObject::OnScenePreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage)
{
	static XMFLOAT3 pxmf3LookAts[6] = { XMFLOAT3(+100.0f, 0.0f, 0.0f), XMFLOAT3(-100.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, +100.0f, 0.0f), XMFLOAT3(0.0f, -100.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, +100.0f), XMFLOAT3(0.0f, 0.0f, -100.0f) };
	static XMFLOAT3 pxmf3Ups[6] = { XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, +1.0f), XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, +1.0f, 0.0f) };

	float pfClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	::SynchronizeResourceTransition(pd3dCommandList, dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(0).Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

	XMFLOAT3 xmf3Position = GetPosition();
	for (int j = 0; j < 6; j++)
	{
		m_pCameras[j]->SetPosition(xmf3Position);
		m_pCameras[j]->GenerateViewMatrix(xmf3Position, Vector3::Add(xmf3Position, pxmf3LookAts[j]), pxmf3Ups[j]);

		pd3dCommandList->ClearRenderTargetView(m_pd3dRtvCPUDescriptorHandles[j], pfClearColor, 0, NULL);
		pd3dCommandList->ClearDepthStencilView(m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

		pd3dCommandList->OMSetRenderTargets(1, &m_pd3dRtvCPUDescriptorHandles[j], TRUE, &m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		if(pStage)
			pStage->Render(pd3dCommandList, m_pCameras[j].get());
	}

	::SynchronizeResourceTransition(pd3dCommandList, dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_MaterialDatas[0]->m_Textures[0]->GetTextureResource(0).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
}

void CDynamicCubeMappingGameObject::OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pStage)
{
	OnScenePreRender(pd3dCommandList, pStage);
}
