/**
 * \file hwmanager.cpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief hardware managing class
 */

#include "hwmanager.hpp"

HWManager::HWManager()
:   _wiegand(_gpioManager)
{}

HWManager::~HWManager() {}

void HWManager::start()
{
    _gpioManager.startPolling();
}

void HWManager::stop()
{
    _gpioManager.stopPolling();
}
