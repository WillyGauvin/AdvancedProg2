#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);

		_cbvBuffer = nullptr;
	}
}


//STEP 9 NEW
void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{
	_reg = reg;//STEP 9 NEW

	// 
	// 0 256 512 768
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
	CreateView();
}

void ConstantBuffer::CreateBuffer()
{
	//DECLARE a variable called bufferSize of type uint32 and ASSIGN it to _elementSize * _elementCount
	uint32 bufferSize = _elementSize * _elementCount;
	//DECLARE a variable called heapProperty of type D3D12_HEAP_PROPERTIES and ASSIGN it to the return value of CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//DECLARE a variable called desc and ASSIGN it to the return value of CD3DX12_RESOURCE_DESC::Buffer(bufferSize)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//CALL CreateCommittedResource() on DEVICE passing in
	//&heapProperty,
	//D3D12_HEAP_FLAG_NONE,
	//&desc,
	//D3D12_RESOURCE_STATE_GENERIC_READ,
	//nullptr,
	//IID_PPV_ARGS(&_cbvBuffer)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	//CALL Map() on _cbvBuffer passing in 0, nullptr, reinterpret_cast<void**>(&_mappedBuffer)
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

void ConstantBuffer::CreateView()
{
	//DECLARE a variable called cbvDesc of type D3D12_DESCRIPTOR_HEAP_DESC and initialize it to {}
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	//SET cbvDesc.NumDescriptors to _elementCount
	cbvDesc.NumDescriptors = _elementCount;
	//SET cbvDesc.Flags to D3D12_DESCRIPTOR_HEAP_FLAG_NONE
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//SET cbvDesc.Type to D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//CALL CreateDescriptorHeap() on DEVICE passing in &cbvDesc, IID_PPV_ARGS(&_cbvHeap)
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap));

	//ASSIGN _cpuHandleBegin to the return value of _cbvHeap->GetCPUDescriptorHandleForHeapStart()
	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	//ASSIGN _handleIncrementSize to the return value of DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < _elementCount; ++i)
	{
		//DECLARE a variable called cbvHandle of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of GetCpuHandle(i)
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);
		//DECLARE a variable called cbvDesc of type D3D12_CONSTANT_BUFFER_VIEW_DESC and initialize it to {}
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		//SET cbvDesc.BufferLocation to _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;
		//SET cbvDesc.SizeInBytes to _elementSize
		cbvDesc.SizeInBytes = _elementSize;   // CB size is required to be 256-byte aligned.
		//CALL CreateConstantBufferView() on DEVICE passing in &cbvDesc, cbvHandle
		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}
//STEP 9 NEW
void ConstantBuffer::PushData(void* buffer, uint32 size)
{
	assert(_currentIndex < _elementCount);//STEP 9 NEW
	assert(_elementSize == ((size + 255) & ~255));//STEP 9 NEW

	//CALL ::memcpy() passing in &_mappedBuffer[_currentIndex * _elementSize], buffer, size
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);
	//DECLARE a variable called cpuHandle of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of GetCpuHandle(_currentIndex)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(_currentIndex);

	//CALL GEngine->GetTableDescHeap()->SetCBV() passing in cpuHandle, _reg
	GEngine->GetTableDescHeap()->SetCBV(cpuHandle, _reg);//STEP 9 NEW

	//INCREMENT _currentIndex++
	_currentIndex++;
	//STEP 9 NEW
}
//Look at this function
D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}
//Look at this function
D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(uint32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}