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

class Process
{
private:
	std::tstring m_strCmdLine;

	FileReadWrite m_fRead;
	FileReadWrite m_fWrite;
	STARTUPINFO m_si;
	PROCESS_INFORMATION m_pi;

public:
	Process(const TString szCmdLine)
	{
		m_strCmdLine = szCmdLine;

		m_si = { sizeof(STARTUPINFO) };
		::ZeroMemory(&m_pi, sizeof(PROCESS_INFORMATION));
	}

	~Process()
	{
		CloseWrite();
		CloseRead();
		Join();
	}

	void OpenReadWrite()
	{
		m_fRead.Create();
		m_fWrite.Create();
	}

	BOOL Start()
	{
		if (m_pi.hProcess != NULL)
		{
			return FALSE;
		}

		SECURITY_ATTRIBUTES* psa = NULL;
		DWORD dwFlags = STARTF_USESHOWWINDOW;
		if (m_fRead.IsAvailable() && m_fWrite.IsAvailable())
		{
			psa = new SECURITY_ATTRIBUTES();
			psa->nLength = sizeof(SECURITY_ATTRIBUTES);
			psa->lpSecurityDescriptor = NULL;
			psa->bInheritHandle = TRUE;

			if (!::CreatePipe(m_fRead.Get(), m_fWrite.Get(), psa, 0))
			{
				return FALSE;
			}

			dwFlags |= STARTF_USESTDHANDLES;
		}
		// run process
		::GetStartupInfo(&m_si);
		m_si.dwFlags = dwFlags;
		m_si.wShowWindow = SW_HIDE;
		m_si.hStdError = m_fWrite;
		m_si.hStdOutput = m_fWrite;

		if (!::CreateProcess(NULL, const_cast<LPTSTR>(m_strCmdLine.c_str()), NULL, NULL, TRUE, NULL, NULL, NULL, &m_si, &m_pi))
		{
			CloseWrite();
			CloseRead();
			return FALSE;
		}
		return TRUE;
	}

	void Join()
	{
		if (m_pi.hProcess != NULL)
		{
			::WaitForSingleObject(m_pi.hProcess, INFINITE);
			::CloseHandle(m_pi.hProcess);
			::CloseHandle(m_pi.hThread);
			m_pi.hProcess = NULL;
			m_pi.hThread = NULL;
		}
	}

	BOOL Kill(UINT uExitCode)
	{
		if (::TerminateProcess(m_pi.hProcess, uExitCode))
		{
			::CloseHandle(m_pi.hProcess);
			::CloseHandle(m_pi.hThread);
			m_pi.hProcess = NULL;
			m_pi.hThread = NULL;
			return TRUE;
		}
		return FALSE;
	}

	FileReadWrite& GetRead()
	{
		return m_fRead;
	}

	FileReadWrite& GetWrite()
	{
		return m_fWrite;
	}

	void CloseRead()
	{
		m_fRead.Close();
		m_fRead.Delete();
	}

	void CloseWrite()
	{
		m_fWrite.Close();
		m_fWrite.Delete();
	}
};