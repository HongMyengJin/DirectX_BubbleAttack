#pragma once
#include "stdafx.h"

class CUIVertex
{
public:
    XMFLOAT3                        m_xmf3Position;
    XMFLOAT2                        m_xmf2TexCoord;
public:
    CUIVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); }
    CUIVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord, XMFLOAT2 xmf2Size) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf2TexCoord = xmf2TexCoord; }
    CUIVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f), XMFLOAT2 xmf2Size = XMFLOAT2(0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf2TexCoord = xmf2TexCoord;}
    ~CUIVertex() { }
};