#pragma once

namespace turenar::koatc {
enum class panel_id : int {
	atc_emergency_braking = 22,
	atc_normal_braking = 23,

	atc_signal_forbidden = 101,
	atc_signal_red = 131,
	atc_signal_green = 132,

	atc_speed_min_0 = 102,
	atc_speed_max_110 = 124,
	atc_speed_120 = 125,

	operation_type = 219,

	atc_next_stop = 220,
	atc_operation_type = 230,
};
}
