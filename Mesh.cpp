#include "Mesh.h"

#define VERTEXT_POSITION				0x01
#define VERTEXT_COLOR					0x02
#define VERTEXT_NORMAL					0x04
#define VERTEXT_TANGENT					0x08
#define VERTEXT_TEXTURE_COORD0			0x10
#define VERTEXT_TEXTURE_COORD1			0x20


void CMeshComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, void* pContext)
{
	UINT nSubSet = 0;
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	//m_nSlot 추가 고민중
	pd3dCommandList->IASetVertexBuffers(0, 1, &m_d3dPositionBufferView);
	if (pContext)
		nSubSet = *static_cast<UINT*>(pContext); // Subset 가져오기

	if ((m_nSubMeshes > 0) && (nSubSet < m_nSubMeshes))
	{
		pd3dCommandList->IASetIndexBuffer(&m_pd3dSubSetIndexBufferViews[nSubSet]);
		pd3dCommandList->DrawIndexedInstanced(m_pnSubSetIndices[nSubSet], 1, 0, 0, 0);
	}
	else//m_nOffSet 추가 고민중
		pd3dCommandList->DrawInstanced(m_nVertices, 1, 0, 0);
}

void CObjectMeshComponent::LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile)
{

	BYTE nStrLength = 0;

	int nDatas = 0;

	UINT nReads = (UINT)fread(&m_nVertices, sizeof(int), 1, pInFile);
	nReads = (UINT)fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	//m_stMeshName.resize(nStrLength + 1);
	nReads = (UINT)fread(m_stMeshName, sizeof(char), nStrLength, pInFile);
	m_stMeshName[nStrLength] = '\0';

	while (1)
	{
		char pstrToken[64] = { '\0' };
		//char pstrTokenName[64] = { '\0' };
		nReads = (UINT)fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)fread(&pstrToken[0], sizeof(char), nStrLength, pInFile);


#define _WITH_DISPLAY_MESH_LOAD_TORKEN_NAME

#ifdef  _WITH_DISPLAY_MESH_LOAD_TORKEN_NAME

		static int nTextures = 0, nRepeatedTextures = 0;
		TCHAR pstrDebug[256] = { 0 };
		_stprintf_s(pstrDebug, 256, _T("Texture Name: %s, Read Data: %d \n"), pstrToken, m_nVertices);
		OutputDebugString(pstrDebug);
#endif
		
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&m_xmf3AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&m_xmf3AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
		else
		{
			nReads = (UINT)::fread(&nDatas, sizeof(int), 1, pInFile);
			if (nDatas <= 0)
				continue;
			if (!strcmp(pstrToken, "<Positions>:"))
			{
				m_nType |= VERTEXT_POSITION;
				m_pxmf3Positions.resize(nDatas);

				nReads = fread(&m_pxmf3Positions[0], sizeof(XMFLOAT3), nDatas, pInFile);

				m_pd3dPositionBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &m_pxmf3Positions[0], sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dPositionUploadBuffer.GetAddressOf());

				m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
				m_d3dPositionBufferView.StrideInBytes = sizeof(XMFLOAT3);
				m_d3dPositionBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
			}
			else if (!strcmp(pstrToken, "<Colors>:"))
			{
				m_nType |= VERTEXT_COLOR;
				m_pxmf4Colors.resize(nDatas);
				nReads = fread(&m_pxmf4Colors[0], sizeof(XMFLOAT4), nDatas, pInFile);
			}
			else if (!strcmp(pstrToken, "<TextureCoords0>:"))
			{
				m_nType |= VERTEXT_TEXTURE_COORD0;
				m_pxmf2TextureCoords0.resize(nDatas);
				nReads = fread(&m_pxmf2TextureCoords0[0], sizeof(XMFLOAT2), nDatas, pInFile);

				m_pd3dTextureCoord0Buffers = CreateBufferResource(pd3dDevice, pd3dCommandList, &m_pxmf2TextureCoords0[0], sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dTextureCoord0UploadBuffer.GetAddressOf());
				
				m_d3dTextureCoord0BufferView.BufferLocation = m_pd3dTextureCoord0Buffers->GetGPUVirtualAddress();
				m_d3dTextureCoord0BufferView.StrideInBytes = sizeof(XMFLOAT2);
				m_d3dTextureCoord0BufferView.SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;
			}
			else if (!strcmp(pstrToken, "<TextureCoords1>:"))
			{
				m_nType |= VERTEXT_TEXTURE_COORD1;
				m_pxmf2TextureCoords1.resize(nDatas);
				nReads = fread(&m_pxmf2TextureCoords1[0], sizeof(XMFLOAT2), nDatas, pInFile);

				m_pd3dTextureCoord1Buffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &m_pxmf2TextureCoords1[0], sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dTextureCoord1UploadBuffer.GetAddressOf());

				m_d3dTextureCoord1BufferView.BufferLocation = m_pd3dTextureCoord1Buffer->GetGPUVirtualAddress();
				m_d3dTextureCoord1BufferView.StrideInBytes = sizeof(XMFLOAT2);
				m_d3dTextureCoord1BufferView.SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;
			}
			else if (!strcmp(pstrToken, "<Normals>:"))
			{
				m_nType |= VERTEXT_NORMAL;
				m_pxmf3Normals.resize(nDatas);
				nReads = fread(&m_pxmf3Normals[0], sizeof(XMFLOAT3), nDatas, pInFile);

				m_pd3dNormalBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &m_pxmf3Normals[0], sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dNormalUploadBuffer.GetAddressOf());

				m_d3dNormalBufferView.BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress();
				m_d3dNormalBufferView.StrideInBytes = sizeof(XMFLOAT3);
				m_d3dNormalBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
			}
			else if (!strcmp(pstrToken, "<Tangents>:"))
			{
				m_nType |= VERTEXT_TANGENT;//VERTEXT_TANGENT
				m_pxmf3Tangents.resize(nDatas);
				nReads = fread(&m_pxmf3Tangents[0], sizeof(XMFLOAT3), nDatas, pInFile);

				m_pd3dTangentBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &m_pxmf3Tangents[0], sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dTangentUploadBuffer.GetAddressOf());

				m_d3dTangentBufferView.BufferLocation = m_pd3dTangentBuffer->GetGPUVirtualAddress();
				m_d3dTangentBufferView.StrideInBytes = sizeof(XMFLOAT3);
				m_d3dTangentBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
			}
			else if (!strcmp(pstrToken, "<BiTangents>:"))
			{
				m_pxmf3BiTangents.resize(nDatas);
				nReads = fread(&m_pxmf3BiTangents[0], sizeof(XMFLOAT3), nDatas, pInFile);

				m_pd3dBiTangentBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, &m_pxmf3BiTangents[0], sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_pd3dBiTangentUploadBuffer.GetAddressOf());

				m_d3dBiTangentBufferView.BufferLocation = m_pd3dBiTangentBuffer->GetGPUVirtualAddress();
				m_d3dBiTangentBufferView.StrideInBytes = sizeof(XMFLOAT3);
				m_d3dBiTangentBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
			}
			else if (!strcmp(pstrToken, "<SubMeshes>:"))
			{
				m_nSubMeshes = nDatas;

				m_pnSubSetIndices.resize(m_nSubMeshes);
				m_ppnSubSetIndices.resize(m_nSubMeshes);

				m_ppd3dSubSetIndexBuffers.resize(m_nSubMeshes);
				m_ppd3dSubSetIndexUploadBuffers.resize(m_nSubMeshes);
				m_pd3dSubSetIndexBufferViews.resize(m_nSubMeshes);

				for (int i = 0; i < m_nSubMeshes; i++)
				{
					nReads = fread(&nStrLength, sizeof(BYTE), 1, pInFile);

					nReads = fread(pstrToken, sizeof(char), nStrLength, pInFile);
					pstrToken[nStrLength] = '\0';

					if (!strcmp(pstrToken, "<SubMesh>:")) // 분명 여기 내부에 문제가 있음
					{
						int nSubsetIndces;
						int nIndex = 0;
						nReads = fread(&nIndex, sizeof(int), 1, pInFile);
						nReads = fread(&nSubsetIndces, sizeof(int), 1, pInFile);
						m_pnSubSetIndices[i] = nSubsetIndces;
						if (nSubsetIndces <= 0)
							continue;

						m_ppnSubSetIndices[i].resize(m_pnSubSetIndices[i]);
						nReads = (UINT)::fread(&m_ppnSubSetIndices[i][0], sizeof(UINT) * m_pnSubSetIndices[i], 1, pInFile);

						m_ppd3dSubSetIndexBuffers[i] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, &m_ppnSubSetIndices[i][0], sizeof(UINT) * m_pnSubSetIndices[i], D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, m_ppd3dSubSetIndexUploadBuffers[i].GetAddressOf());

						m_pd3dSubSetIndexBufferViews[i].BufferLocation = m_ppd3dSubSetIndexBuffers[i]->GetGPUVirtualAddress();
						m_pd3dSubSetIndexBufferViews[i].Format = DXGI_FORMAT_R32_UINT;
						m_pd3dSubSetIndexBufferViews[i].SizeInBytes = sizeof(UINT) * m_pnSubSetIndices[i];
					}
				}
			}
		}
	}
}

void CObjectMeshComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{

	UINT nSubSet = 0;
	if (pContext) nSubSet = *static_cast<UINT*>(pContext); // Subset 가져오기

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[5] = { m_d3dPositionBufferView, m_d3dTextureCoord0BufferView, m_d3dNormalBufferView, m_d3dTangentBufferView, m_d3dBiTangentBufferView };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 5, pVertexBufferViews);

	if ((m_nSubMeshes > 0) && (nSubSet < m_nSubMeshes))
	{
		pd3dCommandList->IASetIndexBuffer(&(m_pd3dSubSetIndexBufferViews[nSubSet]));
		pd3dCommandList->DrawIndexedInstanced(m_pnSubSetIndices[nSubSet], 1, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}
