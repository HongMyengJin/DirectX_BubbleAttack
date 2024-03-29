#pragma once
#include "stdafx.h"
#include "Component.h"

class CMeshComponent : public CComponent
{
public:
	CMeshComponent() : CComponent() {}
	~CMeshComponent() {};

protected:
	char																	m_stMeshName[256];
	
	XMFLOAT3																m_xmf3AABBCenter = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3																m_xmf3AABBExtents = XMFLOAT3(0.f, 0.f, 0.f);

	D3D12_PRIMITIVE_TOPOLOGY												m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

protected:
	UINT																	m_nType = 0x00;
	UINT																	m_nVertices = 0;

	std::vector<XMFLOAT3>													m_pxmf3Positions;

	Microsoft::WRL::ComPtr<ID3D12Resource>									m_pd3dPositionBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>									m_pd3dPositionUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW												m_d3dPositionBufferView;

	UINT																	m_nSubMeshes = 0;
	INT																		m_nSlot = 0;
	UINT																	m_nOffset = 0;

	std::vector<int>														m_pnSubSetIndices;
	std::vector<std::vector<UINT>>											m_ppnSubSetIndices;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>						m_ppd3dSubSetIndexBuffers;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>						m_ppd3dSubSetIndexUploadBuffers;
	std::vector<D3D12_INDEX_BUFFER_VIEW>									m_pd3dSubSetIndexBufferViews;

public:
	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr); // UINT nSubset
};

class CObjectMeshComponent : public CMeshComponent
{
public:
	CObjectMeshComponent() : CMeshComponent() {}
	~CObjectMeshComponent() {}

public:
	void LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr); // UINT nSubset
protected:
	std::vector<XMFLOAT4>						m_pxmf4Colors;
	std::vector<XMFLOAT4>						m_pxmf3Normals;
	std::vector<XMFLOAT4>						m_pxmf3Tangents;
	std::vector<XMFLOAT4>						m_pxmf3BiTangents;
	std::vector<XMFLOAT4>						m_pxmf2TextureCoords0;
	std::vector<XMFLOAT4>						m_pxmf2TextureCoords1;

	D3D12_VERTEX_BUFFER_VIEW					m_d3dTextureCoord0BufferView;
	D3D12_VERTEX_BUFFER_VIEW					m_d3dTextureCoord1BufferView;
	D3D12_VERTEX_BUFFER_VIEW					m_d3dNormalBufferView;
	D3D12_VERTEX_BUFFER_VIEW					m_d3dTangentBufferView;
	D3D12_VERTEX_BUFFER_VIEW					m_d3dBiTangentBufferView;

	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dTextureCoord0Buffers;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dTextureCoord0UploadBuffer;

	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dTextureCoord1Buffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dTextureCoord1UploadBuffer;

	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dNormalBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dNormalUploadBuffer;

	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dTangentBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dTangentUploadBuffer;

	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dBiTangentBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>		m_pd3dBiTangentUploadBuffer;
};

