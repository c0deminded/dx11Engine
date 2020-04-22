#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include <windows.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"

#include "lightshaderclass.h"
#include "lightclass.h"
#include "Gameobject.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int,int);
	bool Render(CameraClass*);
	void SetRenderable(Gameobject*,ModelClass*);

	D3DClass* m_D3D;
	std::vector<Gameobject*> m_GameobjsPool;
	std::vector<ModelClass*> m_ModelsPool;
private:
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
};
#endif