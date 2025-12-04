#pragma once

/**
 * @file Reporter.hpp
 * @class Reporter
 * @brief Declarations of Reporter class. Given level of details, returns raport
 * @author Bartosz Biegalski
 */

#include "core/GmlObject.h"

class GmlReporter
{
private:
    GmlReporter() = delete;

public:
    static void printRaport(const std::unique_ptr<GmlObject> &obj);
};
