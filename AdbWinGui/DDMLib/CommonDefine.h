#pragma once

#include <string>
#include <wtypes.h>

namespace std
{

#ifdef _UNICODE
#define		tstring		wstring
#else
#define		tstring		strinng
#endif

}
