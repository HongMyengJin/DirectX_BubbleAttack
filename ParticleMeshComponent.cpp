#include "ParticleMeshComponent.h"
#include "ParticleVertex.h"

void CParticleMeshComponent::CreateVertexBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Velocity, float fLifetime, XMFLOAT3 xmf3Acceleration, XMFLOAT3 xmf3Color, XMFLOAT2 xmf2Size)
{
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	m_nStride = sizeof(CParticleVertex);

	CParticleVertex pVertices[1];

	pVertices[0].m_xmf3Position = xmf3Position;
	pVertices[0].m_xmf3Velocity = xmf3Velocity;
	pVertices[0].m_fLifetime = fLifetime;
	pVertices[0].m_nType = 0;


	m_pd3dPositionBuffer = CreateParticleBufferResource(pd3dDevice, pd3dCommandList, pVertices, sizeof(CParticleVertex) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dPositionUploadBuffer.GetAddressOf());
	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = sizeof(CParticleVertex);
	m_d3dPositionBufferView.SizeInBytes = sizeof(CParticleVertex) * m_nVertices;

}

void CParticleMeshComponent::CreateStreamOutputBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nMaxParticles)
{
	m_nMaxParticles = nMaxParticles;

	m_pd3dStreamOutputBuffer = CreateParticleBufferResource(pd3dDevice, pd3dCommandList, NULL, (m_nStride * m_nMaxParticles), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_STREAM_OUT, NULL);
	m_pd3dDrawBuffer = CreateParticleBufferResource(pd3dDevice, pd3dCommandList, NULL, (m_nStride * m_nMaxParticles), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	UINT64 nBufferFilledSize = 0;
	m_pd3dDefaultBufferFilledSize = CreateParticleBufferResource(pd3dDevice, pd3dCommandList, &nBufferFilledSize, sizeof(UINT64), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_STREAM_OUT, NULL);

	m_pd3dUploadBufferFilledSize = CreateParticleBufferResource(pd3dDevice, pd3dCommandList, NULL, sizeof(UINT64), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	m_pd3dUploadBufferFilledSize->Map(0, NULL, (void**)&m_pnUploadBufferFilledSize);

	m_pd3dReadBackBufferFilledSize = CreateParticleBufferResource(pd3dDevice, pd3dCommandList, NULL, sizeof(UINT64), D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_STATE_COPY_DEST, NULL);
}

void CParticleMeshComponent::PreRender(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState)
{
	if (nPipelineState == 0)
	{
		if (m_bStart)
		{
			m_bStart = false;

			m_nVertices = 1;

			m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
			m_d3dPositionBufferView.StrideInBytes = m_nStride;
			m_d3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;
		}
		else
		{
			m_d3dPositionBufferView.BufferLocation = m_pd3dDrawBuffer->GetGPUVirtualAddress();
			m_d3dPositionBufferView.StrideInBytes = m_nStride;
			m_d3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;
		}
		m_d3dStreamOutputBufferView.BufferLocation = m_pd3dStreamOutputBuffer->GetGPUVirtualAddress();
		m_d3dStreamOutputBufferView.SizeInBytes = m_nStride * m_nMaxParticles;
		m_d3dStreamOutputBufferView.BufferFilledSizeLocation = m_pd3dDefaultBufferFilledSize->GetGPUVirtualAddress();

		//		*m_pnUploadBufferFilledSize = m_nStride * m_nVertices;
		*m_pnUploadBufferFilledSize = 0;

		::SynchronizeResourceTransition(pd3dCommandList, m_pd3dDefaultBufferFilledSize.Get(), D3D12_RESOURCE_STATE_STREAM_OUT, D3D12_RESOURCE_STATE_COPY_DEST);
		pd3dCommandList->CopyResource(m_pd3dDefaultBufferFilledSize.Get(), m_pd3dUploadBufferFilledSize.Get());
		::SynchronizeResourceTransition(pd3dCommandList, m_pd3dDefaultBufferFilledSize.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_STREAM_OUT);
	}
	else if (nPipelineState == 1)
	{

		::SynchronizeResourceTransition(pd3dCommandList, m_pd3dStreamOutputBuffer.Get(), D3D12_RESOURCE_STATE_STREAM_OUT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		::SynchronizeResourceTransition(pd3dCommandList, m_pd3dDrawBuffer.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, D3D12_RESOURCE_STATE_STREAM_OUT);

		::SwapResourcePointer(m_pd3dDrawBuffer.GetAddressOf(), m_pd3dStreamOutputBuffer.GetAddressOf());

		m_d3dPositionBufferView.BufferLocation = m_pd3dDrawBuffer->GetGPUVirtualAddress();
		m_d3dPositionBufferView.StrideInBytes = m_nStride;
		m_d3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;
	}
}

void CParticleMeshComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState)
{
	if (nPipelineState == 0)
	{
		D3D12_STREAM_OUTPUT_BUFFER_VIEW pStreamOutputBufferViews[1] = { m_d3dStreamOutputBufferView };
		pd3dCommandList->SOSetTargets(0, 1, pStreamOutputBufferViews);

		pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
		pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dPositionBufferView);

		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);

		::SynchronizeResourceTransition(pd3dCommandList, m_pd3dDefaultBufferFilledSize.Get(), D3D12_RESOURCE_STATE_STREAM_OUT, D3D12_RESOURCE_STATE_COPY_SOURCE);
		pd3dCommandList->CopyResource(m_pd3dReadBackBufferFilledSize.Get(), m_pd3dDefaultBufferFilledSize.Get());
		::SynchronizeResourceTransition(pd3dCommandList, m_pd3dDefaultBufferFilledSize.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_STREAM_OUT);
	}
	else if (nPipelineState == 1)
	{
		pd3dCommandList->SOSetTargets(0, 1, NULL);

		pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
		pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dPositionBufferView);

		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CParticleMeshComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState)
{
}

#define _WITH_DEBUG_STREAM_OUTPUT_VERTICES
void CParticleMeshComponent::OnPostRender(int nPipelineState)
{

	if (nPipelineState == 0)
	{
#ifdef _WITH_QUERY_DATA_SO_STATISTICS
		D3D12_RANGE d3dReadRange = { 0, 0 };
		UINT8* pBufferDataBegin = NULL;
		m_pd3dSOQueryBuffer->Map(0, &d3dReadRange, (void**)&m_pd3dSOQueryDataStatistics);
		if (m_pd3dSOQueryDataStatistics) m_nVertices = (UINT)m_pd3dSOQueryDataStatistics->NumPrimitivesWritten;
		m_pd3dSOQueryBuffer->Unmap(0, NULL);
#else
		UINT64* pnReadBackBufferFilledSize = NULL;
		m_pd3dReadBackBufferFilledSize->Map(0, NULL, (void**)&pnReadBackBufferFilledSize);
		m_nVertices = UINT(*pnReadBackBufferFilledSize) / m_nStride;
		m_pd3dReadBackBufferFilledSize->Unmap(0, NULL);
#endif

		::gnCurrentParticles = m_nVertices;
#ifdef _WITH_DEBUG_STREAM_OUTPUT_VERTICES
		TCHAR pstrDebug[256] = { 0 };
		_stprintf_s(pstrDebug, 256, _T("Stream Output Vertices = %d\n"), m_nVertices);
		OutputDebugString(pstrDebug);
#endif
		if ((m_nVertices == 0) || (m_nVertices >= MAX_PARTICLES))
			m_bStart = true;
	}

}