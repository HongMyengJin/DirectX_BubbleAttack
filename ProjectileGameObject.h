#pragma once
#include "GameObject.h"
#include "TextureRectObject.h"

#define MIN_VELOCITY 1.f
#define MAX_VELOCITY 20.f
class CProjectileGameObject : public CGameObject
{
public:
	CProjectileGameObject() {};
	~CProjectileGameObject() {};

public:
	virtual void Update(float fTimeElapsed, XMFLOAT3 xmf3Position, std::shared_ptr<class CTerrainObject> pTerrainObject) {};
	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);


	void SetVelocity(XMFLOAT3 xmf3Velocity) {m_xmfVelocity = xmf3Velocity;}
	void SetDirection(XMFLOAT3 xmf3Direction) { m_xmf3Direction = xmf3Direction; }

	void SetSpriteOffSet(XMFLOAT3 xmf3OffSet) { xmfOffSetSpritePosition = xmf3OffSet; }
	void SetSpriteEffect(std::shared_ptr<class CTextureRectObject> pSpriteEffect, XMFLOAT3 xmf3OffSet);

	void SetOffSetPosition(XMFLOAT3 xmf3OffSet) { xmfOffSetPosition = xmf3OffSet; }

	virtual void Move(float fTimeElapsed) {}

	XMFLOAT3 Acceleration(float fTimeElapsed);
	void Deceleration(float fTimeElapsed);

	XMFLOAT3 GetVelocity();

protected:
	XMFLOAT3 xmfOffSetPosition = XMFLOAT3(-43.f, 35.f, 12.f);
	XMFLOAT3 xmfOffSetSpritePosition = XMFLOAT3(-43.f, 35.f, 12.f);
	std::shared_ptr<CTextureRectObject>	m_pSpriteEffect;

	XMFLOAT3 m_xmfVelocity = XMFLOAT3(8.f, 8.f, 8.f);
	XMFLOAT3 m_xmf3Direction = XMFLOAT3(0.f, 0.f, 0.f);

	float	m_fSpeed = 10.f;
	float	m_fAccelerationSpeed = 5.f;
};

