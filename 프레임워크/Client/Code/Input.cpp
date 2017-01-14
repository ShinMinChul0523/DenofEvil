#include "stdafx.h"
#include "Input.h"

IMPLEMENT_SINGLETON(CInput)

CInput::CInput(void)
	: m_pInput(NULL)
	, m_pKeyBoard(NULL)
	, m_pMouse(NULL)
{
	ZeroMemory(m_byKeyState, sizeof(CHAR) * 256);
	ZeroMemory(m_byKeyStateBefore, sizeof(CHAR) * 256);
	ZeroMemory(m_bKeyDown, sizeof(BOOL) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(m_byMouseBefore, sizeof(CHAR) * 2);
	ZeroMemory(m_bMouseDown, sizeof(BOOL) * 2);
	ZeroMemory(m_bMouseUp, sizeof(BOOL) * 2);
}

CInput::~CInput(void)
{
}

// Key Check
CHAR CInput::Get_InputKeyState(BYTE KeyFlag)
{
	return m_byKeyState[KeyFlag];
}

BOOL CInput::Get_InputKeyDown(BYTE KeyFlag)
{
	return m_bKeyDown[KeyFlag];
}

CHAR CInput::Get_InputMouseState(MOUSECLICK KeyFlag)
{
	return m_MouseState.rgbButtons[KeyFlag];
}

BOOL CInput::Get_InputMouseDown(MOUSECLICK KeyFlag)
{
	return m_bMouseDown[KeyFlag];
}

BOOL CInput::Get_InputMouseUp(MOUSECLICK KeyFlag)
{
	return m_bMouseUp[KeyFlag];
}

LONG CInput::Get_InputMouseMove(MOUSEMOVE KeyFlag)
{
	return *(((LONG*)&m_MouseState) + KeyFlag);
}

void CInput::Set_InputState(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	// Mouse
	for (INT iKeyFlag = 0; iKeyFlag < 2; ++iKeyFlag)
	{
		// Down
		if ((m_MouseState.rgbButtons[(MOUSECLICK)iKeyFlag] & 0x80) && !(m_byMouseBefore[(MOUSECLICK)iKeyFlag] & 0x80))
			m_bMouseDown[iKeyFlag] = TRUE;

		else
			m_bMouseDown[iKeyFlag] = false;

		// Up
		if (!(m_MouseState.rgbButtons[(MOUSECLICK)iKeyFlag] & 0x80) && (m_byMouseBefore[(MOUSECLICK)iKeyFlag] & 0x80))
			m_bMouseUp[iKeyFlag] = TRUE;

		else
			m_bMouseUp[iKeyFlag] = false;

		m_byMouseBefore[(MOUSECLICK)iKeyFlag] = m_MouseState.rgbButtons[(MOUSECLICK)iKeyFlag];
	}

	// Keyboard
	for (INT iKeyFlag = 0; iKeyFlag < 256; ++iKeyFlag)
	{
		// Down
		if ((m_byKeyState[iKeyFlag] & 0x80) && !(m_byKeyStateBefore[iKeyFlag] & 0x80))
			m_bKeyDown[iKeyFlag] = TRUE;

		else
			m_bKeyDown[iKeyFlag] = false;

		m_byKeyStateBefore[iKeyFlag] = m_byKeyState[iKeyFlag];
	}
}

HRESULT CInput::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	FAILED_CHECK_MSG(hr, L"DirectInput8Create Failed");

	hr = Ready_KeyBoard(hWnd);
	FAILED_CHECK_MSG(hr, L"Ready_KeyBoard Failed");

	hr = Ready_Mouse(hWnd);
	FAILED_CHECK_MSG(hr, L"Ready_Mouse Failed");

	return S_OK;
}

HRESULT CInput::Ready_KeyBoard(HWND hWnd)
{
	HRESULT hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	// FOREGROUND(창이 활성화 되있을 때만 키입력을 받음) <-> BACKGROUND
	FAILED_CHECK(hr);

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	FAILED_CHECK(hr);

	// 활성화
	m_pKeyBoard->Acquire();

	return S_OK;
}

HRESULT CInput::Ready_Mouse(HWND hWnd)
{
	HRESULT hr;
	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	FAILED_CHECK(hr);

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	FAILED_CHECK(hr);

	m_pMouse->Acquire();

	return S_OK;
}

void CInput::Release(void)
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInput);

	delete this;
}
