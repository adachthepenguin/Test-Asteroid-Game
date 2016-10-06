#pragma once

#include <d3dx9.h>



class GraphicsHelper;
class TextureManager;
class Entity;



class Graphics
{
	friend class GraphicsManager;

protected:
	Entity* m_pEntity;

	float m_posX;
	float m_posY;
	float m_direction;
	float m_length;
	float m_width;

public:
	Graphics(Entity* pEntity);
	virtual ~Graphics();

	virtual void update(const float seconds);
	virtual void draw(GraphicsHelper* pGraphicsHelper) = 0;
};



class GraphicsHelper
{
private:
	float m_x, m_y;
	float m_direction;
	float m_length, m_width, m_height;
	float m_alpha;

	D3DXMATRIX m_matWorld;

	LPDIRECT3DDEVICE9 m_pDev;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	TextureManager* m_pTextureManager;

public:
	GraphicsHelper(LPDIRECT3DDEVICE9 pDevice,
		LPDIRECT3DVERTEXBUFFER9 pVB,
		TextureManager* pTextureManager);

	void setPosition(const float x, const float y);
	void setDirection(const float direction);
	void setScale(const float length, const float width, const float height = 1.0f);
	void setAlpha(const float alpha);
	void setTexture(const int textureID);

	void drawCircle();
	void drawRectangle();
	void drawCube();

private:
	void invalidateMatrix();
};