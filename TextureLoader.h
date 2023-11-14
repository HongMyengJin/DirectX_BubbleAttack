#pragma once
#include "GameObject.h"
#include "TextureComponent.h"

class CTextureLoader
{
public:
	CTextureLoader() {};
	~CTextureLoader() {};

public:
	void LoadTextureComponent(Microsoft::WRL::ComPtr<ID3D12Resource> pTextureComponent, wchar_t* pszFileName);
	Microsoft::WRL::ComPtr<ID3D12Resource> GetTextureComponent(wchar_t* pszFileName);

public:
	//std::vector<std::shared_ptr<CTextureComponent>> m_vTextureComponent;
	std::vector<std::pair<wchar_t*, Microsoft::WRL::ComPtr<ID3D12Resource>>>	m_vTextureComponent;
};

