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
    // 그래픽 루트 시그니쳐를 설정
    pd3dCommandList->SetGraphicsRootSignature(pd3dRootSignature);

    // 파이프라인 상태 설정
    pd3dCommandList->SetPipelineState(pd3dPipelineState);

    // 프리미티브 토폴로지 설정
    pd3dCommandList->IASetPrimitiveTopology(TopologyType);
}

void CSceneComponent::Render(ID3D12RootSignature* pd3dRootSignature, ID3D12PipelineState* pd3dPipelineState, ID3D12GraphicsCommandList* pd3dCommandList)
{
    PrepareRender(pd3dRootSignature, pd3dPipelineState, pd3dCommandList);

    pd3dCommandList->DrawInstanced(3, 1, 0, 0);
}
