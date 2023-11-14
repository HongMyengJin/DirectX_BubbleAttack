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
	virtual void Init(XMFLOAT3 xmf3Extent);
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent);

	void Move(DWORD dwDirection, float fDistance); // 방향과 거리
	void Move(XMFLOAT3 xmf3Direction, float fDistance); // 방향, 기리
	void Rotate(float x, float y, float z);
	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

	std::shared_ptr<CGameObject> LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName, std::shared_ptr<class CTextureLoader> pTextureLoader);
	std::shared_ptr<CGameObject> LoadFrameHierarchy(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, char* pstrFileName, FILE* pInFile, std::shared_ptr<class CTextureLoader> pTextureLoader);

	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);

	void SetChild(std::shared_ptr<CGameObject> pParentObject, std::shared_ptr<CGameObject> pChildObject);
	void AddShaderComponent(std::shared_ptr<CComponent> pComponent);

	void SetPosition(XMFLOAT3 xmf3Position);

	void SetScale(XMFLOAT3 xmf3Scale);

	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }

	void SetLookVector(XMFLOAT3 xmf3LookVector) {
		m_xmf3Look = xmf3LookVector;
	}
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetPrePosition();
	XMFLOAT3 GetWorldPosition();
	void SetWorldPosition(XMFLOAT3 xmf3Position);
	virtual void Release();

	CGameObject* CGameObject::FindFrame(char* pstrFrameName);

	void UpdateCollisionBox(XMFLOAT3 xmf3Center);
	bool CollisionCheck(std::shared_ptr<CGameObject> pGameObject);

	DirectX::BoundingBox& GetCollisionBox() { return m_xmf3AABBBox; }

	void SetCollision(bool bCollision) 
	{
		m_bCollision = bCollision;
	}

	bool GetCollision()
	{
		return m_bCollision;
	}

	bool	GetEnable() { return m_bEnable; }
	void	SetEnable(bool bEnable) { m_bEnable = bEnable; }
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
	XMFLOAT3																											m_xmf3Scale = XMFLOAT3(1.f, 1.f, 1.f);

	DirectX::XMMATRIX																									m_xmRotationMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX																									m_xmTranslationMatrix = DirectX::XMMatrixIdentity();
	float																												m_fRotationAngle = 0.f;
	float																												m_fSpeed = 0.25f;

	// 충돌 체크
	DirectX::BoundingBox																								m_xmf3AABBBox;

	XMFLOAT3																											m_xmf3AABBCenter = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3																											m_xmf3AABBExtents = XMFLOAT3(0.f, 0.f, 0.f);
	bool																												m_bCollision = false;

	XMFLOAT3																											m_xf3PrePosition;
	bool																												m_bEnable = true;
};
