#ifndef MeshMgr_h__
#define MeshMgr_h__

#include "Define.h"

class CGameObject;
class CAnimationInfo;
class CTexture;
class CMeshMgr
{
	DECLARE_SINGLETON(CMeshMgr)

private:
	CMeshMgr(void);
	~CMeshMgr(void);

public:
	void Add_ObbCheckList(UINT uiObjNum, const XMFLOAT4X4* pWorld, const CGameObject* pObj);
	XMFLOAT4X4 Get_TransMeshBone(UINT uiObjNum, INT iSubsetIdx);
	const XMFLOAT3* Get_MeshMin(UINT uiObjNum);
	const XMFLOAT3* Get_MeshMax(UINT uiObjNum);
	BOOL Get_MeshUseAlpha(UINT uiObjNum);

public:
	void Ready_MeshMgr(MESHDATA* pMeshData);
	void Render_MeshMgr(UINT uiObjNum, BOOL bColliderDraw = FALSE);
	void RenderAnim_MeshMgr(UINT uiObjNum, CAnimationInfo* pAnimInfo, BOOL bColliderDraw = FALSE);
	void RenderInst_MeshMgr(UINT uiObjNum, const vector<XMFLOAT4X4*>& vecObjWorld);
	void Release(void);

private:
	vector<MESHDATA*>	m_vecMeshData;
};

#endif // MeshMgr_h__