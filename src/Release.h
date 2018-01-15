#pragma once

#include <string>

#define VERSION_MAJOR 0
#define VERSION_MINOR 5
#define VERSION_PATCH 185

/**
 * @brief This structure holds some basic release and version information
 */
struct Release {
    std::string version =
            "v" + std::to_string(VERSION_MAJOR) +
            "." + std::to_string(VERSION_MINOR) +
            "." + std::to_string(VERSION_PATCH);


    std::string vendor = "Lindenberg Research Tec.";
    std::string title = "LRT Rack modules";
};

