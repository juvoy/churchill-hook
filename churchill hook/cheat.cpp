#include "framework.h"

Cheat* Cheat::instance = nullptr;

Cheat::Cheat()
{
    this->_hooks = new Hooks();
    this->_config = new Config();

    this->_menu = new Menu(this->_config);

    Cheat::instance = this;
}


Cheat::~Cheat()
{
    delete this->_hooks;
    delete this->_menu;
    delete this->_config;
}

bool Cheat::init()
{
    if (FindWindowA(nullptr, "Hearts of Iron IV (DirectX 11)") == nullptr) { // quickfix
        return false;
    }

    if (!this->_hooks->init(this->_menu)) {
        std::cout << "Failed Hooking" << std::endl;
        return false;
    }

    return true;
}

Menu* Cheat::GetMenu()
{
    return this->_menu;
}

Config* Cheat::GetConfig()
{
    return this->_config;
}

Cheat* Cheat::GetInstance()
{
    return Cheat::instance;
}


