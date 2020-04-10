#include "Scene.h"

//Scene class to derive from

Scene::Scene()
{
	m_Graphics = 0;
	m_Katamari = 0;
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
	if (m_Katamari) 
	{
		m_Katamari->Unload();
		delete m_Katamari;
		m_Katamari = 0;
	}
}

//aka start
bool Scene::Init(int sWidth, int sHeight, HWND hwnd)
{
	bool result;
	m_Graphics = new GraphicsClass;
	result = m_Graphics->Initialize(sWidth, sHeight, hwnd);

	m_Katamari = new Katamari;
	result = m_Katamari->Init(hwnd,m_Graphics->m_D3D);
	return result;
}

bool Scene::Update(int axisX,int axisY)
{
	if (axisX != 0 || axisY != 0)
		m_Katamari->Translate(Vector3(axisX, 0.0f, axisY));
	//m_Katamari->Update();
	Render();
	return true;
}

void Scene::Render()
{
	//temporary crutch
	m_Graphics->Render(m_Katamari,m_Katamari->m_Model);
}
