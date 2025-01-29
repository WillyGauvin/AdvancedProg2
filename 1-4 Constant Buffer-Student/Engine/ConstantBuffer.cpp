#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

//STEP 3
ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	//IF _cbvBuffer
	if(_cbvBuffer)
	{
		//CALL Unmap() on _cbvBuffer passing in 0, nullptr
		_cbvBuffer->Unmap(0, nullptr);

		//SET _cbvBuffer to nullptr
		_cbvBuffer = nullptr;
	}
	//ENDIF
}



void ConstantBuffer::Init(uint32 size, uint32 count)
{
	//STEP 3
	// 0 256 512 768
	_elementSize = (size + 255) & ~255;//SEE BELOW
	_elementCount = count;

	//CALL CreateBuffer() 
	CreateBuffer();
}

void ConstantBuffer::CreateBuffer()
{
	//DECLARE a variable called bufferSize of type uint32 and ASSIGN it to _elementSize * _elementCount
	uint32 bufferSize = _elementSize * _elementCount;

	//DECLARE a variable called heapProperty of type D3D12_HEAP_PROPERTIES and ASSIGN it to the return value of CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//DECLARE a variable called desc and ASSIGN it to the return value of CD3DX12_RESOURCE_DESC::Buffer( ? )
	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//CALL CreateCommittedResource() on DEVICE passing in:
	DEVICE->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_cbvBuffer));

	//CALL Map() on _cbvBuffer passing in 0, nullptr, reinterpret_cast<void**>( ? )
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	assert(_currentIndex < _elementSize);
	//CALL ::memcpy() passing in &?[ ? * ?], ?, ?
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	//DECLARE a variable called address of type D3D12_GPU_VIRTUAL_ADDRESS and ASSIGN it to the return value of GetGpuVirtualAddress(?)
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	//CALL SetGraphicsRootConstantBufferView() on CMD_LIST passing in ?, ?
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	//INCREMENT _currentIndex
	_currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	//DECLARE a variable called objCBAddress of type D3D12_GPU_VIRTUAL_ADDRESS and ASSIGN it to the return value of _cbvBuffer->?()
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	//ADD (index * _elementSize) to objCBAddress
	objCBAddress += (index * _elementSize);
	//RETURN objCBAddress
	return objCBAddress;
}
/*

 The given statement _elementSize = (size + 255) & ~255 in C++ is performing a two-step operation on the value of `size` and assigning the result to `_elementSize`.

1. `(size + 255)`
2. `... & ~255`

Let's break it down:

1. `(size + 255)`: This simply adds 255 to the original `size`.

2. `... & ~255`: This is a bitwise operation. 
   
    a. `~255`: The tilde `~` is a bitwise NOT operation. It flips all bits of the number. For the value `255`, which in binary is `11111111`, the NOT operation will make it `1111111100000000` (assuming an integer representation with at least 16 bits). This number in hexadecimal is `0xFF00`.

    b. `... & ~255`: The ampersand `&` is a bitwise AND operation. By AND-ing with `0xFF00`, this operation effectively rounds up the `size` to the nearest multiple of 256. It masks out the lowest 8 bits, setting them to 0.

In essence, the statement is aligning `size` to the nearest higher (or equal) multiple of 256 and assigning this value to `_elementSize`.

For example:
- If `size` is 100, `_elementSize` will be 256.
- If `size` is 256, `_elementSize` will remain 256.
- If `size` is 257, `_elementSize` will be 512.
- ... and so on.

This kind of operation is often used in low-level programming for aligning memory or data sizes to certain boundaries, which can be beneficial for performance or to satisfy hardware or API requirements.
 */