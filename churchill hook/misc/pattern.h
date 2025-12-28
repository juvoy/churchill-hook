#pragma once

#include <vector>
#include <cstddef>
#include <Windows.h>
#include <Psapi.h>

namespace pattern {
	std::vector<int> PatternToBytes(const char* pattern) {
		std::vector<int> bytes;
		char* start = const_cast<char*>(pattern);
		char* end = const_cast<char*>(pattern) + strlen(pattern);

		for (char* current = start; current < end; ++current) {
			if (*current == '?') {
				bytes.push_back(-1);

				if (current + 1 < end && *(current + 1) == '?') {
					current++;
				}
			}
			else if (isxdigit(*current)) {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	}

	uint8_t* Scan(void* moduleBase, const char* signature) {
		MODULEINFO moduleInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleBase, &moduleInfo, sizeof(MODULEINFO));

		uint8_t* startAddress = static_cast<uint8_t*>(moduleBase);
		size_t size = moduleInfo.SizeOfImage;

		std::vector<int> patternBytes = PatternToBytes(signature);
		size_t patternLength = patternBytes.size();
		int* patternData = patternBytes.data();

		for (size_t i = 0; i <= size - patternLength; ++i) {
			bool found = true;

			for (size_t j = 0; j < patternLength; ++j) {
				if (patternData[j] != -1 && startAddress[i + j] != patternData[j]) {
					found = false;
					break;
				}
			}

			if (found) {
				return &startAddress[i];
			}
		}

		return nullptr;
	}
}