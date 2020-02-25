#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "primitiveclass.h"
using namespace DirectX;

//geo encapsulated
class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	struct Transform
	{
		XMFLOAT3 position;
		XMFLOAT3 scale;
	};
public:
	ModelClass();
	ModelClass(XMFLOAT3* initialPos, PrimitiveType type, XMFLOAT3 scale);
	ModelClass(const ModelClass&);
	~ModelClass();
	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Frame();
	void Render(ID3D11DeviceContext*);
	void Translate(/*ID3D11DeviceContext* deviceContext,*/XMFLOAT3 direction, float distance);
	bool Intersects(ModelClass* other);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	ID3D11Device* device;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
public:
	int m_vertexCount, m_indexCount;
	unsigned long* indices;
	VertexType* vertices;
	Transform* transform;
	XMFLOAT3* initialPos;
	PrimitiveType meshType;
	ID3D11DeviceContext* deviceContext;
};
#endif
