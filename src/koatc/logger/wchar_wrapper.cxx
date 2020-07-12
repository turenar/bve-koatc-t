#include <windows.h>
#include "koatc/logger/wchar_wrapper.hxx"

namespace turenar::koatc::logger::internal {
std::string convert_to_utf8(const std::wstring_view& s) {
	int length = ::WideCharToMultiByte(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr, 0, nullptr, nullptr);
	std::string ret;
	ret.resize(length);
	length = ::WideCharToMultiByte(
			CP_UTF8,
			0,
			s.data(),
			static_cast<int>(s.size()),
			ret.data(),
			static_cast<int>(ret.size()),
			nullptr,
			nullptr);
	ret.resize(length);
	return ret;
}
} // namespace turenar::koatc::logger::internal
