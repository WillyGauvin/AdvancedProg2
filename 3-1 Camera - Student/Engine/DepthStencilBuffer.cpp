#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Engine.h"

void DepthStencilBuffer::Init(const WindowInfo& window, DXGI_FORMAT dsvFormat)
{
	//ASSIGN _dsvFormat = dsvFormat
	_dsvFormat = dsvFormat;

	//DECLARE a variable called heapProperty of type D3D12_HEAP_PROPERTIES and ASSIGN it to the return value of CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	/*Describe a 2D texture*/
	//DECLARE a variable called desc of type D3D12_RESOURCE_DESC and ASSIGN it to the return value of CD3DX12_RESOURCE_DESC::Tex2D() passing in _dsvFormat, window.width, window.height to CD3DX12_RESOURCE_DESC::Tex2D()
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(_dsvFormat, window.width, window.height);
	//SET desc.Flags to D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//DECLARE a variable called optimizedClearValue of type D3D12_CLEAR_VALUE and ASSIGN it to the return value of CD3DX12_CLEAR_VALUE() passing in _dsvFormat, 1.0f, 0
	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(_dsvFormat, 1.0f, 0);

	//CALL CreateCommittedResource() on DEVICE passing in
	//&heapProperty,
	//D3D12_HEAP_FLAG_NONE,
	//&desc,
	//D3D12_RESOURCE_STATE_DEPTH_WRITE,
	//&optimizedClearValue,
	//IID_PPV_ARGS(&_dsvBuffer)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&_dsvBuffer));

	//DECLARE a variable called heapDesc of type D3D12_DESCRIPTOR_HEAP_DESC
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	//SET heapDesc.NumDescriptors to 1
	heapDesc.NumDescriptors = 1;
	//SET heapDesc.Flags to D3D12_DESCRIPTOR_HEAP_FLAG_NONE
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//SET heapDesc.Type to D3D12_DESCRIPTOR_HEAP_TYPE_DSV
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	//CALL CreateDescriptorHeap() on DEVICE passing in &heapDesc, IID_PPV_ARGS(&_dsvHeap)
	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_dsvHeap));

	//ASSIGN _dsvHandle to the return value of _dsvHeap->GetCPUDescriptorHandleForHeapStart()
	_dsvHandle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//CALL CreateDepthStencilView() on DEVICE passing in _dsvBuffer.Get(), nullptr, _dsvHandle
	DEVICE->CreateDepthStencilView(_dsvBuffer.Get(), nullptr, _dsvHandle);
}
