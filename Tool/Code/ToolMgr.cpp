#include "ToolMgr.h"
#include "framework.h"

#include "Export_Utility.h"
#include "ToolObj.h"

_vec3 CToolMgr::m_fDirectionDiffuseColor[3] = { {1.f, 1.f, 1.f} };
_vec3 CToolMgr::m_fDirectionAmbientColor[3] = { {1.f, 1.f, 1.f} };
_vec3 CToolMgr::m_fDirectionSpecularColor[3] = { {1.f, 1.f, 1.f} };

int CToolMgr::iItemCurrentMonsterIdx = 0;
int CToolMgr::iItemCurrentEtcIdx = 0;
int CToolMgr::iItemCurrentItemIdx = 0;
int CToolMgr::iAddObjIdx = 0;

_bool CToolMgr::bObjectAdd = false;
_bool CToolMgr::bItemAdd = false;
_bool CToolMgr::bMonsterAdd = false;
_bool CToolMgr::bObjSaveData = false;
_bool CToolMgr::bObjLoadData = false;
_bool CToolMgr::bPointSaveData = false;
_bool CToolMgr::bPointLoadData = false;
_bool CToolMgr::bLightSaveData = false;
_bool CToolMgr::bLightLoadData = false;
_bool CToolMgr::bTerrainWireFrame = false;

_int CToolMgr::iPickingIndex = -1;
vector<_int> CToolMgr::vecPickingIdex;

_int CToolMgr::iTimeLight = 0; // 밤, 낮 조명 값 저장하기 
_int CToolMgr::iAUtoTime = 1;

_float CToolMgr::fMonsterY = 0.f;
_float CToolMgr::fMonsterScale[4] = { 1.f, 1.f, 1.f, 1.f };

vector<CGameObject*> CToolMgr::m_vecObj;

CToolMgr::CToolMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), 
    item_current_idx(0)
{
}

CToolMgr::~CToolMgr()
{
}

HRESULT CToolMgr::Setup_Imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);

	return S_OK;
}

void CToolMgr::Update_ToolMgr()
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //매 업데이트마다 돌아감
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;

    //ImGui::ShowDemoWindow(&show_demo_window);
    Window_Tile();
    Window_Light();
    Window_Object();
    //Window_Patch();
}

