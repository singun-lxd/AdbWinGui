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
#include "IShellOutputReceiver.h"
#include <memory>
#include "StringUtils.h"

class MultiLineReceiver : public IShellOutputReceiver
{
private:
	bool m_bTrimLines = true;
	std::vector<std::string> m_vecArray;

public:
	void setTrimLine(bool trim)
	{
		m_bTrimLines = trim;
	}

	virtual void AddOutput(char* pData, int offset, int length) override
	{
		if (!IsCancelled())
		{
// 			std::string s;
// 			s.append(pData + offset, length);

			std::stringstream iss;
			iss.width(length);
			iss << pData + offset;

			// now we split the lines
			m_vecArray.clear();

			std::string line;
			while (std::getline(iss, line))
			{
				if (m_bTrimLines)
				{
					StringUtils::TrimString(line);
				}
				m_vecArray.push_back(line);
			}

			if (!m_vecArray.empty())
			{
				// at this point we've split all the lines.
				// copy the array
				std::vector<std::string> vecResult;
				vecResult.assign(m_vecArray.begin(), m_vecArray.end());

				// send it for final processing
				ProcessNewLines(vecResult);
			}
		}
	}

	virtual void Flush() override
	{
		Done();
	}

	virtual void Done() {
		// do nothing.
	}

	virtual void ProcessNewLines(const std::vector<std::string>& vecArray) = 0;
};