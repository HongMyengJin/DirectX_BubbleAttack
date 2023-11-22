#pragma once
#include "stdafx.h"
#include "GameObject.h"
// 카메라 정보
struct CameraInfo
{
	XMFLOAT4X4						m_xmf4x4View;
	XMFLOAT4X4						m_xmf4x4Projection;
	XMFLOAT4X4						m_xmf4x4InverseView;
	XMFLOAT3						m_xmf3Position;
};

class CCamera
{
public:

	CCamera() {};
	~CCamera() { };

	virtual void Init();
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed);
	virtual void Update(CGameObject* pTargetObject, XMFLOAT3& xmf3LookAt, float fTimeElapsed);

	//virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	//virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);
	void SetLookAt(CGameObject* pTargetObject, XMFLOAT3& vLookAt);

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetPosition(XMFLOAT3 xmf3Position);
	XMFLOAT3 GetPosition();

	XMFLOAT4X4&	Getxmf4x4View() { return m_xmf4x4View;}
	XMFLOAT4X4&	Getxmf4x4Projection() { return m_xmf4x4Projection;}

	void Release();

public:
	XMFLOAT3													m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3													m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);;
	XMFLOAT3													m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);;
	XMFLOAT3													m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	float           											m_fPitch;
	float           											m_fRoll;
	float           											m_fYaw;

	XMFLOAT3													m_xmf3LookAtWorld;
	XMFLOAT3													m_xmf3Offset = XMFLOAT3(0.0f, 15.0f * 4.f, -30.0f * 2.5f);
	float           											m_fTimeLag = 0.5f;

	XMFLOAT4X4													m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4													m_xmf4x4Projection = Matrix4x4::Identity();

	D3D12_VIEWPORT												m_d3dViewport = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT													m_d3dScissorRect = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };
	
	std::unique_ptr<CameraInfo>									m_pCameraInfo;
	Microsoft::WRL::ComPtr<ID3D12Resource>						m_pd3dcbCamera;
	Microsoft::WRL::ComPtr<ID3D12Resource>						m_pd3dCameraUploadBuffers;

};

