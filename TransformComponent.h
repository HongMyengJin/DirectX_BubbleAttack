#pragma once
#include "stdafx.h"
#include "Component.h"
class CTransformComponent : public CComponent
{
public:
	CTransformComponent() {};
	~CTransformComponent() {};

	virtual void Init();
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera = nullptr, void* pContext = nullptr);
	virtual void PostRender();


	void SetPosition(float x, float y, float z) { m_xmf4x4Transform._41 = x, m_xmf4x4Transform._42 = y, m_xmf4x4Transform._43 = z; }
	void SetPosition(XMFLOAT3 xmf3Positoin) { SetPosition(xmf3Positoin.x, xmf3Positoin.y, xmf3Positoin.z); }

	void SetScale(float x, float y, float z);
	void MulScale(XMFLOAT3 xmf3Scale);

	void Rotate(float fPitch, float fYaw, float fRoll);


	XMFLOAT3 GetPosition() { return(XMFLOAT3(m_xmf4x4Transform._41, m_xmf4x4Transform._42, m_xmf4x4Transform._43)); };
	XMFLOAT3 GetLook() { return(XMFLOAT3(m_xmf4x4Transform._31, m_xmf4x4Transform._32, m_xmf4x4Transform._33)); };
	XMFLOAT3 GetUp() { return(XMFLOAT3(m_xmf4x4Transform._21, m_xmf4x4Transform._22, m_xmf4x4Transform._23)); };
	XMFLOAT3 GetRight() { return(XMFLOAT3(m_xmf4x4Transform._11, m_xmf4x4Transform._12, m_xmf4x4Transform._13)); };

	XMFLOAT3 GetWorldPosition() { return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43)); };
	// XMFLOAT4X4 UpdateTransform(XMFLOAT4X4* pxmf4x4Parent);
public :
	XMFLOAT4X4	m_xmf4x4Transform = Matrix4x4::Identity();
	XMFLOAT4X4	m_xmf4x4World = Matrix4x4::Identity();
	XMFLOAT3	m_xmfScale = XMFLOAT3(1.f, 1.f, 1.f);
};

