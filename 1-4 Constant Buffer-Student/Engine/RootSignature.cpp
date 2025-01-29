#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	//STEP 3 NEW
	//DECLARE a variable called param of type CD3DX12_ROOT_PARAMETER with 2 elements
	CD3DX12_ROOT_PARAMETER param[2];
	//INITIALIZE param[0] as a Constant Buffer View 0 -> b0 -> CBV (Shader Register 0)
	param[0].InitAsConstantBufferView(0);
	//INITIALIZE param[1] as a Constant Buffer View 1 -> b1 -> CBV (Shader Register 1)
	param[1].InitAsConstantBufferView(1);


	//DECLARE a variable called sigDesc of type D3D12_ROOT_SIGNATURE_DESC and ASSIGN it to the return value of  CD3DX12_ROOT_SIGNATURE_DESC(?, ?)
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);
	//ASSIGN sigDesc.Flags to D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; 
	//END STEP 3 NEW

	//DECLARE a COmPtr of type ID3DBlob called blobSignature
	ComPtr<ID3DBlob> blobSignature;
	//DECLARE a ComPtr of type ID3DBlob called blobError
	ComPtr<ID3DBlob> blobError;
	//CALL ::D3D12SerializeRootSignature() passing in &sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	//CALL CreateRootSignature() on device passing in 0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature)
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}