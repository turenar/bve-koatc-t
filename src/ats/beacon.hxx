#pragma once

#include "atsplugin.hxx"
namespace bve::ats {
struct beacon {
	int type; // Type of Beacon
	int signal; // Signal of Connected Section
	float distance; // Distance to Connected Section (m)
	int optional; // Optional Data

	beacon() = default;
	beacon(const ATS_BEACONDATA& b)// NOLINT(google-explicit-constructor)
			: type(b.Type), signal(b.Signal), distance(b.Distance), optional(b.Optional) {
	}
	operator ATS_BEACONDATA() const { // NOLINT(google-explicit-constructor)
		ATS_BEACONDATA b{};
		b.Type = type;
		b.Signal = signal;
		b.Distance = distance;
		b.Optional = optional;
		return b;
	}
};
} // namespace bve::ats
