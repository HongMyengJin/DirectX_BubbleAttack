#include "MaterialComponent.h"

CMaterialValueComponent::CMaterialValueComponent(int nTextures)
{
	m_nTexture = nTextures;
}

void CMaterialValueComponent::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nType, UINT nRootParameter, _TCHAR* pwstrTextureName, UINT iTextureIndex, FILE* pInFile)
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

		char pstrFilePath[64] = { '\0' };
		strcpy_s(pstrFilePath, 64, "Model/Textures/");

		bDuplicated = (pstrTextureName[0] == '@');
		strcpy_s(pstrFilePath + 15, 64 - 15, (bDuplicated) ? (pstrTextureName + 1) : pstrTextureName);
		strcpy_s(pstrFilePath + 15 + ((bDuplicated) ? (nStrLength - 1) : nStrLength), 64 - 15 - ((bDuplicated) ? (nStrLength - 1) : nStrLength), ".dds");

		size_t nConverted = 0;
		mbstowcs_s(&nConverted, pwstrTextureName, 64, pstrFilePath, _TRUNCATE);

#define _WITH_DISPLAY_TEXTURE_NAME

#ifdef _WITH_DISPLAY_TEXTURE_NAME
		static int nTextures = 0, nRepeatedTextures = 0;
		TCHAR pstrDebug[256] = { 0 };
		_stprintf_s(pstrDebug, 256, _T("Texture Name: %d %c %s\n"), (pstrTextureName[0] == '@') ? nRepeatedTextures++ : nTextures++, (pstrTextureName[0] == '@') ? '@' : ' ', pwstrTextureName);
		OutputDebugString(pstrDebug);
#endif
		m_Textures[iTextureIndex] = std::make_unique<CTextureComponent>(1, (UINT)ResourceTextureType::ResourceTexture2D, 0, 1);
		m_Textures[iTextureIndex]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, pwstrTextureName, (UINT)ResourceTextureType::ResourceTexture2D, 0);

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
	char pstrName[64] = { '\0' };
	std::string	strToken;
	BYTE nStrLength = 0;

	int nDatas = 0;

	UINT nReads = (UINT)fread(&m_nMaterials, sizeof(UINT), 1, pInFile);

	m_MaterialDatas.resize(m_nMaterials);

	while (1)
	{
		nReads = (UINT)fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)fread(&pstrName[0], sizeof(char), 1, pInFile);
		strToken.resize(nStrLength + 1);
		strToken[nStrLength] = '\0';
		strToken = pstrName;

		MaterialData tMaterialData;

		nReads = (UINT)::fread(&nDatas, sizeof(int), 1, pInFile);
		if (nDatas <= 0)
			continue;

		if (strToken ==  "<Material>:")
		{
			nReads = (UINT)::fread(&nDatas, sizeof(int), 1, pInFile);

			// 매개변수 넘어가지x
			m_MaterialDatas[nDatas] = std::unique_ptr<CMaterialValueComponent>(); //0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal
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
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialAlbedoMap, 3, m_MaterialDatas[nDatas]->m_Textures[0]->m_stTextureName[0].data(), 0, pInFile);
		}
		else if (strToken == "<SpecularMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialSpecularMap, 4, m_MaterialDatas[nDatas]->m_Textures[1]->m_stTextureName[1].data(), 1, pInFile);
		}
		else if (strToken == "<NormalMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialNormalMap, 5, m_MaterialDatas[nDatas]->m_Textures[2]->m_stTextureName[2].data(), 2, pInFile);
		}
		else if (strToken == "<MetallicMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialMetallicMap, 6, m_MaterialDatas[nDatas]->m_Textures[3]->m_stTextureName[3].data(), 3, pInFile);
		}
		else if (strToken == "<EmissionMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialEmissionMap, 7, m_MaterialDatas[nDatas]->m_Textures[4]->m_stTextureName[4].data(), 4, pInFile);
		}
		else if (strToken == "<DetailAlbedoMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialDetailAlbedoMap, 8, m_MaterialDatas[nDatas]->m_Textures[5]->m_stTextureName[5].data(), 5, pInFile);
		}
		else if (strToken == "<DetailNormalMap>:")
		{
			m_MaterialDatas[nDatas]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, (UINT)MaterialType::MaterialDetailNormalMap, 9, m_MaterialDatas[nDatas]->m_Textures[6]->m_stTextureName[6].data(), 6, pInFile);
		}
		else if (strToken == "</Materials>")
		{
			break;
		}
	}
	
}

