#include "CHelper.h"
#include <locale.h>

/// @brief
/// @param
/// @return
/// @note
CHelper::CHelper()
{
}

/// @brief
/// @param
/// @return
/// @note
CHelper::~CHelper()
{
}

/// @brief stringŒ^‚ğwstringŒ^‚É•ÏŠ·‚µ‚Ü‚·
/// @param src :•ÏŠ·‘O“ü—Í
/// @param dest:•ÏŠ·Œão—Í
/// @return 
/// @note
void CHelper::Str2Wstr(const string& src, wstring& dest)
{
    setlocale(LC_ALL, "");

    wchar_t* wcs = new wchar_t[src.length() + 1];
    size_t   ret_val;
    mbstowcs_s(&ret_val, wcs, src.length() + 1, src.c_str(), _TRUNCATE);
    dest = wcs; delete[] wcs;

    return;
}
