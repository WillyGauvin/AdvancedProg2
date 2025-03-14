#include "pch.h"
#include "Device.h"

void Device::Init()
{
	// D3D12 
#ifdef _DEBUG
	//CALL ::D3D12GetDebugInterface() passing in IID_PPV_ARGS(&_debugController)
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	//CALL EnableDebugLayer() on _debugController
	_debugController->EnableDebugLayer();
#endif

	/*DXGI(DirectX Graphics Infrastructure)*/
	//CALL ::CreateDXGIFactory() passing in IID_PPV_ARGS(&_dxgi)
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

	/* CreateDevice */
	//CALL ::D3D12CreateDevice() passing in nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
}
