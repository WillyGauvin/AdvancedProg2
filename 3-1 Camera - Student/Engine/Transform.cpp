#include "pch.h"
#include "Transform.h"
#include "Engine.h" //STEP 12 - LOOK AT
#include "Camera.h"//STEP 12 - LOOK AT

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}
//STEP 12 - IMPL
void Transform::FinalUpdate()
{
	//ISROT
	//DECLARE a variable called matScale of type Matrix and set it to a matrix from _localScale
	Matrix matScale = Matrix();
	matScale.CreateScale(_localScale);
	//DECLARE a variable called matRotation of type Matrix and set it to a matrix from _localRotation.x
	Matrix matRotation = Matrix();
	matRotation.CreateRotationX(_localRotation.x);
	//MULTIPLY matRotation by a matrix from _localRotation.y
	Matrix yrot = Matrix();
	yrot.CreateRotationY(_localRotation.y);
	matRotation *= yrot;
	//MULTIPLY matRotation by a matrix from _localRotation.z
	Matrix zrot = Matrix();
	yrot.CreateRotationZ(_localRotation.z);
	matRotation *= zrot;
	//DECLARE a variable called matTranslation of type Matrix and set it to a matrix from _localPosition
	Matrix matTranslation = Matrix();
	matTranslation.CreateTranslation(_localPosition);

	//SET _matLocal, it is the product of  matScale, matRotation, matTranslation
	_matLocal = matScale * matRotation * matTranslation;


	//SET _matWorld to _matLocal
	_matWorld = _matLocal;

	//DECLARE a variable called parent of type shared_ptr<Transform> and ASSIGN it to the parent transform
	shared_ptr<Transform> parent = GetParent().lock();
	//IF parent NOT EQUAL nullptr
	if (parent != nullptr)
	{
		//MULTIPLY _matWorld with the parent's Local To World Matrix. You are combining the two matrices computing the absolute transform relative to the parent
		_matWorld *= parent->GetLocalToWorldMatrix();
	}
	//ENDIF
}

//STEP 12 - IMPL
void Transform::PushData()
{
	//DECLARE a variable called matWVP of type Matrix and set it to the product of _matWorld, Camera::S_MatView, Camera::S_MatProjection
	Matrix matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	//GET the CONST_BUFFER for a TRANSFORM and Push the Data passing in ?, ?
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&matWVP, sizeof(Matrix));
}
