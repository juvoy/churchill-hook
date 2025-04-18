#include "menu.h"


void Menu::init()
{
	this->open = true;
}

void Menu::show()
{
	
	if (ImGui::Begin("###churchill_hook", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
		ImGui::SetWindowSize(ImVec2(500, 300));
		static uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA("hoi4.exe"));

		bool bInGame = *reinterpret_cast<bool*>(baseAddress + offsets::bInGame);

		static uintptr_t pInstance = baseAddress + offsets::game::pInstance;

		/*static std::string version;
		uintptr_t pInstance2 = baseAddress + 0x02EA77B0;
		uint64_t* pFirst = reinterpret_cast<uint64_t*>(pInstance2);
		if (pFirst != nullptr) {
			std::cout << "0x" << std::hex << pFirst << std::dec << std::endl;
		}
		pFirst = reinterpret_cast<uint64_t*>((*reinterpret_cast<uint64_t*>(pFirst)) + 0x130); // CPdxNewDeleteAllocator
		if (pFirst != nullptr) {
			std::cout << "0x" << std::hex << pFirst << std::dec << std::endl;
		}
		pFirst = reinterpret_cast<uint64_t*>((*reinterpret_cast<uint64_t*>(pFirst)) + 0x38); // I Guess Steam
		if (pFirst != nullptr) {
			std::cout << "0x" << std::hex << pFirst << std::dec << std::endl;
		}
		pFirst = reinterpret_cast<uint64_t*>((*reinterpret_cast<uint64_t*>(pFirst)) + 0x188); // IDK
		if (pFirst != nullptr) {
			std::cout << "0x" << std::hex << pFirst << std::dec << std::endl;
		}
		pFirst = reinterpret_cast<uint64_t*>((*reinterpret_cast<uint64_t*>(pFirst)) + 0x0);
		if (pFirst != nullptr) {
			std::cout << "0x" << std::hex << pFirst << std::dec << std::endl;

			version = reinterpret_cast<char*>(pFirst);
			std::cout << version << std::endl;
		}*/


		//ImGui::PushFont(SemiBoldItalic);
		ImGui::Text("churchillhook.cc" /*(%s)", version.c_str()*/);

		ImGui::Separator();

		//ImGui::PopFont();
		//ImGui::PushFont(SemiBold);
		static int page = 0;
		if (ImGui::BeginChild("Selection", ImVec2(ImGui::GetContentRegionAvail().x / 4, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Borders)) {
			if (ImGui::Button("World", ImVec2(ImGui::GetContentRegionAvail().x, 35))) {
				page = 0;
			}

			if (ImGui::Button("Misc", ImVec2(ImGui::GetContentRegionAvail().x, 35))) {
				page = 1;
			}
			ImGui::EndChild();
		}

		ImGui::SameLine();
		//ImGui::PopFont();
		//ImGui::PushFont(Normal);

		if (ImGui::BeginChild("Page", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Borders)) {
			uint64_t pCountryId = *reinterpret_cast<uint64_t*>(pInstance) + offsets::game::pCountry;
			if (page == 0) {
				int countryId = 0;
				if (bInGame) {
					countryId = *(int*)pCountryId;
				}

				static int iLastId;


				ImGui::Text("Current ID: %i", countryId);
				ImGui::SameLine();
				ImGui::Text(" | Last ID: %i", iLastId);


				static char newId[256] = {};
				ImGui::Text("New ID: ");
				ImGui::SameLine();

				ImGui::SetNextItemWidth(30);
				ImGui::InputText("###NewID", newId, IM_ARRAYSIZE(newId));

				ImGui::SameLine();
				if (ImGui::Button("Set ID")) {
					if (bInGame) {
						iLastId = countryId;

						*(int*)pCountryId = std::stoi(newId);
					}
				}

				static bool bTdebug = 0;
				if (ImGui::Button(bTdebug == 0 ? "Enable tdebug" : "Disable tdebug")) {
					if (bInGame) {
						static uintptr_t pDebugStuff = baseAddress + offsets::debug::pDebug;
						uintptr_t* test = reinterpret_cast<uintptr_t*>((*reinterpret_cast<uint64_t*>(pDebugStuff)) + offsets::debug::pTDebug);

						bTdebug = !bTdebug;

						*(int*)test = bTdebug;
					}
				}
			}
			else if (page == 1) {
				static bool allowTraits = 0;


				if (ImGui::Checkbox("Allow Traits", &allowTraits)) {
					if (bInGame) {
						uintptr_t pAllowTraits = baseAddress + offsets::bAllowTraits;

						*(int*)pAllowTraits = allowTraits;
					}
				}
			}

			ImGui::EndChild();
		}
		//ImGui::PopFont();


		ImGui::End();
	}
}

bool Menu::IsOpen()
{
	return this->open;
}

void Menu::SetOpen(bool open)
{
	this->open = open;
}
