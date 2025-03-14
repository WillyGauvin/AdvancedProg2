#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

void RootSignature::Init()
{
	CreateSamplerDesc();
	CreateRootSignature();
}

void RootSignature::CreateRootSignature()
{
	//DECLARE a range of descriptors 
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0), // b0~b4
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4
	};

	//DECLARE a variable called param of type CD3DX12_ROOT_PARAMETER with 1 element
	CD3DX12_ROOT_PARAMETER param[1];
	//CALL InitAsDescriptorTable() on param[0] passing _countof(ranges), ranges
	param[0].InitAsDescriptorTable(_countof(ranges), ranges);

	//DECLARE a variable called sigDesc of type D3D12_ROOT_SIGNATURE_DESC and ASSIGN it to the return value of  CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc) //STEP 6 NEW
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc);
	//ASSIGN sigDesc.Flags to D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 

	//DECLARE a COmPtr of type ID3DBlob called blobSignature
	ComPtr<ID3DBlob> blobSignature;
	//DECLARE a ComPtr of type ID3DBlob called blobError
	ComPtr<ID3DBlob> blobError;
	//CALL ::D3D12SerializeRootSignature() passing in &sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	//CALL CreateRootSignature() on DEVICE passing in 0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature)
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}

void RootSignature::CreateSamplerDesc()
{
	//ASSIGN _samplerDesc to the return value of CD3DX12_STATIC_SAMPLER_DESC(0)
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}