#pragma once
#include "framework.h"

namespace offsets {
	inline uintptr_t bInGame = 0x2ED6894;
	inline uintptr_t bAllowTraits = 0x2ECD598;

	namespace game {
		inline uintptr_t pInstance = 0x2ECD248;

		inline uintptr_t pCountry = 0x588;
	}


	namespace debug {
		inline uintptr_t pDebug = 0x2ECD230;

		inline uintptr_t pTDebug = 0x80;
	}


}