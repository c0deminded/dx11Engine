#ifndef _PRIMITIVECLASS_H_
#define _PRIMITIVECLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

static enum class PrimitiveType { Triangle, Rectangle, Hexagon};
class PrimitiveClass 
{
public: 
	int vertexCount, indexCount;
	unsigned long* indices;
	struct Vertices
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	Vertices* vertices;
	PrimitiveClass(PrimitiveType type);
	~PrimitiveClass();
	void Rescale(float x, float y, float z);
	void SetPosition(float x,float y, float z);
private :
	void BuildTriangle();
	void BuildRectangle();
	void BuildHexagon();
};

#endif