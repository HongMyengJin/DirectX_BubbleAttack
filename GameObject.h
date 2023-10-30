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
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent);

	void Move(DWORD dwDirection, float fDistance); // 방향과 거리
	void Rotate(float x, float y, float z);
	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

	std::shared_ptr<CGameObject> LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName);
	std::shared_ptr<CGameObject> LoadFrameHierarchy(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName, FILE* pInFile);

	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);

	void SetChild(std::shared_ptr<CGameObject> pParentObject, std::shared_ptr<CGameObject> pChildObject);
	void AddShaderComponent(std::shared_ptr<CComponent> pComponent);

	void SetPosition(XMFLOAT3 xmf3Position);

	void SetScale(XMFLOAT3 xmf3Scale);

	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }

	XMFLOAT3 GetPosition();
	virtual void Release();
public:
	char																												m_pstrFrameName[126];
	std::vector<std::shared_ptr<CComponent>>																			m_pComponents;// 함수에서 접근할때 포인터 접근 필요(shared_ptr)
	//std::multimap<ComponentType, std::shared_ptr<CComponent>>								m_pComponents; // 함수에서 접근할때 포인터 접근 필요(shared_ptr)

	std::shared_ptr<CGameObject>																						m_pChildObject;
	std::shared_ptr<CGameObject>																						m_pParentObject;
	std::shared_ptr<CGameObject>																						m_pSiblingObject;


	float           																									m_fPitch = 0.f;
	float           																									m_fYaw = 0.f;
	float           																									m_fRoll = 0.f;

	XMFLOAT3																											m_xmf3Right = XMFLOAT3(1.f, 0.f, 0.f);
	XMFLOAT3																											m_xmf3Up = XMFLOAT3(0.f, 1.f, 0.f);
	XMFLOAT3																											m_xmf3Look = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3																											m_xmf3Position;
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
