#pragma once

#include <limits>
#include <vector>
#include "koatc/section/section_info.hxx"
#include "koatc/section/section_type.hxx"

namespace turenar::koatc::section {
class section_manager {
public:
	void process_beacon(double location, double distance, section_type type);
	void clear();
	void put_section(double start_location, section_type type);
	[[nodiscard]] section_info get(int section) const;

private:
	std::vector<section_info> _sections;
	double _last_beacon_location = std::numeric_limits<int>::min();
};
} // namespace turenar::koatc::section
