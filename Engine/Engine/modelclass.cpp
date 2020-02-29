#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	initialPos = new XMFLOAT3(0.f,0.f,0.f);
}

ModelClass::ModelClass(XMFLOAT3* initialPos, PrimitiveType type,XMFLOAT4 color, XMFLOAT3 scale)
{
	ModelClass();
	this->transform = new Transform();
	this->transform->position = (*initialPos);
	this->transform->scale = scale;
	this->initialPos = initialPos;
	this->meshType = type;
	this->modelColor = color;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

//The Initialize function will call the initialization functions for the vertexand index buffers.

bool ModelClass::Initialize(ID3D11Device * device)
{
	bool result;
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	return true;
}

void ModelClass::Translate(XMFLOAT3 direction, float distance)
{
	XMVECTOR v_offset = XMLoadFloat3(&direction) * distance;
	//now transform position stores real position of it's model
	// add it to all vertices
	XMFLOAT3 offset;
	XMStoreFloat3(&offset, v_offset);
	for (size_t i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position.x += offset.x;
		vertices[i].position.y += offset.y;
		vertices[i].position.z += offset.z;
	}
	transform->position.x += offset.x;
	transform->position.y += offset.y;
	transform->position.z += offset.z;
	vertexData.pSysMem = vertices;
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer); // <--- memory leak!
}

void ModelClass::SetPosition(XMFLOAT3 newPos)
{
	for (size_t i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position.x = origin[i].x + newPos.x;
		vertices[i].position.y = origin[i].y + newPos.y;
		vertices[i].position.z = origin[i].z + newPos.z;
	}
	transform->position.x = newPos.x;
	transform->position.y = newPos.y;
	transform->position.z = newPos.z;

	vertexData.pSysMem = vertices;
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer); // <--- memory leak!
}

bool ModelClass::Intersects(ModelClass* other)
{
	if (transform->position.x* transform->scale.x < (*other).transform->position.x + ((*other).transform->scale.x) &&
		transform->position.x* transform->scale.x + (transform->scale.x	) >(*other).transform->position.x &&
		transform->position.y + transform->scale.y/2 > (*other).transform->position.y &&
		transform->position.y - transform->scale.y/2 < (*other).transform->position.y)
	{
		return true;
	}
	return false;
}

//The Shutdown function will call the shutdown functions for the vertex and index buffers.
void ModelClass::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void ModelClass::Frame()
{
	//update vertex buffer
}

//Render is called from the GraphicsClass::Render function.
//This function calls RenderBuffers to put the vertexand index buffers on 
//the graphics pipeline so the color shader will be able to render them.

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	Frame();
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

//GetIndexCount returns the number of indexes in the model.
//The color shader will need this information to draw this model.

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

//The InitializeBuffers function is where we handle creating the vertexand index buffers.
//Usually you would read in a modeland create the buffers from that data file.
//For this tutorial we will just set the points in the vertexand index buffer manually since it is only a single triangle.

bool ModelClass::InitializeBuffers(ID3D11Device* d11device)
{
	this->device = d11device;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	PrimitiveClass primitive = PrimitiveClass(meshType,modelColor);
	primitive.SetPosition((*initialPos).x, (*initialPos).y, (*initialPos).z);
	primitive.Rescale(transform->scale.x, transform->scale.y, transform->scale.z);
	//primitive.Rescale(0.75f, 5.0f, 0.0f); //hardcode
	
	// Set the number of vertices in the vertex array.
	m_vertexCount = primitive.vertexCount;

	// Set the number of indices in the index array.
	m_indexCount = primitive.indexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	origin = new XMFLOAT3[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (size_t i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = primitive.vertices[i].position;
		vertices[i].color = primitive.vertices[i].color;
		origin[i] = primitive.vertices[i].position;
	}
	for (size_t i = 0; i < m_indexCount; i++)
	{
		indices[i] = primitive.indices[i];
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	/*delete[] vertices;
	vertices = 0;*/

	delete[] initialPos;
	initialPos = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* ddeviceContext)
{
	if (!deviceContext) 
	{
		this->deviceContext = ddeviceContext;
	}

	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Render the triangle.
	deviceContext->DrawIndexed(m_indexCount, 0, 0);
	return;
}
