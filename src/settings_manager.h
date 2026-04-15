#pragma once
#include "settings.h"

class SettingsManager {
public:
    bool Load(Settings& out);
    bool Save(const Settings& in);
};