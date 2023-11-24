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
}

bool CScene::ProcessInput(HWND hWnd, float fTimeElapsed)
{
    return false;
}

void CScene::Release()
{
}

void CScene::SetChangeScene(bool bChangeScene)
{
    m_bChangeScene = bChangeScene;
}

bool CScene::GetChangeScene()
{
    return m_bChangeScene;
}
