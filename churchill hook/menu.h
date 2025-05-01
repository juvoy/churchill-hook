#pragma once

#include "framework.h"



class Menu {
public:
	//Menu();
	void init();
	void show();

	bool IsOpen();
	void SetOpen(bool open);

	bool IsInitialized();
	void SetInitialized(bool init);
private:
	bool open, initialized;
};