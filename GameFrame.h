#include "stdafx.h"
#include "Timer.h"
#include "SceneManager.h"

struct CB_FRAMEWORK_INFO
{
	float					m_fCurrentTime;
	float					m_fElapsedTime;
	float					m_fSecondsPerFirework = 1.0f;
	int						m_nFlareParticlesToEmit = 300;
	XMFLOAT3				m_xmf3Gravity = XMFLOAT3(0.0f, -9.8f, 0.0f);
	int						m_nMaxFlareType2Particles = 150;
	bool					m_bTessellationWire = false;

};
class CBubbleAttackGameFrame
{
public:
	CBubbleAttackGameFrame();
	~CBubbleAttackGameFrame();

	void Init(HINSTANCE hInstance, HWND hMainWnd);
	void CreateFrame(HINSTANCE hInstance, HWND hMainWnd);
	void DestroyFrame();

	void UpdateFrame();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateSwapChainRenderTargetViews();
	void CreateDepthStencilView();

	//void ChangeSwapChainState();

	void WaitForGpuComplete();

	void CreateShaderVariables();
	void UpdateShaderVariables();
	void ReleaseShaderVariables();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;

	int							m_iClientWidth;
	int							m_iClientHeight;

	const UINT					m_iSwapChainBuffers = 2;
	UINT						m_iSwapChainBufferIndex = 0;

	CTimer						m_Timer;
	D3D12_CPU_DESCRIPTOR_HANDLE								m_pd3dSwapChainBackBufferRTVCPUHandles[SWAPCHAINBUFFER_N];
	D3D12_CPU_DESCRIPTOR_HANDLE								m_d3dDsvDescriptorCPUHandle;

	Microsoft::WRL::ComPtr<ID3D12Device>					m_pd3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Resource>					m_ppd3dRenderTargetBuffers[SWAPCHAINBUFFER_N];
	Microsoft::WRL::ComPtr<ID3D12Resource>					m_pd3dDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>			m_pd3dRtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>			m_pd3dDsvDescriptorHeap;

	Microsoft::WRL::ComPtr<IDXGIFactory4>					m_pdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain3>					m_pdxgiSwapChain;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>			m_pd3dCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>				m_pd3dCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>		m_pd3dCommandList;

	Microsoft::WRL::ComPtr<ID3D12Fence>						m_pd3dFence;
	UINT64													m_nFenceValue;
	HANDLE													m_hFenceEvent;

	bool													m_bMsaa4xEnable = false;
	UINT													m_nMsaa4xQualityLevels = 0;

	D3D12_VIEWPORT											m_d3dViewport;
	D3D12_RECT												m_d3dScissorRect;

	// Scene ����
	std::unique_ptr<CSceneManager>							m_pSceneManager;

	Microsoft::WRL::ComPtr<ID3D12Resource>					m_pd3dcbFrameworkInfo;
	std::shared_ptr<CB_FRAMEWORK_INFO>						m_pcbMappedFrameworkInfo;

	bool													m_bStart = false;
	bool													m_bTessellationWire = false;
};