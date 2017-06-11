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

#include "FileListingService.h"
#include <regex>

#define APK_PATTERN	".*\\.apk"

#define  DIRECTORY_DATA "data"
#define  DIRECTORY_SDCARD "sdcard"
#define  DIRECTORY_MNT "mnt"
#define  DIRECTORY_SYSTEM "system"
#define  DIRECTORY_TEMP "tmp"
#define  DIRECTORY_APP "app"

#define REFRESH_RATE	5000L
#define REFRESH_TEST	(long)(REFRESH_RATE * .8)

#define TYPE_FILE 0
#define TYPE_DIRECTORY 1
#define TYPE_DIRECTORY_LINK 2
#define TYPE_BLOCK 3
#define TYPE_CHARACTER 4
#define TYPE_LINK 5
#define TYPE_SOCKET 6
#define TYPE_FIFO 7
#define TYPE_OTHER 8

#define FILE_SEPARATOR		"/"
#define FILE_ROOT			"/"

#define ESCAPE_PATTERN_REGEX	"([\\\\()*+?\"'&#/\\s])"

FileListingService::FileEntry::FileEntry(FileEntry* parent, const char* name, int type, bool isRoot)
{
	m_pParent = parent;
	m_strName = name;
	m_nType = type;
	m_bIsRoot = isRoot;

	CheckAppPackageStatus();
}

const char * FileListingService::FileEntry::GetName()
{
	return m_strName.c_str();
}

const char* FileListingService::FileEntry::GetSize()
{
	return m_strSize.c_str();
}

int FileListingService::FileEntry::GetSizeValue()
{
	return atoi(m_strSize.c_str());
}

const char* FileListingService::FileEntry::GetDate()
{
	return m_strDate.c_str();
}

const char* FileListingService::FileEntry::GetTime()
{
	return m_strTime.c_str();
}

const char* FileListingService::FileEntry::GetPermissions()
{
	return m_strPermissions.c_str();
}

const char* FileListingService::FileEntry::GetOwner()
{
	return m_strOwner.c_str();
}

const char* FileListingService::FileEntry::GetGroup()
{
	return m_strGroup.c_str();
}

const char* FileListingService::FileEntry::GetInfo()
{
	return m_strInfo.c_str();
}

void FileListingService::FileEntry::GetFullPath(std::string& fullPath)
{
	if (m_bIsRoot)
	{
		fullPath = FILE_ROOT;
	}
	FillPathBuilder(fullPath, false);
}

void FileListingService::FileEntry::GetFullEscapedPath(std::string& fullPath)
{
	FillPathBuilder(fullPath, true);
}

void FileListingService::FileEntry::GetPathSegments(std::vector<std::string>& vecSegments)
{
	FillPathSegments(vecSegments);
}

int FileListingService::FileEntry::GetType()
{
	return m_nType;
}

void FileListingService::FileEntry::SetType(int type)
{
	m_nType = type;
}

bool FileListingService::FileEntry::IsDirectory()
{
	return m_nType == TYPE_DIRECTORY || m_nType == TYPE_DIRECTORY_LINK;
}

FileListingService::FileEntry* FileListingService::FileEntry::GetParent()
{
	return m_pParent;
}

void FileListingService::FileEntry::GetCachedChildren(std::vector<FileEntry*>& vecChildren)
{
	vecChildren.assign(m_vecChildren.begin(), m_vecChildren.end());
}

FileListingService::FileEntry* FileListingService::FileEntry::FindChild(const char* name)
{
	if (name == NULL)
	{
		return NULL;
	}
	for (FileEntry* entry : m_vecChildren)
	{
		if (entry->m_strName.compare(name) == 0)
		{
			return entry;
		}
	}
	return NULL;
}

bool FileListingService::FileEntry::IsRoot()
{
	return m_bIsRoot;
}

void FileListingService::FileEntry::AddChild(FileEntry* child)
{
	m_vecChildren.push_back(child);
}

void FileListingService::FileEntry::SetChildren(const std::vector<FileEntry*>& newChildren)
{
	m_vecChildren.clear();
	m_vecChildren.assign(newChildren.begin(), newChildren.end());
}

bool FileListingService::FileEntry::NeedFetch()
{
	if (m_tFetchTime == 0)
	{
		return true;
	}
	time_t current = time(NULL);
	return current - m_tFetchTime > REFRESH_TEST;
}

bool FileListingService::FileEntry::IsApplicationPackage()
{
	return m_bIsAppPackage;
}

bool FileListingService::FileEntry::IsAppFileName()
{
	std::regex rxName(APK_PATTERN, std::tregex::icase);
	std::smatch results;
	if (std::regex_match(m_strName, results, rxName) && results[0].matched)
	{
		return true;
	}
	return false;
}

void FileListingService::FileEntry::FillPathBuilder(std::string& pathBuilder, bool escapePath)
{
	if (m_bIsRoot)
	{
		return;
	}

	if (m_pParent != NULL)
	{
		m_pParent->FillPathBuilder(pathBuilder, escapePath);
	}
	pathBuilder.append(FILE_SEPARATOR);
	if (escapePath)
	{
		std::string escaped;
		Escape(m_strName.c_str(), escaped);
		pathBuilder.append(escaped);
	}
	else
	{
		pathBuilder.append(m_strName);
	}
}

void FileListingService::FileEntry::FillPathSegments(std::vector<std::string>& list)
{
	if (m_bIsRoot)
	{
		return;
	}

	if (m_pParent != NULL)
	{
		m_pParent->FillPathSegments(list);
	}

	list.push_back(m_strName);
}

void FileListingService::FileEntry::CheckAppPackageStatus()
{
	m_bIsAppPackage = false;

	std::vector<std::string> vecSegments;
	GetPathSegments(vecSegments);
	if (m_nType == TYPE_FILE && vecSegments.size() == 3 && IsAppFileName())
	{
		m_bIsAppPackage = vecSegments[1].compare(DIRECTORY_APP) == 0 &&
			(vecSegments[0].compare(DIRECTORY_SYSTEM) == 0 || vecSegments[0].compare(DIRECTORY_DATA) == 0);
	}
}

void FileListingService::FileEntry::Escape(const char* entryName, std::string& escaped)
{
	std::regex rxName(ESCAPE_PATTERN_REGEX);
	std::smatch results;
	std::string enrty(entryName);
	if (std::regex_match(enrty, results, rxName) && results[0].matched)
	{
		// todo escaped
		escaped;
	}
}
