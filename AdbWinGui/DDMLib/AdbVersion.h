#pragma once

#include "CommonDefine.h"

class AdbVersion
{
public:
	static AdbVersion* const UNKNOWN;

public:
	const int m_Major;
	const int m_Minor;
	const int m_Micro;

private:
	AdbVersion(int major, int minor, int micro);

public:
	bool operator == (const AdbVersion&);

	static AdbVersion* ParseFrom(const TCHAR* input);
};
