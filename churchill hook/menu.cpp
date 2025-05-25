#include "framework.h"

Menu::Menu(Config* config)
{
	this->config = config;
}

void Menu::init()
{
	this->open = true;

	this->initialized = true;
}

void Menu::show()
{
	if (ImGui::Begin("###churchill_hook", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
		ImGui::SetWindowSize(ImVec2(500, 300));
		static uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA("hoi4.exe"));

		bool bInGame = *reinterpret_cast<bool*>(baseAddress + offsets::bInGame);
		static uintptr_t pInstance = baseAddress + offsets::game::pInstance;


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

			if (ImGui::Button("Network", ImVec2(ImGui::GetContentRegionAvail().x, 35))) {
				page = 2;
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


				ImGui::Text("Current ID: %i | Last ID: %i", countryId, iLastId);

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


				if (ImGui::Button(!config->bTdebug ? "Enable tdebug" : "Disable tdebug")) {
					if (bInGame) {
						static uintptr_t pDebugStuff = baseAddress + offsets::debug::pDebug;
						uintptr_t* test = reinterpret_cast<uintptr_t*>((*reinterpret_cast<uint64_t*>(pDebugStuff)) + offsets::debug::pTDebug);

						config->bTdebug = !config->bTdebug;

						*(int*)test = config->bTdebug;
					}
				}
			}
			else if (page == 1) {
				if (ImGui::Checkbox("Allow Traits", &config->bAllowTraits)) {
					if (bInGame) {
						uintptr_t pAllowTraits = baseAddress + offsets::bAllowTraits;

						*(int*)pAllowTraits = config->bAllowTraits;
					}
				}
			}
			else if (page == 2) {
				if (ImGui::BeginChild("###Networking_Steam", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2), ImGuiChildFlags_Borders)) {
					ImGui::Checkbox("Custom Steam Name", &config->bCustomSteam);

					ImGui::Text("Steam Name: ");
					ImGui::SameLine();
					ImGui::InputText("###SteamName", config->steamName, IM_ARRAYSIZE(config->steamName));

					ImGui::EndChild();
				}

				
				if (ImGui::BeginChild("###Networking_Paradox", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Borders)) {
					ImGui::Text("Custom Ingamename removed until I fix the crash :c");
					/*
					ImGui::Checkbox("Custom InGame Name", &config->bCustomIngame);


					ImGui::Text("Ingame Name: ");
					ImGui::SameLine();
					ImGui::InputText("###IngameName", config->ingamename, IM_ARRAYSIZE(config->ingamename));
					*/
					ImGui::EndChild();
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

bool Menu::IsInitialized()
{
	return this->initialized;
}

void Menu::SetInitialized(bool init)
{
	this->initialized = init;
}
