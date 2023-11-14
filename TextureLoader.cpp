#include "TextureLoader.h"

void CTextureLoader::LoadTextureComponent(Microsoft::WRL::ComPtr<ID3D12Resource> pTextureComponent, wchar_t* pszFileName)
{
	m_vTextureComponent.push_back(std::make_pair(pszFileName, pTextureComponent));

}


Microsoft::WRL::ComPtr<ID3D12Resource> CTextureLoader::GetTextureComponent(wchar_t* pszFileName)
{
	for (std::pair<wchar_t*, Microsoft::WRL::ComPtr<ID3D12Resource>> &Data : m_vTextureComponent)
	{
		if (!_tcsncmp(Data.first, pszFileName, _tcslen(pszFileName)))
			return Data.second;
	}
	return NULL;
}
