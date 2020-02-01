#pragma once

#include <cassert>
#include "ats/sound_control.hxx"
#include "koatc/sound_id.hxx"

namespace turenar::koatc::wrapper {
class sounds {
	constexpr static int sound_count = 256;

public:
	sounds(int* arr) : _arr(arr) {}

	void set(sound_id id, bve::ats::sound_control control) {
		int num = static_cast<int>(id);
		assert(0 <= num && num <= sound_count);
		_arr[num] = static_cast<int>(control);
	}

private:
	int* const _arr;
};
} // namespace turenar::koatc::wrapper
