#include "directX.h"
#include "graphics.h"
#include "objects.h"



TextureManager::TextureManager()
: m_currentTextures(0), m_size(0), m_pTexturesArray(0), m_pDevice(0), m_isInitialized(false) {}

TextureManager::~TextureManager()
{
	unloadAllTextures();
}

bool TextureManager::initialize(const int size, IDirect3DDevice9* pDevice)
{
	if (!pDevice) { return false; }

	m_pDevice = pDevice;

	m_currentTextures = 0;
	m_size = size;

	m_pTexturesArray = new IDirect3DTexture9*[m_size];
	if (!m_pTexturesArray)
	{
		return false;
	}

	m_isInitialized = true;

	return true;
}

int TextureManager::loadTexture(const char* filename)
{
	if (!m_isInitialized) { return -1; }

	if (m_currentTextures >= m_size) { return -1; }

	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(filename, &info) != D3D_OK)
	{
		return -1;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
		filename,
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		info.Format,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		0,
		0,
		&m_pTexturesArray[m_currentTextures])))
	{
		return -1;
	}

	if (m_pTexturesArray[m_currentTextures])
	{
		IDirect3DSurface9* pS;
		m_pTexturesArray[m_currentTextures]->GetSurfaceLevel(0, &pS);
		D3DSURFACE_DESC sd;
		pS->GetDesc(&sd);
		int r = 0;
	}

	return m_currentTextures++;
}

bool TextureManager::unloadTexture(const int id)
{
	if (id >= m_currentTextures) { return false; }
	else if (id < 0) { return false; }

	if (m_pTexturesArray[m_currentTextures])
	{
		m_pTexturesArray[m_currentTextures]->Release();
	}

	for (int i = id + 1; id < m_currentTextures; i++)
	{
		m_pTexturesArray[i - 1] = m_pTexturesArray[i];
	}

	m_currentTextures--;

	return true;
}

void TextureManager::unloadAllTextures()
{
	for (int i = 0; i < m_currentTextures; i++)
	{
		if (m_pTexturesArray[i])
		{
			m_pTexturesArray[i]->Release();
		}
	}

	m_currentTextures = 0;
}

int TextureManager::getTextureNum() const
{
	return m_currentTextures;
}

IDirect3DTexture9* TextureManager::getTextureFromID(const int id)
{
	if (id >= m_currentTextures) { return 0; }
	else if (id < 0) { return 0; }

	return m_pTexturesArray[id];
}



DirectX::DirectX()
{
	m_pD3D = 0;
	m_pDev = 0;
	m_pVB = 0;
	m_pIB = 0;

	m_eyeX = m_eyeY = 0.0f;
	m_eyeDistance = 100.0f;

	m_pGraphicsHelper = 0;
}

DirectX::~DirectX()
{
	cleanup();
}

bool DirectX::initialize(HWND hWnd)
{
	if (!initD3D()) { return false; }

	if (!initDevice(hWnd)) { return false; }

	if (!initGeomethry()) { return false; }

	if (!initTextures()) { return false; }

	if (!initFonts()) { return false; }

	m_pGraphicsHelper = new GraphicsHelper(m_pDev, m_pVB, &m_textureManager);

	return true;
}

void DirectX::cleanup()
{
	if (m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = 0;
	}
	if (m_pDev)
	{
		m_pDev->Release();
		m_pDev = 0;
	}
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = 0;
	}
	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = 0;
	}

	if (m_pGraphicsHelper)
	{
		delete m_pGraphicsHelper;
	}
}

void DirectX::update(const float seconds)
{
}

void DirectX::render(EntityManager* pEntityManager)
{
	if (SUCCEEDED(m_pDev->BeginScene()))
	{
		m_pDev->SetViewport(&m_vp);

		m_pDev->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 40), 1.0f, 0);

		m_pDev->SetViewport(&m_vp);
		m_pDev->SetFVF(CUSTOM_FVF);
		m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));

		if (pEntityManager)
		{
			pEntityManager->draw(m_pGraphicsHelper);
		}
		
		m_pDev->EndScene();
	}

	m_pDev->Present(0, 0, 0, 0);
}

