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

/// @brief
/// @param
/// @return
/// @note
std::wstring CHelper::conv_string(const std::string& input)
{
	size_t   ptr;
	wchar_t* buffer = new wchar_t[input.size() + 1];

	mbstowcs_s(&ptr, buffer, input.size() + 1, input.c_str(), _TRUNCATE);
	std::wstring str = buffer;
	delete[] buffer;

	return str;
}

/// @brief
/// @param
/// @return
/// @note
std::string CHelper::conv_string(const std::wstring& input)
{
	size_t ptr;
	char*  buffer = new char[input.size() * MB_CUR_MAX + 1];

	wcstombs_s(&ptr, buffer, input.size() * MB_CUR_MAX + 1, input.c_str(), _TRUNCATE);
	std::string str = buffer;
	delete[] buffer;

	return str;
}
