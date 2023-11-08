#pragma once
#include "GameObject.h"
#include "TerrainObject.h"
#include "ProjectileGameObject.h"
enum class BombSTATE
{
	BombWait,
	BombBurn,
	BombPop,
	BombDead,
	BombEnd
};
class CBombGameObject : public CProjectileGameObject
{
public:
	CBombGameObject() {};
	~CBombGameObject() {};

public:
	virtual void Update(float fTimeElapsed, XMFLOAT3 xmf3Position, std::shared_ptr<class CTerrainObject> pTerrainObject);
	virtual void Move(float fTimeElapsed);

	void SetBombState(BombSTATE eState);
	BombSTATE GetBombState() {return m_eBombState;}
private:
	BombSTATE m_eBombState = BombSTATE::BombWait;
};
