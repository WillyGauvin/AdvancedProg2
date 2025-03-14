#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

void Engine::Init(const WindowInfo& info)
{
	_window = info;	

	// 
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_tableDescHeap->Init(256);
	_depthStencilBuffer->Init(_window);
//STEP 12 - LOOK AT 
	//CALL CreateConstantBuffer() passing in CBV_REGISTER::b0, sizeof(TransformParams), 256
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformParams), 256);
	//CALL CreateConstantBuffer() passing in CBV_REGISTER::b1, sizeof(MaterialParams), 256
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 256);

	//CALL ResizeWindow() passing in info.width, info.height
	ResizeWindow(info.width, info.height);

	//CALL GET_SINGLE(Input)->Init(info.hwnd)
	GET_SINGLE(Input)->Init(info.hwnd);
	//CALL GET_SINGLE(Timer)->Init()
	GET_SINGLE(Timer)->Init();
}

void Engine::Update()
{
	//CALL GET_SINGLE(Input)->Update()
	GET_SINGLE(Input)->Update();
	//CALL GET_SINGLE(Timer)->Update()
	GET_SINGLE(Timer)->Update();

	//STEP 12 - IMPL
	//GET the SceneManager and Update it
	GET_SINGLE(SceneManager)->Update();

	//CALL Render()
	Render();

	ShowFps();
}

void Engine::Render()
{
	//CALL RenderBegin()
	RenderBegin();

	//STEP 12 - IMPL
	//GET the SceneManager and Render it
	GET_SINGLE(SceneManager)->Render();

	//CALL RenderEnd()
	RenderEnd();
}

void Engine::RenderBegin()
{
	//CALL RenderBegin() on _cmdQueue passing in &_viewport, &_scissorRect
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

	_depthStencilBuffer->Init(_window);
}

void Engine::ShowFps()
{
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_window.hwnd, text);
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	//DECLARE a variable called typeInt of type uint8 and assign it to reg, static_cast() reg
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	//DECLARE a shared_ptr variable called buffer of type ConstantBuffer and initialize it
	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	//CALL Init() on buffer passing in reg, bufferSize, count
	buffer->Init(reg, bufferSize, count);
	//ADD buffer to _constantBuffers
	_constantBuffers.push_back(buffer);
}
