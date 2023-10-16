#include "Scene.h"

CScene::CScene ()
{
}

CScene::~CScene ()
{
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
    CreateGraphicsRootSignature(pd3dDevice);
    CreateGraphicsPipelineState(pd3dDevice);

    //m_pGameObject = std::make_unique<CGameObject>();
    //m_pGameObject->LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootsignature.Get(), "Model/Mi24.bin");
}

bool CScene::ProcessInput()
{
    return false;
}
