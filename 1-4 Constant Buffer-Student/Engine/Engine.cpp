#include "pch.h"
#include "Engine.h"

void Engine::Init(const WindowInfo& info)
{
	//ASSIGN _window to info
	_window = info;
	//CALL ResizeWindow() passing in info.width, info.height
	ResizeWindow(info.width, info.height);

	// 
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	//STEP 3 NEW
	//CREATE the constant buffer _cb
	_cb = make_shared<ConstantBuffer>();
	//CALL Init() on _device
	_device->Init();
	//CALL Init() on _cmdQueue passing in _device->GetDevice(), _swapChain
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	//CALL Init() on _swapChain passing in info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue()
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	//CALL Init() on _rootSignature passing in _device->GetDevice()
	_rootSignature->Init(_device->GetDevice());

	//STEP 3 NEW
	//INITIALIZE the constant buffer to the size of Transform and count of 256
	_cb->Init(sizeof(Transform), 256);
	
}

void Engine::Render()
{
	//CALL RenderBegin() 
	RenderBegin();

	// TODO : We will eventually render something here 

	//CALL RenderEnd()
	RenderEnd();
}

void Engine::RenderBegin()
{
	//CALL RenderBegin() on _cmdQueue passing &_viewport, &_scissorRect
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	//CALL RenderEnd() on _cmdQueue
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}