#pragma once


// 
class Device
{
public:
	void Init();

	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
	// COM(Component Object Model)
	
	ComPtr<ID3D12Debug>			_debugController; //The D3D Debug Layer
	ComPtr<IDXGIFactory>		_dxgi; // In order to create a WARP (Windows Advanced Rasterization Platform) adapter, we need to create an IDXGIFactory4 object so that we can enumerate the warp adapter
	ComPtr<ID3D12Device>		_device; // The Graphics Device Handle
};

