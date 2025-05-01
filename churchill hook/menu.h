#pragma once

#include "framework.h"



class Menu {
public:
	Menu(Config* config);
	void init();
	void show();

	bool IsOpen();
	void SetOpen(bool open);

	bool IsInitialized();
	void SetInitialized(bool init);
private:
	bool open, initialized;
	Config* config;
};