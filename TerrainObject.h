#pragma once
#include "GameObject.h"
#include "HeightMapGridMesh.h"
//class CHeightMapImage
//{
//private:
//	std::vector<BYTE>			m_HeightMapPixels;
//	int							m_nWidth;
//	int							m_nLength;
//	XMFLOAT3					m_xmf3Scale;
//
//public:
//	CHeightMapImage();
//	~CHeightMapImage();
//
//	virtual void Init(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
//	float GetHeight(float x, float z, bool bReverseQuad = false);
//	XMFLOAT3 GetHeightMapNormal(int x, int z);
//	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
//
//	BYTE* GetHeightMapPixels() { return(&m_HeightMapPixels[0]); }
//	int GetHeightMapWidth() { return(m_nWidth); }
//	int GetHeightMapLength() { return(m_nLength); }
//};

class CTerrainObject : public CGameObject
{
public:
	CTerrainObject() {};
	~CTerrainObject() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, CDescriptorHeap* pDescriptorHeap, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent);

	//virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	//virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

private:
	std::shared_ptr<CHeightMapImage>	m_pHeightMapImage;

	int								m_nWidth;
	int								m_nLength;

	XMFLOAT3						m_xmf3Scale;
};

