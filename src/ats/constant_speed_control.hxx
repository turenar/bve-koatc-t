#pragma once

#include "ats/atsplugin.hxx"

namespace bve::ats {
enum class constant_speed_control : int {
	keep_previous = ATS_CONSTANTSPEED_CONTINUE,
	enable = ATS_CONSTANTSPEED_ENABLE,
	disable = ATS_CONSTANTSPEED_DISABLE,
};
}
