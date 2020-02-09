#pragma once

#include <spdlog/fmt/ostr.h>

namespace turenar::koatc {
enum class beacon_id {
	section = 170,

	overrun_protection_step1 = 172,
	overrun_protection_step2 = 173,

	speed_limit_1 = 176,
	speed_limit_2 = 177,
	speed_limit_3 = 178,
	speed_limit_4 = 179,

	train_number_and_current_stop = 180,

	approach_station = 181,
	control_stop = 182,
	control_stop_emergency = 183,
	control_stop_to_25 = 184,
	control_stop_change_index = 185,
	notify_stop = 186,
	arrive_stop = 187,
};

constexpr inline const char* to_string(beacon_id b) {
	switch (b) {
	case beacon_id::section:
		return "section";
	case beacon_id::overrun_protection_step1:
		return "orp1";
	case beacon_id::overrun_protection_step2:
		return "orp2";
	case beacon_id::speed_limit_1:
		return "speed1";
	case beacon_id::speed_limit_2:
		return "speed2";
	case beacon_id::speed_limit_3:
		return "speed3";
	case beacon_id::speed_limit_4:
		return "speed4";
	case beacon_id::train_number_and_current_stop:
		return "train_number";
	case beacon_id::approach_station:
		return "approach_sta";
	case beacon_id::control_stop:
		return "ctrl_stop";
	case beacon_id::control_stop_emergency:
		return "ctrl_stop_orp2";
	case beacon_id::control_stop_to_25:
		return "ctrl_stop_orp1";
	case beacon_id::control_stop_change_index:
		return "ctrl_stop_chidx";
	case beacon_id::notify_stop:
		return "notify_stop";
	case beacon_id::arrive_stop:
		return "arrive_stop";
	default:
		return "<unknown>";
	}
}

template <typename OStream>
inline OStream& operator<<(OStream& os, beacon_id b) {
	return os << to_string(b);
}
} // namespace turenar::koatc
