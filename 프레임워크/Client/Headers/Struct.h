#ifndef Struct_h__
#define Struct_h__

typedef struct tagVertexTexture
{
	XMFLOAT3 vPos;
	XMFLOAT2 vTexUV;
	XMFLOAT3 vNormal;
}VTXTEX;

typedef struct tagVertexBone
{
	XMFLOAT3 vPos;
	XMFLOAT2 vTexUV;
	XMFLOAT3 vNormal;
	UINT uiBones[4];
	FLOAT fWeights[4];
}VTXBONE;

typedef struct tagBaseShader_CB
{
	XMMATRIX matWorld;
	XMMATRIX matView;
	XMMATRIX matProj;
}BASESHADER_CB;

typedef struct tagInstShader_CB
{
	XMMATRIX matWorld[INSTCNT];
}INSTSHADER_CB;

typedef struct tagDynamicShader_CB
{
	XMMATRIX matBoneWorld[MAX_BONE_MATRICES];
}DYNAMICSHADER_CB;

typedef struct tagObjData
{
	UINT		uiImgNum;
	XMFLOAT3	vScale;
	XMFLOAT3	vAngle;
	XMFLOAT3	vPos;
}OBJDATA;

class CMesh;
typedef struct tagMeshData
{
	CMesh* pMesh;
	BOOL bAlpha;
	BOOL bBillboard;
}MESHDATA;

typedef struct tagToolObjInfo
{
	XMMATRIX					m_matWorld;
	XMFLOAT3					m_vPosition;
	XMFLOAT3					m_vScale;
	XMFLOAT3					m_vDir;
	FLOAT					m_fAngle[3];
	TCHAR					m_strName[256];

}TOOLOBJECT;

#endif // Struct_h__