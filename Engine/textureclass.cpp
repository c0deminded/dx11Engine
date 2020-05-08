#include "textureclass.h"


TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}
bool TextureClass::Initialize(ID3D11Device* device, LPCWSTR filename)
{
	HRESULT result;


	// Load the texture in.
	//CreateShaderResourceView(device, filename, NULL, NULL, &m_texture, NULL);
	ScratchImage image;
	result = LoadFromTGAFile(filename,nullptr,image);
	ID3D11ShaderResourceView* pSRV = nullptr;
	if (SUCCEEDED(result))
	{
		result = CreateShaderResourceView(device,image.GetImages(),
			image.GetImageCount(),image.GetMetadata(), &pSRV);
		m_texture = pSRV;
		return true;
		if (FAILED(result)) 
		{
			return false;
		}
	}

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}