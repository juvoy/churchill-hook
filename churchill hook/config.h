#pragma once


class Config {
public:
	bool bCustomSteam = false, bCustomIngame = false, bAllowTraits = false, bTdebug = false;

	char steamName[256] = "", ingamename[256] = "";
private:
};