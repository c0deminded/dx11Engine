#include "graphicsclass.h"
#include "primitiveclass.h"
GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_colorShader = 0;
	m_LightShader = 0;
	m_Light = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 1.0f, -55.0f);

	m_Model = new ModelClass[5];

	for (size_t i = 0; i < 5; i++)
	{
		m_Model[i] = ModelClass(XMFLOAT3(0.f, 0.f, 0.f));
	}
	// Create the Model.
	if (!m_Model)
	{
		return false;
	}
	
	for (size_t i = 0; i < 5; i++)
	{
		result = m_Model[i].Initialize(m_D3D->GetDevice(), L"../Engine/cube.txt", L"../Engine/brick.tga");
	}
	// Initialize the model.
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	m_colorShader = new ColorShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	result = m_colorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader )
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	if (m_colorShader)
	{
		m_colorShader->Shutdown();
		delete m_colorShader;
		m_colorShader = 0;
	}
	// Release the model object.
	/*if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}*/
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

//perform update
bool GraphicsClass::Frame(int axisL, int axisR)
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	// Set the position of the camera.
	//m_Camera->SetPosition(0.0f, 0.0f, -75.0f);

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//worldMatrix = XMMatrixRotationY(rotation);
	
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.

	worldMatrix = XMMatrixScaling(4.0f , 4.0f, 4.0f) * XMMatrixTranslation(0.0f , 0.0f, 0.0f) * XMMatrixRotationY(-rotation);
	m_Model[0].Render(m_D3D->GetDeviceContext());
	
	result = m_colorShader->Render(m_D3D->GetDeviceContext(), m_Model[0].GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix);

	XMMATRIX lastPlanetMatrix;

	for (int i = 1; i < 4; i++) {
	worldMatrix = XMMatrixScaling(0.7f * i, 0.7f * i, 0.7f * i) * XMMatrixTranslation(6.0f * i , 0.0f, 0.0f)/* * XMMatrixRotationY(i * rotation)*/;
	lastPlanetMatrix = worldMatrix;
	m_Model[i].Render(m_D3D->GetDeviceContext());

	result = m_colorShader->Render(m_D3D->GetDeviceContext(), m_Model[i].GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix);
	}
	worldMatrix =    XMMatrixTranslation(5.0f, 0.0f, 5.0f) * XMMatrixRotationY(-rotation) * lastPlanetMatrix;
	m_Model[4].Render(m_D3D->GetDeviceContext());

	result = m_colorShader->Render(m_D3D->GetDeviceContext(), m_Model[4].GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix);
	// Render the model using the light shader.
	if (!result)
	{
		return false;
	}
	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	return true;
}
