#include "MaterialComponent.h"

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

CMaterialValueComponent::CMaterialValueComponent(int nTextures)
{
	m_nTexture = 1;
	m_Textures.resize(1);
	m_Textures[0] = std::make_shared<CTextureComponent>(nTextures);
	m_Textures[0]->Init(ResourceTextureType::ResourceTexture2D, 0, 1);
}

bool CMaterialValueComponent::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, UINT nType, UINT nRootParameter, UINT iTextureIndex, FILE* pInFile)
{
	char pstrTextureName[64] = { '\0' };

	BYTE nStrLength = 64;
	UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
	pstrTextureName[nStrLength] = '\0';

	bool bDuplicated = false;
	if (strcmp(pstrTextureName, "null"))
	{
		m_tMaterialData.m_nType |= nType;
		char pstrFilePath[64] = { '\0' };
		strcpy_s(pstrFilePath, 64, "Model/Textures/");

		bDuplicated = (pstrTextureName[0] == '@');
		strcpy_s(pstrFilePath + 15, 64 - 15, (bDuplicated) ? (pstrTextureName + 1) : pstrTextureName);
		strcpy_s(pstrFilePath + 15 + ((bDuplicated) ? (nStrLength - 1) : nStrLength), 64 - 15 - ((bDuplicated) ? (nStrLength - 1) : nStrLength), ".dds");

		size_t nConverted = 0;
		mbstowcs_s(&nConverted, &m_Textures[0]->m_stTextureName[iTextureIndex][0], 64, pstrFilePath, _TRUNCATE);

#define _WITH_DISPLAY_TEXTURE_NAME

#ifdef _WITH_DISPLAY_TEXTURE_NAME
		static int nTextures = 0, nRepeatedTextures = 0;
		TCHAR pstrDebug[256] = { 0 };
		_stprintf_s(pstrDebug, 256, _T("Texture Name: %d %c %s\n"), (pstrTextureName[0] == '@') ? nRepeatedTextures++ : nTextures++, (pstrTextureName[0] == '@') ? '@' : ' ', &(m_Textures[0]->m_stTextureName[iTextureIndex][0]));
		OutputDebugString(pstrDebug);
#endif
		m_Textures[0]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, &m_Textures[0]->m_stTextureName[iTextureIndex][0], (UINT)ResourceTextureType::ResourceTexture2D, iTextureIndex);
		return true;
	}
	return false;
}


void CMaterialsComponent::Init(UINT nMaterials, UINT nTextureN)
{
	m_nMaterials = nMaterials;
	m_MaterialDatas.resize(m_nMaterials);

	for(int i = 0; i < m_nMaterials; i++)
		m_MaterialDatas[i] = std::make_unique<CMaterialValueComponent>(nTextureN);
}

void CMaterialsComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CMaterialsComponent::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMaterialsComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, void* pContext)
{
	UINT iIndex = *static_cast<UINT*>(pContext);
	UpdateShaderVariable(pd3dCommandList, iIndex);

	for (int i = 0; i < m_MaterialDatas[iIndex]->m_nTexture; i++)
	{
		if (m_MaterialDatas[iIndex]->m_Textures[i]) 
			m_MaterialDatas[iIndex]->m_Textures[i]->UpdateShaderVariables(pd3dCommandList);
	}
	
}

void CMaterialsComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMaterialsComponent::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, ResourceTextureType nType, UINT nRootParameter, UINT iMaterialIndex, UINT iTextureIndex, wchar_t* pszFileName)
{
	m_MaterialDatas[iMaterialIndex]->m_Textures[0]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, (UINT)ResourceTextureType::ResourceTexture2D, iTextureIndex);

}

void CMaterialsComponent::CreateBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT dxgiFormat, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, UINT iMaterialIndex, UINT nIndex)
{
	m_MaterialDatas[iMaterialIndex]->m_Textures[0]->CreateBuffer(pd3dDevice, pd3dCommandList, pData, nElements, nStride, dxgiFormat, d3dHeapType, d3dResourceStates, nIndex);

}

