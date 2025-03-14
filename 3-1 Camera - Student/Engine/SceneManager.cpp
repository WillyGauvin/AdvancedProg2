#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"//STEP 12 - LOOK AT 
#include "Camera.h"//STEP 12 - LOOK AT 

#include "TestCameraScript.h"//STEP 12 - LOOK AT 

void SceneManager::Update()
{
	//IF _activeScene IS EQUAL TO nullptr
	if (_activeScene == nullptr)
		//RETURN
		return;
	//ENDIF

	//CALL Update() on _activeScene
	_activeScene->Update();
	//CALL LateUpdate() on _activeScene
	_activeScene->LateUpdate();

	//STEP 12 - IMPL
	//CALL FinalUpdate() on _activeScene
	_activeScene->FinalUpdate();
}

//STEP 12 - IMPL
// TEMP
void SceneManager::Render()
{
	if (_activeScene == nullptr)
		return;

	//GET the game objects from the active scene
	//LOOP through the game objects
	for(shared_ptr<GameObject> g : _activeScene->GetGameObjects())
	{
		if (g->GetCamera() == nullptr)
			continue;
		//IF the game object camera is nullptr continue
	
	
		g->GetCamera()->Render();
		//GET the game object's camera and render
	}
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 
	// TODO : 
	//ASSIGN _activeScene to the return value of LoadTestScene()
	_activeScene = LoadTestScene();

	//CALL Awake() on _activeScene
	_activeScene->Awake();
	//CALL Start() on _activeScene
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region TestObject
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();

	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vec2(0.f, 0.f);
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vec2(1.f, 0.f);
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vec2(1.f, 1.f);
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vec2(0.f, 1.f);

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}
//STEP 12 - IMPL
	//ADD a transform component to the game object
	gameObject->AddComponent(make_shared<Transform>());
	//GET the transform from the game object then and SET the transforms Local Position to 0.f, 100.f, 200.f, also set the Local Scale to 100.f, 100.f, 1.f
	gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 200.f));
	gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 1.f));
	
	
//END STEP 12 - IMPL
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVec);
		meshRenderer->SetMesh(mesh);
	}
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\default.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetFloat(0, 0.3f);
		material->SetFloat(1, 0.4f);
		material->SetFloat(2, 0.3f);
		material->SetTexture(0, texture);
		meshRenderer->SetMaterial(material);
	}
//STEP 12 - IMPL
	gameObject->AddComponent(meshRenderer);
//STEP 12 - IMPL
	scene->AddGameObject(gameObject);
#pragma endregion
//STEP 12 - IMPL
#pragma region Camera
	//CREATE a game object callled camera
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	//ADD a transform component to the camera game object
	camera->AddComponent(make_shared<Transform>());
	//ADD a camera component to the camera game object
	camera->AddComponent(make_shared<Camera>());
	//ADD a test camera script component to the camera game object
	camera->AddComponent(make_shared<TestCameraScript>());
	//SET the cameras Local Position to 0.f, 100.f, 0.f
	camera->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
	//ADD the camera game object to the scene
	scene->AddGameObject(camera);
#pragma endregion
//END STEP 12 - IMPL
	return scene;
}