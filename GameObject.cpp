#include "GameObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "ObjectShaderComponent.h"
#include "TextureRectObject.h"
#include "TerrainObject.h"
void CGameObject::Init(XMFLOAT3 xmf3Extent)
{
	m_pComponents.resize(4);
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();
	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CObjectMeshComponent>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_xmf3AABBBox.Extents = xmf3Extent;
}

void CGameObject::Animate(float fTimeElapsed)
{
	if (m_pSiblingObject) m_pSiblingObject->Animate(fTimeElapsed);
	if (m_pChildObject) m_pChildObject->Animate(fTimeElapsed);
}

void CGameObject::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	UpdateCollisionBox(GetPosition()); // CollisionBox 업데이트
	if (m_pSiblingObject)
		m_pSiblingObject->m_pComponents[UINT(ComponentType::ComponentTransform)]->Update(fTimeElapsed, pxmf4x4Parent, nullptr);
	if (m_pChildObject)
		m_pChildObject->m_pComponents[UINT(ComponentType::ComponentTransform)]->Update(fTimeElapsed, &static_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4World, nullptr);
}

void CGameObject::Move(DWORD dwDirection, float fDistance)
{
	XMFLOAT4X4 xmf4x4Transform = dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->m_xmf4x4Transform;
	
	XMFLOAT3 xmf3Right, xmf3Up, xmf3Look, xmf3Position;

	memcpy(&xmf3Right.x, &xmf4x4Transform._11, sizeof(XMFLOAT3));
	memcpy(&xmf3Up.x, &xmf4x4Transform._21, sizeof(XMFLOAT3));
	memcpy(&xmf3Look.x, &xmf4x4Transform._31, sizeof(XMFLOAT3));
	memcpy(&xmf3Position.x, &xmf4x4Transform._41, sizeof(XMFLOAT3));
	
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, xmf3Up, -fDistance);

		XMFLOAT3 Position = dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->GetPosition();
		dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->SetPosition(Vector3::Add(Position, xmf3Shift));
		UpdateTransform(NULL);
	}
}

void CGameObject::Move(XMFLOAT3 xmf3Direction, float fDistance)
{
	XMFLOAT3 xmf3DirectionValue = XMFLOAT3(xmf3Direction.x * fDistance, xmf3Direction.y * fDistance, xmf3Direction.z * fDistance);
	XMFLOAT3 Position = dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->GetPosition();
	dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->SetPosition(Vector3::Add(Position, xmf3DirectionValue));
}

void CGameObject::Rotate(float x, float y, float z)
{
	if (x != 0.0f)
	{
		m_fPitch += x;
		if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
		if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
	}
	if (y != 0.0f)
	{
		m_fYaw += y;
		if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
		if (m_fYaw < 0.0f) m_fYaw += 360.0f;
	}
	if (z != 0.0f)
	{
		m_fRoll += z;
		if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
		if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
	}
	//m_pCamera->Rotate(x, y, z);
	if (y != 0.0f)
	{
		XMFLOAT4X4 xmf4x4Transform = dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->m_xmf4x4Transform;

		XMFLOAT3 xmf3Right, xmf3Up, xmf3Look, xmf3Position;

		memcpy(&xmf3Right.x, &xmf4x4Transform._11, sizeof(XMFLOAT3));
		memcpy(&xmf3Up.x, &xmf4x4Transform._21, sizeof(XMFLOAT3));
		memcpy(&xmf3Look.x, &xmf4x4Transform._31, sizeof(XMFLOAT3));
		memcpy(&xmf3Position.x, &xmf4x4Transform._41, sizeof(XMFLOAT3));

		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		xmf3Look = Vector3::TransformNormal(xmf3Look, xmmtxRotate);
		xmf3Right = Vector3::TransformNormal(xmf3Right, xmmtxRotate);

		m_xmf3Look = Vector3::Normalize(xmf3Look);
		m_xmf3Right = Vector3::CrossProduct(xmf3Up, xmf3Look, true);
		m_xmf3Up = Vector3::CrossProduct(xmf3Look, xmf3Right, true);

		m_xmf3Position = xmf3Position;

		memcpy(&xmf4x4Transform._11, &m_xmf3Right.x, sizeof(XMFLOAT3));
		memcpy(&xmf4x4Transform._21, &m_xmf3Up.x, sizeof(XMFLOAT3));
		memcpy(&xmf4x4Transform._31, &m_xmf3Look.x, sizeof(XMFLOAT3));

		dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->m_xmf4x4Transform = xmf4x4Transform;
		//dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->SetScale(50.f, 50.f, 50.f);

		UpdateTransform(NULL);



	}
}

