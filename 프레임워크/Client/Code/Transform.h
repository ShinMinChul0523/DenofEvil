#pragma once

#include "Component.h"

class CTransform
	: public CComponent
{
private:
	explicit CTransform(void);
	virtual ~CTransform(void);

public:
	virtual void Update(FLOAT fTime);
	void Update_MatrixNotXRot(void);

public:
	static CTransform* Create(void);

public:
	virtual void Release(void);

public:
	XMFLOAT3			m_vScale;
	XMFLOAT3			m_vAngle;
	XMFLOAT3			m_vPos;
	XMFLOAT3			m_vDir;
	XMFLOAT4X4			m_matWorld;
};