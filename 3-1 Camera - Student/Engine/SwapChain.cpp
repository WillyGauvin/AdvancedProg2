#include "pch.h"
#include "SwapChain.h"


void SwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(info, dxgi, cmdQueue);
	CreateRTV(device);
}

void SwapChain::Present()
{
	// Present the frame.
	_swapChain->Present(0, 0);
}

void SwapChain::SwapIndex()
{
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// 
	_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(info.width); // 
	sd.BufferDesc.Height = static_cast<uint32>(info.height); // 
	sd.BufferDesc.RefreshRate.Numerator = 60; // 
	sd.BufferDesc.RefreshRate.Denominator = 1; // 
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 
	sd.OutputWindow = info.hwnd;
	sd.Windowed = info.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//CALL CreateSwapChain() on dxgi passing in cmdQueue.Get(), &sd, &_swapChain
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);

	//FOR i equal 0; i LESS THAN SWAP_CHAIN_BUFFER_COUNT; Increment i
	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		//CALL GetBuffer() on _swapChain passing in i, IID_PPV_ARGS(&_rtvBuffer[i])
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
	//ENDFOR
}

void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// Descriptor (DX12) = View (~DX11)

	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	//CALL CreateDescriptorHeap() on the device passing in &rtvDesc, IID_PPV_ARGS(&_rtvHeap)
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));
	//DECLARE a variable called rtvHeapBegin of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of _rtvHeap->GetCPUDescriptorHandleForHeapStart()
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		//ASSIGN _rtvHandle[i] to the return value of CD3DX12_CPU_DESCRIPTOR_HANDLE() passing in rtvHeapBegin, i * rtvHeapSize
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		//CALL CreateRenderTargetView() on device passing in _rtvBuffer[i].Get(), nullptr, _rtvHandle[i]
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);
	}
}