void CGameObject::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pComponents[UINT(ComponentType::ComponentShader)])
		m_pComponents[UINT(ComponentType::ComponentShader)]->PrepareRender(pd3dCommandList);

	if (m_pSiblingObject)
		m_pSiblingObject->PrepareRender(pd3dCommandList);
	if (m_pChildObject)
		m_pChildObject->PrepareRender(pd3dCommandList);
}

void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	if (!m_bEnable)
		return;
	m_pComponents[UINT(ComponentType::ComponentTransform)]->Render(pd3dCommandList);

	UINT nMaterial = static_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_nMaterials;
	for (int i = 0; i < nMaterial; i++)
	{
		m_pComponents[UINT(ComponentType::ComponentMaterial)]->Render(pd3dCommandList, pCamera, &i);
		m_pComponents[UINT(ComponentType::ComponentMesh)]->Render(pd3dCommandList, pCamera, &i);
	}

	if (m_pSiblingObject)
		m_pSiblingObject->Render(pd3dCommandList, pCamera, pxmf4x4World);
	if (m_pChildObject)
		m_pChildObject->Render(pd3dCommandList, pCamera, pxmf4x4World);

	m_xf3PrePosition = GetPosition();
}

std::shared_ptr<CGameObject> CGameObject::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	return LoadFrameHierarchy(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pDescriptorHeap, pstrFileName, pInFile);
}

std::shared_ptr<CGameObject> CGameObject::LoadFrameHierarchy(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName, FILE* pInFile)
{
	char pstrName[64] = { '\0' };
	BYTE nStrLength = 0;
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;

	// 3개를 합해서 렌더를 해야함(Component 3개를 묶어야함
	std::shared_ptr<CGameObject> pGameObject;
	while (1)
	{
		char pstrName[64] = { '\0' };
		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)::fread(pstrName, sizeof(char), nStrLength, pInFile);
		pstrName[nStrLength] = '\0';
		if (!strcmp(pstrName, "<Frame>:"))
		{
			pGameObject = std::make_shared<CGameObject>();
			pGameObject->Init(XMFLOAT3(0.f, 0.f, 0.f));

			nReads = (UINT)::fread(&nFrame, sizeof(int), 1, pInFile);
			nReads = (UINT)::fread(&nTextures, sizeof(int), 1, pInFile);

			nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pGameObject->m_pstrFrameName, sizeof(char), nStrLength, pInFile);
			pGameObject->m_pstrFrameName[nStrLength] = '\0';
		}
		else if (!strcmp(pstrName, "<Transform>:"))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
		}
		else if (!strcmp(pstrName, "<TransformMatrix>:"))
		{
			XMFLOAT4X4	m_xmf4x4Transform;
			nReads = (UINT)::fread(&(dynamic_cast<CTransformComponent*>(pGameObject->m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4Transform), sizeof(float), 16, pInFile);
		}
		else if (!strcmp(pstrName, "<Mesh>:"))
			dynamic_cast<CObjectMeshComponent*>(pGameObject->m_pComponents[UINT(ComponentType::ComponentMesh)].get())->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);
		else if (!strcmp(pstrName, "<Materials>:"))
			dynamic_cast<CMaterialsComponent*>(pGameObject->m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadMaterialsFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, pInFile);
		else if (!strcmp(pstrName, "<Children>:"))
		{
			int nChilds = 0;
			nReads = (UINT)::fread(&nChilds, sizeof(int), 1, pInFile);

			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					SetChild(pGameObject, CGameObject::LoadFrameHierarchy(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pDescriptorHeap, pstrFileName, pInFile));
				}
			}
		}
		else if (!strcmp(pstrName, "</Frame>"))
			break;
	}
	return(pGameObject);
}

void CGameObject::UpdateTransform(XMFLOAT4X4* pxmf4x4Parent)
{
	XMFLOAT4X4 xmf4x4Transform = static_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4Transform;
	DirectX::XMMATRIX	mf4x4NewTransform =  DirectX::XMLoadFloat4x4(&xmf4x4Transform) * m_xmRotationMatrix * m_xmTranslationMatrix;

	
	static_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(mf4x4NewTransform, *pxmf4x4Parent) : xmf4x4Transform;

	XMFLOAT4X4 xmf4x4World = static_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4World;

	if (m_pSiblingObject) m_pSiblingObject->UpdateTransform(pxmf4x4Parent);
	if (m_pChildObject) m_pChildObject->UpdateTransform(&xmf4x4World);
}

void CGameObject::SetChild(std::shared_ptr<CGameObject> pParentObject, std::shared_ptr<CGameObject> pChildObject)
{
	if (m_pChildObject)
	{
		pChildObject->m_pSiblingObject = m_pChildObject->m_pSiblingObject;
		m_pChildObject->m_pSiblingObject = pChildObject;
	}
	else
	{
		m_pChildObject = pChildObject;
	}

	if (pChildObject) pChildObject->m_pParentObject = pParentObject;
}

