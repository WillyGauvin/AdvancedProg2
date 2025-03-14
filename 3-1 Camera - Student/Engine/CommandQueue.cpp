#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	//ASSIGN _swapChain to swapChain
	_swapChain = swapChain;

	//DECLARE a variable of type D3D12_COMMAND_QUEUE_DESC called queueDesc initialize it to {} ( = {} ) //DONE
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	//ASSIGN queueDesc.Type to D3D12_COMMAND_LIST_TYPE_DIRECT //DONE
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//ASSIGN queueDesc.Flags to D3D12_COMMAND_QUEUE_FLAG_NONE //DONE
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	//CALL CreateCommandQueue() on the device passing in &queueDesc, IID_PPV_ARGS(&_cmdQueue) //DONE
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU
	//CALL CreateCommandAllocator() on device passing in D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc) //DONE
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	/*
	DIRECT or BUNDLE
	Allocator
	*/
	//CALL CreateCommandList() on device passing in 0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList) //DONE
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	/* CommandList Close / Open
	 Close the open command list */
	 //CALL Close() on _cmdList //DONE
	_cmdList->Close();

	//CALL CreateCommandAllocator() on device passing in D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc)
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	//CALL CreateCommandList() on device passing in 0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));

	// CreateFence
	// - CPU, GPU
	//CALL CreateFence() on device passing in 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	//ASSIGN _fenceEvent to the return value of ::CreateEvent() passing in "nullptr, FALSE, FALSE, nullptr" to CreateEvent()
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	//CALL Reset() on _cmdAlloc DONE
	_cmdAlloc->Reset();
	//CALL Reset() on _cmdList passing in _cmdAlloc.Get(), nullptr
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	//DECLARE a variable of type D3D12_RESOURCE_BARRIER called barrier and ASSIGN it to the return value of CD3DX12_RESOURCE_BARRIER::Transition(), pass in
	//_swapChain->GetBackRTVBuffer().Get(),
	//D3D12_RESOURCE_STATE_PRESENT,
	//D3D12_RESOURCE_STATE_RENDER_TARGET to Transition() 
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, //
		D3D12_RESOURCE_STATE_RENDER_TARGET); // 

	//CALL SetGraphicsRootSignature() on _cmdList passing in the return value of ROOT_SIGNATURE.Get()
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	//STEP 9 NEW
		//CALL Clear() on the constant buffer by calling GEngine->GetConstantBuffer() passing in CONSTANT_BUFFER_TYPE::TRANSFORM to GEngine->GetConstantBuffer()
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	//CALL Clear() on the constant buffer by calling GEngine->GetConstantBuffer() passing in CONSTANT_BUFFER_TYPE::MATERIAL to GEngine->GetConstantBuffer()
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();
	//END STEP 9 NEW

		//CALL GEngine->GetTableDescHeap()->Clear()
	GEngine->GetTableDescHeap()->Clear();

	//DECLARE a variable called descHeap of type ID3D12DescriptorHeap and ASSIGN it to the return value of GEngine->GetTableDescHeap()->GetDescriptorHeap().Get()
	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	//CALL SetDescriptorHeaps() on _cmdList passing in 1, &descHeap
	_cmdList->SetDescriptorHeaps(1, &descHeap);
	//CALL ResourceBarrier() on cmdList passing in 1, &barrier
	_cmdList->ResourceBarrier(1, &barrier);

	/*Set the viewport and scissor rect.This needs to be reset whenever the command list is reset.*/
	// CALL RSSetViewports() on _cmdList passing in 1, vp DONE
	_cmdList->RSSetViewports(1, vp);
	//CALL RSSetScissorRects() on _cmdList passing in 1, rect DONE
	_cmdList->RSSetScissorRects(1, rect);

	/* Specify the buffers we are going to render to. */
	//DECLARE a variable called backBufferView of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of _swapChain->GetBackRTV() DONE
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	//CALL ClearRenderTargetView() on _cmdList passing in backBufferView, Colors::LightSteelBlue, 0, nullptr DONE
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);

	//DECLARE a variable called depthStencilView of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of GEngine->GetDepthStencilBuffer()->GetDSVCpuHandle()
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GEngine->GetDepthStencilBuffer()->GetDSVCpuHandle();

	//CALL OMSetRenderTargets() on _cmdList passing in 1, &backBufferView, FALSE, &depthStencilView
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	//CALL ClearDepthStencilView() on _cmdList passing in depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr
	_cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void CommandQueue::RenderEnd()
{
	//DECLARE a variable of type D3D12_RESOURCE_BARRIER called barrier and ASSIGN it to the return value of CD3DX12_RESOURCE_BARRIER::Transition() passing in
	//_swapChain->GetCurrentBackBufferResource().Get(),
	//D3D12_RESOURCE_STATE_RENDER_TARGET,
	//D3D12_RESOURCE_STATE_PRESENT to Transition() DONE
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 
		D3D12_RESOURCE_STATE_PRESENT); // 
	//CALL ResourceBarrier() on cmdList passing in 1, &barrier DONE
	_cmdList->ResourceBarrier(1, &barrier);
	//CALL Close() on _cmdList DONE
	_cmdList->Close();

	/*define an array of pointer to ID3D12CommandList*/
	// DECLARE a variable called cmdListArr[] of type ID3D12CommandList* and ASSIGN it to  { _cmdList.Get() } DONE 
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	//CALL ExecuteCommandLists() on _cmdQueue passing in _countof(cmdListArr), cmdListArr DONE
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);
	//CALL Present() on _swapChain DONE
	_swapChain->Present();

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	//CALL WaitSync() DONE
	WaitSync();
	//CALL SwapIndex() on _swapChain DONE
	_swapChain->SwapIndex();
}

void CommandQueue::FlushResourceCommandQueue()
{
	//CALL Close() on _resCmdList
	_resCmdList->Close();
	// DECLARE a variable called cmdListArr[] of type ID3D12CommandList* and ASSIGN it to  { _resCmdList.Get() }  
	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	// CALLExecuteCommandLists() on _cmdQueue passing in _countof(cmdListArr), cmdListArr
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);
	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	//CALL WaitSync()
	WaitSync();
	//CALL Reset() on _resCmdAlloc
	_resCmdAlloc->Reset();
	//CALL Reset() on _resCmdList passing in _resCmdAlloc.Get(), nullptr
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}