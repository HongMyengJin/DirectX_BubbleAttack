#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "DescriptorHeap.h"

class CGameObject
{
public:
	CGameObject() {  };
	~CGameObject() {};

public:
	virtual void Init();
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, class CTransformComponent* pTransformComponent);

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

	std::shared_ptr<CComponent> GetComponent(std::string st, ComponentType pComponentType);
	std::shared_ptr<CGameObject> LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName);
	std::shared_ptr<CGameObject> LoadFrameHierarchy(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName, FILE* pInFile);

	void SetChild(std::shared_ptr<CGameObject> pParentObject, std::shared_ptr<CGameObject> pChildObject);
	void AddShaderComponent(std::shared_ptr<CComponent> pComponent);

public:
	char																												m_pstrFrameName[126];
	std::map<std::string, std::array<std::shared_ptr<CComponent>, UINT(ComponentType::ComponentTypeEnd)>>			m_pComponents;// 함수에서 접근할때 포인터 접근 필요(shared_ptr)
	//std::multimap<ComponentType, std::shared_ptr<CComponent>>								m_pComponents; // 함수에서 접근할때 포인터 접근 필요(shared_ptr)

	std::shared_ptr<CGameObject>																						m_pChildObject;
	std::shared_ptr<CGameObject>																						m_pParentObject;
	std::shared_ptr<CGameObject>																						m_pSiblingObject;
};


class CPlayerGameObject
{
public:
	CPlayerGameObject() {};
	~CPlayerGameObject() {};

public:
	virtual void Init();
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed);

	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

};
