#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	//ASSIGN _vertexCount to vec.size(), static_cast the return value of vec.size()
	_vertexCount = static_cast<uint32>(vec.size());
	//DECLARE a variable called bufferSize of type uint32 and ASSIGN it to _vertexCount * sizeof(Vertex)
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	//DECLARE a variable called heapProperty of type D3D12_HEAP_PROPERTIES and ASSIGN it to the return of CD3DX12_HEAP_PROPERTIES() passing in D3D12_HEAP_TYPE_UPLOAD
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//DECLARE a variable called desc of type D3D12_RESOURCE_DESC and ASSIGN it to the return of CD3DX12_RESOURCE_DESC::Buffer() passing in bufferSize
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	/*Where is DEVICE coming from now, what change has been made and why*/
	//CALL CreateCommittedResource() on DEVICE passing in &heapProperty, D3D12_HEAP_FLAG_NONE,& desc, D3D12_RESOURCE_STATE_GENERIC_READ,nullptr, IID_PPV_ARGS(&_vertexBuffer)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	/* Copy the triangle data to the vertex buffer. */
	//CREATE a variable called vertexDataBuffer of type void* and ASSIGN it to nullptr
	void* vertexDataBuffer = nullptr;
	//DECLARE a variable called readRange of type CD3DX12_RANGE and initialize it with 0,0
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	//CALL Map() on _vertexBuffer passing in 0, &readRange, &vertexDataBuffer
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	//CALL memcpy() passing in vertexDataBuffer, &vec[0], bufferSize
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	//CALL Unmap() on _vertexBuffer passing in 0, nullptr
	_vertexBuffer->Unmap(0, nullptr);

	/* Initialize the vertex buffer view. */
	//ASSIGN _vertexBufferView.BufferLocation to _vertexBuffer->GetGPUVirtualAddress()
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	//ASSIGN _vertexBufferView.StrideInBytes to the size of Vertex
	_vertexBufferView.StrideInBytes = sizeof(Vertex); //
	//ASSIGN _vertexBufferView.SizeInBytes to bufferSize
	_vertexBufferView.SizeInBytes = bufferSize; //
}

void Mesh::Render()
{
	/*Where is CMD_LIST coming from now, what change has been made and why*/
	//CALL IASetPrimitiveTopology() on CMD_LIST passing in D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//CALL IASetVertexBuffers() on CMD_LIST passing in 0, 1, &_vertexBufferView
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	//STEP 3 NEW
	// TODO
	// 1) Buffer  
	// 2) Buffer ¸¦ register
	
	//PUSH the _transform data via GEngine's Constant Buffer at root parameter index 0
	GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
	//PUSH the _transform data via GEngine's Constant Buffer at root parameter index 1
	GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform));

	//CALL DrawInstanced() on CMD_LIST passing in _vertexCount, 1, 0, 0
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}