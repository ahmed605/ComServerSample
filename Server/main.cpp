#include <Windows.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "ServerInfo.h"
#include "AcidHash.h"

template<typename CLASS, typename FACTORY>
void RegisterClassFactory()
{
	constexpr auto clsid = AcidHash::clsid_hash_of<CLASS>();
	auto factory = winrt::make<FACTORY>();

	DWORD cookie = 0;
	winrt::check_hresult(CoRegisterClassObject(
		clsid,
		(::IUnknown*)winrt::get_abi(factory),
		CLSCTX_LOCAL_SERVER,
		REGCLS_MULTIPLEUSE | REGCLS_AGILE,
		&cookie));
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	winrt::init_apartment();

	RegisterClassFactory<winrt::ComServerSample::ServerInfo, winrt::ComServerSample::factory_implementation::ServerInfo>();

	size_t event = NULL;
	std::wstringstream(lpCmdLine) >> event;
	if (event) SetEvent((HANDLE)event);

	MSG msg;
	BOOL ret;
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}