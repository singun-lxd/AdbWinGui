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

class File
{
private:
	const std::tstring m_strPath;

public:
	File(const TString szPath);
	static const TString GetName(const TString szPath);
	const TString GetName() const;
	BOOL Exists() const;
	BOOL IsDirectory() const;
	BOOL IsFile() const;
	DWORD GetLength() const;
	DOUBLE GetLastModifiedTime() const;
	FileReadWrite GetRead() const;
	FileReadWrite GetWrite() const;
};