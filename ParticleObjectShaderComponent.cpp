#include "ParticleObjectShaderComponent.h"
#include "ParticleVertex.h"

void CParticleObjectShaderComponent::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType)
{
	m_pd3dVertexShaderBlob.resize(2);
	m_pd3dPixelShaderBlob.resize(1);
	m_pd3dGeometryShaderBlob.resize(2);
	m_ppd3dPipelineStates.resize(2);
	CreateStreamOutputShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateDrawShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

void CParticleObjectShaderComponent::CreateStreamOutputShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType)
{
	::ZeroMemory(&m_d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	m_d3dPipelineStateDesc.VS = CreateStreamOutputVertexShader();
	m_d3dPipelineStateDesc.PS = CreatePixelShader();
	m_d3dPipelineStateDesc.GS = CreateStreamOutputGeometryShader();
	m_d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	m_d3dPipelineStateDesc.BlendState = CreateBlendState();
	m_d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	m_d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	m_d3dPipelineStateDesc.StreamOutput = CreateStreamOutputLayout();
	m_d3dPipelineStateDesc.SampleMask = UINT_MAX;
	m_d3dPipelineStateDesc.PrimitiveTopologyType = eprimitiveTopologyType;
	m_d3dPipelineStateDesc.NumRenderTargets = 1;
	m_d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_d3dPipelineStateDesc.SampleDesc.Count = 1;
	m_d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)(m_ppd3dPipelineStates[0].GetAddressOf()));
}

void CParticleObjectShaderComponent::CreateDrawShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType)
{
	::ZeroMemory(&m_d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	m_d3dPipelineStateDesc.VS = CreateDrawVertexShader();
	m_d3dPipelineStateDesc.PS = CreateDrawPixelShader();
	m_d3dPipelineStateDesc.GS = CreateDrawGeometryShader();
	m_d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	m_d3dPipelineStateDesc.BlendState = CreateBlendState();
	m_d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	m_d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	//m_d3dPipelineStateDesc.StreamOutput = CreateStreamOutputLayout();
	m_d3dPipelineStateDesc.SampleMask = UINT_MAX;
	m_d3dPipelineStateDesc.PrimitiveTopologyType = eprimitiveTopologyType;
	m_d3dPipelineStateDesc.NumRenderTargets = 1;
	m_d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_d3dPipelineStateDesc.SampleDesc.Count = 1;
	m_d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)(m_ppd3dPipelineStates[1].GetAddressOf()));
}

void CParticleObjectShaderComponent::Init()
{

}

void CParticleObjectShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CParticleObjectShaderComponent::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShaderComponent::PrepareRender(pd3dCommandList);
	m_iCurrentPipelineState = m_iCurrentPipelineState == 1 ? 0 : 1; // 스트림 -> 드로우
}

void CParticleObjectShaderComponent::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

D3D12_SHADER_BYTECODE CParticleObjectShaderComponent::CreateStreamOutputVertexShader()
{
	D3DReadFileToBlob(L"ParticleObjectStreamOutputVertexShader.cso", m_pd3dVertexShaderBlob[0].GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob[0]->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob[0]->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CParticleObjectShaderComponent::CreateDrawVertexShader()
{
	D3DReadFileToBlob(L"ParticleObjectDrawVertexShader.cso", m_pd3dVertexShaderBlob[1].GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob[1]->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob[1]->GetBufferPointer();

	return d3dShaderByteCode;
}


D3D12_SHADER_BYTECODE CParticleObjectShaderComponent::CreateDrawPixelShader()
{
	D3DReadFileToBlob(L"ParticleObjectDrawPixelShader.cso", m_pd3dPixelShaderBlob[0].GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob[0]->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob[0]->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CParticleObjectShaderComponent::CreateStreamOutputGeometryShader()
{
	D3DReadFileToBlob(L"ParticleObjectStreamOutputGeometryShader.cso", m_pd3dGeometryShaderBlob[0].GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dGeometryShaderBlob[0]->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dGeometryShaderBlob[0]->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CParticleObjectShaderComponent::CreateDrawGeometryShader()
{
	D3DReadFileToBlob(L"ParticleObjectDrawGeometryShader.cso", m_pd3dGeometryShaderBlob[1].GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dGeometryShaderBlob[1]->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dGeometryShaderBlob[1]->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_BLEND_DESC CParticleObjectShaderComponent::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_INPUT_LAYOUT_DESC CParticleObjectShaderComponent::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	UINT nInputElementDescs = 4;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "PARTICLETYPE", 0, DXGI_FORMAT_R32_UINT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_STREAM_OUTPUT_DESC CParticleObjectShaderComponent::CreateStreamOutputLayout()
{
	D3D12_STREAM_OUTPUT_DESC d3dStreamOutputDesc;
	::ZeroMemory(&d3dStreamOutputDesc, sizeof(D3D12_STREAM_OUTPUT_DESC));

	UINT nStreamOutputDecls = 4;
	D3D12_SO_DECLARATION_ENTRY* pd3dStreamOutputDecls = new D3D12_SO_DECLARATION_ENTRY[nStreamOutputDecls];
	pd3dStreamOutputDecls[0] = { 0, "POSITION", 0, 0, 3, 0 };
	pd3dStreamOutputDecls[1] = { 0, "VELOCITY", 0, 0, 3, 0 };
	pd3dStreamOutputDecls[2] = { 0, "LIFETIME", 0, 0, 1, 0 };
	pd3dStreamOutputDecls[3] = { 0, "PARTICLETYPE", 0, 0, 1, 0 };

	UINT* pBufferStrides = new UINT[1];
	pBufferStrides[0] = sizeof(CParticleVertex);

	d3dStreamOutputDesc.NumEntries = nStreamOutputDecls;
	d3dStreamOutputDesc.pSODeclaration = pd3dStreamOutputDecls;
	d3dStreamOutputDesc.NumStrides = 1;
	d3dStreamOutputDesc.pBufferStrides = pBufferStrides;
	d3dStreamOutputDesc.RasterizedStream = D3D12_SO_NO_RASTERIZED_STREAM;
	

	return(d3dStreamOutputDesc);
}

void CParticleObjectShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CParticleObjectShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CParticleObjectShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
}

D3D12_DEPTH_STENCIL_DESC CParticleObjectShaderComponent::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(d3dDepthStencilDesc);
}