void DirectX::setPosition(const float x, const float y)
{
	m_eyeX = x;
	m_eyeY = y;

	invalidateCamera();
}

void DirectX::setDistance(const float distance)
{
	m_eyeDistance = distance;
	if (m_eyeDistance < 1.0f)
	{
		m_eyeDistance = 1.0f;
	}

	invalidateCamera();
}

void DirectX::moveCamera(const float x, const float y, const float distance)
{
	setPosition(m_eyeX + x, m_eyeY + y);
	setDistance(m_eyeDistance + distance);
}

float DirectX::getWorldX(const float x) const
{
	float result = 2.0f * (x - 0.5f) * m_aspect * m_eyeDistance + m_eyeX;
	return result;
}

float DirectX::getWorldY(const float y) const
{
	float result = -2.0f * (y - 0.5f) * m_eyeDistance + m_eyeY;
	return result;
}

bool DirectX::initD3D()
{
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return false;
	}

	return true;
}

bool DirectX::initDevice(HWND hWnd)
{
	bool isWindowed = true;
	DWORD msaaQualityLevels;

	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3ddm.Format,
		isWindowed,
		D3DMULTISAMPLE_16_SAMPLES,
		&msaaQualityLevels)))
	{
		m_antialias = true;
	}
	else { m_antialias = false; }

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferWidth = d3ddm.Width;
	d3dpp.BackBufferHeight = d3ddm.Height;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	//d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (m_antialias)
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
		d3dpp.MultiSampleQuality = msaaQualityLevels - 1;
	}

	d3dpp.Windowed = isWindowed;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDev)))
	{
		return false;
	}

	if (m_antialias)
	{
		m_pDev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	}

	m_pDev->SetRenderState(D3DRS_ZENABLE, true);
	m_pDev->SetRenderState(D3DRS_STENCILENABLE, true);

	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDev->SetRenderState(D3DRS_LIGHTING, false);

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_vp.X = 0;
	m_vp.Y = 0;
	m_vp.Width = d3ddm.Width;
	m_vp.Height = d3ddm.Height;
	m_vp.MinZ = 0.0f;
	m_vp.MaxZ = 1.0f;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	m_aspect = (float)screenWidth / screenHeight;

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, m_aspect, 0.0001f, 300.0f);
	m_pDev->SetTransform(D3DTS_PROJECTION, &matProj);

	return true;
}

