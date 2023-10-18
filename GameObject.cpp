#include "GameObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "ObjectShaderComponent.h"
void CGameObject::Init()
{
}

void CGameObject::Animate(float fTimeElapsed)
{
	if (m_pSiblingObject) m_pSiblingObject->Animate(fTimeElapsed);
	if (m_pChildObject) m_pChildObject->Animate(fTimeElapsed);
}

void CGameObject::Update(float fTimeElapsed)
{
	for (std::shared_ptr<CComponent> pComponent : m_pComponents)
		pComponent->Update(fTimeElapsed);
}

void CGameObject::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (std::shared_ptr<CComponent> pComponent : m_pComponents)
		pComponent->PrepareRender(pd3dCommandList);
}

void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	for (std::shared_ptr<CComponent> pComponent : m_pComponents)
		pComponent->Render(pd3dCommandList, pCamera, pxmf4x4World);
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
			std::shared_ptr<CTransformComponent> pTransformComponent = std::make_shared<CTransformComponent>(); // Mesh 持失
			XMFLOAT4X4 xmf4x4Transform;
			nReads = (UINT)::fread(&(pTransformComponent->m_xmf4x4Transform), sizeof(float), 16, pInFile);
			m_pComponents.push_back(pTransformComponent);
		}
		else if (!strcmp(pstrName, "<Mesh>:"))
		{
			std::shared_ptr<CObjectMeshComponent> pObjectMeshComponent = std::make_shared<CObjectMeshComponent>(pd3dDevice, pd3dCommandList); // Mesh 持失
			pObjectMeshComponent->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);
			m_pComponents.push_back(pObjectMeshComponent);
		}
		else if (!strcmp(pstrName, "<Materials>:"))
		{
			std::shared_ptr<CMaterialsComponent> pMaterialsComponent = std::make_shared<CMaterialsComponent>(); // Mesh 持失
			pMaterialsComponent->LoadMaterialsFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, pInFile);
			m_pComponents.push_back(pMaterialsComponent);
		}
		else if (!strcmp(pstrName, "<Children>:"))
		{
			
			int nChilds = 0;
			nReads = (UINT)::fread(&nChilds, sizeof(int), 1, pInFile);
			if (nChilds > 0)
			{
				nChilds = 1;
				for (int i = 0; i < nChilds; i++)
				{
					SetChild(pGameObject, CGameObject::LoadFrameHierarchy(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pDescriptorHeap, pstrFileName, pInFile));
				}
			}
		}
		else if (!strcmp(pstrName, "</Frame>"))
		{
			break;
		}
	}
	return(pGameObject);
}

void CGameObject::SetChild(std::shared_ptr<CGameObject> pParentObject, std::shared_ptr<CGameObject> pChildObject)
{
	if (m_pChildObject)
	{
		pChildObject->m_pSiblingObject = m_pChildObject;
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
	m_pComponents.push_back(pComponent);
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
