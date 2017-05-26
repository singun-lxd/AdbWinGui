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

#include <regex>
#include "AdbVersion.h"

#define ADB_VERSION_PATTERN	_T("^.*(\\d+)\\.(\\d+)\\.(\\d+).*")
AdbVersion* const AdbVersion::UNKNOWN = new AdbVersion(-1, -1, -1);

AdbVersion::AdbVersion(int major, int minor, int micro) :
	m_Major(major), 
	m_Minor(minor),
	m_Micro(micro)
{
}

bool AdbVersion::operator==(const AdbVersion& o)
{
	if (m_Major != o.m_Major) {
		return false;
	}

	if (m_Minor != o.m_Minor) {
		return false;
	}

	return m_Micro - o.m_Micro == 0;
}

bool AdbVersion::operator>(const AdbVersion& o)
{
	if (m_Major < o.m_Major) {
		return false;
	}

	if (m_Minor < o.m_Minor) {
		return false;
	}

	return m_Micro - o.m_Micro > 0;
}

bool AdbVersion::operator<(const AdbVersion& o)
{
	if (m_Major > o.m_Major) {
		return false;
	}

	if (m_Minor > o.m_Minor) {
		return false;
	}

	return m_Micro - o.m_Micro < 0;
}

AdbVersion* AdbVersion::ParseFrom(const TCHAR* input)
{
	std::tregex rxAdb(ADB_VERSION_PATTERN, std::tregex::icase);
	std::tsmatch results;
	std::tstring parseLine(input);
	if (std::regex_match(parseLine, results, rxAdb) && results.size() == 4)
	{
		if (results[0].matched)
		{
			std::tstringstream tss;
			int major = 0, minor = 0, micro = 0;
			tss << results[1].str();
			tss >> major;
			tss.clear();
			tss << results[2].str();
			tss >> minor;
			tss.clear();
			tss << results[3].str();
			tss >> micro;
			tss.clear();
			return new AdbVersion(major, minor, micro);
		}
	}
	return UNKNOWN;
}
