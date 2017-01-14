#ifndef Player_h__
#define Player_h__

#include "GameObject.h"

class CEquipment;
class CAnimationInfo;
class CTransform;
class CPlayer
	: public CGameObject
{
private:
	explicit CPlayer(ID3D11DeviceContext* pContext);
	virtual ~CPlayer(void);

public:
	static CPlayer* Create(ID3D11DeviceContext* pContext);

public:
	virtual HRESULT	Initialize(void);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

private:
	virtual HRESULT Ready_Component(void);

private:
	void Move(const FLOAT& fTimeDelta);
	
private:
	CAnimationInfo* m_pAnimInfo;
	CAnimationInfo* m_pMonAnimInfo;
	CTransform*		m_pTransform;
	UINT			m_uiObjNum;
	UINT			m_uiNavIdx;
	FLOAT			m_fSpeed;
	FLOAT			m_fHeight;

	XMFLOAT3		m_vLook;

private:
	BOOL			m_bJump;
	FLOAT			m_fJumpTime;

	//Temp
	BOOL m_UseNavMesh;
};

#endif // Player_h__
