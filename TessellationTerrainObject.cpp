#include "TessellationTerrainObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"

void CTessellationTerrainObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, CDescriptorHeap* pDescriptorHeap, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
	m_pComponents.resize(4);
	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	m_xmf3Scale = xmf3Scale;

	m_pHeightMapImage = std::make_shared<CHeightMapImage>();
	m_pHeightMapImage->Init(pFileName, nWidth, nLength, xmf3Scale);

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CTessellationTerrainMeshComponent>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();

	dynamic_cast<CTessellationTerrainMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->ResizeMesh(cxBlocks * czBlocks);

	int iIndex = 0;
	CHeightMapGridMesh* pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			std::shared_ptr<CTessellationGridMeshComponent> pHeightMapGridMesh = std::make_shared<CTessellationGridMeshComponent>();
			pHeightMapGridMesh->Init(pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage.get());
			dynamic_cast<CTessellationTerrainMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->SetMesh(iIndex++, pHeightMapGridMesh);
		}
	}

	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(1);
	m_vTextureType[0] = ResourceTextureType::ResourceTexture2D;

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 5, m_vTextureType);

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 0, L"Image/Snow_Base.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, 1); // 수정 필요
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 1, L"Image/Snow_Base.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 4, 1); // 수정 필요
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 2, L"Image/Snow_Base.dds"); // 
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 5, 1); // 수정 필요
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 3, L"Image/Snow_Base.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 6, 1); // 수정 필요
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 4, L"Image/HeightMap2(Flipped)Alpha.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 7, 1); // 수정 필요
}

void CTessellationTerrainObject::Animate(float fTimeElapsed)
{
}

void CTessellationTerrainObject::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
}

void CTessellationTerrainObject::SetShaderCurrentPipeline(UINT iCurrentPipeline)
{
	if (m_pComponents[UINT(ComponentType::ComponentShader)])
	{
		dynamic_cast<CShaderComponent*>(m_pComponents[UINT(ComponentType::ComponentShader)].get())->SetCurrentPipeline(iCurrentPipeline);
	}
}

float CTessellationTerrainObject::GetHeight(float x, float z)
{
	float fHeight = m_pHeightMapImage->GetTerrainTessellationHeight(x, z, m_xmf3Scale);

	return(fHeight);
}
