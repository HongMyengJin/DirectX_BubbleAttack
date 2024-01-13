#include "ProjectileGameObject.h"

void CProjectileGameObject::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::PrepareRender(pd3dCommandList);
}

void CProjectileGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	CGameObject::Render(pd3dCommandList, pCamera, pxmf4x4World);
	//if (m_pSpriteEffect)
	//{
	//	m_pSpriteEffect->PrepareRender(pd3dCommandList);
	//	m_pSpriteEffect->Render(pd3dCommandList, pCamera, pxmf4x4World);
	//}
}

void CProjectileGameObject::SetSpriteEffect(std::shared_ptr<CTextureRectObject> pSpriteEffect, XMFLOAT3 xmf3OffSet)
{
	m_pSpriteEffect = pSpriteEffect;
	SetSpriteOffSet(xmf3OffSet);
}

XMFLOAT3 CProjectileGameObject::Acceleration(float fTimeElapsed)
{
	float fAccelerationValue = m_fAccelerationSpeed * fTimeElapsed;
	DirectX::XMStoreFloat3(&m_xmfVelocity, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmfVelocity), DirectX::XMLoadFloat3(&XMFLOAT3(fAccelerationValue, fAccelerationValue, fAccelerationValue))));
	return m_xmfVelocity;
}

void CProjectileGameObject::Deceleration(float fTimeElapsed)
{
	float fAccelerationValue = m_fAccelerationSpeed * fTimeElapsed;
	DirectX::XMStoreFloat3(&m_xmfVelocity, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_xmfVelocity), DirectX::XMLoadFloat3(&XMFLOAT3(-fAccelerationValue, -fAccelerationValue, -fAccelerationValue))));
}

XMFLOAT3 CProjectileGameObject::GetVelocity()
{
	return m_xmfVelocity;
}
