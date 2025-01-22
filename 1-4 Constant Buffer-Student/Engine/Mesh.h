#pragma once

// 
class Mesh
{
public:
	void Init(vector<Vertex>& vec);
	void Render();

//STEP 3 (3) CREATE a Setter function to set _transform
	void SetTransform(Transform t) { _transform = t; }

private:
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	//STEP 3 (2) DECLARE a variable called _transform
	Transform _transform;
};

