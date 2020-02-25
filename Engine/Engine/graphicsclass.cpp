#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_BarLeft = 0;
	m_BarRight = 0;
	m_ColorShader = 0;
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
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);

	// Create the left bar.
	m_BarLeft = new ModelClass(new XMFLOAT3(-50.f,0.f,0.f),0);
	if (!m_BarLeft)
	{
		return false;
	}
	// Create the right bar.
	m_BarRight = new ModelClass(new XMFLOAT3(50.f, 0.f, 0.f),1);
	if (!m_BarLeft)
	{
		return false;
	}

	// Initialize the left bar.
	result = m_BarLeft->Initialize(m_D3D->GetDevice());
	result = m_BarRight->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize at least one bar(pong).", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_BarLeft)
	{
		m_BarLeft->Shutdown();
		delete m_BarLeft;
		m_BarLeft = 0;
	}
	if (m_BarRight)
	{
		m_BarRight->Shutdown();
		delete m_BarRight;
		m_BarRight = 0;
	}
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


bool GraphicsClass::Frame(int axisL,int axisR)
{
	bool result;
	// Set the location of the model!!! but how?.
	//m_Model->/*GetIndexCount(mouseX, mouseY, m_D3D->GetDeviceContext());*/
	
	result = true; 
	if (!result)
	{
		return false;
	}
	if (axisL != 0) 
	{
		m_BarLeft->Translate(XMFLOAT3(0.0f, float(axisL), 0.0f), 0.5f);
	}
	if (axisR != 0)
	{
		m_BarRight->Translate(XMFLOAT3(0.0f, float(axisR), 0.0f), 0.5f);
	}
	// Set the position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -75.0f);

	return true;
}


bool GraphicsClass::Render()
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

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BarLeft->Render(m_D3D->GetDeviceContext());
	m_BarRight->Render(m_D3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_BarLeft->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	//result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_BarRight->GetIndexCo	unt(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	return true;
}
