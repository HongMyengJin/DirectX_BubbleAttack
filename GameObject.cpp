#include "GameObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
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

std::shared_ptr<CGameObject> CGameObject::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrName[64] = { '\0' };
	BYTE nStrLength = 0;
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;

	std::shared_ptr<CGameObject> pGameObject;
	for (; ; )
	{
		char pstrName[64] = { '\0' };
		std::string	strToken;
		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		strToken.resize(nStrLength + 1);
		//strToken[nStrLength] = '\0';
		nReads = (UINT)::fread(pstrName, sizeof(char), nStrLength, pInFile);
		strToken = pstrName;
		int i = 0;
		if (strToken == "<Frame>:")
		{
			pGameObject = std::make_shared<CGameObject>();

			nReads = (UINT)::fread(&nFrame, sizeof(int), 1, pInFile);
			nReads = (UINT)::fread(&nTextures, sizeof(int), 1, pInFile);

			nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(&(pGameObject->m_pstrFrameName[0]), sizeof(char), nStrLength, pInFile);
			pGameObject->m_pstrFrameName.resize(nStrLength);
			pGameObject->m_pstrFrameName[nStrLength] = '\0';
		}
		else if (strToken == "<Transform>:")
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
		}
		else if (strToken == "<TransformMatrix>:")
		{
			std::shared_ptr<CTransformComponent> pTransformComponent = std::make_shared<CTransformComponent>(); // Mesh 持失
			nReads = (UINT)::fread(&pTransformComponent->m_xmf4x4Transform, sizeof(float), 16, pInFile);
		}
		else if (strToken == "<Mesh>:")
		{
			std::shared_ptr<CObjectMeshComponent> pObjectMeshComponent = std::make_shared<CObjectMeshComponent>(pd3dDevice, pd3dCommandList); // Mesh 持失
			m_pComponents.emplace_back(pObjectMeshComponent);
			pObjectMeshComponent->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);
		}
		else if (strToken == "<Materials>:")
		{
			std::shared_ptr<CMaterialsComponent> pMaterialsComponent = std::make_shared<CMaterialsComponent>(); // Mesh 持失
			pMaterialsComponent->LoadMaterialsFromFile(pd3dDevice, pd3dCommandList, pInFile);
		}
		else if (strToken == "<Children>:")
		{
			int nChilds = 0;
			nReads = (UINT)::fread(&nChilds, sizeof(int), 1, pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					//std::shared_ptr<CGameObject> pObject = ;
					//if (pObject) 
						//SetChild(CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pstrFileName));
				}
			}
		}
		else if (strToken == "</Frame>")
		{
			break;
		}
	}
	return(pGameObject);
}

void CGameObject::SetChild(std::shared_ptr<CGameObject> pChild)
{
	if (m_pChildObject)
	{
		pChild->m_pSiblingObject = m_pChildObject;
		m_pChildObject->m_pSiblingObject = pChild;
	}
	else
	{
		m_pChildObject = pChild;
	}

	if (pChild) pChild->m_pParentObject = static_cast<std::shared_ptr<CGameObject>>(this);
}
