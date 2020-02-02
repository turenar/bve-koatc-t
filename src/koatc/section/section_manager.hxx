#pragma once

#include <vector>
#include "koatc/section/section_info.hxx"
#include "koatc/section/section_type.hxx"

namespace turenar::koatc::section {
class section_manager {
public:
	void clear();
	void put_section(double start_location, section_type type);
	section_info get(int section) const;

private:
	std::vector<section_info> _sections;
};
} // namespace turenar::koatc::section
