#pragma once

#include <spdlog/fmt/ostr.h>
#include "koatc/pattern/pattern_const.hxx"

namespace turenar::koatc::pattern {
class flat_pattern {
public:
	explicit flat_pattern() {}

	void generate_pattern(int bottom) {
		_bottom = bottom;
	}
	void clear() {
		_bottom = no_pattern;
	}

	explicit operator bool() const {
		return _bottom != no_pattern;
	}
	[[nodiscard]] double limit_of(double /*location*/) const {
		return _bottom;
	}
	[[nodiscard]] int bottom() const {
		return _bottom;
	}

private:
	int _bottom;

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const flat_pattern& c) {
		return os << c._bottom;
	}
};
} // namespace turenar::koatc::pattern
