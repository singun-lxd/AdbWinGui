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

#include "File.h"
#include <Shlwapi.h>
#include "FileReadWrite.h"

#pragma comment(lib, "Shlwapi.lib")

File::File(const TString szPath) : m_strPath(szPath)
{
}

const TString File::GetName(const TString szPath)
{
	return ::PathFindFileName(szPath);
}

const TString File::GetName() const
{
	return GetName(m_strPath.c_str());
}

BOOL File::Exists() const
{
	return ::PathFileExists(m_strPath.c_str());
}

BOOL File::IsDirectory() const
{
	return ::PathIsDirectory(m_strPath.c_str());
}

BOOL File::IsFile() const
{
	return Exists() && !IsDirectory();
}

DWORD File::GetLength() const
{
	HANDLE hFile = ::CreateFile(m_strPath.c_str(), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize = ::GetFileSize(hFile, NULL);
		::CloseHandle(hFile);
		return dwSize;
	}
	return 0;
}

DOUBLE File::GetLastModifiedTime() const
{
	DOUBLE dTime = 0;
	SYSTEMTIME time;
	FILETIME filetime;
	FILETIME localtime;
	HANDLE hFile = ::CreateFile(m_strPath.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		::GetFileTime(hFile, NULL, NULL, &filetime);
		::FileTimeToLocalFileTime(&filetime, &localtime);
		::FileTimeToSystemTime(&localtime, &time);
		::SystemTimeToVariantTime(&time, &dTime);
		::CloseHandle(hFile);
		return TRUE;
	}
	return FALSE;
}

FileReadWrite File::GetRead() const
{
	FileReadWrite fRead;
	fRead.Create();
	fRead = ::CreateFile(m_strPath.c_str(), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (!fRead.IsValid())
	{
		fRead.Delete();
	}
	return fRead;
}

FileReadWrite File::GetWrite() const
{
	FileReadWrite fWrite;
	fWrite.Create();
	fWrite = ::CreateFile(m_strPath.c_str(), FILE_WRITE_EA, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (!fWrite.IsValid())
	{
		fWrite.Delete();
	}
	return fWrite;
}
