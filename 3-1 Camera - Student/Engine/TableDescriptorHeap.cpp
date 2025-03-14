#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	//ASSIGN  _groupCount to count
	_groupCount = count;
	//DECLARE a variable called desc of type D3D12_DESCRIPTOR_HEAP_DESC and initialize (ASSIGN) it to {}
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	//ASSIGN desc.NumDescriptors to count * REGISTER_COUNT
	desc.NumDescriptors = count * REGISTER_COUNT;
	//ASSIGN desc.Flags to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//ASSIGN desc.Type to D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//CALL CreateDescriptorHeap() on DEVICE passing in &desc, IID_PPV_ARGS(&_descHeap)
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	//ASSIGN _handleSize to the return value of DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//ASSIGN _groupSize to _handleSize * REGISTER_COUNT
	_groupSize = _handleSize * REGISTER_COUNT;
}

void TableDescriptorHeap::Clear()
{
	_currentGroupIndex = 0;
}

void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	//DECLARE a variable called destHandle of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of  GetCPUHandle(reg)
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	//CALL CopyDescriptors() on DEVICE passing in 1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	//DECLARE a variable called destHandle of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of GetCPUHandle(reg)
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	//CALL CopyDescriptors() on DEVICE passing in 1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::CommitTable()
{
	//DECLARE a variable called handle of type D3D12_GPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of _descHeap->GetGPUDescriptorHandleForHeapStart()
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	//INCREMENT the handle.ptr variable by ADDING _currentGroupIndex * _groupSize to it
	handle.ptr += _currentGroupIndex * _groupSize;
	//CALL SetGraphicsRootDescriptorTable() on the CMD_LIST passing in 0, handle
	CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);
	//INCREMENT _currentGroupIndex
	_currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint8>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint8>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint8 reg)
{
	//DECLARE a variable called handle of type D3D12_CPU_DESCRIPTOR_HANDLE and ASSIGN it to the return value of _descHeap->GetCPUDescriptorHandleForHeapStart()
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	//INCREMENT the handle.ptr variable by ADDING _currentGroupIndex * _groupSize to it
	handle.ptr += _currentGroupIndex * _groupSize;
	//INCREMENT the handle.ptr variable by ADDING reg * _handleSize to it
	handle.ptr += reg * _handleSize;
	//return handle
	return handle;
}