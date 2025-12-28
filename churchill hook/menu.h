#pragma once

#include "framework.h"

#include <string.h>

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

	enum Page {
		WORLD,
		MISC,
		NETWORK,
		DEBUG,
	} page;

};