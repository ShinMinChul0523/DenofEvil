#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "Input.h"
#include "TimeMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"

//For.Getter------------------------------------------------------------------------------------------------------------
inline IDXGISwapChain* Get_SwapChain();
inline ID3D11RenderTargetView* Get_RenderTargetView();
inline ID3D11DepthStencilView* Get_DepthStencilView();
inline ID3D11Buffer* Get_BaseShaderCB();
inline ID3D11Buffer* Get_InstShaderCB();
inline ID3D11Buffer* Get_DynamicShaderCB();
inline ID3D11SamplerState* Get_BaseSampler();
inline FLOAT Get_TimeDelta(const TCHAR*);
inline BOOL Get_Activate(const TCHAR*, const FLOAT&);
inline CHAR Get_InputKeyState(BYTE);
inline BOOL Get_InputKeyDown(BYTE);
inline CHAR Get_InputMouseState(CInput::MOUSECLICK);
inline BOOL Get_InputMouseDown(CInput::MOUSECLICK);
inline BOOL Get_InputMouseUp(CInput::MOUSECLICK);
inline LONG Get_InputMouseMove(CInput::MOUSEMOVE);

//For.Setter------------------------------------------------------------------------------------------------------------
inline void Set_AlphaEnable(BOOL);
inline void Set_CullEnable(BOOL);
inline void Set_InputState(void);
inline void Set_TimeDelta(const TCHAR*);

//For.Generic------------------------------------------------------------------------------------------------------------

// Init
inline HRESULT Ready_GraphicDev(CGraphicDev::WINMODE, HWND, const WORD&, const WORD&, WORD, BOOL, ID3D11Device*&, ID3D11DeviceContext*&);
inline HRESULT Ready_InputDev(HINSTANCE, HWND);
inline HRESULT Ready_Timer(const TCHAR*);
inline HRESULT Ready_Frame(const TCHAR*, const FLOAT&);
inline HRESULT Ready_Font(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*);

// Update & Render
inline void Render_Font(const TCHAR*, const TCHAR*, FLOAT, FLOAT, FLOAT, UINT);

//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_System(void);

#include "Export_System.inl"

#endif // Export_System_h__