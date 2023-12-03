#include "Wsa.h"

void Wsa::initialize(int major, int minor)
{
	if (FAILED(WSAStartup(MAKEWORD(major, minor), &ws)))
	{
		requireNoErrors();
	}
}

std::string Wsa::errorCodeToString(int code)
{
	wchar_t *s = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&s, 0, nullptr);
	std::wstring ws( s );
	std::string errorMessage( ws.begin(), ws.end() );
	LocalFree(s);
	return errorMessage;
}

void Wsa::requireNoErrors()
{
	const int errorCode = WSAGetLastError();
	if (errorCode != 0)
	{
		throw std::runtime_error(Wsa::errorCodeToString(errorCode));
	}
}
