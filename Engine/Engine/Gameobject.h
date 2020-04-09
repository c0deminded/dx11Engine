#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "modelclass.h"
#include "d3dclass.h"

using namespace std;
using namespace DirectX;

class Gameobject 
{
public:
	Gameobject();
	~Gameobject();
	struct Transform
	{
		XMMATRIX trs;

		/*XMFLOAT3 translation;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;*/
	};

	virtual void Load();
	virtual void  Unload();
	virtual bool Init(HWND,D3DClass*);
	virtual bool  Update();
	virtual void  Render();

	virtual void SetActive(bool);
	ModelClass* m_Model;
	Transform* m_Transform;
private:
	D3DClass* m_D3D;
};
#endif