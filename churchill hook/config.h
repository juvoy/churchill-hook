#pragma once


class Config {
public:
    bool bCustomSteam = false;
    bool bCustomIngame = false;
    bool bAllowTraits = false;
    bool bTdebug = false;
    bool watermark = false;

    char steamName[256];
    char ingamename[256];

    float dpi = 1.f;

    Config() {
        steamName[0] = '\0';
        ingamename[0] = '\0';
    }
};