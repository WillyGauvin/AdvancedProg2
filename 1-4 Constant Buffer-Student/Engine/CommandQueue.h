#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return	_cmdList; }

private:
	// CommandQueue : DX12
	ComPtr<ID3D12CommandQueue>			_cmdQueue;//The ID3D12CommandQueue is a new construct that represents the ability of a GPU to execute multiple jobs at once. Various architectures have the capabilities to do graphics, compute and copy commands simultaneously. In Direct3D 11, this was managed by the driver based on the commands submitted by the user.
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;//A command allocator allows the app to manage the memory that is allocated for command lists
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;// It represents an ordered set of commands that the GPU executes

	// Fence : 
	ComPtr<ID3D12Fence>					_fence; //Represents a fence, an object used for synchronization of the CPU and one or more GPUs
	uint32								_fenceValue = 0;// Keep track of the fence point
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;// A handle to the fence event

	shared_ptr<SwapChain>		_swapChain;//Pointer to the swap chain (Swap between Back and Front Buffer)
};

