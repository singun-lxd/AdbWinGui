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

class ArrayUtils
{
public:
	static void Swap32bitsToArray(int value, char* dest, int offset)
	{
		dest[offset] = (char)(value & 0x000000FF);
		dest[offset + 1] = (char)((value & 0x0000FF00) >> 8);
		dest[offset + 2] = (char)((value & 0x00FF0000) >> 16);
		dest[offset + 3] = (char)((value & 0xFF000000) >> 24);
	}

	static int Swap32bitFromArray(char* value, int offset) {
		int v = 0;
		v |= ((int)value[offset]) & 0x000000FF;
		v |= (((int)value[offset + 1]) & 0x000000FF) << 8;
		v |= (((int)value[offset + 2]) & 0x000000FF) << 16;
		v |= (((int)value[offset + 3]) & 0x000000FF) << 24;

		return v;
	}
};