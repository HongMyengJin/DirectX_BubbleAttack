#include "TransformComponent.h"
#include "GameObject.h"
void CTransformComponent::Init()
{
}

void CTransformComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
	m_xmf4x4World = (pData) ? Matrix4x4::Multiply(m_xmf4x4Transform, *static_cast<XMFLOAT4X4*>(pData)) : m_xmf4x4Transform;
}
void CTransformComponent::PrepareRender()
{
}

void CTransformComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void CTransformComponent::PostRender()
{
}

void CTransformComponent::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxScale, m_xmf4x4Transform);
}

void CTransformComponent::SetScale(XMFLOAT3 xmf3Scale)
{
	XMMATRIX mtxScale = XMMatrixScaling(xmf3Scale.x, xmf3Scale.y, xmf3Scale.z);
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxScale, m_xmf4x4Transform);
}

void CTransformComponent::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);
}
