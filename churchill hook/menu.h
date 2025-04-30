#pragma once
/*extern bool bCustomSteam = false;

extern char steamName[256] = "";*/
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