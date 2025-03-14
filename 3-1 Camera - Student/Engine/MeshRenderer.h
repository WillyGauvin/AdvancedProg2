#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }

//STEP 12 - LOOK AT REMOVED Update()

	void Render();

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
};
