#include "Gameobject.h"

Gameobject::Gameobject()
{
	m_D3D = 0;
	m_Model = 0;
	m_Transform = 0;
}

Gameobject::~Gameobject()
{
}

void Gameobject::Load()
{
}

void Gameobject::Unload()
{
	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
}


bool Gameobject::Init(HWND hwnd, D3DClass* d3d)
{
	bool result;
	m_D3D = d3d;
	m_Transform = new Transform;
	m_Transform->trs = XMMatrixIdentity();
	// Create the Model.
	m_Model = new ModelClass();
	if (!m_Model)
	{
		return false;
	}
	// Initialize the model.
	result = m_Model->Initialize(m_D3D->GetDevice(), "Data\\Objects\\m_cube.obj", L"../Engine/brick.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model.", L"Error", MB_OK);
		return false;
	}
	return result;
}

bool Gameobject::Update()
{
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	m_Transform->trs = XMMatrixRotationY(rotation);
	return true;
}

void Gameobject::Render()
{
}

void Gameobject::SetActive(bool)
{
}
