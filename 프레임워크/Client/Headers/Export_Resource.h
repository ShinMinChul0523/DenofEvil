#ifndef Export_Resource_h__
#define Export_Resource_h__

#include "ResourceMgr.h"
#include "MeshMgr.h"

//For.Getter------------------------------------------------------------------------------------------------------------
inline XMFLOAT4X4 Get_TransMeshBone(UINT, INT);
inline const XMFLOAT3* Get_MeshMin(UINT);
inline const XMFLOAT3* Get_MeshMax(UINT);
inline BOOL Get_MeshUseAlpha(UINT);

//For.Setter-------------------------------------------------------------------------------------------------------------
inline void Add_ObbCheckList(UINT, const XMFLOAT4X4*, const CGameObject*);
inline HRESULT Reserve_ContainerSize(const WORD&);

//For.Generic------------------------------------------------------------------------------------------------------------

// Init
inline HRESULT Ready_Buffer(ID3D11Device*, ID3D11DeviceContext*, const WORD&, CResourceMgr::BUFFERTYPE, const TCHAR*, const WORD& wCntX = 2, const WORD& wCntZ = 2, const WORD& wItv = 1);
inline HRESULT Ready_Texture(ID3D11Device*, ID3D11DeviceContext*, const WORD&, const TCHAR*, CTexture::TEXTURETYPE, const TCHAR*, const WORD& wCnt = 0);
inline HRESULT Ready_Mesh(ID3D11Device*, ID3D11DeviceContext*, const WORD&, CResourceMgr::MESHTYPE, const TCHAR*, const TCHAR*);
inline void Ready_MeshMgr(MESHDATA*);

// Update & Render
inline CResource* Clone_ResourceMgr(const WORD&, const TCHAR*);
inline void Reset_Resource(const WORD&);
inline void Render_MeshMgr(UINT, BOOL bColliderDraw = FALSE);
inline void RenderAnim_MeshMgr(UINT, CAnimationInfo*, BOOL bColliderDraw = FALSE);
inline void RenderInst_MeshMgr(UINT, const vector<XMFLOAT4X4*>&);

//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_Resource(void);

#include "Export_Resource.inl"

#endif // Export_Resource_h__