#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "LoginScene.h"
#include "StageScene.h"

class CSceneSelector
{
public:
	enum SCENE { SCENE_LOGIN, SCENE_LOAD, SCENE_STAGE, SCENE_END };

public:
	explicit CSceneSelector(SCENE eSceneID) : m_eSceneID(eSceneID) {}
	~CSceneSelector(void) {}

public:
	HRESULT operator () (CScene** ppScene, ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	{
		switch (m_eSceneID)
		{
		case SCENE_LOGIN:
			//*ppScene = CLoginScene::Create(pGraphicDev, pContext);
			break;

		case SCENE_LOAD:
			//*ppScene = CLoadScene::Create(pGraphicDev, m_eLoadingType);
			break;

		case SCENE_STAGE:
			*ppScene = CStageScene::Create(pGraphicDev, pContext);
			break;
		}

		if (*ppScene == NULL)
			return E_FAIL;

		return S_OK;
	}

private:
	SCENE m_eSceneID;
	//CLoading::LOADINGTYPE	m_eLoadingType;
};


#endif // SceneSelector_h__
