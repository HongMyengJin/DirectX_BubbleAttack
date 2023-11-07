#pragma once
#include "GameObject.h"
class CUIGameObject : public CGameObject
{
public:
	CUIGameObject() {};
	~CUIGameObject() {};
public:
	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, XMFLOAT2 fSize, wchar_t* pstrFileName);
	virtual void Animate(float fTimeElapsed);

	void SetScreenPosition(XMFLOAT2 xmf2Position) {m_xmf2ScreenPosition = xmf2Position;}
	void SetSize(XMFLOAT2 xmf2Size) { m_xmf2Size = xmf2Size; }

protected:
	XMFLOAT2 m_xmf2Size = XMFLOAT2(100.f, 100.f);
	XMFLOAT2 m_xmf2ScreenPosition = XMFLOAT2(0.f, 0.f);
};



class CUINumberGameObject : public CUIGameObject
{
public:
	CUINumberGameObject() {};
	~CUINumberGameObject() {};
public:
	void UpdateNumber(UINT iNumber);
	void UpdateNumberTexture(UINT Number, UINT Order);

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

private:

	std::vector<UINT>	m_vecNumber;
	UINT				m_iNumber;
};

