// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"

#include "cheat.h"


VOID CheatEntry(HMODULE hModule) {
#if _DEBUG
    AllocConsole();

    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

#endif // _DEBUG

    Cheat* cheat = new Cheat();
    if (!cheat->init()) {
        std::cout << "Failed..." << std::endl;
        return;
    }

    while (true) {
        if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
            Beep(200, 200);
            break;
        }
    }

    kiero::unbind(8);
    delete cheat;

#if _DEBUG
    FreeConsole();
#endif // _DEBUG

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)CheatEntry, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        kiero::shutdown();
        break;
    }
    return TRUE;
}

