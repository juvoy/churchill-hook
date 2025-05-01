#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <string>
#include <cstdint>
#include <iostream>

#include <d3d11.h>

#include <kiero/kiero.h>

#include <minhook/MinHook.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>


#include "config.h"

#include "offsets.h"

#include "menu.h"
#include "hooks.h"

#include "cheat.h"