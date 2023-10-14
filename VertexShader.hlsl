// 정점 쉐이더 정의
float4 VSTriangle(uint nVertexID : SV_VertexID) : SV_POSITION
{
	float4 output;

	// 프리미티브(삼각형)를 구성하는 정점의 인덱스(SV_VertexID)에 따라 정점 반환
	// 정점의 위치 좌표는 변환 된 좌표(SV_POSITION)이다. -> 투영좌표계의 좌표

	if (nVertexID == 0) output = float4(0.f, 0.5f, 0.5f, 1.f);
	else if (nVertexID == 1) output = float4(0.5f, -0.5f, 0.5f, 1.f);
	else if (nVertexID == 2) output = float4(-0.5f, -0.5f, 0.5f, 1.f);

	return output;
}