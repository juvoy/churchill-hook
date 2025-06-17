#include "framework.h"

#include <string.h>

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


typedef void(__fastcall* AddPlayerCommand)(void*);
AddPlayerCommand oAddPlayerCommand = nullptr;

void __fastcall hAddPlayer(void* pCAddPlayerCommand)
{
	Config* config = Cheat::GetInstance()->GetConfig();
	uint64_t* pSteamName = (uint64_t*)((uint8_t*)pCAddPlayerCommand + 0x30);
	uint64_t* pIngameName = (uint64_t*)((uint8_t*)((uint8_t*)pCAddPlayerCommand + 0x50) + 0x0);
	if (pSteamName && config->bCustomSteam)
	{
		memcpy(pSteamName, config->steamName, strlen(config->steamName) + 1);
	}

	
	/*if (pIngameName && config->bCustomIngame) {
		std::cout << pIngameName << std::endl;
	}*/

	oAddPlayerCommand(pCAddPlayerCommand);
}


inline ID3D11DeviceContext* pContext = NULL;
inline ID3D11RenderTargetView* mainRenderTargetView;
inline Hooks::Present oPresent;
inline HWND window = NULL;

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

			menu->init();
		}
		else 
			return oPresent(pSwapChain, SyncInterval, Flags);
		

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

	uintptr_t hMod = (uintptr_t)GetModuleHandleA("hoi4.exe");
	MH_DisableHook((void*)(hMod + offsets::fAddPlayerCommand));
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

	uintptr_t hMod = (uintptr_t)GetModuleHandleA("hoi4.exe");
	void* target = (void*)(hMod + offsets::fAddPlayerCommand);

	if (MH_CreateHook(target, &hAddPlayer, reinterpret_cast<void**>(&oAddPlayerCommand)) != MH_OK) {
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