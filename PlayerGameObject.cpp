#include "PlayerGameObject.h"

std::shared_ptr<CBombGameObject> CPlayerGameObject::GetBombGameObject()
{
	return m_pPlayerBombGameObject;
}

void CPlayerGameObject::SetboolMove(bool bMove)
{
	m_bMove = bMove;
}

void CPlayerGameObject::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent, std::shared_ptr<CTerrainObject> pTerrainObject)
{
	CGameObject::Update(fTimeElapsed, pxmf4x4Parent); // 동료, 자식 업데이트

	if (m_pPlayerBombGameObject)
	{
		m_pPlayerBombGameObject->Update(fTimeElapsed, GetWorldPosition(), pTerrainObject);
		m_pPlayerBombGameObject->UpdateCollisionBox(m_pPlayerBombGameObject->GetPosition());
	}
}

void CPlayerGameObject::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CPlayerGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	//CGameObject::PrepareRender(pd3dCommandList);
	CGameObject::Render(pd3dCommandList, pCamera, pxmf4x4World);

	//if (m_pPlayerBombGameObject)
	//{
	//	m_pPlayerBombGameObject->PrepareRender(pd3dCommandList);
	//	m_pPlayerBombGameObject->Render(pd3dCommandList, pCamera, NULL);
	//}
}

void CPlayerGameObject::LoadPlayerFrameData()
{
	pGameObject[0] = FindFrame("BP_Brother_Penguin_A_1");
	pGameObject[1] = FindFrame("BP_Brother_Penguin_A_2");
	pGameObject[2] = FindFrame("BP_Brother_Penguin_A_3");
	pGameObject[3] = FindFrame("BP_Brother_Penguin_A_4");
	pGameObject[4] = FindFrame("BP_Brother_Penguin_A_5");
	pGameObject[5] = FindFrame("BP_Brother_Penguin_A_6");
	pGameObject[6] = FindFrame("BP_Brother_Penguin_A_7");
}
void CPlayerGameObject::UpdateFrame(float fTimeElapsed)
{
	if(m_bMove)
		UpdateLegFrame(fTimeElapsed);
}

void CPlayerGameObject::UpdateLegFrame(float fTimeElapsed)
{
	//static float angle = 0.f;
	//angle += fTimeElapsed;
	//for (int i = 0; i < 7; i++)
	//{
	//	pGameObject[i]->m_xmTranslationMatrix = DirectX::XMMatrixTranslation(0.f, 10.f, 0.f);
	//}
}
	//for (int i = 0; i < 3; i++)
	//{

	//	// Leg
	//	DirectX::XMMATRIX m_xmLegTranslationMatrix1;
	//	DirectX::XMMATRIX m_xmLegTranslationMatrix2;
	//	m_fRotationAngle += fTimeElapsed * m_fSpeed;
	//	pRightLegGameObject[i]->m_xmRotationMatrix = DirectX::XMMatrixRotationX(m_fRotationAngle);
	//	pLeftLegGameObject[i]->m_xmRotationMatrix = DirectX::XMMatrixRotationX(-m_fRotationAngle);

	//	// Spring
	//	DirectX::XMMATRIX m_xmSpringTranslationMatrix = DirectX::XMMatrixTranslation(0.f, -15.f, 0.f);
	//	DirectX::XMMATRIX m_xmSprinTranslationMatrix2 = DirectX::XMMatrixTranslation(0.f, 15.f, 0.f);
	//	pSpringGameObject[0]->m_xmRotationMatrix = m_xmSpringTranslationMatrix * DirectX::XMMatrixRotationZ(angle) * m_xmSprinTranslationMatrix2;/* * m_xmTranslationMatrix*/;
	//	pSpringGameObject[1]->m_xmRotationMatrix = m_xmSpringTranslationMatrix * DirectX::XMMatrixRotationZ(angle) * m_xmSprinTranslationMatrix2;/* * m_xmTranslationMatrix*/;
	//	if (abs(m_fRotationAngle) > 0.55f)
	//	{
	//		if (m_fRotationAngle > 0.f) m_fRotationAngle = 0.55f;
	//		else m_fRotationAngle = -0.55f;

	//		m_fSpeed *= -1;
	//	}
	//}

void CPlayerGameObject::UpdateSpringFrame(float fTimeElapsed)
{
}

void CPlayerGameObject::LoadPlayerBombObject(std::shared_ptr<CBombGameObject> pAttackGameObject)
{
	m_pPlayerBombGameObject = pAttackGameObject;
	m_pPlayerBombGameObject->SetOffSetPosition(XMFLOAT3(-43.f, 27.f, 5.f));
	m_pPlayerBombGameObject->SettingVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void CPlayerGameObject::MoveBomb(float fTimeElapsed, XMFLOAT3 xmf3Direction)
{
	m_pPlayerBombGameObject->SetDirection(xmf3Direction);
	m_pPlayerBombGameObject->SetBombState(BombSTATE::BombBurn);
}

void CPlayerGameObject::Acceleration(float fTimeElapsed)
{
	XMFLOAT3 xmf3Velocity = m_pPlayerBombGameObject->Acceleration(fTimeElapsed); // 속도를 올림
	if (xmf3Velocity.y >= MAX_VELOCITY)
		m_pPlayerBombGameObject->SetVelocity(XMFLOAT3(MIN_VELOCITY, MIN_VELOCITY, MIN_VELOCITY));
}

XMFLOAT3 CPlayerGameObject::GetBombPosition()
{
	if (m_pPlayerBombGameObject)
		m_pPlayerBombGameObject->GetPosition();
	return XMFLOAT3();
}
