#pragma once

#include <string>

using namespace std;

//============================================================================
// define’è‹`
#define Bitcheck(a, b)                  ((a >> b) & 1)
#define Bitset(a, b)                    (a |= (1 << b))
#define Bitclear(a, b)                  (a &= ~(1 << b))

class CHelper
{
public:
    CHelper();
    ~CHelper();

    // •¶Žš—ñ
    static void Str2Wstr(const string& src, wstring& dest);                                   // string¨wstring‚É•ÏŠ·

protected:

private:
};
