#pragma once
#include "Mesh.h"

class CParticleMeshComponent : public CMeshComponent
{

public:
	CParticleMeshComponent() {};
	~CParticleMeshComponent() {};
public:

	virtual void CreateVertexBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Velocity, float fLifetime, XMFLOAT3 xmf3Acceleration, XMFLOAT3 xmf3Color, XMFLOAT2 xmf2Size);
	virtual void CreateStreamOutputBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nMaxParticles);

	virtual void PreRender(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState);

	virtual void OnPostRender(int nPipelineState);

protected:
	D3D12_STREAM_OUTPUT_BUFFER_VIEW				m_d3dStreamOutputBufferView;
	UINT										m_nMaxParticles = MAX_PARTICLES;
	UINT										m_nStride;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dStreamOutputBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dDrawBuffer;
				  
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dDefaultBufferFilledSize;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dUploadBufferFilledSize;
				  
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dReadBackBufferFilledSize;
	UINT64*										m_pnUploadBufferFilledSize;


	bool										m_bStart = true;

};

