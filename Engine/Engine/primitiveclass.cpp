#include "primitiveclass.h"

PrimitiveClass::PrimitiveClass(PrimitiveType type)
{
	switch (type)
	{
	case PrimitiveType::Triangle:BuildTriangle();
		break;
	case PrimitiveType::Rectangle:BuildRectangle();
		break;
	case PrimitiveType::Hexagon:BuildHexagon();
		break;
	default:
		break;
	}
}

PrimitiveClass::~PrimitiveClass()
{
}

void PrimitiveClass::Rescale(float x, float y, float z)
{
	for (size_t i = 0; i < vertexCount; i++)
	{
		vertices[i].position.x *= x;
		vertices[i].position.y *= y;
		vertices[i].position.z *= z;
	}
}

void PrimitiveClass::SetPosition(float x, float y, float z)
{
	for (size_t i = 0; i < vertexCount; i++)
	{
		vertices[i].position.x += x;
		vertices[i].position.y += y;
		vertices[i].position.z += z;
	}
}

void PrimitiveClass::BuildTriangle()
{
	this->vertexCount = 3;
	this->indexCount = 3;
	vertices = new Vertices[vertexCount];
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	for (size_t i = 0; i < vertexCount; i++)
	{
		vertices[i].color = XMFLOAT4(0.1f, 1.0f, 0.1f, 1.0f);
	}
	indices = new unsigned long[indexCount];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
}

void PrimitiveClass::BuildRectangle() 
{
	this->vertexCount = 4;
	this->indexCount = 6;
	vertices = new Vertices[vertexCount];
	vertices[0].position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertices[1].position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[2].position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[3].position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	for (size_t i = 0; i < vertexCount; i++)
	{
		vertices[i].color = XMFLOAT4(0.1f, 1.0f, 0.1f, 1.0f);
	}
	indices = new unsigned long[indexCount];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;
}
void PrimitiveClass::BuildHexagon()
{
	this->vertexCount = 6;
	this->indexCount = 12;
	vertices = new Vertices[vertexCount];
	vertices[0].position = XMFLOAT3(-1.f, 0.6f, 0.0f);
	vertices[1].position = XMFLOAT3(0.0f, 1.2f, 0.0f);
	vertices[2].position = XMFLOAT3(1.f, 0.6f, 0.0f);
	vertices[3].position = XMFLOAT3(1.f, -0.6f, 0.0f);
	vertices[4].position = XMFLOAT3(0.0f, -1.2f, 0.0f);
	vertices[5].position = XMFLOAT3(-1.f, -0.6f, 0.0f);
	for (size_t i = 0; i < vertexCount; i++)
	{
		vertices[i].color = XMFLOAT4(0.1f, 1.0f, 0.1f, 1.0f);
	}
	indices = new unsigned long[indexCount];
	indices[0] = 5;	indices[1] = 0; indices[2] = 1;
	indices[3] = 1; indices[4] = 2; indices[5] = 5;
	indices[6] = 5;	indices[7] = 2; indices[8] = 4;
	indices[9] = 4; indices[10] = 2; indices[11] = 3;
}

