#ifndef _SCENE_H_
#define _SCENE_H_

#include "graphicsclass.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

class Scene : GraphicsClass {
public:
	Scene();
	~Scene();

	virtual void Load();
	virtual void  Unload();
	bool Init(int, int, HWND);
	virtual bool  Update();
	virtual void  Render();

private:
	GraphicsClass* m_Graphics;
};
#endif