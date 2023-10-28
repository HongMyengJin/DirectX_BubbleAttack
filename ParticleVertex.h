#pragma once
#include "stdafx.h"
class CParticleVertex
{
public:
	XMFLOAT3						m_xmf3Position;
	XMFLOAT3						m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float							m_fLifetime = 0.0f;
	UINT							m_nType = 0;

public:
	CParticleVertex() { }
	~CParticleVertex() { }
};

