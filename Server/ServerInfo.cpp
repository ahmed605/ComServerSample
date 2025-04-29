#include "ServerInfo.h"
#include "IntegrityHelper.h"

namespace winrt::ComServerSample::implementation
{
    uint32_t ServerInfo::ProcessId()
    {
        return GetCurrentProcessId();
    }

    winrt::ComServerSample::ProcessIntegrityLevel ServerInfo::IntegrityLevel()
    {
		return IntegrityHelper::GetProcessIntegrityLevel();
    }
}
