#pragma once

#include <string>

using namespace std;

//============================================================================
// define定義
#define Bitcheck(a, b)                  ((a >> b) & 1)
#define Bitset(a, b)                    (a |= (1 << b))
#define Bitclear(a, b)                  (a &= ~(1 << b))

class CHelper
{
public:
    CHelper();
    ~CHelper();

    // 文字列
    static void Str2Wstr(const string& src, wstring& dest);                                   // string→wstringに変換

protected:

private:
};
