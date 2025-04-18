#pragma once
#include "framework.h"

namespace offsets {
	inline uintptr_t bInGame = 0x2EAF564;
	inline uintptr_t bAllowTraits = 0x2EA6288;

	namespace game {
		inline uintptr_t pInstance = 0x2EA5F38;

		inline uintptr_t pCountry = 0x588;
	}


	namespace debug {
		inline uintptr_t pDebug = 0x2EA5F20;

		inline uintptr_t pTDebug = 0x80;
	}


}