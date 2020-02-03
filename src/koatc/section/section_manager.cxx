#include <spdlog/spdlog.h>
#include "koatc/section/section_manager.hxx"

namespace turenar::koatc::section {
void section_manager::process_beacon(double location, double distance, section_type type) {
	if (_last_beacon_location < location || location < _last_beacon_location) {
		// _last_beacon_location != location cause warning with -Wfloat-equal
		clear();
		_last_beacon_location = location;
	}
	double section_location = location + distance;
	if (!_sections.empty() && section_location < _sections.back().start_location) {
		spdlog::warn("new_section < most_distant_section. location jumped?");
		clear();
	}
	put_section(section_location, type);
}
void section_manager::clear() {
	_sections.clear();
}
void section_manager::put_section(double start_location, section_type type) {
	_sections.emplace_back(section_info{start_location, type});
}
section_info section_manager::get(int section) const {
	if (section < 0) {
		return _sections.front();
	} else if (static_cast<std::size_t>(section) >= _sections.size()) {
		return _sections.back();
	} else {
		return _sections[section];
	}
}
} // namespace turenar::koatc::section
