#include "stdafx.h"
class CScene
{
public:
	CScene();
	~CScene();

	virtual void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	virtual void CreateGraphicsPipelineState(ID3D12Device* pd3dDevice);

	void BuildObjects(ID3D12Device* pd3dDevice);

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void UpdateObjects(float fTimeElapsed);

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature>		m_pd3dGraphicsRootsignature; // ��Ʈ �ñ״����� �������̽� ������
	Microsoft::WRL::ComPtr<ID3D12PipelineState>		m_pd3dPipelineState; // ���������� ���¸� ��Ÿ���� �������̽� ������
};