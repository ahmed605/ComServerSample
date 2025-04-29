#pragma once
#include <winrt/ComServerSample.h>

namespace EnumHelper
{
	inline constexpr const wchar_t* to_string(winrt::ComServerSample::ProcessIntegrityLevel e)
	{
		switch (e)
		{
		case winrt::ComServerSample::ProcessIntegrityLevel::Low:
			return L"Low IL";
		case winrt::ComServerSample::ProcessIntegrityLevel::Medium:
			return L"Medium IL";
		case winrt::ComServerSample::ProcessIntegrityLevel::High:
			return L"High IL";
		default:
			return L"Unknown IL";
		}
	}
}