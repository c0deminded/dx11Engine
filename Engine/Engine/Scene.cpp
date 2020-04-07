#include "Scene.h"
#include <assimp/Importer.hpp>
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
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	Render(rotation);
	return true;
}

void Scene::Render(float rotation)
{
	m_Graphics->Render(rotation);
}
