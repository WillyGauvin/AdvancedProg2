#pragma once

//STEP 3
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer;// Encapsulates a generalized ability of the CPU and GPU to read and write to physical memory, or heaps
	BYTE*					_mappedBuffer = nullptr; // This is the output parameter that will contain a pointer to the data in the resource after the function call
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	uint32					_currentIndex = 0;
};

