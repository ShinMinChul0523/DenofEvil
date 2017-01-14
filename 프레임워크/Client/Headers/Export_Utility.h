#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "ShaderMgr.h"
#include "CameraMgr.h"
#include "RenderTargetMgr.h"
#include "Frustum.h"
#include "Collision.h"

//For.Getter------------------------------------------------------------------------------------------------------------
inline ID3D11VertexShader* Get_VertexShader(const TCHAR*);
inline ID3D11InputLayout* Get_InputLayout(const TCHAR*);
inline ID3D11PixelShader* Get_PixelShader(const TCHAR* );
inline const XMFLOAT4X4* Get_CurCameraProj(void);
inline const XMFLOAT4X4* Get_CurCameraView(void);
inline BOOL Sphere_InFrustum(const XMFLOAT3*, const FLOAT&);
inline BOOL Vertex_InFrustum(const XMFLOAT3*);
inline FLOAT Get_TerrainColY(const XMFLOAT3&);
inline XMFLOAT3 Pick_Terrain(const XMFLOAT3&, const XMFLOAT3&);
inline ID3D11RenderTargetView** Get_RanderTargetViewInMgr(const TCHAR*);
inline ID3D11DepthStencilView* Get_DepthStencilViewInMgr(const TCHAR*);

//For.Setter-------------------------------------------------------------------------------------------------------------
inline void Add_RenderGroup(CRenderer::RENDERTYPE, CGameObject*, FLOAT fViewZ = 0);
inline void Add_RenderInstGroup(CRenderer::RENDERTYPE, UINT, XMFLOAT4X4*);
inline void Set_CurCamera(CCameraMgr::CAMERALIST);
inline void Set_TerrainVtxInCollision(const VTXTEX*);
inline void Set_RenderTarget(const TCHAR*, const UINT&, BOOL);
inline void Set_Texture(const TCHAR*, const UINT&, const UINT);

//For.Generic------------------------------------------------------------------------------------------------------------

// Init
inline HRESULT Ready_Management(ID3D11Device*, ID3D11DeviceContext*);
inline HRESULT Ready_ShaderFromFiles(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*, WCHAR*, BYTE);
inline HRESULT Ready_Frustum(void);
inline void Ready_DynamicCamera(ID3D11DeviceContext*, CCameraMgr::CAMERALIST, FLOAT, FLOAT, XMFLOAT3&, XMFLOAT3&);
inline void Ready_StaticCamera(ID3D11DeviceContext*, CCameraMgr::CAMERALIST, const CTransform*, FLOAT, FLOAT, FLOAT, FLOAT, XMFLOAT3&, XMFLOAT3&);
inline HRESULT Ready_RenderTarget(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*, const DXGI_FORMAT&, const WORD&, const WORD&, FLOAT, FLOAT);

// Update & Render
inline INT Update_Management(const FLOAT&);
inline void Render_Management(void);
template <typename T> HRESULT Change_Scene(T&);
inline void Transform_Frustum(void);
inline void Update_CurCamera(const FLOAT&);
inline void Render_RenderTarget(const TCHAR*);


//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_Utility(void);

#include "Export_Utility.inl"

#endif // Export_Utility_h__