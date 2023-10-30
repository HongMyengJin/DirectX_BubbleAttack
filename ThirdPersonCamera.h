#pragma once
#include "Camera.h"
class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera() {}
	~CThirdPersonCamera() {}

public:
	virtual void Update(CGameObject* pObject, XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(CGameObject* pObject, XMFLOAT3& vLookAt);
};