bool DirectX::initGeomethry()
{
	const int VERTICES_COUNT = 46;
	// 0 - 33		circle
	// 34 - 37		rectangle
	// 38 - 45		cube

	CUSTOMVERTEX vertices[VERTICES_COUNT];

	vertices[0].x = 0.0f;
	vertices[0].y = 0.0f;
	vertices[0].z = 0.0f;
	vertices[0].tu = 0.5f;
	vertices[0].tv = 0.5f;
	vertices[0].color = 0xffffffff;
	for (int i = 0; i < 33; i++)
	{
		float angle = i * D3DX_PI / 16;
		vertices[1 + i].x = 0.5f * cosf(angle);
		vertices[1 + i].y = 0.5f * sinf(angle);
		vertices[1 + i].z = 0.0f;
		vertices[1 + i].tu = vertices[1 + i].x + 0.5f;
		vertices[1 + i].tv = vertices[1 + i].y + 0.5f;
		vertices[1 + i].color = 0xffffffff;
	}

	vertices[34].x = -0.5f;
	vertices[34].y = -0.5f;
	vertices[35].x = +0.5f;
	vertices[35].y = -0.5f;
	vertices[36].x = +0.5f;
	vertices[36].y = +0.5f;
	vertices[37].x = -0.5f;
	vertices[37].y = +0.5f;
	vertices[34].tu = 0.0f;
	vertices[34].tv = 1.0f;
	vertices[35].tu = 1.0f;
	vertices[35].tv = 1.0f;
	vertices[36].tu = 1.0f;
	vertices[36].tv = 0.0f;
	vertices[37].tu = 0.0f;
	vertices[37].tv = 0.0f;
	for (int i = 34; i < 38; i++)
	{
		vertices[i].z = 0.0f;
		vertices[i].color = 0xffffffff;
	}

	vertices[38].x = -0.5f;
	vertices[38].y = -0.5f;
	vertices[38].z = -0.5f;
	vertices[39].x = -0.5f;
	vertices[39].y = +0.5f;
	vertices[39].z = -0.5f;
	vertices[40].x = +0.5f;
	vertices[40].y = +0.5f;
	vertices[40].z = -0.5f;
	vertices[41].x = +0.5f;
	vertices[41].y = -0.5f;
	vertices[41].z = -0.5f;
	vertices[42].x = -0.5f;
	vertices[42].y = -0.5f;
	vertices[42].z = +0.5f;
	vertices[43].x = -0.5f;
	vertices[43].y = +0.5f;
	vertices[43].z = +0.5f;
	vertices[44].x = +0.5f;
	vertices[44].y = +0.5f;
	vertices[44].z = +0.5f;
	vertices[45].x = +0.5f;
	vertices[45].y = -0.5f;
	vertices[45].z = +0.5f;
	for (int i = 38; i < 46; i++)
	{
		vertices[i].tu = 0.0f;
		vertices[i].tv = 0.0f;
		vertices[i].color = 0xffffffff;
	}

	if (FAILED(m_pDev->CreateVertexBuffer(VERTICES_COUNT * sizeof(CUSTOMVERTEX), 0, CUSTOM_FVF, D3DPOOL_DEFAULT, &m_pVB, 0)))
	{
		return false;
	}

	void* ptr;
	if (FAILED(m_pVB->Lock(0, VERTICES_COUNT * sizeof(CUSTOMVERTEX), (void**)&ptr, 0)))
	{
		return false;
	}

	memcpy(ptr, vertices, VERTICES_COUNT * sizeof(CUSTOMVERTEX));

	if (FAILED(m_pVB->Unlock()))
	{
		return false;
	}

	return true;
}

bool DirectX::initTextures()
{
	if (!m_textureManager.initialize(100, m_pDev))
	{
		return false;
	}

	if (m_textureManager.loadTexture("data\\spaceship.png") == -1)
	{
		MessageBox(0, "Unable to load texture", "spaceship.png", MB_OK);
		return false;
	}
	if (m_textureManager.loadTexture("data\\projectile.png") == -1)
	{
		MessageBox(0, "Unable to load texture", "projectile.png", MB_OK);
		return false;
	}
	if (m_textureManager.loadTexture("data\\asteroid.png") == -1)
	{
		MessageBox(0, "Unable to load texture", "asteroid.png", MB_OK);
		return false;
	}

	//FILE* pFile = 0;
	//fopen_s(&pFile, "data\\textures\\textures.txt", "rb");
	//if (!pFile)
	//{
	//	return false;
	//}

	//char buffer[247];
	//while (fgets(buffer, 246, pFile))
	//{
	//	int length = strcspn(buffer, "\r\n ");
	//	buffer[length] = '\0';
	//	const char* filenameBase = "data\\textures\\";
	//	char filename[256];
	//	ZeroMemory(filename, 256);
	//	memcpy(&filename[0], filenameBase, strlen(filenameBase));
	//	memcpy(&filename[strlen(filenameBase)], buffer, strlen(buffer));

	//	if (m_textureManager.loadTexture(filename) == -1) { return false; }
	//};

	//fclose(pFile);

	return true;
}

bool DirectX::initFonts()
{
	if (FAILED(D3DXCreateFont(m_pDev,
		24,
		0,
		FW_BOLD,
		0,
		false,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		//"buxton sketch",
		"SketchFlow Print",
		&m_pFont)))
	{
		return false;
	}

	return true;
}

void DirectX::invalidateCamera()
{
	D3DXVECTOR3 vEye(m_eyeX, m_eyeY, -m_eyeDistance);
	D3DXVECTOR3 vLookAt(m_eyeX, m_eyeY, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEye, &vLookAt, &vUp);
	m_pDev->SetTransform(D3DTS_VIEW, &matView);
}