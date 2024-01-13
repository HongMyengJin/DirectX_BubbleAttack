#pragma once
#include "GameObject.h"
#include "BombGameObject.h"

class CMonsterGameObject : public CGameObject
{
public:
	CMonsterGameObject() {};
	~CMonsterGameObject() {};

public:
	void MoveBomb(float fTimeElapsed, XMFLOAT3 xmf3Direction);
	void LoadMonsterBombObject(std::shared_ptr<CBombGameObject> pAttackGameObject);
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, XMFLOAT3 xmfTargetPosition, std::shared_ptr<class CTerrainObject> pTerrainObject);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);
	std::shared_ptr<CBombGameObject> GetBombGameObject();
public:
	std::shared_ptr<CBombGameObject>		m_pBombObject;
};