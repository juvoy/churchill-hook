#pragma once
#include "framework.h"

namespace offsets {
	inline uintptr_t bInGame = 0x2EDFFB8;
	inline uintptr_t bAllowTraits = 0x2ED6BC8;
	inline uintptr_t fAddPlayerCommand = 0x170BB70;

	namespace game {
		inline uintptr_t pInstance = 0x2ED6878;

		inline uintptr_t pCountry = 0x588;
	}


	namespace debug {
		inline uintptr_t pDebug = 0x2ED6868;

		inline uintptr_t pTDebug = 0x80;
	}
}