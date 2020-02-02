#include "koatc/section/section_manager.hxx"

namespace turenar::koatc::section {
void section_manager::clear() {
	_sections.clear();
}
void section_manager::put_section(double start_location, section_type type) {
	_sections.emplace_back(section_info{start_location, type});
}
section_info section_manager::get(int section) const {
	if (section < 0) {
		return _sections.front();
	} else if (section >= _sections.size()) {
		return _sections.back();
	} else {
		return _sections[section];
	}
}
} // namespace turenar::koatc::section
