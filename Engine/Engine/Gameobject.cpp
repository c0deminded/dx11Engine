#include "Gameobject.h"

Gameobject::Gameobject()
{
	m_D3D = 0;
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

}


bool Gameobject::Init(HWND hwnd, D3DClass* d3d)
{

	m_D3D = d3d;
	m_Transform = new Transform;
	m_Transform->trs = XMMatrixIdentity();
	// Create the Model.

	return true;
}

bool Gameobject::Update()
{
	return true;
}
