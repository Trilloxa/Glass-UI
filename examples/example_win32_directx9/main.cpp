#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")

#include <tchar.h>

#include "background_pic.h"
#include "blur.hpp"

#include "segue_font.h"
#include "ico_font.h"

float color_edit4[4] = { 1.00f, 1.00f, 1.00f, 1.000f };

float accent_color[4] = { 29 / 255.f, 29 / 255.f, 29 / 255.f, 90 / 255.f };

static int select_count = 0;

bool active = false;

float size_child = 0;

static int key0, m0 = 1;
static int key1, m1 = 1;

bool menu = false;
ImFont* ico = nullptr;
ImFont* ico_combo = nullptr;
ImFont* ico_button = nullptr;
ImFont* ico_grande = nullptr;
ImFont* segu = nullptr;
ImFont* default_segu = nullptr;
ImFont* bold_segu = nullptr;
ImFont* ico_bold = nullptr;
LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool translation = false;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IDirect3DTexture9* scene = nullptr;

int tab_count = 0, tabs = 0, sub_tabs = 0;

int main(int, char**)
{

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_POPUP, 0, 0, 1920, 1060, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiContext& g = *GImGui;

    io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());

    default_segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());

    segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());

    bold_segu = io.Fonts->AddFontFromMemoryTTF(&bold_segue, sizeof bold_segue, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 24, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_combo = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 27, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_button = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 26, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_bold = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 35, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_grande = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);


    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        DWORD window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::SetNextWindowPos(ImVec2(1200 / 2, 952 / 2));

        ImGui::NewFrame();
        {

            ImGui::SetNextWindowSize(ImVec2(1000, 752));

            ImGui::Begin("Menu", &menu, window_flags);
            {
                ImVec2 P1, P2;
                ImDrawList* pDrawList;
                const auto& p = ImGui::GetWindowPos();
                const auto& pWindowDrawList = ImGui::GetWindowDrawList();
                const auto& pBackgroundDrawList = ImGui::GetBackgroundDrawList();
                const auto& pForegroundDrawList = ImGui::GetForegroundDrawList();

                DrawBackgroundBlur(ImGui::GetWindowDrawList(), g_pd3dDevice);

                ImGui::SetCursorPos(ImVec2(790, 28));

                ImGui::PushItemWidth(130);

                static int selectedTr = 0;
                static const char* itemsTr[]{ "English", "Russian" };
                ImGui::Combo("D##translator", &selectedTr, itemsTr, IM_ARRAYSIZE(itemsTr), 5);
                ImGui::PopItemWidth();

                translation = selectedTr;

                pWindowDrawList->AddCircleFilled(ImVec2(25.000f + p.x, 23.000f + p.y), 5, ImColor(254 / 255.f, 94 / 255.f, 87 / 255.f, 255 / 255.f));
                pWindowDrawList->AddCircleFilled(ImVec2(40.000f + p.x, 23.000f + p.y), 5, ImColor(250 / 255.f, 188 / 255.f, 56 / 255.f, 255 / 255.f));
                pWindowDrawList->AddCircleFilled(ImVec2(55.000f + p.x, 23.000f + p.y), 5, ImColor(116 / 255.f, 170 / 255.f, 92 / 255.f, 255 / 255.f));
                if (scene == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &background, sizeof(background), 1920, 1080, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &scene);
                ImGui::GetBackgroundDrawList()->AddImage(scene, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(color_edit4[0], color_edit4[1], color_edit4[2], color_edit4[3]));
                pBackgroundDrawList->AddRectFilled(ImVec2(0.000f + p.x, 0.000f + p.y), ImVec2(1000 + p.x, 752 + p.y), ImColor(19 / 255.f, 19 / 255.f, 19 / 255.f, 150 / 255.f), 10); // Background
                pBackgroundDrawList->AddRect(ImVec2(0.000f + p.x, 0.000f + p.y), ImVec2(1000 + p.x, 752 + p.y), ImColor(36 / 255.f, 36 / 255.f, 36 / 255.f, 259 / 255.f), 10); // Background border

                static char SearchList[64] = { "" };

                ImGui::SetCursorPos(ImVec2(31, 65));

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(37, 2.5f));
                ImGui::PushItemWidth(220);
                ImGui::InputText("A", SearchList, 64);

                if (!SearchList[0]) pWindowDrawList->AddText(default_segu, 25.f, ImVec2(68 + p.x, 72 + p.y), ImColor(140, 141, 142, 255), translation ? u8"Искать..." : "Search...");

                ImGui::PopItemWidth();

                ImGui::PopStyleVar(1);

                pWindowDrawList->AddText(ico_bold, 35.f, ImVec2(274 + p.x, 68 + p.y), ImColor(140, 141, 142, 255), "C");

                if (ImGui::OptButton("O", ImVec2(35, 35), ImVec2(732, 15), true));

                pWindowDrawList->AddRectFilled(ImVec2(0.000f + p.x, 45.000f + p.y), ImVec2(325 + p.x, 46 + p.y), ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255 / 255.f));
                pWindowDrawList->AddRectFilled(ImVec2(0.000f + p.x, 125.000f + p.y), ImVec2(325 + p.x, 126 + p.y), ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255 / 255.f));
                pWindowDrawList->AddRectFilled(ImVec2(325 + p.x, 0.000f + p.y), ImVec2(326 + p.x, 752 + p.y), ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255 / 255.f));
                pWindowDrawList->AddRectFilled(ImVec2(325 + p.x, 85.000f + p.y), ImVec2(1000 + p.x, 86 + p.y), ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255 / 255.f));

                ImGui::SetCursorPos(ImVec2(30, 165));

                ImGui::BeginGroup();

                if (ImGui::TabButton("G", translation ? u8"Ярость-бот" : "Aim Bot", 0 == tabs, ImVec2(190, 40)) && tabs != 0) {
                    tabs = 0;
                    active = true;
                }

                if (ImGui::TabButton("H", translation ? u8"Анти цель" : "My Self", 1 == tabs, ImVec2(190, 40)) && tabs != 1) {
                    tabs = 1;
                    active = true;
                }

                ImGui::SetCursorPos(ImVec2(30, 290));

                pWindowDrawList->AddRectFilled(ImVec2(30 + p.x, 273 + p.y), ImVec2(296 + p.x, 274 + p.y), ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255 / 255.f));

                if (ImGui::TabButton("I", translation ? u8"Визуальное" : "Keep", 2 == tabs, ImVec2(190, 40)) && tabs != 2) {
                    tabs = 2;
                    active = true;
                }

                if (ImGui::TabButton("J", translation ? u8"Скины" : "Tools", 3 == tabs, ImVec2(190, 40)) && tabs != 3) {
                    tabs = 3;
                    active = true;
                }

                ImGui::SetCursorPos(ImVec2(30, 415));

                pWindowDrawList->AddRectFilled(ImVec2(30 + p.x, 397 + p.y), ImVec2(296 + p.x, 398 + p.y), ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255 / 255.f));

                if (ImGui::TabButton("K", translation ? u8"Разное" : "Settings", 4 == tabs, ImVec2(190, 40)) && tabs != 4) {
                    tabs = 4;
                    active = true;
                }

                if (ImGui::TabButton("L", translation ? u8"Конфиги" : "Keep Configs", 5 == tabs, ImVec2(190, 40)) && tabs != 5) {
                    tabs = 5;
                    active = true;
                }

                if (ImGui::TabButton("M", translation ? u8"Скрипты" : "Lua", 6 == tabs, ImVec2(190, 40)) && tabs != 6) {
                    tabs = 6;
                    active = true;
                }

                pWindowDrawList->AddRectFilled(ImVec2(30 + p.x, 577 + p.y), ImVec2(296 + p.x, 578 + p.y), ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255 / 255.f));

                ImGui::EndGroup();

                pWindowDrawList->AddCircleFilled(ImVec2(80.000f + p.x, 663.000f + p.y), 25, ImColor(45 / 255.f, 47 / 255.f, 49 / 255.f, 200 / 255.f));
                pWindowDrawList->AddCircle(ImVec2(80.000f + p.x, 663.000f + p.y), 25, ImColor(80 / 255.f, 80 / 255.f, 80 / 255.f, 200 / 255.f), 100, 2);
                pWindowDrawList->AddText(default_segu, 25.f, ImVec2(119.000f + p.x, 639.000f + p.y), ImColor(255, 255, 255, 255), "Past");
                pWindowDrawList->AddText(default_segu, 20.f, ImVec2(119.000f + p.x, 663.000f + p.y), ImColor(100, 104, 106, 255), translation ? u8"Осталось 30 дней" : "30 days remaning");

                if (tabs == 0) {

                    ImGui::SetCursorPos(ImVec2(340, 100));

                    ImGui::BeginChild("##0", ImVec2(315, 263), true);

                    static bool check_0 = false;
                    ImGui::Checkbox(translation ? u8"главный выключатель" : "Master switch", &check_0);

                    static bool check_1 = false;
                    ImGui::Checkbox(translation ? u8"Тихая цель" : "Silent aim", &check_1);

                    static int slider_intager_0 = 90;
                    ImGui::SliderInt(translation ? u8"Поле зрения" : "Field of view", &slider_intager_0, 0, 180);

                    static bool check_2 = false;
                    ImGui::Checkbox(translation ? u8"преобразователь" : "Override resolver", &check_2);

                    static float color_edit4[4] = { 1.00f, 1.00f, 1.00f, 1.000f };
                    ImGui::ColorEdit4(translation ? u8"Фоновый цвет" : "Background Color", (float*)&color_edit4, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    static int key0, m0 = 1;
                    ImGui::Keybind(translation ? u8"Резольвер активен" : "Resolver active", &key0, &m0);

                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(340, 377));

                    ImGui::BeginChild("##1", ImVec2(315, 360), true);

                    static bool check_3 = false;
                    ImGui::Checkbox(translation ? u8"Авто стена" : "Autowall", &check_3);

                    static int slider_intager_1 = 50;
                    ImGui::SliderInt(translation ? u8"Шанс" : "Hitchance", &slider_intager_1, 0, 150);

                    static bool check_4 = false;
                    ImGui::Checkbox(translation ? u8"Шкала стат. точек" : "Static point scale", &check_4);

                    static int slider_intager_2 = 0.f;
                    ImGui::SliderInt(translation ? u8"Шкала головы" : "Head scale", &slider_intager_2, 0, 100);

                    static int selectedItem = 0;
                    const char* items_rus[] = { u8"Голова", u8"Шея", u8"Тело", u8"Ноги" };
                    const char* items_eng[] = { "Head", "Neck", "Body", "Legs" };

                    ImGui::Combo("D##7", &selectedItem, translation ? items_rus : items_eng, IM_ARRAYSIZE(translation ? items_rus : items_eng), 5);

                    static int slider_intager_3 = 0.f;
                    ImGui::SliderInt(translation ? u8"Масштаб тела" : "Body scale", &slider_intager_3, 0, 100);

                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(670, 100));

                    ImGui::BeginChild("##2", ImVec2(315, 420), true);

                    static int slider_intager_4 = 0;
                    ImGui::SliderInt(translation ? u8"Урон" : "Damage", &slider_intager_4, 0, 100);

                    static bool check_5 = false;
                    ImGui::Checkbox(translation ? u8"Авто. проникновение" : "Automatic penetration", &check_5);

                    static bool check_6 = false;
                    ImGui::Checkbox(translation ? u8"Сила прицел. тела" : "Force body aim", &check_6);

                    static bool check_7 = false;
                    ImGui::Checkbox(translation ? u8"безопасная точка" : "Force safe point", &check_7);

                    static bool multi_items_count[5];
                    const char* multi_items[5] = { translation ? u8"Один" : "One", translation ? u8"Два" : "Two", translation ? u8"Три" : "Three", translation ? u8"Четыре" : "Four", translation ? u8"Пять" : "Five" };
                    ImGui::MultiCombo("N##0", multi_items_count, multi_items, 5);

                    static int selectedItem2 = 0;
                    const char* items2_rus[] = { u8"Мульти-точки", u8"Одна-точка" };
                    const char* items2_eng[] = { "Multi-points", "One-points" };

                    ImGui::Combo("D##0", &selectedItem2, translation ? items2_rus : items2_eng, IM_ARRAYSIZE(translation ? items2_rus : items2_eng), 5);

                    static int selectedItem3 = 0;
                    const char* items3_rus[] = { u8"Условия автостопа" };
                    const char* items3_eng[] = { "Auto stop conditions" };

                    ImGui::Combo("D##1", &selectedItem3, translation ? items3_rus : items3_eng, IM_ARRAYSIZE(translation ? items3_rus : items3_eng), 5);

                    static int key1, m1 = 1;
                    ImGui::Keybind(translation ? u8"Привязка клавиш" : "Keybind", &key1, &m1);

                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(670, 533));

                    ImGui::BeginChild("##3", ImVec2(315, 205), true);

                    static bool check_8 = false;
                    ImGui::Checkbox(translation ? u8"Двойное нажатие" : "Double tap", &check_8);

                    static int key2, m2 = 1;
                    ImGui::Keybind(translation ? u8"Привязать кран" : "Bind tap", &key2, &m2);

                    static float slider_intager_5 = 0.5f;
                    ImGui::SliderFloat(translation ? u8"подделка" : "Exploit fake", &slider_intager_5, 0.f, 3.f, "%.1f");

                    ImGui::EndChild();

                } if (tabs == 1) {

                    ImGui::SetCursorPos(ImVec2(340, 100));

                    ImGui::BeginChild("##1-tabs02", ImVec2(645, 563), true);

                    ImGui::PushItemWidth(540);

                    static bool check_9 = false;
                    ImGui::Checkbox(translation ? u8"Тест Чекбокса" : "Test checkbox", & check_9);

                    static float slider_intager_7 = 0.5f;
                    ImGui::SliderFloat(translation ? u8"Тест интового слайдера" : "Test slider integer", & slider_intager_7, 0.f, 3.f, "%.1f");

                    static int key2, m2 = 1;
                    ImGui::Keybind(translation ? u8"Тест Кейбинда" : "Test Keybind", & key2, & m2);

                    static bool multi_items_count7[5];
                    const char* multi_items7[5] = { translation ? u8"Один" : "One", translation ? u8"Два" : "Two", translation ? u8"Три" : "Three", translation ? u8"Четыре" : "Four", translation ? u8"Пять" : "Five" };
                    ImGui::MultiCombo("N##0-tab1", multi_items_count7, multi_items7, 5);

                    static int selectedItem8 = 0;
                    const char* items3_rus7[] = { u8"Условия автостопа" };
                    const char* items3_eng7[] = { "Auto stop conditions" };

                    ImGui::Combo("D##1", &selectedItem8, translation ? items3_rus7 : items3_eng7, IM_ARRAYSIZE(translation ? items3_rus7 : items3_eng7), 5);

                    static float color_edit5[4] = { 1.00f, 1.00f, 1.00f, 1.000f };
                    ImGui::ColorEdit4(translation ? u8"Фоновый цвет" : "Background Color", (float*)&color_edit5, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::PopItemWidth();

                    ImGui::EndChild();
                }
            }
            ImGui::End();

        }
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
