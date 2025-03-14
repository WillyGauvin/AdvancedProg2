#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(COMPONENT_TYPE type) : _type(type)
{

}

Component::~Component()
{
}

shared_ptr<GameObject> Component::GetGameObject()
{
	//RETURN _gameObject.lock() <-- Look this up
	return _gameObject.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
	//RETURN _gameObject.lock()->GetTransform()
	return _gameObject.lock()->GetTransform();
}