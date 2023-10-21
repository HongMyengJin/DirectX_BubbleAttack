#include "Scene.h"
#include "ObjectShaderComponent.h"
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

}

bool CScene::ProcessInput()
{
    return false;
}

void CScene::Release()
{
}
