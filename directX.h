#pragma once



#include <d3dx9.h>


class EntityManager;
class GraphicsHelper;

#define CUSTOM_FVF D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE

struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
	float tu, tv;
};



class TextureManager
{
private:
	int m_size;
	int m_currentTextures;
	IDirect3DTexture9** m_pTexturesArray;
	IDirect3DDevice9* m_pDevice;

	bool m_isInitialized;

public:
	TextureManager();
	virtual ~TextureManager();

	bool initialize(const int size, IDirect3DDevice9* pDevice);
	int loadTexture(const char* filename);
	bool unloadTexture(const int id);
	void unloadAllTextures();
	int getTextureNum() const;
	IDirect3DTexture9* getTextureFromID(const int id);
};



class DirectX
{
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pDev;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPD3DXFONT m_pFont;

	bool m_antialias;
	float m_aspect;

	D3DVIEWPORT9 m_vp;

	TextureManager m_textureManager;

	// CAMERA
	float m_eyeX;
	float m_eyeY;
	float m_eyeDistance;

	GraphicsHelper* m_pGraphicsHelper;

public:
	DirectX();
	~DirectX();

	bool initialize(HWND hWnd);
	void cleanup();

	void update(const float seconds);
	void render(EntityManager* pEntityManager);

	void setPosition(const float x, const float y);
	void setDistance(const float distance);
	void moveCamera(const float x, const float y, const float distance);
	float getWorldX(const float x) const;
	float getWorldY(const float y) const;

private:
	bool initD3D();
	bool initDevice(HWND hWnd);
	bool initGeomethry();
	bool initTextures();
	bool initFonts();

	void invalidateCamera();
};