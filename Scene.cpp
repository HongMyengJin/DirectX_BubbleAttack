#include "Scene.h"

CScene::CScene ()
{
}

CScene::~CScene ()
{
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice)
{
    CreateGraphicsRootSignature(pd3dDevice);
    CreateGraphicsPipelineState(pd3dDevice);
}

bool CScene::ProcessInput()
{
    return false;
}
