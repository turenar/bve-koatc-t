#pragma once

#include "ats/atsplugin.hxx"
#include "ats/constant_speed_control.hxx"
#include "ats/notch_position.hxx"
#include "ats/reverser_position.hxx"

namespace bve::ats {
struct handles {
	notch_position brake; // Brake Notch
	notch_position power; // Power Notch
	reverser_position reverser; // Reverser Position
	constant_speed_control constant_speed; // Constant Speed Control

	handles() = default;
	handles(notch_position brake,
			notch_position power,
			reverser_position reverser,
			constant_speed_control constant_speed)
			: brake(brake), power(power), reverser(reverser), constant_speed(constant_speed) {}
	handles(const ATS_HANDLES& h) // NOLINT(google-explicit-constructor)
			: brake(h.Brake), power(h.Power), reverser(static_cast<reverser_position>(h.Reverser)),
			  constant_speed(static_cast<constant_speed_control>(h.ConstantSpeed)) {}
	operator ATS_HANDLES() const { // NOLINT(google-explicit-constructor)
		ATS_HANDLES h = {};
		h.Brake = static_cast<int>(brake);
		h.Power = static_cast<int>(power);
		h.Reverser = static_cast<int>(reverser);
		h.ConstantSpeed = static_cast<int>(constant_speed);
		return h;
	}
};
} // namespace bve::ats
