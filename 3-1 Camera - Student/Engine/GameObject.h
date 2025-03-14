#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class Camera;
class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

//STEP 12 - LOOK AT  REMOVED Init()
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();//STEP 12 - LOOK AT 

	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);//STEP 12 - LOOK AT 

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();//STEP 12 - LOOK AT 
	shared_ptr<Camera> GetCamera();//STEP 12 - LOOK AT 

	void AddComponent(shared_ptr<Component> component);

private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

