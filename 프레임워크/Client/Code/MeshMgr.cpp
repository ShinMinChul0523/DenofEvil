#include "stdafx.h"
#include "MeshMgr.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Texture.h"
#include "Export_System.h"

IMPLEMENT_SINGLETON(CMeshMgr)

CMeshMgr::CMeshMgr(void)
{
}

CMeshMgr::~CMeshMgr(void)
{
}

void CMeshMgr::Add_ObbCheckList(UINT uiObjNum, const XMFLOAT4X4* pWorld, const CGameObject* pObj)
{
	dynamic_cast<CDynaicMesh*>(m_vecMeshData[uiObjNum]->pMesh)->Add_ObbCheckList(pWorld, pObj);
}

XMFLOAT4X4 CMeshMgr::Get_TransMeshBone(UINT uiObjNum, INT iSubsetIdx)
{
	if (dynamic_cast<CDynaicMesh*>(m_vecMeshData[uiObjNum]->pMesh))
		return dynamic_cast<CDynaicMesh*>(m_vecMeshData[uiObjNum]->pMesh)->Get_TransBoneMatrix(iSubsetIdx);

	else
		return nullptr;
}

const XMFLOAT3* CMeshMgr::Get_MeshMin(UINT uiObjNum)
{
	return m_vecMeshData[uiObjNum]->pMesh->Get_Min();
}

const XMFLOAT3* CMeshMgr::Get_MeshMax(UINT uiObjNum)
{
	return m_vecMeshData[uiObjNum]->pMesh->Get_Max();
}

BOOL CMeshMgr::Get_MeshUseAlpha(UINT uiObjNum)
{
	return m_vecMeshData[uiObjNum]->bAlpha;
}

void CMeshMgr::Ready_MeshMgr(MESHDATA* pMeshData)
{
	m_vecMeshData.push_back(pMeshData);
}

void CMeshMgr::Render_MeshMgr(UINT uiObjNum, BOOL bColliderDraw)
{
	if (m_vecMeshData[uiObjNum]->bBillboard)
		Set_CullEnable(FALSE);

	m_vecMeshData[uiObjNum]->pMesh->Render(bColliderDraw);

	if (m_vecMeshData[uiObjNum]->bBillboard)
		Set_CullEnable(TRUE);
}

void CMeshMgr::RenderAnim_MeshMgr(UINT uiObjNum, CAnimationInfo* pAnimInfo, BOOL bColliderDraw /*= FALSE*/)
{
	if (dynamic_cast<CDynaicMesh*>(m_vecMeshData[uiObjNum]->pMesh))
		dynamic_cast<CDynaicMesh*>(m_vecMeshData[uiObjNum]->pMesh)->RenderAnim(pAnimInfo, bColliderDraw);
}

void CMeshMgr::RenderInst_MeshMgr(UINT uiObjNum, const vector<XMFLOAT4X4*>& vecObjWorld)
{
	if (m_vecMeshData[uiObjNum]->bBillboard)
		Set_CullEnable(FALSE);

	m_vecMeshData[uiObjNum]->pMesh->RenderInst(vecObjWorld);

	if (m_vecMeshData[uiObjNum]->bBillboard)
		Set_CullEnable(TRUE);
}

void CMeshMgr::Release(void)
{
	size_t dwSize = m_vecMeshData.size();

	for (size_t i = 0; i < dwSize; ++i)
	{
		Safe_Release(m_vecMeshData[i]->pMesh);
		Safe_Delete(m_vecMeshData[i]);
	}

	m_vecMeshData.clear();

	delete this;
}