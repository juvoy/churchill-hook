#pragma once

#include "framework.h"

class CGameState
{
public:
	char pad_0000[168]; //0x0000
	void* pCHuman; //0x00A8
	char pad_00B0[912]; //0x00B0
	void* CGameRulesInstance; //0x0440
	char pad_0448[216]; //0x0448
	uint32_t countryId; //0x0520
};

namespace offsets {
	constexpr uintptr_t bInGame = 0x325B5A4;
	constexpr uintptr_t bAllowTraits = 0x324F448;
	constexpr uintptr_t cTeleportFlag = 0x324F44B; // 0F 94 05 0F 74 FC 02
	constexpr uintptr_t cFOWFlag = 0x324F46A;

	namespace game {
		constexpr uintptr_t pCurrentGameState = 0x324F0A8;

		constexpr uintptr_t pCountry = 0x520;
	}


	namespace debug {
		constexpr uintptr_t pDebug = 0x325C268;

		constexpr uintptr_t pTDebug = 0x80;
	}
}