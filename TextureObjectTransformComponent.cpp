#include "TextureObjectTransformComponent.h"

void CTextureObjectTransformComponent::UVAnimate(float fTime)
{
	// 0 ~ 1 값이 나온다	// 인덱스 기준이기 때문에 1을 빼준다.
	UINT nTextureIndex = UINT((m_nRows  * m_nCols - 1) * fTime) ;
	m_xmf4x4World._11 = 1.0f / float(m_nRows); // 크기를 넘김(width)
	m_xmf4x4World._22 = 1.0f / float(m_nCols); // 크기를 넘김(height)
	m_xmf4x4World._31 = float(nTextureIndex / m_nCols) / m_nRows; // 시작 지점(Row firt Point) -> 행의 시작 지점
	m_xmf4x4World._32 = float(nTextureIndex % m_nCols) / m_nCols; // 시작 지점(Col firt Point)

	// ((텍스쳐 순서 / 텍스쳐 열 개수) -> 몇번째 행인지) / 행 개수
	//  ((텍스쳐 순서 % 텍스쳐 열 개수) -> 몇번째 열인지) / 열 개수
	// => UINT(총 이미지 수 * 이미지 비율) == 이미지 순서 
}
