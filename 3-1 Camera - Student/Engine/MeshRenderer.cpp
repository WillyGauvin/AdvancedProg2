#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	//STEP 12 - IMPL
	//PUSH the transform data
	GetTransform()->PushData();
	//CALL PushData() on _material
	_material->PushData();//STEP 12 - LOOK AT 
	//CALL Render() on _mesh
	_mesh->Render();
}