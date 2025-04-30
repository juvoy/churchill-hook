#pragma once

#include "menu.h"

#include <d3d11.h>
#include <kiero/kiero.h>

#include <minhook/MinHook.h>


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class Hooks {
public:
	~Hooks();

	bool init(Menu* menu);
private:
};