void CGameObject::AddShaderComponent(std::shared_ptr<CComponent> pComponent)
{
	m_pComponents[UINT(ComponentType::ComponentShader)] = pComponent;
}

void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->SetPosition(xmf3Position);
	UpdateTransform(NULL);
}

void CGameObject::SetScale(XMFLOAT3 xmf3Scale)
{
	m_xmf3Scale = xmf3Scale;
	dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->MulScale(xmf3Scale);
	UpdateTransform(NULL);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->GetPosition();
}

XMFLOAT3 CGameObject::GetPrePosition()
{
	return m_xf3PrePosition;
}

XMFLOAT3 CGameObject::GetWorldPosition()
{
	return dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->GetWorldPosition();
}

void CGameObject::SetWorldPosition(XMFLOAT3 xmf3Position) // 바로 직접 넣어주는 함수
{
	memcpy(&dynamic_cast<CTransformComponent*>(m_pComponents[(UINT)ComponentType::ComponentTransform].get())->m_xmf4x4World._41, &xmf3Position, sizeof(XMFLOAT3));
}

void CGameObject::Release()
{
	for (int i = 0; i < m_pComponents.size(); i++)
		m_pComponents[i]->Release();
}
CGameObject* CGameObject::FindFrame(char* pstrFrameName)
{
	CGameObject* pObject = this;
	if (!strncmp(m_pstrFrameName, pstrFrameName, strlen(pstrFrameName))) return(pObject);

	if (m_pSiblingObject) if (pObject = (m_pSiblingObject->FindFrame(pstrFrameName))) return(pObject);
	if (m_pChildObject) if (pObject = (m_pChildObject->FindFrame(pstrFrameName))) return(pObject);

	return(NULL);
}

void CGameObject::UpdateCollisionBox(XMFLOAT3 xmf3Center)
{
	m_xmf3AABBBox.Center = xmf3Center;
	//m_xmf3AABBBox.Extents = xmf3Extents;
}

bool CGameObject::CollisionCheck(std::shared_ptr<CGameObject> pGameObject)
{
	DirectX::BoundingBox AABBBox = pGameObject->GetCollisionBox();
	return pGameObject->GetCollisionBox().Intersects(m_xmf3AABBBox);
}

void CPlayerGameObject::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent, std::shared_ptr<CTerrainObject> pTerrainObject)
{
	CGameObject::Update(fTimeElapsed, pxmf4x4Parent); // 동료, 자식 업데이트

	if (m_pPlayersGameObject)
	{
		m_pPlayersGameObject->Update(fTimeElapsed, pTopGameObject->GetWorldPosition(), pTerrainObject);
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

void CPlayerGameObject::LoadPlayerFrameData()
{
	pRightLegGameObject[0] = FindFrame("bobomb_Skeleton_8");
	pRightLegGameObject[1] = FindFrame("bobomb_Skeleton_10");
	pRightLegGameObject[2] = FindFrame("bobomb_Skeleton_12");

	pLeftLegGameObject[0] = FindFrame("bobomb_Skeleton_9");
	pLeftLegGameObject[1] = FindFrame("bobomb_Skeleton_11");
	pLeftLegGameObject[2] = FindFrame("bobomb_Skeleton_13");

	pTopGameObject = FindFrame("bobomb_Skeleton_2");
	pSpringGameObject[0] = FindFrame("bobomb_Skeleton_15");
	pSpringGameObject[1] = FindFrame("bobomb_Skeleton_17");
}
void CPlayerGameObject::UpdateFrame(float fTimeElapsed)
{
	if(m_bMove)
		UpdateLegFrame(fTimeElapsed);
}

void CPlayerGameObject::UpdateLegFrame(float fTimeElapsed)
{
	static float angle = 0.f;
	angle += fTimeElapsed;
	for (int i = 0; i < 3; i++)
	{

		// Leg
		DirectX::XMMATRIX m_xmLegTranslationMatrix1;
		DirectX::XMMATRIX m_xmLegTranslationMatrix2;
		m_fRotationAngle += fTimeElapsed * m_fSpeed;
		pRightLegGameObject[i]->m_xmRotationMatrix = DirectX::XMMatrixRotationX(m_fRotationAngle);
		pLeftLegGameObject[i]->m_xmRotationMatrix = DirectX::XMMatrixRotationX(-m_fRotationAngle);

		// Spring
		DirectX::XMMATRIX m_xmSpringTranslationMatrix = DirectX::XMMatrixTranslation(0.f, -15.f, 0.f);
		DirectX::XMMATRIX m_xmSprinTranslationMatrix2 = DirectX::XMMatrixTranslation(0.f, 15.f, 0.f);
		pSpringGameObject[0]->m_xmRotationMatrix = m_xmSpringTranslationMatrix * DirectX::XMMatrixRotationZ(angle) * m_xmSprinTranslationMatrix2;/* * m_xmTranslationMatrix*/;
		pSpringGameObject[1]->m_xmRotationMatrix = m_xmSpringTranslationMatrix * DirectX::XMMatrixRotationZ(angle) * m_xmSprinTranslationMatrix2;/* * m_xmTranslationMatrix*/;
		if (abs(m_fRotationAngle) > 0.55f)
		{
			if (m_fRotationAngle > 0.f) m_fRotationAngle = 0.55f;
			else m_fRotationAngle = -0.55f;

			m_fSpeed *= -1;
		}
	}
}

void CPlayerGameObject::UpdateSpringFrame(float fTimeElapsed)
{
}

void CPlayerGameObject::LoadPlayerBombObject(std::shared_ptr<CBombGameObject> pAttackGameObject)
{
	m_pPlayersGameObject = pAttackGameObject;
}

void CPlayerGameObject::MoveBomb(float fTimeElapsed, XMFLOAT3 xmf3Direction)
{
	m_pPlayersGameObject->SetDirection(xmf3Direction);
	m_pPlayersGameObject->SetBombState(BombSTATE::BombBurn);
}

XMFLOAT3 CPlayerGameObject::GetBombPosition()
{
	if (m_pPlayersGameObject)
		m_pPlayersGameObject->GetPosition();
	return XMFLOAT3();
}

std::shared_ptr<CBombGameObject> CPlayerGameObject::GetBombGameObject()
{
	return m_pPlayersGameObject;
}

void CPlayerGameObject::SetboolMove(bool bMove)
{
	m_bMove = bMove;
}

void CBombGameObject::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::PrepareRender(pd3dCommandList);
		
}

void CBombGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	CGameObject::Render(pd3dCommandList, pCamera, pxmf4x4World);
	if (m_pSpriteEffect)
	{
		m_pSpriteEffect->PrepareRender(pd3dCommandList);
		m_pSpriteEffect->Render(pd3dCommandList, pCamera, pxmf4x4World);
	}
}

