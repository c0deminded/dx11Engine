#ifndef _SCENE_H_
#define _SCENE_H_

#include "graphicsclass.h"
#include "Katamari.h"
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
	GraphicsClass* m_Graphics;
	Katamari* m_Katamari;
};
#endif