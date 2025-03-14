1-2 Device:
Device - REVIEW
CommandQueue - REVIEW
SwapChain - REVIEW We create the descriptor heap here 
DescriptorHeap -- GONE

Engine
Client
Game
-------------------------------------------------------------------------------------
1-3 Triangle:
RootSignature
Mesh -- Init Unmap() ??
Shader

EnginePch
-------------------------------------------------------------------------------------
1-4 Constant Buffer: //STEP 3 NEW
CommandQueue::RenderBegin()
Mesh::Render()

ConstantBuffer

EnginePch
-------------------------------------------------------------------------------------
1-5 Root Signature: STEP 4
ConstantBuffer::Init()
ConstantBuffer::CreateView()
Engine::Init()
RootSignature::Init()

TableDescriptorHeap
EnginePch
-------------------------------------------------------------------------------------
1-6 Index Buffer: STEP 5
Mesh

Game

-------------------------------------------------------------------------------------
1-7 Texture Mapping: STEP 6
Mesh
Game
CommandQueue

Texture

-------------------------------------------------------------------------------------
1-8 Depth Stencil View: STEP 7
Mesh
Game
CommandQueue

Texture

-------------------------------------------------------------------------------------
2-1 Input Timer: STEP 8
Engine

Input
Timer

-------------------------------------------------------------------------------------
2-2 Material: STEP 9
CommandQueue
ConstantBuffer
Engine
Input
Mesh

Material

-------------------------------------------------------------------------------------
2-3 Component: STEP 10
CommandQueue
ConstantBuffer
Engine
Input
Mesh

Material

-------------------------------------------------------------------------------------
2-4 Scene: STEP 11
Engine
Input
Timer

Scene
SceneManager

-------------------------------------------------------------------------------------
3-1 Camera and Transform: STEP 12
Engine
Input
Timer

Transform
GameObject
Camera

Scene
SceneManager