#include "TextureObjectTransformComponent.h"

void CTextureObjectTransformComponent::UVAnimate(float fTime)
{
	// 0 ~ 1 ���� ���´�	// �ε��� �����̱� ������ 1�� ���ش�.
	UINT nTextureIndex = UINT((m_nRows  * m_nCols - 1) * fTime) ;
	m_xmf4x4World._11 = 1.0f / float(m_nRows); // ũ�⸦ �ѱ�(width)
	m_xmf4x4World._22 = 1.0f / float(m_nCols); // ũ�⸦ �ѱ�(height)
	m_xmf4x4World._31 = float(nTextureIndex / m_nCols) / m_nRows; // ���� ����(Row firt Point) -> ���� ���� ����
	m_xmf4x4World._32 = float(nTextureIndex % m_nCols) / m_nCols; // ���� ����(Col firt Point)

	// ((�ؽ��� ���� / �ؽ��� �� ����) -> ���° ������) / �� ����
	//  ((�ؽ��� ���� % �ؽ��� �� ����) -> ���° ������) / �� ����
	// => UINT(�� �̹��� �� * �̹��� ����) == �̹��� ���� 
}
