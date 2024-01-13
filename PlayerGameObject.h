#pragma once
#include "GameObject.h"
#include "BombGameObject.h"
#include "TerrainObject.h"

class CPlayerGameObject : public CGameObject
{
public:
	CPlayerGameObject() {};
	~CPlayerGameObject() {};

	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent, std::shared_ptr<CTerrainObject> pTerrainObject);
	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

public:
	void LoadPlayerFrameData();
	void UpdateFrame(float fTimeElapsed);
	void UpdateLegFrame(float fTimeElapsed);
	void UpdateSpringFrame(float fTimeElapsed);
	void LoadPlayerBombObject(std::shared_ptr<CBombGameObject> pAttackGameObject);

	void MoveBomb(float fTimeElapsed, XMFLOAT3 xmf3Direction);
	void Acceleration(float fTimeElapsed);

	XMFLOAT3 GetBombPosition();

	std::shared_ptr<CBombGameObject> GetBombGameObject();

	void SetboolMove(bool bMove);
public:
	CGameObject* pGameObject[7];
	//CGameObject* pRightLegGameObject[3];
	//CGameObject* pLeftLegGameObject[3];
	//CGameObject* pTopGameObject;
	//CGameObject* pSpringGameObject[2];

	std::shared_ptr<CBombGameObject>		m_pPlayerBombGameObject;
	bool									m_bMove = false;
};

