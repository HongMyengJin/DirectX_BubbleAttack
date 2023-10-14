// ���� ���̴� ����
float4 VSTriangle(uint nVertexID : SV_VertexID) : SV_POSITION
{
	float4 output;

	// ������Ƽ��(�ﰢ��)�� �����ϴ� ������ �ε���(SV_VertexID)�� ���� ���� ��ȯ
	// ������ ��ġ ��ǥ�� ��ȯ �� ��ǥ(SV_POSITION)�̴�. -> ������ǥ���� ��ǥ

	if (nVertexID == 0) output = float4(0.f, 0.5f, 0.5f, 1.f);
	else if (nVertexID == 1) output = float4(0.5f, -0.5f, 0.5f, 1.f);
	else if (nVertexID == 2) output = float4(-0.5f, -0.5f, 0.5f, 1.f);

	return output;
}