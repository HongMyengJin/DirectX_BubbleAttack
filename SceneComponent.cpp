#include "SceneComponent.h"

CSceneComponent::CSceneComponent()
{
}

CSceneComponent::~CSceneComponent()
{
}

bool CSceneComponent::ProcessInput()
{
    return false;
}

void CSceneComponent::AnimateObjects(float fTimeElapsed)
{
}

void CSceneComponent::UpdateObjects(float fTimeElapsed)
{
}

void CSceneComponent::PrepareRender(ID3D12RootSignature* pd3dRootSignature, ID3D12PipelineState* pd3dPipelineState, ID3D12GraphicsCommandList* pd3dCommandList)
{
    // �׷��� ��Ʈ �ñ״��ĸ� ����
    pd3dCommandList->SetGraphicsRootSignature(pd3dRootSignature);

    // ���������� ���� ����
    pd3dCommandList->SetPipelineState(pd3dPipelineState);

    // ������Ƽ�� �������� ����
    pd3dCommandList->IASetPrimitiveTopology(TopologyType);
}

void CSceneComponent::Render(ID3D12RootSignature* pd3dRootSignature, ID3D12PipelineState* pd3dPipelineState, ID3D12GraphicsCommandList* pd3dCommandList)
{
    PrepareRender(pd3dRootSignature, pd3dPipelineState, pd3dCommandList);

    pd3dCommandList->DrawInstanced(3, 1, 0, 0);
}
