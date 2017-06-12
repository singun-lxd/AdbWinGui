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
class StreamWriter
{
private:
	typedef std::basic_string<T, std::char_traits<T>, std::allocator<T>> string_t;
	typedef std::basic_stringstream<T, std::char_traits<T>, std::allocator<T>> stringstream_t;
private:
	FileReadWrite m_fWrite;
	int m_nBuffSize;
	int m_nCurrentSize;
	stringstream_t m_tss;

public:
	StreamWriter(FileReadWrite fWrite, int buffSize)
	{
		m_fWrite = fWrite;
		m_nBuffSize = buffSize;
	}

	~StreamWriter()
	{
	}

	void Clear()
	{
		T szEmpty[1] = { 0 };
		m_tss.str(szEmpty);
		m_nCurrentSize = 0;
	}

	LONG WriteDataWithBuffer(const T *pBuff, int length)
	{
		int nTotalLen = m_nCurrentSize + length;
		m_tss << pBuff;
		m_nCurrentSize += length;

		if (nTotalLen > m_nBuffSize)
		{
			return Flush();
		}
		return length;
	}

	LONG WriteData(const T *pBuff, int length)
	{
		DWORD dwWrite = 0;
		string_t& strValue = m_tss.str();
		BOOL bWrite = ::WriteFile(m_fWrite, pBuff, length, &dwWrite, NULL);
		if (bWrite)
		{
			return static_cast<LONG>(dwWrite);
		}
		else
		{
			return -1;
		}
	}

	LONG Flush()
	{
		LONG lRet = 0;
		string_t& strValue = m_tss.str();
		if (!strValue.empty())
		{
			lRet = WriteData(strValue.c_str(), strValue.length() * sizeof(T));
			if (lRet >= 0)
			{
				Clear();
			}
		}
		return lRet;
	}
};

typedef	 StreamWriter<CHAR>     CharStreamWriter;
typedef	 StreamWriter<WCHAR>    WCharStreamWriter;
typedef	 StreamWriter<TCHAR>    TCharStreamWriter;