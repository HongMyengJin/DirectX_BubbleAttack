#include "TerrainTessellationShaderComponent.h"

void CTerrainTessellationShaderComponent::Init()
{
}

void CTerrainTessellationShaderComponent::Update(float fTimeElapsed, void* pData, void* pData2)
{
}



D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreateVertexShader()
{
	D3DReadFileToBlob(L"TerrainTessellationObjectVertexShader.cso", m_pd3dVertexShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dVertexShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dVertexShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreatePixelShader()
{
	D3DReadFileToBlob(L"TerrainTessellationObjectPixelShader.cso", m_pd3dPixelShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreatePixel2Shader()
{
	D3DReadFileToBlob(L"TerrainTessellationObjectWirePixel.cso", m_pd3dPixelShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dPixelShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dPixelShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreateDomainShader()
{
	D3DReadFileToBlob(L"TerrainTessellationObjectDomainShader.cso", m_pd3dDomainShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dDomainShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dDomainShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_SHADER_BYTECODE CTerrainTessellationShaderComponent::CreateHullShader()
{
	D3DReadFileToBlob(L"TerrainTessellationObjectHullShader.cso", m_pd3dHullShaderBlob.GetAddressOf());

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = m_pd3dHullShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = m_pd3dHullShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC CTerrainTessellationShaderComponent::CreateInputLayout()
{
	UINT nInputElementDescs = 5;

	m_d3dPdInputElementDescs.resize(nInputElementDescs);

	m_d3dPdInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_d3dPdInputElementDescs[4] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = &m_d3dPdInputElementDescs[0];
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_RASTERIZER_DESC CTerrainTessellationShaderComponent::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

void CTerrainTessellationShaderComponent::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, DXGI_FORMAT pdxgiRtvFormats, DXGI_FORMAT dxgiDsvFormat, D3D12_PRIMITIVE_TOPOLOGY_TYPE eprimitiveTopologyType)
{
	m_ppd3dPipelineStates.resize(2);

	::ZeroMemory(&m_d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	m_d3dPipelineStateDesc.VS = CreateVertexShader();
	m_d3dPipelineStateDesc.PS = CreatePixel2Shader();
	m_d3dPipelineStateDesc.GS = CreateGeometryShader();
	m_d3dPipelineStateDesc.HS = CreateHullShader();
	m_d3dPipelineStateDesc.DS = CreateDomainShader();
	m_d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	m_d3dPipelineStateDesc.BlendState = CreateBlendState();
	m_d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	m_d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	m_d3dPipelineStateDesc.StreamOutput = CreateStreamOutputLayout();
	m_d3dPipelineStateDesc.SampleMask = UINT_MAX;
	m_d3dPipelineStateDesc.PrimitiveTopologyType = eprimitiveTopologyType;
	m_d3dPipelineStateDesc.NumRenderTargets = 1;
	m_d3dPipelineStateDesc.RTVFormats[0] = pdxgiRtvFormats;//DXGI_FORMAT_R8G8B8A8_UNORM;
	m_d3dPipelineStateDesc.DSVFormat = dxgiDsvFormat;//DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_d3dPipelineStateDesc.SampleDesc.Count = 1;
	m_d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)(m_ppd3dPipelineStates[0].GetAddressOf()));

	m_d3dPipelineStateDesc.PS = CreatePixelShader();
	m_d3dPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)(m_ppd3dPipelineStates[1].GetAddressOf()));
}

void CTerrainTessellationShaderComponent::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainTessellationShaderComponent::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainTessellationShaderComponent::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
}
