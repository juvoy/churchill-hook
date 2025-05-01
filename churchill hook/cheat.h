#pragma once

class Cheat {
public:
	Cheat();
	~Cheat();

	bool init();

	Menu* GetMenu();
	Config* GetConfig();

	static Cheat* GetInstance();
private:
	Hooks* _hooks;
	Menu* _menu;
	Config* _config;

	static Cheat* instance;
};