void CBombGameObject::SetBomb(BombSTATE eBombState)
{
	if (m_eBombState == eBombState) // 똑같은 스테이트일때는 설정 바꾸지XS
		return;
	m_eBombState = eBombState;

	switch (eBombState) // 설정값 셋팅
	{
	case BombSTATE::BombWait:
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
	if(m_pSpriteEffect)
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
			m_xmfVelocity = XMFLOAT3(8.f, 8.f, 8.f);
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

void CBombGameObject::SetSpriteEffect(std::shared_ptr<CTextureRectObject> pSpriteEffect, XMFLOAT3 xmf3OffSet)
{
	m_pSpriteEffect = pSpriteEffect;
	SetSpriteOffSet(xmf3OffSet);
}

void CBombGameObject::Move(float fTimeElapsed)
{
	//xmfVelocity = XMFLOAT3(xmfLook.x * fSpeed, xmfLook.y * fSpeed, xmfLook.z * fSpeed);
	
	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position.x += m_xmfVelocity.x * m_xmf3Direction.x * fTimeElapsed * m_fSpeed;
	xmf3Position.y += m_xmfVelocity.y * fTimeElapsed * m_fSpeed;
	xmf3Position.z += m_xmfVelocity.z * m_xmf3Direction.z * fTimeElapsed * m_fSpeed;
	m_xmfVelocity.y -= 9.8f * fTimeElapsed;
	SetPosition(xmf3Position);
}

void CMonsterGameObject::MoveBomb(float fTimeElapsed, XMFLOAT3 xmf3Direction)
{
	m_pMonstersGameObject->SetDirection(xmf3Direction);
	m_pMonstersGameObject->SetBombState(BombSTATE::BombBurn);
}

void CMonsterGameObject::LoadMonsterBombObject(std::shared_ptr<CBombGameObject> pAttackGameObject)
{
	m_pMonstersGameObject = pAttackGameObject;
	m_pMonstersGameObject->SetOffSet(XMFLOAT3(-45.f, 25.f, 12.f));
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

		
	if (m_pMonstersGameObject)
	{
		m_pMonstersGameObject->Update(fTimeElapsed, GetWorldPosition(), fTerrainObject);
		m_pMonstersGameObject->UpdateCollisionBox(m_pMonstersGameObject->GetPosition());
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
	CGameObject::PrepareRender(pd3dCommandList);
	CGameObject::Render(pd3dCommandList, pCamera, pxmf4x4World);

	if (m_pMonstersGameObject)
	{
		m_pMonstersGameObject->PrepareRender(pd3dCommandList);
		m_pMonstersGameObject->Render(pd3dCommandList, pCamera, NULL);
	}
}

std::shared_ptr<CBombGameObject> CMonsterGameObject::GetBombGameObject()
{
	return m_pMonstersGameObject;
}
