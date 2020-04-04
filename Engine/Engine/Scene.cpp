#include "Scene.h"

Scene::Scene()
{
	m_Graphics = 0;
}

Scene::~Scene()
{
}

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
}

bool Scene::Init(int sWidth, int sHeight, HWND m_hwnd)
{
	m_Graphics = new GraphicsClass;
	return m_Graphics->Initialize(sWidth, sHeight, m_hwnd);
}

bool Scene::Update()
{
	m_Graphics->Frame(0,0);
	Render();
	return true;
}

void Scene::Render()
{
	m_Graphics->Render(0.f);
}
