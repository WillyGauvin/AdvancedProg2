#pragma once

// 

// CPU [        ]    GPU [        ]
//
class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature>	_signature; //6.6.5 Root Signature and Descriptor Tables
											//The root signature defines what resources the application will bind to the
											//rendering pipeline before a draw call can be executed and where those resources get
											//mapped to shader input registers.
};

