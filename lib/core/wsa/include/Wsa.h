#pragma once
#include "Singleton.h"
#include "winsock2.h"

class Wsa : public Singleton<Wsa>
{
public:
	~Wsa();
	static std::string errorCodeToString(int code);
	void initialize(int major, int minor);
	void requireNoErrors();

private:
	WSADATA ws;
};