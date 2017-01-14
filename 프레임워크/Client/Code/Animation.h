#ifndef Animation_h__
#define Animation_h__

#include "Define.h"

class CAnimationInfo;
class CAnimation
{
public:
	typedef struct tagClusData{
		XMFLOAT4 R;
		XMFLOAT3 T;
		XMFLOAT3 S;
	}CLUSDATA;

	using FRAME = vector<CLUSDATA>;
	using ANIMDATA = vector<FRAME>;

private:
	explicit CAnimation(ID3D11DeviceContext* pContext);
	~CAnimation(void);

public:
	void Push_AnimData(ANIMDATA& tAniData);
	void UpdateSubresource(CAnimationInfo* pAnimInfo, XMFLOAT4X4* pBoneWorld = nullptr);

public:
	static CAnimation* Create(ID3D11DeviceContext* pContext);
	void Release(void);

private:
	ID3D11DeviceContext*	m_pContext;

private:
	vector<ANIMDATA>	m_vecAnimData;
};

#endif // Animation_h__