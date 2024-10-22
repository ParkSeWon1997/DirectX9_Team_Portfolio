#pragma once
#include "VIBuffer.h"
#include "Engine_Define.h"

namespace Engine
{
    class CTerrainCubeTex :
        public CVIBuffer
    {
    private:
        explicit CTerrainCubeTex() = delete;
        explicit CTerrainCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
        explicit CTerrainCubeTex(const CTerrainCubeTex& rhs);
        virtual ~CTerrainCubeTex();

    public:
        virtual HRESULT		Ready_Buffer(const _ulong& dwCntX,
            const _ulong& dwCntZ,
            const _ulong& dwVtxItv);
        virtual void		Render_Buffer();

    private:
        //
        _vec3* m_pPos;
        VTXCUBE* m_pCube;

    public:
        static CTerrainCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
        // CVIBuffer을(를) 통해 상속됨
        CComponent* Clone() override;

    private:
        virtual void	Free();
    };
}

