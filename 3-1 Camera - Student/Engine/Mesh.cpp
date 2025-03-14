#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Material.h"

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	//CALL CreateVertexBuffer() passing in vertexBuffer
	CreateVertexBuffer(vertexBuffer);
	//CreateIndexBuffer() passing in indexBuffer
	CreateIndexBuffer(indexBuffer);
}

void Mesh::Render()
{
	//CALL IASetPrimitiveTopology() on CMD_LIST passing in D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//CALL IASetVertexBuffers() on CMD_LIST passing in 0, 1, &_vertexBufferView
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	//CALL IASetIndexBuffer() on CMD_LIST passing in &_indexBufferView
	CMD_LIST->IASetIndexBuffer(&_indexBufferView);

	//CALL GEngine->GetTableDescHeap()->CommitTable()
	GEngine->GetTableDescHeap()->CommitTable();

	//CALL DrawIndexedInstanced() on CMD_LIST passing in _indexCount, 1, 0, 0, 0
	CMD_LIST->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	//ASSIGN _vertexCount to buffer.size(), you will have to static_cast<>() the return of buffer.size() as a uint32
	_vertexCount = static_cast<uint32>(buffer.size());
	//DECLARE a variable called bufferSize and ASSIGN it to _vertexCount * sizeof(Vertex)
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	//DECLARE a variable called heapProperty of type D3D12_HEAP_PROPERTIES and ASSIGN it to the return value of CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//DECLARE a variable called desc of type D3D12_RESOURCE_DESC and ASSIGN it to the return value of CD3DX12_RESOURCE_DESC::Buffer(bufferSize)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//CALL CreateCommittedResource() on DEVICE passing in &heapProperty, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_vertexBuffer)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	/*Copy the triangle data to the vertex buffer.*/
	//DECLARE a variable called vertexDataBuffer of type void pointer and ASSIGN it to nullptr
	void* vertexDataBuffer = nullptr;
	//DECLARE a variable called readRange of type CD3DX12_RANGE and INITIALIZE it to 0, 0
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	//CALL Map() on _vertexBuffer passing in 0, &readRange, &vertexDataBuffer
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	//CALL ::memcpy() passing in vertexDataBuffer, &buffer[0], bufferSize
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	//CALL Unmap() on _vertexBuffer passing in 0, nullptr
	_vertexBuffer->Unmap(0, nullptr);

	/* Initialize the vertex buffer view.*/
	//ASSIGN _vertexBufferView.BufferLocation to the return value of _vertexBuffer->GetGPUVirtualAddress()
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	//ASSIGN _vertexBufferView.StrideInBytes to the return value of sizeof(Vertex)
	_vertexBufferView.StrideInBytes = sizeof(Vertex); //
	//ASSIGN _vertexBufferView.SizeInBytes to bufferSize
	_vertexBufferView.SizeInBytes = bufferSize; // 
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	//ASSIGN _indexCount to buffer.size(), you will have to static_cast<>() the return of buffer.size() as a uint32
	_indexCount = static_cast<uint32>(buffer.size());
	//DECLARE a variable called bufferSize and ASSIGN it to _indexCount * sizeof(uint32)
	uint32 bufferSize = _indexCount * sizeof(uint32);

	//DECLARE a variable called heapProperty of type D3D12_HEAP_PROPERTIES and ASSIGN it to the return value of CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//DECLARE a variable called desc of type D3D12_RESOURCE_DESC and ASSIGN it to the return value of CD3DX12_RESOURCE_DESC::Buffer(bufferSize)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//CALL CreateCommittedResource() on DEVICE passing in &heapProperty, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_indexBuffer)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_indexBuffer));

	//DECLARE a variable called indexDataBuffer of type void pointer and ASSIGN it to nullptr
	void* indexDataBuffer = nullptr;
	//DECLARE a variable called readRange of type CD3DX12_RANGE and INITIALIZE it to 0, 0
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	//CALL Map() on _indexBuffer passing in 0, &readRange, &indexDataBuffer
	_indexBuffer->Map(0, &readRange, &indexDataBuffer);
	//CALL ::memcpy() passing in indexDataBuffer, &buffer[0], bufferSize
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	//CALL Unmap() on _indexBuffer passing in 0, nullptr
	_indexBuffer->Unmap(0, nullptr);

	//ASSIGN _indexBufferView.BufferLocation to the return value of _indexBuffer->GetGPUVirtualAddress()
	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	//ASSIGN _indexBufferView.Format to DXGI_FORMAT_R32_UINT
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	//ASSIGN _indexBufferView.SizeInBytes to bufferSize
	_indexBufferView.SizeInBytes = bufferSize;
}