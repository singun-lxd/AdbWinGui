
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
