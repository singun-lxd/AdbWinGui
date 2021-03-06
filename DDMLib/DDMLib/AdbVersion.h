/*
AdbWinGui (Android Debug Bridge Windows GUI)
Copyright (C) 2017  singun

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "CommonDefine.h"

struct AdbVersion
{
public:
	static AdbVersion* const UNKNOWN;

public:
	const int m_nMajor;
	const int m_nMinor;
	const int m_nMicro;

private:
	AdbVersion(int major, int minor, int micro);

public:
	bool operator == (const AdbVersion&);
	bool operator > (const AdbVersion&);
	bool operator < (const AdbVersion&);

	static AdbVersion* ParseFrom(const TString input);
};
