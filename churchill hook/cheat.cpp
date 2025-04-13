#include "cheat.h"


Cheat::Cheat()
{
    this->_hooks = new Hooks();
    this->_menu = new Menu();
}


Cheat::~Cheat()
{
    delete this->_hooks;
    delete this->_menu;
}

bool Cheat::init()
{
    this->_menu = new Menu();

    if (!this->_hooks->init(this->_menu)) {
        std::cout << "Failed Hooking" << std::endl;
        return false;
    }

    return true;
}

Menu* Cheat::getMenu()
{
    return this->_menu;
}


