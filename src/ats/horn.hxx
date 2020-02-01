#pragma once

#include "ats/atsplugin.hxx"

namespace bve::ats {
enum class horn : int {
	primary = ATS_HORN_PRIMARY,
	secondary = ATS_HORN_SECONDARY,
	music = ATS_HORN_MUSIC,
};
}
