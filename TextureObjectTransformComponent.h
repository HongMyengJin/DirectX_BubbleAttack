#pragma once
#include "TransformComponent.h"
class CTextureObjectTransformComponent : public CTransformComponent
{
public:
	CTextureObjectTransformComponent() {};
	~CTextureObjectTransformComponent() {};

	void UVAnimate(float fTime); // uv ���� �ٸ��� �־� uv �ִϸ��̼� �۵�

protected:
	UINT	m_nRows = 8;
	UINT	m_nCols = 8;
};

