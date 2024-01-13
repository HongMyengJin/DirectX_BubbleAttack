#include "MonsterGameObject.h"
#include "TransformComponent.h"

void CMonsterGameObject::MoveBomb(float fTimeElapsed, XMFLOAT3 xmf3Direction)
{
	m_pBombObject->SetDirection(xmf3Direction);
	m_pBombObject->SetBombState(BombSTATE::BombBurn);
	m_pBombObject->SettingVelocity(XMFLOAT3(8.f, 8.f, 8.f));
}

void CMonsterGameObject::LoadMonsterBombObject(std::shared_ptr<CBombGameObject> pAttackGameObject)
{
	m_pBombObject = pAttackGameObject;
	m_pBombObject->SetOffSetPosition(XMFLOAT3(-45.f, 25.f, 12.f));
}

void CMonsterGameObject::Animate(float fTimeElapsed)
{
}

void CMonsterGameObject::Update(float fTimeElapsed, XMFLOAT3 xmfTargetPosition, std::shared_ptr<CTerrainObject> fTerrainObject)
{
	XMFLOAT3 xmf3Distance;
	DirectX::XMStoreFloat3(&xmf3Distance, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&xmfTargetPosition) - DirectX::XMLoadFloat3(&GetPosition())));
	if(xmf3Distance.x < 200.f/* && (m_pMonstersGameObject->GetBombState() != BombSTATE::BombWait)*/)
		MoveBomb(0.001f, GetLookVector());

		
	if (m_pBombObject)
	{
		m_pBombObject->Update(fTimeElapsed, GetWorldPosition(), fTerrainObject);
		m_pBombObject->UpdateCollisionBox(m_pBombObject->GetPosition());
	}

	XMFLOAT3 xmf3StartPosition = GetPosition(); // 출발 지점
	XMVECTOR xmVectorTargetPosition = { xmfTargetPosition.x, xmfTargetPosition.y, xmfTargetPosition.z }; // 도착 지점
	XMVECTOR xmVectorPosition = { xmf3StartPosition.x, xmf3StartPosition.y, xmf3StartPosition.z }; // 벡터로 변환

	XMFLOAT3 MoveDirection;
	DirectX::XMStoreFloat3(&MoveDirection, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(xmVectorTargetPosition, xmVectorPosition))); // 목표 방향 벡터
	//DirectX::XMStoreFloat3(&MoveDirection, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&MoveDirection), XMVECTOR{ -1.f , -1.f, -1.f }));

	MoveDirection.x = -MoveDirection.x;
	if (!XMVector3Equal(DirectX::XMLoadFloat3(&(MoveDirection)), XMVectorZero()))
	{
		DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixLookToLH(DirectX::XMVectorZero(), DirectX::XMLoadFloat3(&(MoveDirection)), DirectX::XMVectorSet(0, 1, 0, 0));

		XMFLOAT4X4 RotationMatrixs;
		DirectX::XMStoreFloat4x4(&RotationMatrixs, RotationMatrix);
		XMFLOAT4X4 xmf4x4Transform = dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->m_xmf4x4Transform;

		XMFLOAT3 xmf3Right, xmf3Up, xmf3Look, xmf3Position;

		memcpy(&xmf3Right.x, &RotationMatrixs._11, sizeof(XMFLOAT3));
		memcpy(&xmf3Up.x, &RotationMatrixs._21, sizeof(XMFLOAT3));
		memcpy(&xmf3Look.x, &RotationMatrixs._31, sizeof(XMFLOAT3));
		memcpy(&xmf3Position.x, &xmf4x4Transform._41, sizeof(XMFLOAT3));


		m_xmf3Position = xmf3Position;


		memcpy(&xmf4x4Transform._11, &xmf3Right.x, sizeof(XMFLOAT3));
		memcpy(&xmf4x4Transform._21, &xmf3Up.x, sizeof(XMFLOAT3));
		memcpy(&xmf4x4Transform._31, &xmf3Look.x, sizeof(XMFLOAT3));

		dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->m_xmf4x4Transform = xmf4x4Transform;

		SetLookVector(xmf3Look);
		MoveDirection.x = -MoveDirection.x;
		if(!m_bCollision)
			Move(MoveDirection, 0.5f);
	}
	UpdateCollisionBox(GetPosition());
}

void CMonsterGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	CGameObject::Render(pd3dCommandList, pCamera, pxmf4x4World);
}


std::shared_ptr<CBombGameObject> CMonsterGameObject::GetBombGameObject()
{
	return m_pBombObject;
}
