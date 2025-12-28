#include "framework.h"

Menu::Menu(Config* config) : config(config), initialized(false), open(false), page(WORLD)
{
}


void Menu::init()
{
	this->open = true;
	this->initialized = true;
}


void Menu::show()
{
	float dpi = config->dpi;
	ImGui::SetNextWindowSize(ImVec2(500 * dpi, 300 * dpi));

	if (ImGui::Begin("churchillhook.cc", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize)) {
		static uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA("hoi4.exe"));
		bool bInGame = *reinterpret_cast<bool*>(baseAddress + offsets::bInGame);
		
		if (ImGui::BeginChild("Selection", ImVec2(ImGui::GetContentRegionAvail().x / 4, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Borders)) {
			if (ImGui::Button(page == 0 ? "> World" : "World", ImVec2(ImGui::GetContentRegionAvail().x, 35 * dpi))) {
				page = WORLD;
			}
			if (ImGui::Button(page == 1 ? "> Misc" : "Misc", ImVec2(ImGui::GetContentRegionAvail().x, 35 * dpi))) {
				page = MISC;
			}
			if (ImGui::Button(page == 2 ? "> Network" : "Network", ImVec2(ImGui::GetContentRegionAvail().x, 35 * dpi))) {
				page = NETWORK;
			}

#if _DEBUG
			if (ImGui::Button(page == DEBUG ? "> Debug" : "Debug", ImVec2(ImGui::GetContentRegionAvail().x, 35 * dpi))) {
				page = DEBUG;
			}
#endif

			ImGui::EndChild();
		}

		ImGui::SameLine();

		if (ImGui::BeginChild("Page", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_MenuBar)) {
			CGameState* state = reinterpret_cast<CGameState*>(*(uintptr_t*)(baseAddress + offsets::game::pCurrentGameState));

			if (ImGui::BeginMenuBar()) {
				ImGui::Text(page == WORLD ? "World" : page == MISC ? "Misc" : page == NETWORK ? "Network" : "Debug");
				ImGui::EndMenuBar();
			}

			switch (this->page) {
			case WORLD: {
				int countryId = 0;
				if (bInGame) {
					countryId = state->countryId;
				}

				static int iLastId;
				ImGui::Text("Current ID: %i | Last ID: %i", countryId, iLastId);

				static char newId[256] = {};
				ImGui::Text("New ID: ");
				ImGui::SameLine();

				ImGui::SetNextItemWidth(30 * dpi);

				ImGui::InputText("###NewID", newId, IM_ARRAYSIZE(newId));

				ImGui::SameLine();
				if (ImGui::Button("Set ID")) {
					if (bInGame) {
						iLastId = countryId;

						try {
							int idConverted = std::stoi(newId);
							state->countryId = idConverted;
						}
						catch (std::exception&) {
							MessageBoxA(nullptr, "Please only input numbers!", "churchill.cc", MB_ICONERROR | MB_OK);
						}
					}
				}


				if (ImGui::Button(!config->bTdebug ? "Enable tdebug" : "Disable tdebug")) {
					if (bInGame) {
						uintptr_t pDebugStuff = *(uintptr_t*)(baseAddress + offsets::debug::pDebug);
						uintptr_t debug = (uintptr_t)(pDebugStuff + 0x80);

						config->bTdebug = !config->bTdebug;

						*(char*)debug = config->bTdebug;
					}
				}
				break;
			}
			case MISC: {
				if (ImGui::BeginChild("##misc", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2), ImGuiChildFlags_Borders)) {
					if (ImGui::Checkbox("Allow Traits", &config->bAllowTraits)) {
						if (bInGame) {
							uintptr_t pAllowTraits = baseAddress + offsets::bAllowTraits;

							*(int*)pAllowTraits = config->bAllowTraits;
						}
					}

					static bool bTeleport = 0;
					if (ImGui::Checkbox("Teleport", &bTeleport)) {
						if (bInGame) {
							*(char*)(baseAddress + offsets::cTeleportFlag) = bTeleport;
						}
					}

					static bool bFOW = 0;
					if (ImGui::Checkbox("Disable FOW", &bFOW)) {
						if (bInGame) {
							*(char*)(baseAddress + offsets::cFOWFlag) = !bFOW;
						}
					}
				}
				ImGui::EndChild();

				if (ImGui::BeginChild("##misc_menu", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border)) {
					ImGui::SliderFloat("##DPI", &config->dpi, 1.f, 2.f, "DPI: %.1f");


					ImGui::EndChild();
				}
				break;
			}
			case NETWORK: {
				if (ImGui::BeginChild("###Networking_Steam", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2), ImGuiChildFlags_Borders)) {
					ImGui::Checkbox("Custom Steam Name", &config->bCustomSteam);

					ImGui::Text("Steam Name: ");
					ImGui::SameLine();
					ImGui::InputText("###SteamName", config->steamName, IM_ARRAYSIZE(config->steamName));


					ImGui::EndChild();
				}


				if (ImGui::BeginChild("###Networking_Paradox", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border)) {
					//ImGui::Text("Custom Ingamename removed until I fix the crash :c");

					ImGui::Checkbox("Custom InGame Name", &config->bCustomIngame);


					ImGui::Text("Ingame Name: ");
					ImGui::SameLine();
					ImGui::InputText("###IngameName", config->ingamename, IM_ARRAYSIZE(config->ingamename));

					ImGui::EndChild();
				}


				break;
			}
			case DEBUG: {
				ImGui::Text("Build on %s %s", __DATE__, __TIME__);

				break;
			}
			}

			ImGui::EndChild();
		}


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
