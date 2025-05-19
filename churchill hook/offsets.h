#pragma once
#include "framework.h"

namespace offsets {
	inline uintptr_t bInGame = 0x2EDBCF4;
	inline uintptr_t bAllowTraits = 0x2ED29F8;
	inline uintptr_t fAddPlayerCommand = 0x1714250;

	namespace game {
		inline uintptr_t pInstance = 0x2ED26A8;

		inline uintptr_t pCountry = 0x588;
	}


	namespace debug {
		inline uintptr_t pDebug = 0x2ED2690;

		inline uintptr_t pTDebug = 0x80;
	}


}