#pragma once

#include <filesystem>
#include <spdlog/fmt/ostr.h>

namespace turenar::koatc {
class configuration {
public:
	configuration();
	explicit configuration(const std::filesystem::path& file);

	[[nodiscard]] double deceleration() const {
		return _deceleration;
	}
	[[nodiscard]] double deceleration_emergency() const {
		return _deceleration_emergency;
	}
	[[nodiscard]] int pattern_offset() const {
		return _pattern_offset;
	}
	[[nodiscard]] int pattern_offset_full() const {
		return _pattern_offset_full;
	}
	[[nodiscard]] int pattern_offset_emergency() const {
		return _pattern_offset_emergency;
	}

	[[nodiscard]] int pattern_approaching() const {
		return _pattern_approaching;
	}
	[[nodiscard]] int bell_threshold() const {
		return _bell_threshold;
	}
	[[nodiscard]] int section_margin() const {
		return _section_margin;
	}
	[[nodiscard]] int emergency_time() const {
		return _emergency_time;
	}

private:
	double _deceleration = 2.5;
	double _deceleration_emergency = 2.5;
	int _pattern_offset = 1;
	int _pattern_offset_full = 5;
	int _pattern_offset_emergency = 10;

	int _pattern_approaching = 10;
	int _bell_threshold = 2;
	int _section_margin = 20;
	int _emergency_time = 4000;
};

template <typename OStream>
OStream& operator<<(OStream& os, const configuration& c) {
	return os << "configuration{pattern_offset=[" << c.pattern_offset() << ", " << c.pattern_offset_full() << ", "
			  << c.pattern_offset_emergency() << "], deceleration=[" << c.deceleration() << ", "
			  << c.deceleration_emergency() << "], pattern_approaching=" << c.pattern_approaching()
			  << ", bell_threshold=" << c.bell_threshold() << ", section_margin=" << c.section_margin() << "}";
}
} // namespace turenar::koatc
