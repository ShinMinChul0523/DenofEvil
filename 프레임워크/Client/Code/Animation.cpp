#include "stdafx.h"
#include "Animation.h"
#include "Export_System.h"
#include "AnimationInfo.h"

CAnimation::CAnimation(ID3D11DeviceContext* pContext)
	: m_pContext(pContext)
{
}

CAnimation::~CAnimation(void)
{
}

void CAnimation::Push_AnimData(ANIMDATA& tAniData)
{
	m_vecAnimData.push_back(tAniData);
}

void CAnimation::UpdateSubresource(CAnimationInfo* pAnimInfo, XMFLOAT4X4* pBoneWorld)
{
	XMFLOAT4X4 matBoneReturn;
	XMFLOAT4X4 matBoneWorld[MAX_BONE_MATRICES];

	for (int i = 0; i < MAX_BONE_MATRICES; ++i)
	{
		XMStoreFloat4x4(&matBoneWorld[i], XMMatrixIdentity());

		if (pBoneWorld)
			XMStoreFloat4x4(&pBoneWorld[i], XMMatrixIdentity());
	}

	if (pAnimInfo != nullptr)
	{
		const WORD& wCurKey = pAnimInfo->Get_CurKey();
		const WORD& wNextKey = pAnimInfo->Get_NextKey();
		const WORD& wCurFrame = pAnimInfo->Get_CurFrame();
		const WORD& wNextFrame = pAnimInfo->Get_NextFrame();
		const FLOAT& fRatio = pAnimInfo->Get_Ratio();

		size_t uiClusterSize = m_vecAnimData[wCurKey][wCurFrame].size();

		for (size_t iCluster = 0; iCluster < uiClusterSize; ++iCluster)
		{
			XMMATRIX matRot = XMMatrixRotationQuaternion(XMQuaternionSlerp(XMLoadFloat4(&m_vecAnimData[wCurKey][wCurFrame][iCluster].R)
				, XMLoadFloat4(&m_vecAnimData[wNextKey][wNextFrame][iCluster].R), fRatio));

			XMMATRIX matTrans = XMMatrixTranslationFromVector(XMLoadFloat3(&m_vecAnimData[wCurKey][wCurFrame][iCluster].T));
			XMMATRIX matTrans2 = XMMatrixTranslationFromVector(XMLoadFloat3(&m_vecAnimData[wNextKey][wNextFrame][iCluster].T));

			XMMATRIX matScale = XMMatrixScalingFromVector(XMLoadFloat3(&m_vecAnimData[wCurKey][wCurFrame][iCluster].S));
			XMMATRIX matScale2 = XMMatrixScalingFromVector(XMLoadFloat3(&m_vecAnimData[wNextKey][wNextFrame][iCluster].S));

			for (int iRow = 0; iRow < 4; ++iRow)
			{
				for (int iColumn = 0; iColumn < 4; ++iColumn)
				{
					matTrans.m[iRow][iColumn] = (matTrans.m[iRow][iColumn] * (1.f - fRatio)) + (matTrans2.m[iRow][iColumn] * fRatio);
					matScale.m[iRow][iColumn] = (matScale.m[iRow][iColumn] * (1.f - fRatio)) + (matScale2.m[iRow][iColumn] * fRatio);
				}
			}

			XMStoreFloat4x4(&matBoneWorld[iCluster], XMMatrixTranspose(matRot * matTrans * matScale));
			XMStoreFloat4x4(&pBoneWorld[iCluster], matRot * matTrans * matScale);
		}
	}

	ID3D11Buffer* pDynamicShaderCB = Get_DynamicShaderCB();
	m_pContext->UpdateSubresource(pDynamicShaderCB, 0, NULL, &matBoneWorld, 0, 0);
}

CAnimation* CAnimation::Create(ID3D11DeviceContext* pContext)
{
	CAnimation* pAnimation = new CAnimation(pContext);
	return pAnimation;
}

void CAnimation::Release(void)
{
	delete this;
}