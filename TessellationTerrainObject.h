#pragma once
#include "GameObject.h"
#include "TessellationTerrainMeshComponent.h"
#include "HeightMapGridMesh.h"
class CTessellationTerrainObject : public CGameObject
{
public:
	CTessellationTerrainObject() {};
	~CTessellationTerrainObject() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, CDescriptorHeap* pDescriptorHeap, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent);

	float GetHeight(float x, float z);
	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetTerrainTessellationHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z), m_xmf3Scale)); }

	//virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	//virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

	int GetRawImageWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetRawImageLength() { return(m_pHeightMapImage->GetHeightMapLength()); }

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }

	int GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	int GetLength() { return(m_nLength * m_xmf3Scale.z); }
private:
	std::shared_ptr<CHeightMapImage>	m_pHeightMapImage;

	int								m_nWidth;
	int								m_nLength;

	XMFLOAT3						m_xmf3Scale;
	float							m_fOffSetPositoin = 0.f;
};