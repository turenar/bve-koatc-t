#pragma once

namespace bve::ats {
class notch_position {
public:
	notch_position() = default;
	explicit constexpr notch_position(int position) : _position(position) {}
	explicit constexpr operator int() const {
		return _position;
	}
	explicit constexpr operator bool() const {
		return _position != 0;
	}

	[[nodiscard]] constexpr notch_position half() const {
		return notch_position{(_position + 1) / 2};
	}

private:
	int _position;
};

constexpr bool operator<(notch_position lhs, notch_position rhs) {
	return static_cast<int>(lhs) < static_cast<int>(rhs);
}
constexpr bool operator>(notch_position lhs, notch_position rhs) {
	return rhs < lhs;
}
constexpr bool operator<=(notch_position lhs, notch_position rhs) {
	return !(lhs > rhs);
}
constexpr bool operator>=(notch_position lhs, notch_position rhs) {
	return !(lhs < rhs);
}
constexpr bool operator==(notch_position lhs, notch_position rhs) {
	return static_cast<int>(lhs) == static_cast<int>(rhs);
}
constexpr bool operator!=(notch_position lhs, notch_position rhs) {
	return !(lhs == rhs);
}
} // namespace bve::ats
