#ifndef Input_h__
#define Input_h__

#include "Define.h"

class CInput
{
	DECLARE_SINGLETON(CInput)

public:
	enum MOUSECLICK	{ DIM_LBUTTON, DIM_RBUTTON };
	enum MOUSEMOVE	{ DIM_X, DIM_Y, DIM_Z };

private:
	explicit CInput(void);
	~CInput(void);

public:
	CHAR Get_InputKeyState(BYTE KeyFlag);
	BOOL Get_InputKeyDown(BYTE KeyFlag);
	CHAR Get_InputMouseState(MOUSECLICK KeyFlag);
	BOOL Get_InputMouseDown(MOUSECLICK KeyFlag);
	BOOL Get_InputMouseUp(MOUSECLICK KeyFlag);
	LONG Get_InputMouseMove(MOUSEMOVE KeyFlag);

public:
	void Set_InputState(void);

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);

private:
	HRESULT Ready_KeyBoard(HWND hWnd);
	HRESULT Ready_Mouse(HWND hWnd);

public:
	void Release(void);

private:
	LPDIRECTINPUT8			m_pInput;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

private:
	CHAR			m_byKeyState[256];
	CHAR			m_byKeyStateBefore[256];		// 이전 키보드 상태
	BOOL			m_bKeyDown[256];				// 키보드가 눌렸을 때 한번만 TRUE를 반환
	
	DIMOUSESTATE	m_MouseState;
	CHAR			m_byMouseBefore[2];				// 이전 마우스 상태
	BOOL			m_bMouseDown[2];				// 마우스가 클릭 했을 때 한번만 TRUE를 반환
	BOOL			m_bMouseUp[2];					// 마우스 클릭이 해제 됐을 때 한번만 TRUE를 반환
};

#endif // Input_h__