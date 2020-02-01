#pragma once

#include <cassert>
#include "koatc/panel_id.hxx"

namespace turenar::koatc::wrapper {
class panels {
private:
	constexpr static int panel_count = 256;

public:
	panels(int* arr) : _arr(arr) {}

	void set(panel_id id, int value) {
		int num = static_cast<int>(id);
		assert(0 <= num && num < panel_count);
		_arr[num] = value;
	}

private:
	int* const _arr;
};
} // namespace turenar::koatc::wrapper
