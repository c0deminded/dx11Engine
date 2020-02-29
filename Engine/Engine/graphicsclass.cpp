#include "graphicsclass.h"
#include "primitiveclass.h"
GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_BarLeft = 0;
	m_BarRight = 0;
	m_Ball = 0;
	m_BorderUp = 0;
	m_BorderDown = 0;
	m_ColorShader = 0;
	ballDirection = XMFLOAT3(1.f, 0.f, 0.f);
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
	m_BarLeft = new ModelClass(new XMFLOAT3(-25.f,0.f,0.f),PrimitiveType::Rectangle, XMFLOAT3(1.5f, 10.0f, 0.0f));
	if (!m_BarLeft)
	{
		return false;
	}
	// Create the right bar.
	m_BarRight = new ModelClass(new XMFLOAT3(25.f, 0.f, 0.f), PrimitiveType::Rectangle, XMFLOAT3(1.5f, 10.0f, 0.0f));
	if (!m_BarRight)
	{
		return false;
	}
	// Create the ball.
	m_Ball = new ModelClass(new XMFLOAT3(0.0f, 0.0f, 0.0f), PrimitiveType::Hexagon,XMFLOAT3(1.0f,1.0f,1.0f));
	if (!m_Ball)
	{
		return false;
	}
	// Create the upper border.
	m_BorderUp = new ModelClass(new XMFLOAT3(0.f, 30.5f, 0.f), PrimitiveType::Rectangle, XMFLOAT3(85.0f, 1.0f, 0.0f));
	if (!m_BorderUp)
	{
		return false;
	}
	// Create the lower border.
	m_BorderDown = new ModelClass(new XMFLOAT3(0.f, -30.5f, 0.f), PrimitiveType::Rectangle, XMFLOAT3(85.0f, 1.0f, 0.0f));
	if (!m_BorderDown)
	{
		return false;
	}

	

	// Initialize the left bar.
	result = m_BarLeft->Initialize(m_D3D->GetDevice());
	result = m_BarRight->Initialize(m_D3D->GetDevice());
	result = m_Ball->Initialize(m_D3D->GetDevice());
	result = m_BorderUp->Initialize(m_D3D->GetDevice());
	result = m_BorderDown->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize at least one model(pong).", L"Error", MB_OK);
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
	if (m_Ball)
	{
		m_Ball->Shutdown();
		delete m_Ball;
		m_Ball = 0;
	}
	if (m_BorderUp)
	{
		m_BorderUp->Shutdown();
		delete m_BorderUp;
		m_BorderUp = 0;
	}
	if (m_BorderDown)
	{
		m_BorderDown->Shutdown();
		delete m_BorderDown;
		m_BorderDown = 0;
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

//perform update
bool GraphicsClass::Frame(int axisL, int axisR)
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

	//check intersection && translate
	if (m_BarRight->Intersects(m_Ball))
	{
		XMFLOAT3 ballPos = m_Ball->transform->position;
		XMFLOAT3 barPos = m_BarRight->transform->position;

		XMFLOAT3 directon = XMFLOAT3(ballPos.x - barPos.x, ballPos.y - barPos.y, 0);
		float magnitude = sqrt((directon.x * directon.x) + (directon.y * directon.y));
		directon = XMFLOAT3(directon.x/magnitude,directon.y/magnitude,0);
		ballDirection = XMFLOAT3(-directon.x, directon.y, 0.0f);
		
	}
	else if (m_BarLeft->Intersects(m_Ball))
	{
		XMFLOAT3 ballPos = m_Ball->transform->position;
		XMFLOAT3 barPos = m_BarLeft->transform->position;

		XMFLOAT3 directon = XMFLOAT3(ballPos.x - barPos.x, ballPos.y - barPos.y, 0);
		float magnitude = sqrt((directon.x * directon.x) + (directon.y * directon.y));
		directon = XMFLOAT3(directon.x / magnitude, directon.y / magnitude, 0);
		ballDirection = XMFLOAT3(-directon.x, directon.y, 0.0f);	
	}

	if (m_Ball->transform->position.x > 50.0f ||
		m_Ball->transform->position.x < -50.0f) 
	{
		m_Ball->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
	}
	if (m_Ball->transform->position.y > 29.f ||
		m_Ball->transform->position.y < -29.f)
	{
		ballDirection = XMFLOAT3(ballDirection.x, -ballDirection.y, 0.0f);
	}
	m_Ball->Translate(XMFLOAT3(ballDirection.x, ballDirection.y, 0.0f), 0.4f);
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
	m_Ball->Render(m_D3D->GetDeviceContext());
	m_BorderUp->Render(m_D3D->GetDeviceContext());
	m_BorderDown->Render(m_D3D->GetDeviceContext());

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
