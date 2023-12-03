#pragma once
#include "Mesh.h"
class CTessellationGridMeshComponent : public CMeshComponent
{
public:
	CTessellationGridMeshComponent() {}
	~CTessellationGridMeshComponent() {}

	void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void* pContext = NULL, XMINT2 xmi2ImageScale = XMINT2(1, 1));
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr); // UINT nSubset
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	virtual XMFLOAT4 OnGetColor(int x, int z, void* pContext) { return(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)); }

protected:
	UINT						m_nStride;
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_xmf3Scale;
	std::vector<UINT>			m_nIndice;
};

