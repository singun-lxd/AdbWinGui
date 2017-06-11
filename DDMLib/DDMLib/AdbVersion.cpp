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

#include "AdbVersion.h"
#include <regex>

#define ADB_VERSION_PATTERN	_T("^.*(\\d+)\\.(\\d+)\\.(\\d+).*")
AdbVersion* const AdbVersion::UNKNOWN = new AdbVersion(-1, -1, -1);

AdbVersion::AdbVersion(int major, int minor, int micro) :
	m_nMajor(major), 
	m_nMinor(minor),
	m_nMicro(micro)
{
}

bool AdbVersion::operator == (const AdbVersion& o)
{
	if (m_nMajor != o.m_nMajor)
	{
		return false;
	}

	if (m_nMinor != o.m_nMinor)
	{
		return false;
	}

	return m_nMicro == o.m_nMicro;
}

bool AdbVersion::operator > (const AdbVersion& o)
{
	if (m_nMajor > o.m_nMajor)
	{
		return true;
	}
	else if (m_nMajor == o.m_nMajor)
	{
		if (m_nMinor > o.m_nMinor)
		{
			return true;
		}
		else if (m_nMinor == o.m_nMinor)
		{
			if (m_nMicro > o.m_nMicro)
			{
				return true;
			}
		}
	}
	return false;
}

bool AdbVersion::operator < (const AdbVersion& o)
{
	if (m_nMajor < o.m_nMajor)
	{
		return true;
	}
	else if (m_nMajor == o.m_nMajor)
	{
		if (m_nMinor < o.m_nMinor)
		{
			return true;
		}
		else if (m_nMinor == o.m_nMinor)
		{
			if (m_nMicro < o.m_nMicro)
			{
				return true;
			}
		}
	}
	return false;
}

AdbVersion* AdbVersion::ParseFrom(const TString input)
{
	std::tregex rxAdb(ADB_VERSION_PATTERN);
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
