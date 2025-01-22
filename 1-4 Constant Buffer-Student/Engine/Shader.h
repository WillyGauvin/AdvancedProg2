#pragma once

// 
class Shader
{
public:
	void Init(const wstring& path);
	void Update();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ComPtr<ID3DBlob>					_vsBlob;//ID3DBlob data structure that stores the compiled shader object bytecode
	ComPtr<ID3DBlob>					_psBlob; //ID3DBlob data structure that stores the compiled shader object bytecode
	ComPtr<ID3DBlob>					_errBlob;//ID3DBlob data structure that stores a string containing the compilation errors, if any

	ComPtr<ID3D12PipelineState>			_pipelineState; //The PSO (Pipeline State Object). Represents the state of all currently set shaders as well as certain fixed function state objects.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {}; //Describes a graphics pipeline state object.
};

