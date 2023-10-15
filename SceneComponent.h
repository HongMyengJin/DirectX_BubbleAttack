#include "stdafx.h"
#include "Component.h"
class CSceneComponent : public CComponent
{
public:
	CSceneComponent();
	~CSceneComponent();

	virtual bool ProcessInput();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void UpdateObjects(float fTimeElapsed);

	virtual void PrepareRender(ID3D12RootSignature* pd3dRootSignature, ID3D12PipelineState* pd3dPipelineState, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12RootSignature* pd3dRootSignature, ID3D12PipelineState* pd3dPipelineState, ID3D12GraphicsCommandList* pd3dCommandList);

protected:
	D3D_PRIMITIVE_TOPOLOGY TopologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

};