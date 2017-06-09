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

#include "SysDef.h"
#include "FileReadWrite.h"

template<class T>
class StreamReader
{
private:
	FileReadWrite m_fRead;
	int m_nBuffSize;
	T *m_pBuff;
	std::tstringstream m_tss;

public:
	StreamReader(FileReadWrite fRead, int buffSize)
	{
		m_fRead = fRead;
		m_nBuffSize = buffSize;
		m_pBuff = new T[buffSize];
	}

	~StreamReader()
	{
		if (m_pBuff != NULL)
		{
			delete[] m_pBuff;
			m_pBuff = NULL;
		}
	}

	void Clear()
	{
		m_tss.clear();
	}

	std::tstringstream& ReadData()
	{
		::ZeroMemory(m_pBuff, m_nBuffSize * sizeof(T));

		DWORD dwRead = 0;
		while (::ReadFile(m_fRead, m_pBuff, m_nBuffSize, &dwRead, NULL))
		{
			// read and convert to tchar stream
			m_tss << m_pBuff;
		}

		return m_tss;
	}

	LONG ReadData(T *pBuff, int length)
	{
		if (length > m_nBuffSize)
		{
			return -1;
		}
		DWORD dwRead = 0;
		BOOL bRead = ::ReadFile(m_fRead, pBuff, length, &dwRead, NULL);
		if (bRead)
		{
			return static_cast<LONG>(dwRead);
		}
		else
		{
			return -1;
		}
	}
};

typedef	 StreamReader<CHAR>     CharStreamReader;
typedef	 StreamReader<WCHAR>    WCharStreamReader;
typedef	 StreamReader<TCHAR>    TCharStreamReader;