#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}
//STEP 12 - IMPL
void Camera::FinalUpdate()
{
	//ASSIGN _matView to the transforms Local To World Matrix's inverse
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	//DECLARE a variable called  width of type float and assign it to the return value of GEngine->GetWindow().width, static_cast GEngine->GetWindow().width
	float width = static_cast<float>(GEngine->GetWindow().width);

	//DECLARE a variable called  height of type float and assign it to the return value of GEngine->GetWindow().height, static_cast GEngine->GetWindow().height
	float height = static_cast<float>(GEngine->GetWindow().height);

	//IF _type IS EQUAL TO PROJECTION_TYPE::PERSPECTIVE
	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		//ASSIGN _matProjection to the return value of ::XMMatrixPerspectiveFovLH(?, ?, ?, ?)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _scale, _near, _far);
	//ELSE
	else
		//ASSIGN _matProjection to the return value of ::XMMatrixOrthographicLH(?, ?, ?, ?)
		_matProjection = ::XMMatrixOrthographicLH(width, height, _near, _far);
	//ENDIF

	//ASSIGN S_MatView to _matView
	S_MatView = _matView;
	//ASSIGN S_MatProjection to _matProjection
	S_MatProjection = _matProjection;
}
//STEP 12 - IMPL
void Camera::Render()
{
	/*Get the scene*/
	//DECLARE a variable called scene of type shared_ptr<Scene> and ASSIGN it to the SceneManager's active scene 
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();


	/*Get the game objects from the scene*/
	//DECLARE a variable called gameObjects of type const vector<shared_ptr<GameObject>>& and ASSIGN it to the scene's Game Objects
	const vector<shared_ptr<GameObject>>& gameobjects = scene->GetGameObjects();

	//FOREACH gameObject in gameObjects
	for(shared_ptr<GameObject> g : gameobjects)
	{
		//IF gameObject does not have a MeshRenderer
		if (g->GetMeshRenderer() == nullptr)
			//CONTINUE
			continue;
		//ENDIF
		g->GetMeshRenderer()->Render();
		//CALL Render() on the gameObject's Mesh Renderer
	}
	//END FOREACH
}