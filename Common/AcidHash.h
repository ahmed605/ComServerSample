#pragma once
#include <Windows.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <iostream>

namespace AcidHash
{
	namespace _AcidHash
	{
		#include "constexpr-hash/compile_time_md5.hpp"
	}

	template <typename T>
	inline constexpr GUID clsid_hash_of() noexcept
	{
		constexpr const wchar_t* const name = winrt::name_of<T>().data();
		constexpr auto hash = _AcidHash::MD5(name);
		constexpr auto clsid = std::bit_cast<GUID>(hash);

		return clsid;
	}

	inline GUID clsid_hash_of(const wchar_t* name) noexcept
	{
		auto hash = _AcidHash::MD5(name);
		return std::bit_cast<GUID>(hash);
	}
}