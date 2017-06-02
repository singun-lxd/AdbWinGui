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

#include "../CommonDefine.h"
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
			m_tss << m_pBuff;
		}

		return m_tss;
	}

// 	template<class _Fn, class... _Args>
// 	void ReadLine(_Fn&& _Fx, _Args&&... _Ax)
// 	{
// 		std::tstring line;
// 		while (std::getline(tss, line)) {
// 			_Fx(line, _Ax);
// 		}
// 	}
};

typedef	 StreamReader<CHAR>     CharStreamReader;
typedef	 StreamReader<WCHAR>    WCharStreamReader;
typedef	 StreamReader<TCHAR>    TCharStreamReader;