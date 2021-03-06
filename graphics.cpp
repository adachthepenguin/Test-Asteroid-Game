#include "graphics.h"
#include "gameObjects.h"
#include "directX.h"



Graphics::Graphics(Entity* pEntity)
{
	m_pEntity = pEntity;
}

Graphics::~Graphics() {}

void Graphics::update(const float seconds)
{
	if (m_pEntity)
	{
		m_posX = m_pEntity->getX();
		m_posY = m_pEntity->getY();
		m_direction = m_pEntity->getDirection();
		m_length = m_pEntity->getLength();
		m_width = m_pEntity->getWidth();
	}
}



GraphicsHelper::GraphicsHelper(LPDIRECT3DDEVICE9 pDevice,
	LPDIRECT3DVERTEXBUFFER9 pVB,
	TextureManager* pTextureManager)
{
	m_pDev = pDevice;
	m_pVB = pVB;
	m_pTextureManager = pTextureManager;
}

void GraphicsHelper::setPosition(const float x, const float y)
{
	m_x = x;
	m_y = y;

	invalidateMatrix();
}

void GraphicsHelper::setDirection(const float direction)
{
	m_direction = direction;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;

	invalidateMatrix();
}

void GraphicsHelper::setRotation(const float xAxis, const float yAxis, const float zAxis)
{
	m_rotationX = xAxis;
	m_rotationY = yAxis;
	m_direction = zAxis;

	invalidateMatrix();
}

void GraphicsHelper::setScale(const float length, const float width, const float height)
{
	m_length = length;
	m_width = width;
	m_height = height;

	invalidateMatrix();
}

void GraphicsHelper::setAlpha(const float alpha)
{
	m_alpha = alpha;
}

void GraphicsHelper::setTexture(const int textureID)
{
	m_pDev->SetTexture(0, m_pTextureManager->getTextureFromID(textureID));
}

void GraphicsHelper::drawCircle()
{
	m_pDev->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_ARGB((int)(m_alpha * 255), 255, 255, 255));
	m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDev->SetFVF(CUSTOM_FVF);
	m_pDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 32);
}

void GraphicsHelper::drawRectangle()
{
	m_pDev->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_ARGB((int)(m_alpha * 255), 255, 255, 255));
	m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDev->SetFVF(CUSTOM_FVF);
	m_pDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 34, 2);
}

void GraphicsHelper::drawCube()
{
	m_pDev->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_ARGB((int)(m_alpha * 255), 255, 255, 255));
	m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pDev->SetFVF(CUSTOM_FVF);
	m_pDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	for (int i = 0; i < 6; i++)
	{
		m_pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 38 + 4 * i, 2);
	}
}

void GraphicsHelper::invalidateMatrix()
{
	D3DXMATRIX matTranslation, matRotation, matScaling, matRotationX, matRotationY, matRotationZ;
	D3DXMatrixTranslation(&matTranslation, m_x, m_y, 0.0f);
	D3DXMatrixRotationX(&matRotationX, m_direction * D3DX_PI / 180.0f);
	D3DXMatrixRotationY(&matRotationY, m_direction * D3DX_PI / 180.0f);
	D3DXMatrixRotationZ(&matRotationZ, m_direction * D3DX_PI / 180.0f);
	matRotation = matRotationX * matRotationY * matRotationZ;
	D3DXMatrixScaling(&matScaling, m_length, m_width, m_height);

	m_matWorld = matScaling * matRotation * matTranslation;
}