#include "GameObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "ObjectShaderComponent.h"
void CGameObject::Init()
{
	m_pComponents.resize(4);
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();
	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CObjectMeshComponent>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();

}

void CGameObject::Animate(float fTimeElapsed)
{
	if (m_pSiblingObject) m_pSiblingObject->Animate(fTimeElapsed);
	if (m_pChildObject) m_pChildObject->Animate(fTimeElapsed);
}

void CGameObject::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	if (m_pSiblingObject)
		m_pSiblingObject->m_pComponents[UINT(ComponentType::ComponentTransform)]->Update(fTimeElapsed, pxmf4x4Parent, nullptr);
	if (m_pChildObject)
		m_pChildObject->m_pComponents[UINT(ComponentType::ComponentTransform)]->Update(fTimeElapsed, &static_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4World, nullptr);
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
			pGameObject->Init();

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
	static_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(xmf4x4Transform, *pxmf4x4Parent) : xmf4x4Transform;

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

void CGameObject::Release()
{
	for (int i = 0; i < m_pComponents.size(); i++)
		m_pComponents[i]->Release();
}

void CPlayerGameObject::Init()
{
}

void CPlayerGameObject::Animate(float fTimeElapsed)
{
}

void CPlayerGameObject::Update(float fTimeElapsed)
{
}

void CPlayerGameObject::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CPlayerGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
}
