#include "PlayerGameObject.h"

std::shared_ptr<CBombGameObject> CPlayerGameObject::GetBombGameObject()
{
	return m_pPlayersGameObject;
}

void CPlayerGameObject::SetboolMove(bool bMove)
{
	m_bMove = bMove;
}

void CPlayerGameObject::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent, std::shared_ptr<CTerrainObject> pTerrainObject)
{
	CGameObject::Update(fTimeElapsed, pxmf4x4Parent); // 동료, 자식 업데이트

	if (m_pPlayersGameObject)
	{
		m_pPlayersGameObject->Update(fTimeElapsed, GetWorldPosition(), pTerrainObject);
		m_pPlayersGameObject->UpdateCollisionBox(m_pPlayersGameObject->GetPosition());
	}
}

void CPlayerGameObject::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CPlayerGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	CGameObject::PrepareRender(pd3dCommandList);
	CGameObject::Render(pd3dCommandList, pCamera, pxmf4x4World);

	if (m_pPlayersGameObject)
	{
		m_pPlayersGameObject->PrepareRender(pd3dCommandList);
		m_pPlayersGameObject->Render(pd3dCommandList, pCamera, NULL);
	}
}

//void CPlayerGameObject::LoadPlayerFrameData()
//{
//	pRightLegGameObject[0] = FindFrame("bobomb_Skeleton_8");
//	pRightLegGameObject[1] = FindFrame("bobomb_Skeleton_10");
//	pRightLegGameObject[2] = FindFrame("bobomb_Skeleton_12");
//
//	pLeftLegGameObject[0] = FindFrame("bobomb_Skeleton_9");
//	pLeftLegGameObject[1] = FindFrame("bobomb_Skeleton_11");
//	pLeftLegGameObject[2] = FindFrame("bobomb_Skeleton_13");
//
//	pTopGameObject = FindFrame("bobomb_Skeleton_2");
//	pSpringGameObject[0] = FindFrame("bobomb_Skeleton_15");
//	pSpringGameObject[1] = FindFrame("bobomb_Skeleton_17");
//}
//void CPlayerGameObject::UpdateFrame(float fTimeElapsed)
//{
//	if(m_bMove)
//		UpdateLegFrame(fTimeElapsed);
//}
//
//void CPlayerGameObject::UpdateLegFrame(float fTimeElapsed)
//{
//	static float angle = 0.f;
//	angle += fTimeElapsed;
//	for (int i = 0; i < 3; i++)
//	{
//
//		// Leg
//		DirectX::XMMATRIX m_xmLegTranslationMatrix1;
//		DirectX::XMMATRIX m_xmLegTranslationMatrix2;
//		m_fRotationAngle += fTimeElapsed * m_fSpeed;
//		pRightLegGameObject[i]->m_xmRotationMatrix = DirectX::XMMatrixRotationX(m_fRotationAngle);
//		pLeftLegGameObject[i]->m_xmRotationMatrix = DirectX::XMMatrixRotationX(-m_fRotationAngle);
//
//		// Spring
//		DirectX::XMMATRIX m_xmSpringTranslationMatrix = DirectX::XMMatrixTranslation(0.f, -15.f, 0.f);
//		DirectX::XMMATRIX m_xmSprinTranslationMatrix2 = DirectX::XMMatrixTranslation(0.f, 15.f, 0.f);
//		pSpringGameObject[0]->m_xmRotationMatrix = m_xmSpringTranslationMatrix * DirectX::XMMatrixRotationZ(angle) * m_xmSprinTranslationMatrix2;/* * m_xmTranslationMatrix*/;
//		pSpringGameObject[1]->m_xmRotationMatrix = m_xmSpringTranslationMatrix * DirectX::XMMatrixRotationZ(angle) * m_xmSprinTranslationMatrix2;/* * m_xmTranslationMatrix*/;
//		if (abs(m_fRotationAngle) > 0.55f)
//		{
//			if (m_fRotationAngle > 0.f) m_fRotationAngle = 0.55f;
//			else m_fRotationAngle = -0.55f;
//
//			m_fSpeed *= -1;
//		}
//	}
//}

void CPlayerGameObject::UpdateSpringFrame(float fTimeElapsed)
{
}

void CPlayerGameObject::LoadPlayerBombObject(std::shared_ptr<CBombGameObject> pAttackGameObject)
{
	m_pPlayersGameObject = pAttackGameObject;
	m_pPlayersGameObject->SettingVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void CPlayerGameObject::MoveBomb(float fTimeElapsed, XMFLOAT3 xmf3Direction)
{
	m_pPlayersGameObject->SetDirection(xmf3Direction);
	m_pPlayersGameObject->SetBombState(BombSTATE::BombBurn);
}

void CPlayerGameObject::Acceleration(float fTimeElapsed)
{
	XMFLOAT3 xmf3Velocity = m_pPlayersGameObject->Acceleration(fTimeElapsed); // 속도를 올림
	if (xmf3Velocity.y >= MAX_VELOCITY)
		m_pPlayersGameObject->SetVelocity(XMFLOAT3(MIN_VELOCITY, MIN_VELOCITY, MIN_VELOCITY));
}

XMFLOAT3 CPlayerGameObject::GetBombPosition()
{
	if (m_pPlayersGameObject)
		m_pPlayersGameObject->GetPosition();
	return XMFLOAT3();
}
