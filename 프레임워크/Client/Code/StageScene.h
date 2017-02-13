#ifndef StageScene_h__
#define StageScene_h__

#include "Scene.h"

class CStageScene
	: public CScene
{
private:
	explicit CStageScene(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CStageScene(void);

public:
	static CStageScene* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);

public:
	virtual HRESULT Ready_Scene(void);
	virtual INT Update(const FLOAT& fTimeDelta);

private:
	HRESULT Ready_GameLogic(void);
	HRESULT Ready_Environment(void);
	void Ready_NavCell(void);
	vector<D3DXVECTOR3*>   m_vecPoint;

public:
	virtual void Release(void);
};


#endif // StageScene_h__
