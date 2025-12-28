#include "framework.h"

#include <string.h>

#include "misc/pattern.h"

inline WNDPROC oWndProc;
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Menu* menu = Cheat::GetInstance()->GetMenu();
	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
		menu->SetOpen(!menu->IsOpen());

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	if (menu->IsOpen())
	{
		switch (uMsg) { // https://learn.microsoft.com/en-us/windows/win32/inputdev/mouse-input-notifications
		case WM_CAPTURECHANGED:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEACTIVATE:
		case WM_MOUSEHOVER:
		case WM_MOUSEHWHEEL:
		case WM_MOUSELEAVE:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_NCHITTEST:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMOUSEHOVER:
		case WM_NCMOUSELEAVE:
		case WM_NCMOUSEMOVE:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
		case WM_NCXBUTTONDBLCLK:
		case WM_NCXBUTTONDOWN:
		case WM_NCXBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			return true;
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

typedef char(__fastcall* AddPlayerCommandFn)(__int64 a1, __int64 a2);
AddPlayerCommandFn oAddPlayerCustom = nullptr;



char hAddPlayerCustom(__int64 a1, __int64 a2)
{
	Config* config = Cheat::GetInstance()->GetConfig();
	uint64_t* pSteamName = (uint64_t*)((uint8_t*)a1 + 0x28);
	uintptr_t pIngameName = (uintptr_t)a1 + 0x48;
	if (pSteamName && config->bCustomSteam)
	{
		memcpy(pSteamName, config->steamName, strlen(config->steamName) + 1);
	}

	constexpr size_t MAX_NAME_LEN = 32;
	if (pIngameName && config->bCustomIngame) {
		char* ingameNamePtr = *(char**)(pIngameName);
		if (ingameNamePtr) {
			strncpy_s(ingameNamePtr, 32, config->ingamename, 32 - 1);

			ingameNamePtr[MAX_NAME_LEN - 1] = '\0';
		}
	}

	return oAddPlayerCustom(a1, a2);
}

inline ID3D11DeviceContext* pContext = NULL;
inline ID3D11RenderTargetView* mainRenderTargetView;
inline Hooks::Present oPresent;
inline HWND window = NULL;


static float dpi = 1.f;
HRESULT __stdcall hPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	Menu* menu = Cheat::GetInstance()->GetMenu();

	if (!menu->IsInitialized())
	{
		ID3D11Device* pDevice = NULL;
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;

			pSwapChain->GetDesc(&sd);

			window = sd.OutputWindow;

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);

			pBackBuffer->Release();

			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(pDevice, pContext);

			ImGui::StyleColorsDark();

			menu->init();
		}
		else 
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (dpi != Cheat::GetInstance()->GetConfig()->dpi)
	{
		dpi = Cheat::GetInstance()->GetConfig()->dpi;

		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->Clear(); 

		ImFontConfig cfg;
		cfg.SizePixels = 13.0f * dpi;
		cfg.OversampleH = 1;
		cfg.OversampleV = 1;
		cfg.PixelSnapH = true;

		io.Fonts->AddFontDefault(&cfg);
		io.Fonts->Build();

		ImGui_ImplDX11_InvalidateDeviceObjects();
		ImGui_ImplDX11_CreateDeviceObjects();

		ImGuiStyle& style = ImGui::GetStyle();
		style = ImGuiStyle();
		style.ScaleAllSizes(dpi);
	}



	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();



	ImGui::NewFrame();

	if (menu->IsOpen()) {
		menu->show();
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

Hooks::~Hooks()
{
	oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	MH_DisableHook(MH_ALL_HOOKS);
}


bool Hooks::init(Menu* menu)
{
	bool init = false;
	while (!init) {
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			if (kiero::bind(8, (void**)&oPresent, hPresent) == kiero::Status::Success) {
				init = true;
			}
		}
	}
	// 00 00 00 00 00 F0 53 9C F7 7F 00 00 D0 E9 62 46
	void* target = pattern::Scan(GetModuleHandleA("hoi4.exe"), "48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 48 8D 68 A1 48 81 EC E0 00 00 00");


	if (MH_CreateHook(target, &hAddPlayerCustom, reinterpret_cast<void**>(&oAddPlayerCustom)) != MH_OK) {
		std::cout << "Failed hook AddPlayerCommand" << std::endl;
		return 0;
	}

	if (MH_EnableHook(target) != MH_OK) {
		std::cout << "Failed enable all hooks" << std::endl;
		return 0;
	}

	std::cout << "All hooks were initialized successfully" << std::endl;

    return init;
}