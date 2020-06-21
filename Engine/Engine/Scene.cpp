#include "Scene.h"

//Scene class to derive from

Scene::Scene()
{
	m_Graphics = 0;
	m_Camera = 0;
	m_Gameplane = 0;
	m_Katamari = 0;
	m_Katavictim = 0;
	m_Katavictim2 = 0;
	m_Katavictim3 = 0;
	m_Light = 0;
	m_SimpleText = 0;
	numberOfUnattachedObjects = 3;
	numberOfAttachedObjects = 0;
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
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	if (m_Katamari)
	{
		m_Katamari->Unload();
		delete m_Katamari;
		m_Katamari = 0;
	}
	if (m_Katavictim)
	{
		m_Katavictim->Unload();
		delete m_Katavictim;
		m_Katavictim = 0;
	}
	if (m_Katavictim2)
	{
		m_Katavictim2->Unload();
		delete m_Katavictim2;
		m_Katavictim2 = 0;
	}
	if (m_Katavictim3)
	{
		m_Katavictim3->Unload();
		delete m_Katavictim3;
		m_Katavictim3 = 0;
	}
	if (m_Gameplane)
	{
		m_Gameplane->Unload();
		delete m_Gameplane;
		m_Gameplane = 0;
	}
	if (m_SimpleText)
	{
		m_SimpleText->DiscardDeviceResources();
		m_SimpleText = 0;
	}
	//todo destroy others
}

//aka start
bool Scene::Init(int sWidth, int sHeight, HWND hwnd)
{
	bool result;
	m_Graphics = new GraphicsClass;
	result = m_Graphics->Initialize(sWidth, sHeight, hwnd);

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	m_Camera->SetPosition(0.0f, 9.0f, -25.0f);
	m_Camera->SetRotation(0.0f, 10.0f, 0.0f);

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	// Initialize the light object.
	m_Light->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularColor(0.0f, 0.0f, 0.0f, 0.5f);
	m_Light->SetSpecularPower(32.0f);
	m_Light->SetLookAt(0.0f, 7.0f, -7.0f);
	m_Light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	m_Katamari = new Katamari;
	result = m_Katamari->Init(hwnd, m_Graphics->m_D3D);
	m_Graphics->SetRenderable((Gameobject*)m_Katamari, m_Katamari->m_Model);

	m_Katavictim = new KataVictim;
	result = m_Katavictim->Init(hwnd, "Data\\Objects\\gus.obj", L"../Engine/gus.tga", Vector3(0.05f, 0.05f, 0.05f), m_Graphics->m_D3D);
	m_Katavictim->Place(Vector3(150.0f, -10.0f, 0.0f));
	m_Graphics->SetRenderable((Gameobject*)m_Katavictim, m_Katavictim->m_Model);

	m_Katavictim2 = new KataVictim;
	result = m_Katavictim2->Init(hwnd, "Data\\Objects\\peng.obj", L"../Engine/peng.tga", Vector3(2.25f, 2.25f, 2.25f), m_Graphics->m_D3D);
	m_Katavictim2->Place(Vector3(-1.0f, -0.4f, 3.0f));
	m_Graphics->SetRenderable((Gameobject*)m_Katavictim2, m_Katavictim2->m_Model);

	m_Katavictim3 = new KataVictim;
	result = m_Katavictim3->Init(hwnd, "Data\\Objects\\hand.obj", L"../Engine/teapot.tga", Vector3(0.1f, 0.1f, 0.1f), m_Graphics->m_D3D);
	m_Katavictim3->Place(Vector3(-100.0f, 0.0f, 0.0f));
	m_Graphics->SetRenderable((Gameobject*)m_Katavictim3, m_Katavictim3->m_Model);

	m_Gameplane = new Gameplane;
	result = m_Gameplane->Init(hwnd, m_Graphics->m_D3D);
	m_Graphics->SetRenderable((Gameobject*)m_Gameplane, m_Gameplane->m_Model);
	
	m_SimpleText = new SimpleText;
	m_SimpleText->Init(hwnd, COLORREF RGB(1, 1, 1), m_Graphics->m_D3D);
	wchar_t pretext[200];
	swprintf(pretext, 200, L"Number of unattached objects: %u\nNumber of attached objects: %u", numberOfUnattachedObjects, numberOfAttachedObjects);
	m_SimpleText->DrawTextOnScene(240, 50, pretext);

	return result;
}

bool Scene::Update(int axisX,int axisY)
{
	//todo: move to katamari update
	if (axisX != 0 || axisY != 0) {
		m_Katamari->Translate(Vector3(axisX, 0.0f, axisY));
		m_Camera->Follow(m_Katamari->m_Transform->trs,Vector3(0.0f, 9.0f,-25.0f));
		if (m_Katavictim->m_HasParent)
			m_Katavictim->Update(m_Katamari);

		if (m_Katavictim2->m_HasParent)
			m_Katavictim2->Update(m_Katamari);

		if (m_Katavictim3->m_HasParent)
			m_Katavictim3->Update(m_Katamari);
	}
;

	//todo: move to katavictim update
	if (m_Katamari->IsCloser(m_Katavictim, 1.0f) && !m_Katavictim->m_HasParent) 
	{
		m_Katavictim->AttachTo(m_Katamari);
		numberOfAttachedObjects++;
		numberOfUnattachedObjects--;
	}
	if (m_Katamari->IsCloser(m_Katavictim2, 1.0f) && !m_Katavictim2->m_HasParent)
	{
		m_Katavictim2->AttachTo(m_Katamari);
		numberOfAttachedObjects++;
		numberOfUnattachedObjects--;
	}
	if (m_Katamari->IsCloser(m_Katavictim3, 1.0f) && !m_Katavictim3->m_HasParent)
	{
		m_Katavictim3->AttachTo(m_Katamari);
		numberOfAttachedObjects++;
		numberOfUnattachedObjects--;
	}

	// this what makes light rotate around area
	static float lightPositionX = -20.0f;

	//// Update the position of the light each frame.
	lightPositionX += 0.05f;
	if (lightPositionX > 20.0f)
	{
		lightPositionX = -26.0f;
	}
	///////////////////////////////////////////

	// Update the position of the light.
	m_Light->SetPosition(lightPositionX, 15.0f, -30.0f);

	m_Graphics->Render(m_Camera, m_Light, m_SimpleText, numberOfUnattachedObjects, numberOfAttachedObjects);

	return true;
}
