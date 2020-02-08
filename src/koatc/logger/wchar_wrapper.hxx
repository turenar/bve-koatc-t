#pragma once

#include <spdlog/fmt/ostr.h>

namespace turenar::koatc::logger {
namespace internal {
	std::string convert_to_utf8(const std::wstring_view&);
}

struct wchar_wrapper {
	wchar_wrapper(std::wstring_view str) : str(str) {}

	const std::wstring_view str;
};

template <typename OStream>
OStream& operator<<(OStream& os, const wchar_wrapper& w) {
	return os << internal::convert_to_utf8(w.str);
}
} // namespace turenar::koatc::logger
