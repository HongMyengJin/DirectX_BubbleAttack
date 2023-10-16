#include "TransformComponent.h"

void CTransformComponent::Init()
{
}

void CTransformComponent::Update(float fTimeElapsed)
{
}

void CTransformComponent::PrepareRender()
{
}

void CTransformComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, void* pContext)
{
}

void CTransformComponent::PostRender()
{
}

void CTransformComponent::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);
}
