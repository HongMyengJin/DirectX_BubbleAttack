#pragma once
#include "UIGameObject.h"
class CBarUIGameObject : public CUIGameObject
{
public:
	CBarUIGameObject() {}
	~CBarUIGameObject() {}

public:
	virtual void Animate(float fTimeElapsed);
	void UpdateBarValue(float fTotalBarValue, float fCurrentBarValue);

protected:
	float	m_fTotalBarValue = 0.f;
	float	m_fCurrentBarValue = 0.f;
};

