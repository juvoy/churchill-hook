#pragma once

class Cheat {
public:
	Cheat();
	~Cheat();

	bool init();

	Menu* getMenu();
private:


	Hooks* _hooks;
	Menu* _menu;
};

static Cheat* cheat = nullptr;