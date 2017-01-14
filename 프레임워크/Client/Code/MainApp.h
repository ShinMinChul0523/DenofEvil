#ifndef MainApp_h__
#define MainApp_h__

#include "Define.h"

class CMainApp
{
private:
	explicit CMainApp(void);
	virtual ~CMainApp(void);

public:
	HRESULT Initialize(void);
	INT Update(const FLOAT& fTimeDelta);
	void Render(void);

public:
	static CMainApp* Create(void);

public:
	void Release(void);

private:
	void Ready_StaticMeshFromFile(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	void Ready_DynamicMeshFromFile(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	void Ready_TextureFromFile(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
};

#endif // MainApp_h__
