#pragma once
#include <Windows.h>
#include <winrt/ComServerSample.h>

namespace IntegrityHelper
{
	inline winrt::ComServerSample::ProcessIntegrityLevel GetProcessIntegrityLevel()
	{
		auto ret = winrt::ComServerSample::ProcessIntegrityLevel::Medium;

		HANDLE token = nullptr;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
		{
			DWORD dwSize = 0;
			TOKEN_MANDATORY_LABEL* integrityLevel = (TOKEN_MANDATORY_LABEL*)alloca(TOKEN_INTEGRITY_LEVEL_MAX_SIZE);
			if (GetTokenInformation(token, TokenIntegrityLevel, integrityLevel, TOKEN_INTEGRITY_LEVEL_MAX_SIZE, &dwSize))
			{
				auto sid = integrityLevel->Label.Sid;
				auto level = *GetSidSubAuthority(sid, *GetSidSubAuthorityCount(sid) - 1);

				if (level <= SECURITY_MANDATORY_LOW_RID)
					ret = winrt::ComServerSample::ProcessIntegrityLevel::Low;
				else if (level >= SECURITY_MANDATORY_HIGH_RID)
					ret = winrt::ComServerSample::ProcessIntegrityLevel::High;
			}
		}

		CloseHandle(token);
		return ret;
	}
}