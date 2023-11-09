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

	void SettingVelocity(XMFLOAT3 xmf3SettingVelocity) { m_xmfSettingVelocity = xmf3SettingVelocity; }
private:
	BombSTATE m_eBombState = BombSTATE::BombWait;
	XMFLOAT3 m_xmfSettingVelocity = XMFLOAT3(0.f, 0.f, 0.f);
};
