#include "UIMesh.h"
#include "UIVertex.h"

void CUIMesh::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
    m_nVertices = 6;
    CUIVertex pVertices[6];
    m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    float fx = 1.f, fy = 0.5f, fz = 0.f;

    pVertices[0] = CUIVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
    pVertices[1] = CUIVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(1.0f, 1.0f));
    pVertices[2] = CUIVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
    pVertices[3] = CUIVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
    pVertices[4] = CUIVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(0.0f, 0.0f));
    pVertices[5] = CUIVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));

    m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, sizeof(CUIVertex) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

    m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
    m_d3dPositionBufferView.StrideInBytes = sizeof(CUIVertex);
    m_d3dPositionBufferView.SizeInBytes = sizeof(CUIVertex) * m_nVertices;
}
