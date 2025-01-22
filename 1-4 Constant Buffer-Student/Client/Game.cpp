#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo& info)
{
	//CALL Init() on GEngine passing in info
	GEngine->Init(info);

	//STEP 2
	//DECLARE a variable called vec of type vector<Vertex> with 3 elements.
	vector<Vertex> vec(3);
	//SET vec[0]'s pos and color
	vec[0].pos = Vec3(0.f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	//SET vec[1]'s pos and color
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);
	//SET vec[2]'s pos and color
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	//CALL Init() on mesh passing in vec
	mesh->Init(vec);
	//CALL Init() on shader passing in L"..\\Resources\\Shader\\default.hlsli"
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");
	//CALL WaitSync() on the GEngine's CommandQueue
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	//STEP 2
	//CALL RenderBegin() on GEngine
	GEngine->RenderBegin();

	//CALL Update() on shader
	shader->Update();

	////STEP 3 Do this last
	//{
	//	//DECLARE a variable called t of type Transform
	//	Transform t;
	//	//SET t's offset variable to something like 0.75f, 0.f, 0.f, 0.f
	//	t.offset = Vec4(0.0f, 0.0f, 0.f, 0.f);
	//	//CALL SetTransform() on mesh passing in t
	//	mesh->SetTransform(t);
	//	//RENDER the mesh
	//	mesh->Render();
	//}

	//STEP 3 Do this last
	{
		//DECLARE a variable called t of type Transform
		Transform t;

		//SET t's offset variable to something like 0.f, 0.75f, 0.f, 0.f
		t.offset = Vec4(0.f, 0.0f, 0.f, 0.f);

		//CALL SetTransform() on mesh passing in t
		mesh->SetTransform(t);

		//RENDER the mesh
		mesh->Render();
	}

	//CALL RenderEnd() on GEngine
	GEngine->RenderEnd();
}
