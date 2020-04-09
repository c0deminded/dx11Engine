#include "Scene.h"

//Scene class to derive from

Scene::Scene()
{
	m_Graphics = 0;
	m_SomeGo = 0;
}

Scene::~Scene()
{
}

//use to load some resources
//not gameplay objects
void Scene::Load()
{

}

void Scene::Unload()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}
	if (m_SomeGo) 
	{
		m_SomeGo->Unload();
		delete m_SomeGo;
		m_SomeGo = 0;
	}
}

//aka start
bool Scene::Init(int sWidth, int sHeight, HWND hwnd)
{
	bool result;
	m_Graphics = new GraphicsClass;
	result = m_Graphics->Initialize(sWidth, sHeight, hwnd);

	m_SomeGo = new Gameobject;
	result = m_SomeGo->Init(hwnd,m_Graphics->m_D3D);
	return result;
}

bool Scene::Update()
{
	m_SomeGo->Update();
	Render();
	return true;
}

void Scene::Render()
{
	m_SomeGo->Render();
	//temporary crutch
	m_Graphics->Render(m_SomeGo);
}