void CMaterialsComponent::LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, FILE* pInFile)
{

	std::string	strToken;
	BYTE nStrLength = 0;

	int nDatas = 0;

	UINT nReads = (UINT)fread(&m_nMaterials, sizeof(UINT), 1, pInFile);

	m_MaterialDatas.resize(m_nMaterials);
	int iIndex = 0;
	while (1)
	{
		char pstrName[64] = { '\0' };
		nReads = (UINT)fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)fread(&pstrName[0], sizeof(char), nStrLength, pInFile);
		strToken = pstrName;

		MaterialData tMaterialData;
		if (strToken ==  "<Material>:")
		{
			nReads = (UINT)::fread(&nDatas, sizeof(int), 1, pInFile);
			iIndex = 0;
			// 매개변수 넘어가지x
			m_MaterialDatas[nDatas] = std::make_unique<CMaterialValueComponent>(7); //0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal
		}
		else if (strToken == "<AlbedoColor>:")
		{
			
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_xmf4AlbedoColor), sizeof(float), 4, pInFile);
		}
		else if (strToken == "<EmissiveColor>:")
		{
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_xmf4EmissiveColor), sizeof(float), 4, pInFile);
		}
		else if (strToken == "<SpecularColor>:")
		{
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_xmf4SpecularColor), sizeof(float), 4, pInFile);
		}
		else if (strToken == "<Glossiness>:")
		{
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_fGlossiness), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<Smoothness>:")
		{
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_fSmoothness), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<Metallic>:")
		{
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_fSpecularHighlight), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<SpecularHighlight>:")
		{
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_fMetallic), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<GlossyReflection>:")
		{
			nReads = (UINT)::fread((void*)&(m_MaterialDatas[nDatas]->GetMaterialData().m_fGlossyReflection), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<AlbedoMap>:")
		{
			if (m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, MATERIAL_ALBEDO_MAP, 3, iIndex, pInFile))
				iIndex++;
		}
		else if (strToken == "<SpecularMap>:")
		{
			if (m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, MATERIAL_SPECULAR_MAP, 4, iIndex, pInFile))
				iIndex++;
		}
		else if (strToken == "<NormalMap>:")
		{
			if (m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, MATERIAL_NORMAL_MAP, 5, iIndex, pInFile))
				iIndex++;
		}
		else if (strToken == "<MetallicMap>:")
		{
			if (m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, MATERIAL_METALLIC_MAP, 6, iIndex, pInFile))
				iIndex++;
		}
		else if (strToken == "<EmissionMap>:")
		{
			if (m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, MATERIAL_EMISSION_MAP, 7, iIndex, pInFile))
				iIndex++;
		}
		else if (strToken == "<DetailAlbedoMap>:")
		{
			if (m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, MATERIAL_DETAIL_ALBEDO_MAP, 8, iIndex, pInFile))
				iIndex++;
		}
		else if (strToken == "<DetailNormalMap>:")
		{
			if (m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, MATERIAL_DETAIL_NORMAL_MAP, 9, iIndex, pInFile))
				iIndex++;
		}
		else if (strToken == "</Materials>")
		{
			CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, iIndex); // 수정 필요
			break;
		}
	}
	
}

void CMaterialsComponent::CreateShaderResourceView(ID3D12Device* pd3dDevice, CDescriptorHeap* pDescriptorHeap, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex, UINT nTextureN)
{
	m_MaterialDatas[0]->m_Textures[0]->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, nRootParameterStartIndex, nTextureN); // 수정 필요
}

void CMaterialsComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int iIndex)
{

	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_MaterialDatas[iIndex]->GetMaterialData().m_xmf4AmbientColor, 16);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_MaterialDatas[iIndex]->GetMaterialData().m_xmf4AlbedoColor, 20);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_MaterialDatas[iIndex]->GetMaterialData().m_xmf4SpecularColor, 24);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_MaterialDatas[iIndex]->GetMaterialData().m_xmf4EmissiveColor, 28);

	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 1, &m_MaterialDatas[iIndex]->GetMaterialData().m_nType, 32);
	
	if (!m_MaterialDatas[iIndex]->m_Textures.empty())
	{
		for(int i = 0; i< m_MaterialDatas[iIndex]->m_Textures.size(); i++)
			if(m_MaterialDatas[iIndex] && m_MaterialDatas[iIndex]->m_Textures[i])
				m_MaterialDatas[iIndex]->m_Textures[i]->UpdateShaderVariables(pd3dCommandList);
	}

}

