//For.Getter------------------------------------------------------------------------------------------------------------
IDXGISwapChain* Get_SwapChain()
{
	return CGraphicDev::GetInstance()->Get_SwapChain();
}

ID3D11RenderTargetView* Get_RenderTargetView()
{
	return CGraphicDev::GetInstance()->Get_RenderTargetView();
}

ID3D11DepthStencilView* Get_DepthStencilView()
{
	return CGraphicDev::GetInstance()->Get_DepthStencilView();
}

ID3D11Buffer* Get_BaseShaderCB()
{
	return CGraphicDev::GetInstance()->Get_BaseShaderCB();
}

ID3D11Buffer* Get_InstShaderCB()
{
	return CGraphicDev::GetInstance()->Get_InstShaderCB();
}

ID3D11Buffer* Get_DynamicShaderCB()
{
	return CGraphicDev::GetInstance()->Get_DynamicShaderCB();
}


ID3D11SamplerState* Get_BaseSampler()
{
	return CGraphicDev::GetInstance()->Get_BaseSampler();
}

FLOAT Get_TimeDelta(const TCHAR* pTimerTag)
{
	return CTimeMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}

BOOL Get_Activate(const TCHAR* pFrameTag, const FLOAT& fTimeDelta)
{
	return CFrameMgr::GetInstance()->Get_Activate(pFrameTag, fTimeDelta);
}

CHAR Get_InputKeyState(BYTE KeyFlag)
{
	return CInput::GetInstance()->Get_InputKeyState(KeyFlag);
}

BOOL Get_InputKeyDown(BYTE KeyFlag)
{
	return CInput::GetInstance()->Get_InputKeyDown(KeyFlag);
}

CHAR Get_InputMouseState(CInput::MOUSECLICK KeyFlag)
{
	return CInput::GetInstance()->Get_InputMouseState(KeyFlag);
}

BOOL Get_InputMouseDown(CInput::MOUSECLICK KeyFlag)
{
	return CInput::GetInstance()->Get_InputMouseDown(KeyFlag);
}

BOOL Get_InputMouseUp(CInput::MOUSECLICK KeyFlag)
{
	return CInput::GetInstance()->Get_InputMouseUp(KeyFlag);
}

LONG Get_InputMouseMove(CInput::MOUSEMOVE KeyFlag)
{
	return CInput::GetInstance()->Get_InputMouseMove(KeyFlag);
}

//For.Setter------------------------------------------------------------------------------------------------------------
void Set_AlphaEnable(BOOL bEnable)
{
	CGraphicDev::GetInstance()->Set_AlphaEnable(bEnable);
}

void Set_CullEnable(BOOL bEnable)
{
	CGraphicDev::GetInstance()->Set_CullEnable(bEnable);
}

void Set_InputState(void)
{
	CInput::GetInstance()->Set_InputState();
}

void Set_TimeDelta(const TCHAR* pTimerTag)
{
	CTimeMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}

//For.Generic------------------------------------------------------------------------------------------------------------

// Init
HRESULT Ready_GraphicDev(CGraphicDev::WINMODE eWinMode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY, WORD wFPS, BOOL bEnable4xMsaa, ID3D11Device*& pGraphicDev, ID3D11DeviceContext*& pContext)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(eWinMode, hWnd, wSizeX, wSizeY, wFPS, bEnable4xMsaa, pGraphicDev, pContext);
}

HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInput::GetInstance()->Ready_InputDev(hInst, hWnd);
}

HRESULT Ready_Timer(const TCHAR* pTimerTag)
{
	return CTimeMgr::GetInstance()->Ready_Timer(pTimerTag);
}

HRESULT Ready_Frame(const TCHAR* pFrameTag, const FLOAT& fFps)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fFps);
}

HRESULT Ready_Font(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pFontName)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pContext, pFontName);
}

// Update & Render
void Render_Font(const TCHAR* pFontName, const TCHAR* pString, FLOAT fSize, FLOAT fPosX, FLOAT fPosY, UINT uiColor)
{
	CFontMgr::GetInstance()->Render_Font(pFontName, pString, fSize, fPosX, fPosY, uiColor);
}

//For.Release------------------------------------------------------------------------------------------------------------
void Release_System(void)
{
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CInput::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}