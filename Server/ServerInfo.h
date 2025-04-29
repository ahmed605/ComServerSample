#pragma once
#include "ServerInfo.g.h"

namespace winrt::ComServerSample::implementation
{
    struct ServerInfo
    {
        ServerInfo() = default;

        static uint32_t ProcessId();
        static winrt::ComServerSample::ProcessIntegrityLevel IntegrityLevel();
    };
}

namespace winrt::ComServerSample::factory_implementation
{
    struct ServerInfo : ServerInfoT<ServerInfo, implementation::ServerInfo>
    {
    };
}
