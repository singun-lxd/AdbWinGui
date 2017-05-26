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
		return m_Major - o.m_Major;
	}

	if (m_Minor != o.m_Minor) {
		return m_Minor - o.m_Minor;
	}

	return m_Micro - o.m_Micro;
}

AdbVersion* AdbVersion::ParseFrom(const TCHAR* input)
{
	return UNKNOWN;
}
