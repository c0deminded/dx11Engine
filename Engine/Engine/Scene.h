#ifndef _SCENE_H_
#define _SCENE_H_

#include "graphicsclass.h"
#include "Katamari.h"
#include "KataVictim.h"
#include "Gameplane.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

class Scene  {
public:
	Scene();
	~Scene();

	virtual void Load();
	virtual void  Unload();
	bool Init(int, int, HWND);
	virtual bool  Update(int axisX, int axisY);
	virtual void  Render();

private:
	
	CameraClass* m_Camera;
	GraphicsClass* m_Graphics;
	Gameplane* m_Gameplane;	
	Katamari* m_Katamari;	
	KataVictim* m_Katavictim;
	KataVictim* m_Katavictim2;
};
#endif