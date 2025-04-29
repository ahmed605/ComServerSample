#include <iostream>
#include <Windows.h>
#include <roapi.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/ComServerSample.h>
#include <IntegrityHelper.h>
#include <filesystem>
#include <detours/detours.h>
#include <AcidHash.h>
#include <EnumHelper.h>

HANDLE CreateClientJob()
{
    HANDLE hJob = CreateJobObjectW(nullptr, nullptr);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info =
    {
        .BasicLimitInformation =
        {
            .LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
        }
    };

    SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &info, sizeof(info));
	return hJob;
}

BOOL InitializeServer(HANDLE hJob)
{
	if (!hJob) return FALSE;

    SECURITY_ATTRIBUTES security =
    {
        .nLength = sizeof(security),
        .lpSecurityDescriptor = nullptr,
        .bInheritHandle = TRUE
    };

	HANDLE event = CreateEventW(&security, TRUE, FALSE, NULL);
	if (!event) return FALSE;

	wchar_t clientPath[MAX_PATH];
	GetModuleFileNameW(nullptr, clientPath, MAX_PATH);

	auto serverPath = std::filesystem::path(clientPath).parent_path() / L"..\\Server\\";

    std::wstringstream cmd;
	cmd << serverPath.c_str() << L"Server.exe " << reinterpret_cast<std::size_t>(event);

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

    BOOL result = FALSE;
    if (result = CreateProcessW(
        nullptr,
        (wchar_t*)cmd.str().c_str(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_SUSPENDED,
        nullptr,
        serverPath.c_str(),
        &si,
        &pi
    ))
    {
        if (result = AssignProcessToJobObject(hJob, pi.hProcess))
        {
            if (!(result = ResumeThread(pi.hThread) != (DWORD)-1))
            {
                TerminateProcess(pi.hProcess, 0);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }

        WaitForSingleObject(event, INFINITE);
	}

	CloseHandle(event);
    return result;
}

decltype(&RoGetActivationFactory) g_RoGetActivationFactory = nullptr;

HRESULT RoGetActivationFactoryHook(HSTRING activatableClassId, REFIID iid, void** factory)
{
    if (g_RoGetActivationFactory)
    {
        HRESULT result;
        if (FAILED(result = g_RoGetActivationFactory(activatableClassId, iid, factory)))
        {
            winrt::hstring acid;
			winrt::copy_from_abi(acid, activatableClassId);

			auto clsid = AcidHash::clsid_hash_of(acid.c_str());
			return CoGetClassObject(
				clsid,
				CLSCTX_LOCAL_SERVER,
				nullptr,
				iid,
				factory
			);
        }

		return result;
    }

	return E_FAIL;
}

BOOL InitializeHooks()
{
    auto mod = LoadLibraryW(L"combase.dll");
	if (!mod) return FALSE;

	g_RoGetActivationFactory = reinterpret_cast<decltype(&RoGetActivationFactory)>(GetProcAddress(mod, "RoGetActivationFactory"));
	if (!g_RoGetActivationFactory) return FALSE;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)g_RoGetActivationFactory, RoGetActivationFactoryHook);
	return DetourTransactionCommit() == NO_ERROR;
}

int main()
{
	if (!InitializeHooks()) return 1;

	winrt::init_apartment();

	HANDLE hJob;
	if (!(hJob = CreateClientJob())) return 1;
	if (!InitializeServer(hJob)) return 1;

    printf("Client PID: %u\n", GetCurrentProcessId());
    printf("Server PID: %u\n", winrt::ComServerSample::ServerInfo::ProcessId());
    printf("Client Integrity Level: %S\n", EnumHelper::to_string(IntegrityHelper::GetProcessIntegrityLevel()));
    printf("Server Integrity Level: %S\n", EnumHelper::to_string(winrt::ComServerSample::ServerInfo::IntegrityLevel()));
    system("pause");

	CloseHandle(hJob);
	return 0;
}
