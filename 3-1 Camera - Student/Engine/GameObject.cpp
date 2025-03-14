#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h" //STEP 12 - LOOK AT 
#include "MonoBehaviour.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

//STEP 12 - LOOK AT AT REMOVED void GameObject::Init()

void GameObject::Awake()
{
	//FOREACH component in _components
	for (shared_ptr<Component>& component : _components)
	{
		//IF component is valid
		if (component)
			//CALL Awake() on component
			component->Awake();
		//ENDIF
	}
	//END FOREACH

	//FOREACH component in script in _scripts
	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		//CALL Awake() on script
		script->Awake();
	}
	//ENDFOR
}

void GameObject::Start()
{
	//FOREACH component in _components
	for (shared_ptr<Component>& component : _components)
	{
		//IF component is valid
		if (component)
			//CALL Start() on component
			component->Start();
		//ENDIF
	}
	//END FOREACH

	//FOREACH component in script in _scripts
	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		//CALL Start() on script
		script->Start();
	}
	//ENDFOR
}

void GameObject::Update()
{
	//FOREACH component in _components
	for (shared_ptr<Component>& component : _components)
	{
		//IF component is valid
		if (component)
			//CALL Update() on component
			component->Update();
		//ENDIF
	}
	//END FOREACH

	//FOREACH script in _scripts
	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		//CALL Update() on script
		script->Update();
	}
	//ENDFOR
}

void GameObject::LateUpdate()
{
	//FOREACH component in _components
	for (shared_ptr<Component>& component : _components)
	{
		//IF component is valid
		if (component)
			//CALL LateUpdate() on component
			component->LateUpdate();
		//ENDIF
	}
	//END FOREACH

	//FOREACH script in _scripts
	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		//CALL LateUpdate() on script
		script->LateUpdate();
	}
	//END FOREACH
}
//STEP 12 - LOOK AT 
void GameObject::FinalUpdate()
{
	//FOREACH component in _components
	for (shared_ptr<Component>& component : _components)
	{
		//IF component is valid
		if (component)
			//CALL FinalUpdate() on component
			component->FinalUpdate();
		//ENDIF
	}
	//END FOREACH
}
//STEP 12 - IMPL
shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	//STATIC CAST type to a uint8 index
	uint8 index = static_cast<uint8>(type);
	//ASSERT index less than FIXED_COMPONENT_COUNT
	assert(index < FIXED_COMPONENT_COUNT);
	//RETURN the component at index
	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);//STEP 12 - LOOK AT 
	return static_pointer_cast<Transform>(component);//STEP 12 - LOOK AT 
}
//STEP 12 - LOOK AT
shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER);
	return static_pointer_cast<MeshRenderer>(component);
}
//STEP 12 - LOOK AT
shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(component);
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	//CALL SetGameObject() on component passing in the return value of shared_from_this() <-- Look this up
	component->SetGameObject(shared_from_this());

	//DECLARE a variable called index of type uint8 and assign it to the return value of component->GetType(), static_cast<>() the return value
	uint8 index = static_cast<uint8>(component->GetType());
	//IF index IS LESS THAN FIXED_COMPONENT_COUNT
	if (index < FIXED_COMPONENT_COUNT)
	{
		//ASSIGN _components[index] to component
		_components[index] = component;
	}
	//ELSE
	else
	{
		//CALL push_back() on _scripts passing in dynamic_pointer_cast<MonoBehaviour>(component)
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
	//ENDIF
}