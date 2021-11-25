#pragma once

#include <string>
#include <vector>

namespace kc::core {

void strip(std::string& str, const char characterToStrip = '\0');

std::vector<std::string> split(const std::string& str, const char delimiter);

std::string extractNameFromPath(const std::string& path);

}