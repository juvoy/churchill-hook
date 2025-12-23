#pragma once

#include "framework.h"

namespace offsets {
	constexpr uintptr_t bInGame = 0x325B5A4;
	constexpr uintptr_t bAllowTraits = 0x324F448;
	constexpr uintptr_t fAddPlayerCommand = 0x1900B80;

	namespace game {
		constexpr uintptr_t pCurrentGameState = 0x324F0A8;

		constexpr uintptr_t pCountry = 0x520;
	}


	namespace debug {
		constexpr uintptr_t pDebug = 0x325C268;

		constexpr uintptr_t pTDebug = 0x80;
	}
}