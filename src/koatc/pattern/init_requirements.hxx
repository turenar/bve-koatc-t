#pragma once

namespace bve::ats {
struct vehicle_state;
}
namespace turenar::koatc {
class configuration;
namespace section {
	class section_manager;
}
namespace signal {
	class signal_manager;
}
namespace station {
	class station_manager;
}
} // namespace turenar::koatc

namespace turenar::koatc::pattern {
struct init_requirements {
	const configuration& config;
	const bve::ats::vehicle_state& state;
	const section::section_manager& section;
	const signal::signal_manager& signal;
	const station::station_manager& station;
};
} // namespace turenar::koatc::pattern
