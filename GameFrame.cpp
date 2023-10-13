#include "GameFrame.h"

CBubbleAttackGameFrame::CBubbleAttackGameFrame()
{
}

CBubbleAttackGameFrame::~CBubbleAttackGameFrame()
{
}

void CBubbleAttackGameFrame::Init(HINSTANCE hInstance, HWND hMainWnd)
{
	CreateFrame(hInstance, hMainWnd);
}

void CBubbleAttackGameFrame::CreateFrame(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();

	CreateSwapChain();
#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateSwapChainRenderTargetViews();
#endif
	CreateDepthStencilView();

	//BuildObjects();
}

void CBubbleAttackGameFrame::DestroyFrame()
{
	WaitForGpuComplete();
	// GPU가 모든 명령 리스트를 실행할때까지 대기

	CloseHandle(m_hFenceEvent);

	//for (int i = 0; i < SWAPCHAINBUFFER_N; i++) if (m_ppd3dRenderTargetBuffers[i]) m_ppd3dRenderTargetBuffers[i]->Release();
	//if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();

}

void CBubbleAttackGameFrame::UpdateFrame()
{
	HRESULT hResult = m_pd3dCommandAllocator->Reset();
	hResult = m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), nullptr);
	// 명령 할당자, 리스트 리셋

	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = m_ppd3dRenderTargetBuffers[m_iSwapChainBufferIndex].Get();
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	// 현재 렌더 타겟에 대한 렌더링이 끝날때까지 대기
	// 프리젠트가 끝나면 렌더 타겟 버퍼의 상태는 프리젠트 상태(D3D12_RESOURCE_STATE_PRESENT)에서 렌더 타겟 상태(D3D12_RESOURCE_STATE_RENDER_TARGET)로 바뀔 것

	D3D12_CPU_DESCRIPTOR_HANDLE	d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_iSwapChainBufferIndex * gnRtvDescriptorIncrementSize);
	// 현재의 렌더 타겟에 해당하는 서술자의 CPU 주소(핸들) 계산

	float gfClearColor[4] = { 1.f, 1.f, 0.f, 1.f };
	m_pd3dCommandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, gfClearColor, 0, NULL);
	// 원하는 색상으로 렌더 타겟(뷰) 지움

	D3D12_CPU_DESCRIPTOR_HANDLE	d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// 깊이-스텐실 서술자의 cpu 주소를 계산

	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, NULL);
	// 특정 값으로 깊이-스텐실(뷰)을 지움

	m_pd3dCommandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);
	// 렌더 타겟 뷰(서술자)와 깊이-스텐실 뷰(서술자)를 출렵-병합 단계(OM)에 연결

	//Rendering

	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; 
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);
	// 현재 렌더 타겟에 대한 렌더링이 끝날때까지 대기
	// GPU가 렌더 타겟(버퍼)을 더 이상 사용하지 않으면 렌더 타겟 상테는 프리젠트 상태(D3D12_RESOURCE_SATE_PRESENT)로 바뀐다.


	hResult = m_pd3dCommandList->Close();
	// 명령 리스트를 닫힌 상태로 설정

	ID3D12CommandList* ppd3dCommandList[] = { m_pd3dCommandList.Get() };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandList);
	// 명령 리스트를 명령 큐에 추가하여 실행

	WaitForGpuComplete();
	//GPU가 모든 명령 리스트를 실행할때까지 대기

	DXGI_PRESENT_PARAMETERS	dxgiPresentParameters;
	dxgiPresentParameters.DirtyRectsCount = 0;
	dxgiPresentParameters.pDirtyRects = NULL;
	dxgiPresentParameters.pScrollRect = NULL;
	m_pdxgiSwapChain->Present1(1, 0, &dxgiPresentParameters);
	// 스왑체인을 프리젠트(현재 렌더 타겟(후면버퍼)의 내용이 전면버퍼로 옮겨지고 렌더 타겟 인덱스가 변경됨

	m_iSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	//렌더 타겟 인덱스가 변경됨
}

void CBubbleAttackGameFrame::CreateSwapChain()
{
#define _WITH_ONLY_RESIZE_BACKBUFFERS

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 2;
	dxgiSwapChainDesc.BufferDesc.Width = FRAME_BUFFER_WIDTH;
	dxgiSwapChainDesc.BufferDesc.Height = FRAME_BUFFER_HEIGHT;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE;
	dxgiSwapChainDesc.Flags = 0;

	//dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	HRESULT hResult = m_pdxgiFactory->CreateSwapChain(m_pd3dCommandQueue.Get(), &dxgiSwapChainDesc, (IDXGISwapChain**)m_pdxgiSwapChain.GetAddressOf());
// 스왑 체인 생성
	hResult = m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	// "Alt + Enter" 키의 동작 비활성화
	m_iSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	// 스왑체인의 현재 후면버퍼 인덱스 저장
}

