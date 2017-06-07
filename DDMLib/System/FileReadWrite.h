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

class FileReadWrite
{
private:
	HANDLE* m_pHandle;

public:
	FileReadWrite()
	{
		m_pHandle = NULL;
	}

	FileReadWrite(HANDLE* pHandle)
	{
		m_pHandle = pHandle;
	}

	operator HANDLE()
	{
		if (m_pHandle == NULL)
		{
			return NULL;
		}
		return *m_pHandle;
	}

	FileReadWrite& operator =(const FileReadWrite& rhs)
	{
		m_pHandle = rhs.m_pHandle;
		return *this;
	}

	BOOL IsAvailable() const
	{
		return m_pHandle != NULL;
	}

	void Set(HANDLE* pHandle)
	{
		m_pHandle = pHandle;
	}

	HANDLE* Get() const
	{
		return m_pHandle;
	}

	void Create()
	{
		m_pHandle = new HANDLE();
	}

	void Delete()
	{
		if (m_pHandle != NULL)
		{
			delete m_pHandle;
			m_pHandle = NULL;
		}
	}

	void Close()
	{
		if (m_pHandle != NULL && *m_pHandle != NULL)
		{
			::CloseHandle(*m_pHandle);
			*m_pHandle = NULL;
		}
	}
};