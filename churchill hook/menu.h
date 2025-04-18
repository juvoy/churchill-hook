#pragma once

#include "framework.h"

#include "offsets.h"

class Menu {
public:
	//Menu();
	void init();
	void show();

	bool IsOpen();
	void SetOpen(bool open);
private:
	bool open;
};