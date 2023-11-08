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
