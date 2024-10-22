#include "stdafx.h"
#include "CFire.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include <MainApp.h>
#include "CSmoke.h"

CFire::CFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
	, m_iLightNum(++CMainApp::g_iLightNum)
	, m_fAccTime(0.f)
{
}

CFire::CFire(const CFire& rhs)
	: CResObject(rhs.m_pGraphicDev)
	, m_iLightNum(++CMainApp::g_iLightNum)
	, m_fAccTime(0.f)
{

}

CFire::~CFire()
{
}

HRESULT CFire::Ready_GameObject()
{
	Ready_Light();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObject_id = BON_FIRE;
	m_efireCurState = FIRE_LEVEL_2;
	m_fFrame = 0.0f;
	return S_OK;
}

_int CFire::Update_GameObject(const _float& fTimeDelta)
{
	Change_Light();

	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrame > m_fFrameEnd)
	{
		m_fFrame = 0.0f;
	}



	Engine::Update_Sound(_vec3{ 1,0,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), STEREO_BGM, 5.f);
	Check_FrameState();


	m_fDownTime += fTimeDelta;
	m_MaxfTimeChek -= fTimeDelta;


	//1분이 되고, 불이 켜져있으면 불 단계를 낮추고 시간을 초기화
	if (m_fDownTime >= 60.f && m_bIsOff == false)
	{
		Level_Down();
		m_fDownTime = 0.f;
	}


	//최대 지속시간이 240초가 넘어가고 불이 1단계일 때 불을 꺼버림 그리고 최대 지속시간은 초기화
	if (m_MaxfTimeChek <= 0 && m_efireCurState == FIRE_LEVEL_1)
	{
		m_bIsOff = true;
		m_MaxfTimeChek = 240.f;
	}

	//JMK
	//불이 켜져있을때만 시간추가후 1초마다 불뿜기
	if (!m_bIsOff)
	{
		m_fAccTime += fTimeDelta;

		if (m_fAccTime >= 0.09f)
		{
			m_fAccTime = 0.f;
			_vec3 vSmokePos = GetTransForm()->Get_Pos();
			vSmokePos.y += 0.3f;
			CGameObject* pGameObject = CSmoke::Create(m_pGraphicDev, L"PARTICLE_SMOKE", 5, vSmokePos, 0.08f, 4.f, 1.6f);
			CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::PARTICLE, pGameObject);
		}

	}


	//PSW 공간음향

	//Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT_CONTINUE_CH2, 2);
	//Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT_CONTINUE_CH3, 3);
	//Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT_CONTINUE_CH4, 4);


	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);



	return 0;
}

void CFire::LateUpdate_GameObject()
{




	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CFire::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	//불은 이미지가 꺼져도 항상 있어야함 그래서 이미지만 꺼버림
	if (m_bIsOff == false)
	{
		if (m_bShader)
		{
			m_pFireTexCom[m_efireCurState]->Set_Texture(m_pShaderCom, "g_Texture", m_fFrame);
			_matrix maxView, maxProj;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &maxView);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &maxProj);

			if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransForm->Get_WorldMatrix())))
				return;
			if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &maxView)))
				return;
			if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &maxProj)))
				return;

			if (FAILED(m_pShaderCom->Begin_Shader(0)))
				return;

			m_pBufferCom->Render_Buffer();

			if (FAILED(m_pShaderCom->End_Shader()))
				return;
		}
		else
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
			m_pFireTexCom[m_efireCurState]->Set_Texture((_uint)m_fFrame);
			FAILED_CHECK_RETURN(SetUp_Material(), );
			m_pBufferCom->Render_Buffer();
		}

	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CFire::Set_Pos(_vec3 _Pos)
{
	m_pTransForm->Set_Pos(_Pos);
}

void CFire::Set_NextLevel()
{


	if (m_efireCurState == FIRE_LEVEL_1)
	{
		m_efireCurState = FIRE_LEVEL_2;
	}
	else if (m_efireCurState == FIRE_LEVEL_2)
	{
		m_efireCurState = FIRE_LEVEL_3;
	}
	else if (m_efireCurState == FIRE_LEVEL_3)
	{
		m_efireCurState = FIRE_LEVEL_4;
	}


}

void CFire::Set_MoreTime()
{
	//떨어지는 단계를 막기위해 시간을 줄여줌
	m_fDownTime = 0.0f;


	//최대 지속시간 240초 보다 작을때만 증가 
	if (m_MaxfTimeChek <= 220)
	{
		m_MaxfTimeChek += 20.0f;

	}

}

HRESULT CFire::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pFireTexCom[FIRE_LEVEL_1] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_1"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_1", pComponent });


	pComponent = m_pFireTexCom[FIRE_LEVEL_2] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_2", pComponent });

	pComponent = m_pFireTexCom[FIRE_LEVEL_3] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_3"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_3", pComponent });

	pComponent = m_pFireTexCom[FIRE_LEVEL_4] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_4"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_4", pComponent });


	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(proto::Clone_Proto(L"Proto_Shader_Rect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Shader_Rect", pComponent });





	m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 0.0f, vPos.z);

	return S_OK;
}

