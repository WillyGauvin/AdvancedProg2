#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"

TestCameraScript::TestCameraScript()
{
}

TestCameraScript::~TestCameraScript()
{
}

//STEP 12 - IMPL
void TestCameraScript::LateUpdate()
{
	//DECLARE a variable called pos of type vec3 and set it to the transforms Local Position
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
		//INCREMENT pos by the transforms Look Vector multiplied by speed and delta time
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		//DECREMENT pos by the transforms Look Vector multiplied by speed and delta time
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;


	if (INPUT->GetButton(KEY_TYPE::A))
		//DECREMENT pos by the transforms Right Vector multiplied by speed and delta time
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;


	if (INPUT->GetButton(KEY_TYPE::D))
		//INCREMENT pos by the transforms Right Vector multiplied by speed and delta time
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;


	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		//DECLARE a variable called rotation of type Vec3 and set it to the transform's local rotation
		Vec3 rotation = GetTransform()->GetLocalRotation();
		//INCREMENT the rotation's x by delta time multiplied by 0.5f
		rotation.x += DELTA_TIME * 0.5f;
		//SET the rotation on the transforms local rotation
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		//DECLARE a variable called rotation of type Vec3 and set it to the transform's local rotation
		Vec3 rotation = GetTransform()->GetLocalRotation();

		//DECREMENT the rotation's x by delta time multiplied by 0.5f
		rotation.x -= DELTA_TIME * 0.5f;

		//SET the rotation on the transforms local rotation
		GetTransform()->SetLocalRotation(rotation);

	}
	//SET the transforms local position to pos
	GetTransform()->SetLocalPosition(pos);
}