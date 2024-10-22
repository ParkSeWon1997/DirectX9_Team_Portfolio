#pragma once
#include "Export_Utility.h"
#include "GameObject.h"

namespace Engine
{
    class ENGINE_DLL CTile :
        public CGameObject
    {
    private:
        explicit CTile(LPDIRECT3DDEVICE9 pGraphicDev);
        virtual ~CTile();

    public:
        Engine::CCubeTex* Get_CubeTex() { return m_pBufferCom; }

    public:
        virtual HRESULT Ready_GameObject()						 override;
        virtual _int Update_GameObject(const _float& fTimeDelta) override;
        virtual void LateUpdate_GameObject()					 override;
        virtual void Render_GameObject()override;

    private:
        HRESULT Add_Component();

    private:
        Engine::CCubeTex* m_pBufferCom;
        Engine::CTexture* m_pTextureCom;
        Engine::CCalculator* m_pCalculatorCom;

    public:
        static CTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    private:
        virtual void Free();
    };
}

