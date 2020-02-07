#pragma once

#include <spdlog/fmt/ostr.h>
#include "koatc/pattern/pattern_const.hxx"

namespace turenar::koatc::pattern {
class curve_pattern {
public:
	explicit curve_pattern(double deceleration);

	void generate_pattern(double location, int speed, int bottom);
	void clear();

	[[nodiscard]] bool active(double location) const {
		return location < _flat_start_location;
	}
	[[nodiscard]] double limit_of(double location) const;
	[[nodiscard]] int bottom() const {
		return _bottom;
	}

private:
	/** km/1000 h^2 */
	double _deceleration;
	double _flat_start_location = minimum_location;
	double _zero_location = maximum_location;
	int _target = no_pattern;
	int _bottom = no_pattern;

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const curve_pattern& c) {
		if (c._flat_start_location <= minimum_location) {
			return os << "(inactive)";
		} else {
			return os << c._target << '@' << c._flat_start_location << " -> " << c._bottom << " -> 0@"
					  << c._zero_location;
		}
	}
};
} // namespace turenar::koatc::pattern
