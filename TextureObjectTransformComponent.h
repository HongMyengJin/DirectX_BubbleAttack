#pragma once
#include "TransformComponent.h"
class CTextureObjectTransformComponent : public CTransformComponent
{
public:
	CTextureObjectTransformComponent() {};
	~CTextureObjectTransformComponent() {};

	void UVAnimate(float fTime); // uv 값을 다르게 주어 uv 애니메이션 작동

protected:
	UINT	m_nRows = 8;
	UINT	m_nCols = 8;
};

