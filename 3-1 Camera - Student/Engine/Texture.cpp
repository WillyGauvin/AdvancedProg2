#include "pch.h"
#include "Texture.h"
#include "Engine.h"

void Texture::Init(const wstring& path)
{
	//CALL CreateTexture() passing in path
	CreateTexture(path);
	//CALL CreateView()
	CreateView();
}

void Texture::CreateTexture(const wstring& path)
{
	// 
	wstring ext = fs::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
	else if (ext == L".tga" || ext == L".TGA")
		::LoadFromTGAFile(path.c_str(), nullptr, _image);
	else // png, jpg, jpeg, bmp
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);

	//DECLARE a variable called hr and ASSIGN it to the return value of ::CreateTexture() passing in DEVICE.Get(), _image.GetMetadata(), &_tex2D to ::CreateTexture()
	HRESULT hr = ::CreateTexture(DEVICE.Get(), _image.GetMetadata(), &_tex2D);
	if (FAILED(hr))
		assert(nullptr);

	//DECLARE a vector variable called subResources of type vector<D3D12_SUBRESOURCE_DATA>
	vector<D3D12_SUBRESOURCE_DATA> subResources;

	//ASSIGN hr to the return value of ::PrepareUpload() passing in
	//DEVICE.Get(),
	//_image.GetImages(),
	//_image.GetImageCount(),
	//_image.GetMetadata(),
	//	subResources
	hr = ::PrepareUpload(DEVICE.Get(),
		_image.GetImages(),
		_image.GetImageCount(),
		_image.GetMetadata(),
		subResources);

	if (FAILED(hr))
		assert(nullptr);

	//DECLARe a variable called bufferSize of type const uint64 and ASSIGN it to the return value of ::GetRequiredIntermediateSize() passing in _tex2D.Get(), 0, static_cast<uint32>(subResources.size()) to ::GetRequiredIntermediateSize
	const uint64 bufferSize = ::GetRequiredIntermediateSize(_tex2D.Get(), 0, static_cast<uint32>(subResources.size()));

	//DECLARE a variable called heapProperty of type D3D12_HEAP_PROPERTIES and ASSIGN it to the return value of CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//DECLARE a variable called desc of type D3D12_RESOURCE_DESC and ASSIGN it to the return value of CD3DX12_RESOURCE_DESC::Buffer(bufferSize)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//DECLARE a variable called textureUploadHeap of type ComPtr<ID3D12Resource>
	ComPtr<ID3D12Resource> textureUploadHeap;
	//Initialize it by:
	//ASSIGN hr to the return value of DEVICE->CreateCommittedResource() passing in
	//&heapProperty,
	//D3D12_HEAP_FLAG_NONE,
	//&desc,
	//D3D12_RESOURCE_STATE_GENERIC_READ,
	//nullptr,
	//IID_PPV_ARGS(textureUploadHeap.GetAddressOf())
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));

	if (FAILED(hr))
		assert(nullptr);

	//CALL ::UpdateSubresources() passing in
	//RESOURCE_CMD_LIST.Get(),
	//_tex2D.Get(),
	//textureUploadHeap.Get(),
	//0, 0,
	//static_cast<unsigned int>(subResources.size()),
	//subResources.data())
	::UpdateSubresources(RESOURCE_CMD_LIST.Get(),
		_tex2D.Get(),
		textureUploadHeap.Get(),
		0, 0,
		static_cast<unsigned int>(subResources.size()),
		subResources.data());

	//CALL GEngine->GetCmdQueue()->FlushResourceCommandQueue()
	GEngine->GetCmdQueue()->FlushResourceCommandQueue();
}

void Texture::CreateView()
{
	//DECLARE a variable called srvHeapDesc of type D3D12_DESCRIPTOR_HEAP_DESC
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	//SET srvHeapDesc.NumDescriptors to 1
	srvHeapDesc.NumDescriptors = 1;
	//SET srvHeapDesc.Type to D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//SET srvHeapDesc.Flags to D3D12_DESCRIPTOR_HEAP_FLAG_NONE
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//CALL CreateDescriptorHeap() on DEVICE to &srvHeapDesc, IID_PPV_ARGS(&_srvHeap)
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

	//ASSIGN _srvHandle to the return value of _srvHeap->GetCPUDescriptorHandleForHeapStart()
	_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();

	//DECLARE a variable called srvDesc of type D3D12_SHADER_RESOURCE_VIEW_DESC
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//SET srvDesc.Format to _image.GetMetadata().format
	srvDesc.Format = _image.GetMetadata().format;
	//SET srvDesc.ViewDimension to D3D12_SRV_DIMENSION_TEXTURE2D
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//SET srvDesc.Shader4ComponentMapping to D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//SET srvDesc.Texture2D.MipLevels to 1
	srvDesc.Texture2D.MipLevels = 1;
	//CALL CreateShaderResourceView() on DEVICE passing in _tex2D.Get(), &srvDesc, _srvHandle
	DEVICE->CreateShaderResourceView(_tex2D.Get(), &srvDesc, _srvHandle);
}