void CFire::Check_FrameState()
{
	if (m_efireCurState != m_efirerPrevState)
	{

		switch (m_efireCurState)
		{
		case CFire::FIRE_LEVEL_1:
			m_pTransForm->Set_Scale(_vec3(0.4f, 0.4f, 0.4f));
			Engine::StopSound(STEREO_BGM);
			
			//공간 음향/ 이걸로 사운드 재생
			//Engine::SpatialPlay_Sound(L"Obj_Campfire_Lv1.mp3", SOUND_EFFECT_CONTINUE_CH1);
			Engine::PlayEffectContinue(L"Obj_Campfire_Lv1.mp3", 0.8f, STEREO_BGM);
			//Engine::PlayBGM(L"Obj_Campfire_Lv1.mp3", 1.f);
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		case CFire::FIRE_LEVEL_2:
			m_pTransForm->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
			//Engine::StopSound(SOUND_EFFECT_CONTINUE_CH3);
			Engine::StopSound(STEREO_BGM);
			
			//공간 음향/ 이걸로 사운드 재생
			//Engine::SpatialPlay_Sound(L"Obj_Campfire_Lv2.mp3", SOUND_EFFECT_CONTINUE_CH2);
			Engine::PlayEffectContinue(L"Obj_Campfire_Lv2.mp3", 0.8f, STEREO_BGM);
			//Engine::PlayBGM(L"Obj_Campfire_Lv2.mp3", 2.f);
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		case CFire::FIRE_LEVEL_3:
			m_pTransForm->Set_Scale(_vec3(0.6f, 0.6f, 0.6f));
			//Engine::StopSound(SOUND_EFFECT_CONTINUE_CH1);
			Engine::StopSound(STEREO_BGM);
			
			//공간 음향/ 이걸로 사운드 재생
			//Engine::SpatialPlay_Sound(L"Obj_Campfire_Lv3.mp3", SOUND_EFFECT_CONTINUE_CH3);
			Engine::PlayEffectContinue(L"Obj_Campfire_Lv3.mp3", 0.8f, STEREO_BGM);
		
			//Engine::PlayBGM(L"Obj_Campfire_Lv3.mp3", 3.f);
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		case CFire::FIRE_LEVEL_4:
			m_pTransForm->Set_Scale(_vec3(0.8f, 0.8f, 0.8f));
			Engine::StopSound(STEREO_BGM);
			
			//공간 음향/ 이걸로 사운드 재생
			//Engine::SpatialPlay_Sound(L"Obj_Campfire_Lv4.mp3", SOUND_EFFECT_CONTINUE_CH4);
			Engine::PlayEffectContinue(L"Obj_Campfire_Lv4.mp3", 0.8f, STEREO_BGM);
			
			//Engine::PlayBGM(L"Obj_Campfire_Lv4.mp3", 4.f);
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		default:
			break;
		}

		m_efirerPrevState = m_efireCurState;
		m_fFrame = 0.f;
	}


}



HRESULT CFire::Ready_Light()
{
	//점광원
// 최초 생성 후 플레이어 횟불 사용 시에만 켜지도록 
	D3DLIGHT9 tPointLightInfo;
	ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo.Type = D3DLIGHT_POINT;

	tPointLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tPointLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tPointLightInfo.Attenuation0 = 0.00001f;

	tPointLightInfo.Range = 5.f;
	tPointLightInfo.Position = { 0.f, 0.f, 0.f };

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tPointLightInfo, m_iLightNum), E_FAIL);
	light::Get_Light(m_iLightNum)->Close_Light();

	return S_OK;
}

void CFire::Change_Light()
{
	if (m_bIsOff)
	{
		light::Get_Light(m_iLightNum)->Close_Light();
		return;
	}

	D3DLIGHT9* tPointLightInfo = light::Get_Light(m_iLightNum)->Get_Light();
	//ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo->Type = D3DLIGHT_POINT;

	tPointLightInfo->Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tPointLightInfo->Attenuation0 = 0.1f;

	tPointLightInfo->Range = 10.f;

	_vec3 pPigHousePos;
	m_pTransForm->Get_Info(INFO_POS, &pPigHousePos); // player pos 값 설정
	tPointLightInfo->Position = pPigHousePos;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);
	light::Get_Light(m_iLightNum)->Update_Light();
}

void CFire::Level_Down()
{


	if (m_efireCurState == FIRE_LEVEL_4)
	{
		m_efireCurState = FIRE_LEVEL_3;

	}
	else if (m_efireCurState == FIRE_LEVEL_3)
	{
		m_efireCurState = FIRE_LEVEL_2;
	}
	else if (m_efireCurState == FIRE_LEVEL_2)
	{
		m_efireCurState = FIRE_LEVEL_1;
	}
	else if (m_efireCurState == FIRE_LEVEL_1)
	{
		Engine::StopSound(STEREO_BGM);
		m_bIsOff = true;
	}


}

CFire* CFire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFire* pInstance = new CFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFire::Free()
{
	CGameObject::Free();
}


