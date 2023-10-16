#include "MaterialComponent.h"

CMaterialValueComponent::CMaterialValueComponent(int nTextures)
{
	m_nTexture = nTextures;
	m_Textures.resize(nTextures);
}

void CMaterialValueComponent::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nType, UINT nRootParameter, UINT iTextureIndex, FILE* pInFile)
{
	char pstrTextureName[64] = { '\0' };

	BYTE nStrLength = 64;
	UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
	pstrTextureName[nStrLength] = '\0';

	bool bDuplicated = false;
	if (strcmp(pstrTextureName, "null"))
	{
		//SetMaterialType(nType);
		m_Textures[iTextureIndex] = std::make_unique<CTextureComponent>(1, (UINT)ResourceTextureType::ResourceTexture2D, 0, 1);

		char pstrFilePath[64] = { '\0' };
		strcpy_s(pstrFilePath, 64, "Model/Textures/");

		bDuplicated = (pstrTextureName[0] == '@');
		strcpy_s(pstrFilePath + 15, 64 - 15, (bDuplicated) ? (pstrTextureName + 1) : pstrTextureName);
		strcpy_s(pstrFilePath + 15 + ((bDuplicated) ? (nStrLength - 1) : nStrLength), 64 - 15 - ((bDuplicated) ? (nStrLength - 1) : nStrLength), ".dds");

		size_t nConverted = 0;
		mbstowcs_s(&nConverted, &m_Textures[iTextureIndex]->m_stTextureName[0], 64, pstrFilePath, _TRUNCATE);

#define _WITH_DISPLAY_TEXTURE_NAME

#ifdef _WITH_DISPLAY_TEXTURE_NAME
		static int nTextures = 0, nRepeatedTextures = 0;
		TCHAR pstrDebug[256] = { 0 };
		_stprintf_s(pstrDebug, 256, _T("Texture Name: %d %c %s\n"), (pstrTextureName[0] == '@') ? nRepeatedTextures++ : nTextures++, (pstrTextureName[0] == '@') ? '@' : ' ', &(m_Textures[iTextureIndex]->m_stTextureName[0]));
		OutputDebugString(pstrDebug);
#endif
		m_Textures[iTextureIndex]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, &m_Textures[iTextureIndex]->m_stTextureName[0], (UINT)ResourceTextureType::ResourceTexture2D, 0);

		//CScene::CreateShaderResourceViews(pd3dDevice, *ppTexture, 0, nRootParameter);

	}
}

void CMaterialsComponent::Init()
{
}

void CMaterialsComponent::Update(float fTimeElapsed)
{
}

void CMaterialsComponent::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMaterialsComponent::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World, void* pContext)
{
}

void CMaterialsComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMaterialsComponent::LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile)
{

	std::string	strToken;
	BYTE nStrLength = 0;

	int nDatas = 0;

	UINT nReads = (UINT)fread(&m_nMaterials, sizeof(UINT), 1, pInFile);

	m_MaterialDatas.resize(m_nMaterials);

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

			// 매개변수 넘어가지x
			m_MaterialDatas[nDatas] = std::make_unique<CMaterialValueComponent>(7); //0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal
		}
		else if (strToken == "<AlbedoColor>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_xmf4AlbedoColor), sizeof(float), 4, pInFile);
		}
		else if (strToken == "<EmissiveColor>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_xmf4EmissiveColor), sizeof(float), 4, pInFile);
		}
		else if (strToken == "<SpecularColor>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_xmf4SpecularColor), sizeof(float), 4, pInFile);
		}
		else if (strToken == "<Glossiness>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_fGlossiness), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<Smoothness>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_fSmoothness), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<Metallic>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_fSpecularHighlight), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<SpecularHighlight>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_fMetallic), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<GlossyReflection>:")
		{
			nReads = (UINT)::fread(&(tMaterialData.m_fGlossyReflection), sizeof(float), 1, pInFile);
		}
		else if (strToken == "<AlbedoMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialAlbedoMap, 3, 0, pInFile);
		}
		else if (strToken == "<SpecularMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialSpecularMap, 4,  1, pInFile);
		}
		else if (strToken == "<NormalMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialNormalMap, 5, 2, pInFile);
		}
		else if (strToken == "<MetallicMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialMetallicMap, 6, 3, pInFile);
		}
		else if (strToken == "<EmissionMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialEmissionMap, 7, 4, pInFile);
		}
		else if (strToken == "<DetailAlbedoMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialDetailAlbedoMap, 8, 5, pInFile);
		}
		else if (strToken == "<DetailNormalMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialDetailNormalMap, 9, 6, pInFile);
		}
		else if (strToken == "</Materials>")
		{
			break;
		}
	}
	
}

