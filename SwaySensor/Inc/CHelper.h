#pragma once

#include <string>

//============================================================================
// define��`
#define Bitcheck(a, b)  ((a >> b) & 1)
#define Bitset(a, b)    (a |= (1 << b))
#define Bitclear(a, b)  (a &= ~(1 << b))

//============================================================================
// �\����/���p�̒�`

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
