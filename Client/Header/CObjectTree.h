#pragma once
#include "GameObject.h"
#include "ResObject.h"

namespace Engine {
class CRcTex;
class CTexture;
class CTransform;
}

class CObjectTree :
	public CResObject
{
private:
	explicit CObjectTree(LPDIRECT3DDEVICE9 pGraphicDev );
	explicit CObjectTree(const CObjectTree& rhs);
	virtual ~CObjectTree();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

private:
	HRESULT			Add_Component() override;
	void Check_FrameState() override;
	void Ready_Stat() override;
	void Change_Frame_Event() override;

public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

	void RandomSoundPlay();


	_vec3 m_vOriginPos;
};


