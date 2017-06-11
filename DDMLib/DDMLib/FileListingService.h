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

#include "CommonDefine.h"

class FileListingService
{
public:
	class FileEntry
	{
	private:
		FileEntry* m_pParent;
		std::string m_strName;
		std::string m_strInfo;
		std::string m_strPermissions;
		std::string m_strSize;
		std::string m_strDate;
		std::string m_strTime;
		std::string m_strOwner;
		std::string m_strGroup;
		int m_nType;
		bool m_bIsAppPackage;
		bool m_bIsRoot;
		time_t m_tFetchTime = 0;

		std::vector<FileEntry*> m_vecChildren;

	private:
		FileEntry(FileEntry* parent, const char* name, int type, bool isRoot);

	public:
		const char* GetName();
		const char* GetSize();
		int GetSizeValue();
		const char* GetDate();
		const char* GetTime();
		const char* GetPermissions();
		const char* GetOwner();
		const char* GetGroup();
		const char* GetInfo();
		void GetFullPath(std::string& fullPath);
		void GetFullEscapedPath(std::string& fullPath);
		void GetPathSegments(std::vector<std::string>& vecSegments);
		int GetType();
		void SetType(int type);
		bool IsDirectory();
		FileEntry* GetParent();
		void GetCachedChildren(std::vector<FileEntry*>& vecChildren);
		FileEntry* FindChild(const char* name);
		bool IsRoot();
		void AddChild(FileEntry* child);
		void SetChildren(const std::vector<FileEntry*>& newChildren);
		bool NeedFetch();
		bool IsApplicationPackage();
		bool IsAppFileName();
	protected:
		void FillPathBuilder(std::string& pathBuilder, bool escapePath);
		void FillPathSegments(std::vector<std::string>& list);
	private:
		void CheckAppPackageStatus();
	public:
		static void Escape(const char* entryName, std::string& escaped);
	};
};
