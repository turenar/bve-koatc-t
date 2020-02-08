#pragma once

#include <limits>

namespace turenar::koatc::pattern {
constexpr double minimum_location = std::numeric_limits<int>::min();
constexpr double maximum_location = std::numeric_limits<int>::max();

constexpr double safety_pattern_offset = 15;
constexpr double default_deceleration = 2.5;
constexpr int no_pattern = 999;
} // namespace turenar::koatc::pattern
