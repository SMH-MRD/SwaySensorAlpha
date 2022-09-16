#pragma once

#include <string>

//============================================================================
// define定義
#define Bitcheck(a, b)  ((a >> b) & 1)
#define Bitset(a, b)    (a |= (1 << b))
#define Bitclear(a, b)  (a &= ~(1 << b))

//============================================================================
// 構造体/共用体定義

//////////////////////////////////////////////////////////////////////////////
// CHelper

class CHelper
{
public:
    CHelper();
    ~CHelper();

    static std::wstring conv_string(const std::string& input);
    static std::string  conv_string(const std::wstring& input);

protected:

private:
};
