#include "BarUIGameObject.h"

void CBarUIGameObject::Animate(float fTimeElapsed)
{
	m_xUV[1].x = 0.f;
	m_xUV[1].y = (m_fCurrentBarValue / m_fTotalBarValue);
	CUIGameObject::Animate(fTimeElapsed);
}

void CBarUIGameObject::UpdateBarValue(float fTotalBarValue, float fCurrentBarValue)
{
	m_fTotalBarValue = fTotalBarValue;
	m_fCurrentBarValue = fCurrentBarValue;
}

