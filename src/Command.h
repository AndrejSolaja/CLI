#pragma once

#include "DataTypes.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

extern const std::unordered_map<std::string, std::function<void(const CommandNode&)>> command_map;