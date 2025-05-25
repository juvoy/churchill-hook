#pragma once


class Config {
public:
    bool bCustomSteam = false;
    bool bCustomIngame = false;
    bool bAllowTraits = false;
    bool bTdebug = false;

    char steamName[256];
    char ingamename[256];

    Config() {
        steamName[0] = '\0';
        ingamename[0] = '\0';
    }
};