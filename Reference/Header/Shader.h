#pragma once
#include "Component.h"

namespace Engine
{
	class ENGINE_DLL CShader final : public CComponent
	{
	private:
		CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
		CShader(const CShader& rhs);
		virtual ~CShader() = default;

	public:
		HRESULT Ready_Shader(const wstring& strShaderFilePath);
		HRESULT Begin_Shader(_uint iPassIndex);
		HRESULT End_Shader();
	public:
		HRESULT Bind_Matrix(D3DXHANDLE hParameter, const _matrix* pMatrix);
		HRESULT Bind_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture);
		HRESULT Bind_Vector(D3DXHANDLE hParameter, const _vec4* pVecter);

	private:
		LPD3DXEFFECT				m_pEffect = { nullptr };

	public:
		static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const wstring& strShaderFilePath);
		virtual CComponent* Clone() override;
		virtual void Free() override;
	};
}