void CToolMgr::Render_ToolMgr()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//타일툴
void CToolMgr::Window_Tile()
{
    ImGui::Begin("Tile Tool");

    ImGui::Checkbox("TerrainWireFrame", &bTerrainWireFrame);

    ImGui::NewLine();
    ImGui::Text("Picking VTX : (%d)", iPickingIndex);

    if (ImGui::BeginListBox("Unused List"))
    {
        for (int i = 0; i < vecPickingIdex.size(); ++i)
        {
            const bool bSelected = (item_current_idx == i);

            char buf[32];
            sprintf(buf, "%d", vecPickingIdex[i]);

            if (ImGui::Selectable(buf, bSelected))
                item_current_idx = i;

            if (bSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();

        }

    if (ImGui::SmallButton("Save"))
        bPointSaveData = true;

    ImGui::SameLine();

    if (ImGui::SmallButton("Load"))
        bPointLoadData = true;

    ImGui::End();
}

//조명 툴
void CToolMgr::Window_Light()
{
    ImGui::Begin("Light");

    ImGui::Text("Auto Time Set");
        ImGui::RadioButton("AutoTime", &iAUtoTime, 0); ImGui::SameLine();
    ImGui::RadioButton("SetLight", &iAUtoTime, 1);

    ImGui::NewLine();

    ImGui::RadioButton("Morning", &iTimeLight, 0); ImGui::SameLine();
    ImGui::RadioButton("Afternoon", &iTimeLight, 1); ImGui::SameLine();
    ImGui::RadioButton("Night", &iTimeLight, 2);

    ImGui::NewLine();
    ImGui::Text("Scene Direction");
    ImGui::ColorEdit3("DirectionDiffuse", m_fDirectionDiffuseColor[iTimeLight]);
    ImGui::ColorEdit3("DirectionAmbient", m_fDirectionAmbientColor[iTimeLight]);
    ImGui::ColorEdit3("DirectionSpecular", m_fDirectionSpecularColor[iTimeLight]);

    if (ImGui::SmallButton("Save"))
        bLightSaveData = true;

    ImGui::SameLine();

    if (ImGui::SmallButton("Load"))
        bLightLoadData = true;


    ImGui::End();
}

//오브젝트 배치 툴 
void CToolMgr::Window_Object()
{
    //지형지물
    //몬스터
    //플레이어 
    //아이템 
    ImGui::Begin("Object Tool");
    if (ImGui::CollapsingHeader("Monster", ImGuiTreeNodeFlags_None))
    {
        /*ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
        for (int i = 0; i < 5; i++)
            ImGui::Text("Some content %d", i);*/

        if (ImGui::BeginListBox("Monster List"))
        {
            const char* Items[] = { "Spider", "Pig", "Beefalo", "Boss", "Tallbird"};

            for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
            {
                const bool bSelected = (CToolMgr::iItemCurrentMonsterIdx == i);
                if (ImGui::Selectable(Items[i], bSelected))
                {
                    CToolMgr::iItemCurrentMonsterIdx = i;
                    CToolMgr::bItemAdd = false;
                    CToolMgr::bObjectAdd = false;
                }
                    
                if (bSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        if (ImGui::SmallButton("MonsterAdd")) bMonsterAdd = true;
    }
    if (ImGui::CollapsingHeader("Etc", ImGuiTreeNodeFlags_None))
    {
        if (ImGui::BeginListBox("Etc List"))
        {
            const char* Items[] = { "Tree", "Rock", "Grass", "Pig_House", 
                "Pig_House_Dead", "Berry", "FireFlies", "Teleporter", "BossDoor", "TrapSpike", "Capapult"
                ,"TrapToothHide"
            };
            
            for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
            {
                const bool bSelected = (CToolMgr::iItemCurrentEtcIdx == i);
                if (ImGui::Selectable(Items[i], bSelected))
                {
                    CToolMgr::iItemCurrentEtcIdx = i;
                    CToolMgr::bItemAdd = false;
                    CToolMgr::bMonsterAdd = false;
                }
                    
                if (bSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        if (ImGui::SmallButton("ObjectAdd")) bObjectAdd = true;
    }

    if (ImGui::CollapsingHeader("Item", ImGuiTreeNodeFlags_None))
    {
        if (ImGui::BeginListBox("Item List"))
        {
            const char* Items[] = { 
                "Stick", 
                "Stone", 
                "Firestone", //부싯돌
                "Grass_Cut",
                };

            for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
            {
                const bool bSelected = (iItemCurrentItemIdx == i);
                if (ImGui::Selectable(Items[i], bSelected))
                {
                    iItemCurrentItemIdx = i;
                    CToolMgr::bMonsterAdd = false;
                    CToolMgr::bObjectAdd = false;
                }
                    
                if (bSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        if (ImGui::SmallButton("ItemAdd")) 
            bItemAdd = true;
    }

    ImGui::InputFloat("Pos Y", &fMonsterY, 0.10f, 10.0f, "%.3f");

    ImGui::InputFloat3("Scale", fMonsterScale);
    

    if (ImGui::SmallButton("Save"))
        bObjSaveData = true;
    
    ImGui::SameLine();

    if (ImGui::SmallButton("Load"))
        bObjLoadData = true;

    ImGui::End();
}

void CToolMgr::Window_Patch()
{
    ImGui::Begin("Obj Patch");
        if (ImGui::BeginListBox("Add List"))
        {
            //const char* Items[] = { "Spider", "Pig", "Beefalo", "Boss" };

            for (int i = 0; i < m_vecObj.size(); ++i)
            {
                size_t len = std::wcstombs(nullptr, m_vecObj[i]->GetObjName().c_str(), 0);
                char* buf = new char[len + 1];
                std::wcstombs(buf, m_vecObj[i]->GetObjName().c_str(), len + 1);

                const bool bSelected = (CToolMgr::iAddObjIdx == i);
                if (ImGui::Selectable(buf, bSelected))
                {
                    CToolMgr::iAddObjIdx = i;
                    //CToolMgr::bItemAdd = false;
                    //CToolMgr::bObjectAdd = false;
                }

                if (bSelected)
                    ImGui::SetItemDefaultFocus();

                delete[] buf;
            }
            ImGui::EndListBox();
        }

        static float fY = 0.10f;
        ImGui::InputFloat("Pos Y", &fY, 0.10f, 10.0f, "%.3f");

        if (!m_vecObj.empty())
        {
            _vec3 vPos;
            m_vecObj[iAddObjIdx]->GetTransForm()->Get_Info(INFO_POS, &vPos);
            vPos.y = fY;
            m_vecObj[iAddObjIdx]->GetTransForm()->Set_Pos(vPos);
        }

        static float vec4a[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        ImGui::InputFloat3("Scale", vec4a);
        if (!m_vecObj.empty())
        {
            _vec3 vScale = _vec3(vec4a[0], vec4a[1], vec4a[2]);
            m_vecObj[iAddObjIdx]->GetTransForm()->Set_Scale(vScale);
        }

        ImGui::End();
}

CToolMgr* CToolMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CToolMgr* pInstance = new CToolMgr(pGraphicDev);

    if (FAILED(pInstance->Setup_Imgui()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }

	return pInstance;
}

void CToolMgr::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    //TODO::delete 해주기
    for (int i = 0; i < m_pTileTexture.size(); ++i)
        Engine::Safe_Release(m_pTileTexture[i]);
}
