#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"
#include "CAnimation.h"

namespace Engine {
	class CAnimation;

	class ENGINE_DLL CProtoMgr : public CBase
	{
		DECLARE_SINGLETON(CProtoMgr)

	private:
		explicit CProtoMgr();
		virtual ~CProtoMgr();

	public:
		HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
		CComponent* Clone_Proto(const _tchar* pProtoTag);
		HRESULT			Ready_ProtoAnim(const _tchar* pProtoAnimTag, CAnimation* pAnim);
		CAnimation* Clone_ProtoAnim(const _tchar* pProtoAnimTag);

	private:
		CComponent* Find_Prototype(const _tchar* pProtoTag);
		CAnimation* Find_ProtoAnim(const _tchar* pProtoAnimTag);

	private:
		map<const _tchar*, CComponent*>		m_mapProto;
		map<const _tchar*, CAnimation*>		m_mapProtoAnim;

	public:
		virtual void	Free();
	};

}