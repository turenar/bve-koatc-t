#pragma once

#include <type_traits>
#include <spdlog/fmt/ostr.h>

namespace turenar::koatc::pattern {
class handle_command {
public:
	constexpr static handle_command neutral() {
		return {0};
	};
	constexpr static handle_command half_brake() {
		return {1};
	}
	constexpr static handle_command full_brake() {
		return {2};
	}
	constexpr static handle_command emergency() {
		return {3};
	}

	constexpr explicit operator bool() const {
		return _command != 0;
	}

	constexpr bool operator<(handle_command another) const {
		return _command < another._command;
	}
	constexpr bool operator==(handle_command another) const {
		return _command == another._command;
	}
	void promote(handle_command rhs) {
		if (_command < rhs._command) {
			_command = rhs._command;
		}
	}

private:
	int _command;

	constexpr handle_command(int command) : _command(command) { // NOLINT(google-explicit-constructor)
	}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const handle_command& c) {
		if (c == neutral()) {
			os << "neut";
		} else if (c == half_brake()) {
			os << "half";
		} else if (c == full_brake()) {
			os << "full";
		} else if (c == emergency()) {
			os << "emrg";
		}
		return os;
	}
};

constexpr bool operator!=(handle_command lhs, handle_command rhs) {
	return !(lhs == rhs);
}
} // namespace turenar::koatc::pattern
