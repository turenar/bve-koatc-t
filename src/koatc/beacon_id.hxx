#pragma once

namespace turenar::koatc {
enum class beacon_id {
	section = 170,

	speed_limit_1 = 176,
	speed_limit_2 = 177,
	speed_limit_3 = 178,
	speed_limit_4 = 179,

	train_number_and_current_station = 180,

	approach_station = 181,
	control_stop = 182,
	control_stop_emergency = 183,
	control_stop_to_25 = 184,
	control_stop_change_index = 185,
	notify_stop = 186,
	arrive_stop = 187,
};
} // namespace turenar::koatc
