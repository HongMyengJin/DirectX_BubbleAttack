#include "BombGameObject.h"



void CBombGameObject::SetBombState(BombSTATE eBombState)
{
	if (m_eBombState == eBombState) // 똑같은 스테이트일때는 설정 바꾸지XS
		return;
	m_eBombState = eBombState;

	switch (eBombState) // 설정값 셋팅
	{
	case BombSTATE::BombWait:
		m_xmfVelocity = m_xmfSettingVelocity;
		break;
	case BombSTATE::BombBurn:
		break;
	case BombSTATE::BombPop:
		break;
	case BombSTATE::BombDead:
		break;
	case BombSTATE::BombEnd:
		break;
	default:
		break;
	}
}

void CBombGameObject::Update(float fTimeElapsed, XMFLOAT3 xmf3Position, std::shared_ptr<CTerrainObject> pTerrainObject)
{
	if (m_pSpriteEffect)
		m_pSpriteEffect->SetWorldPosition(XMFLOAT3(GetPosition().x + xmfOffSetSpritePosition.x, GetPosition().y + xmfOffSetSpritePosition.y, GetPosition().z + xmfOffSetSpritePosition.z));
	UpdateCollisionBox(GetPosition());
	switch (m_eBombState)
	{
	case BombSTATE::BombWait:
		m_pSpriteEffect->AnimateUV(fTimeElapsed); // 불타는것
		SetPosition(XMFLOAT3(xmf3Position.x + xmfOffSetPosition.x, xmf3Position.y + xmfOffSetPosition.y, xmf3Position.z + xmfOffSetPosition.z));
		break;
	case BombSTATE::BombBurn:
	{

		m_pSpriteEffect->AnimateUV(fTimeElapsed); // 불타는것
		if (pTerrainObject->GetHeight(GetPosition().x, GetPosition().z) - 200.f > GetPosition().y - 15.f)
		{
			SetPosition(XMFLOAT3(xmf3Position.x + xmfOffSetPosition.x, xmf3Position.y + xmfOffSetPosition.y, xmf3Position.z + xmfOffSetPosition.z));
			SetBombState(BombSTATE::BombWait); // 다시 기본 상태로

		}
		else
		{
			Move(fTimeElapsed);
		}

		break;
	}
	case BombSTATE::BombPop:
		break;
	case BombSTATE::BombDead:
		break;
	case BombSTATE::BombEnd:
		break;
	default:
		break;
	}
}


void CBombGameObject::Move(float fTimeElapsed)
{	
	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position.x += m_xmfVelocity.x * m_xmf3Direction.x * fTimeElapsed * m_fSpeed;
	xmf3Position.y += m_xmfVelocity.y * fTimeElapsed * m_fSpeed;
	xmf3Position.z += m_xmfVelocity.z * m_xmf3Direction.z * fTimeElapsed * m_fSpeed;
	m_xmfVelocity.y -= 9.8f * fTimeElapsed;
	SetPosition(xmf3Position);
}