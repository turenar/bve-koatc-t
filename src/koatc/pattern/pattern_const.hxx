#pragma once

#include <limits>

namespace turenar::koatc::pattern {
constexpr double minimum_location = std::numeric_limits<int>::min();
constexpr double maximum_location = std::numeric_limits<int>::max();

constexpr double safety_pattern_offset = 15;
constexpr int no_pattern = 999;

constexpr int too_low_speed = 5;
} // namespace turenar::koatc::pattern