void CBubbleAttackGameFrame::CreateDirect3DDevice()
{
	HRESULT hResult;

	UINT nDXGIFactoryFlags = 0;
#if defined(_DEBUG)
	ID3D12Debug* pd3dDebugController = NULL;
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&pd3dDebugController);
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	hResult = ::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void**)&m_pdxgiFactory);

	IDXGIAdapter1* pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)continue;
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&m_pd3dDevice)))break;
	}
	// 모든 하드웨어 어댑터에 대하여 특성 레벨 12.0을 지원하는 하드웨어 디바이스 생성

	if (!m_pd3dDevice)
	{
		hResult = m_pdxgiFactory->EnumWarpAdapter(_uuidof(IDXGIAdapter1), (void**)&pd3dAdapter);
		hResult = D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&m_pd3dDevice);
	}
	// 하드웨어 디바이스가 생성되지 않았을 경우 WARP 디바이스 생성
	if (!m_pd3dDevice)
	{
		MessageBox(NULL, L"Direct3D 12 Device Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4; // Msaa4x 다중 샘플링
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	hResult = m_pd3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	// 디바이스가 지원하는 다중 샘플의 품질 수준을 확인
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;
	// 다중 샘플의 품질 수준이 1보다 클 경우 다중 샘플링 활성화

	hResult = m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&m_pd3dFence);
	// 펜스를 생성한다.(펜스값:0)
	m_nFenceValue = 0;
	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	// 이벤트 객체 생성(펜스와 동기화를 위해), 이벤트 객체 초기값은 FALSE, 이벤트 실행(Signal)되면 이벤트의 값을 자동적으로 False가 되도록 생성
	
	::gnRtvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	::gnCbvSrvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	if (pd3dAdapter)pd3dAdapter->Release();
}


void CBubbleAttackGameFrame::CreateCommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hResult = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void**)&m_pd3dCommandQueue);
	// Direct 명령 큐 생성

	hResult = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)m_pd3dCommandAllocator.GetAddressOf());
	// Direct 명령 할당자 생성

	hResult = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)m_pd3dCommandList.GetAddressOf());
	// Direct 명령 리스트 생성

	hResult = m_pd3dCommandList->Close();
	// 닫힌 상태로 설정 (명령 리스트는 생성되면 열린 상태이기 때문)
}

void CBubbleAttackGameFrame::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = 2;// + 5; //서술자의 개수 == 스왑체인 버퍼의 개수
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HRESULT hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dRtvDescriptorHeap);
	// 렌더 타겟 서술자 힙 생성


	d3dDescriptorHeapDesc.NumDescriptors = 1;  // 깊이-스텐실 서술자 힙의 개수
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);
	// 깊이-스텐실 서술자 힙 생성
}

// 스왑체인의 각 후면 버퍼에 대한 렌더 타겟 뷰 생성
void CBubbleAttackGameFrame::CreateSwapChainRenderTargetViews()
{
	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
	d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < SWAPCHAINBUFFER_N; i++)
	{
		m_pdxgiSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)(m_ppd3dRenderTargetBuffers[i].GetAddressOf()));
		m_pd3dDevice->CreateRenderTargetView(m_ppd3dRenderTargetBuffers[i].Get(), &d3dRenderTargetViewDesc, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += ::gnRtvDescriptorIncrementSize;
	}
}

void CBubbleAttackGameFrame::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = FRAME_BUFFER_WIDTH;
	d3dResourceDesc.Height = FRAME_BUFFER_HEIGHT;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapProperties.CreationNodeMask = 1;
	d3dHeapProperties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;

	m_pd3dDevice->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void**)&m_pd3dDepthStencilBuffer);
	// 깊이-스텐실 버퍼 생성

	m_d3dDsvDescriptorCPUHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer.Get(), NULL, m_d3dDsvDescriptorCPUHandle);
	// 깊이-스텐실 버퍼 뷰 생성
}


void CBubbleAttackGameFrame::WaitForGpuComplete()
{

	const UINT64 nFenceValue = ++m_nFenceValue;
	//CPU 펜스 값 증가

	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFenceValue);
	//멸령큐에 GPU가 펜스 값을 설정하는 명령 추가

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		// 펜스의 현재 값이 설정한 값보다 작으면 대기
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}
