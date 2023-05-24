#pragma once

#include <string>

//============================================================================
// define定義
#pragma region CONSTANT_DEFINITION
#define Bitcheck(a, b)              ((a >> b) & 1)
#define Bitset(a, b)                (a |= (1 << b))
#define Bitclear(a, b)              (a &= ~(1 << b))

#define MOVE_AVERAGE_BUFFER_SIZE    20  // 移動平均バッファサイズ
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum定義
#pragma region ENUM_DEFINITION
#pragma endregion ENUM_DEFINITION

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
// 移動平均データ
typedef struct TAG_MOVE_AVERAGE_DATA {
    double  data[MOVE_AVERAGE_BUFFER_SIZE]; // データ
    int32_t wptr;                           // 書込みポインタ
    int32_t data_count;                     // データ数
    double  total_val;                      // 輝度積算
    double  max_val;                        // 最大輝度(移動平均後)
} MOVE_AVERAGE_DATA, *PMOVE_AVERAGE_DATA;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CHelper

class CHelper
{
public:
    CHelper();
    ~CHelper();

    // メンバー変数

    // メンバー関数
    static std::wstring conv_string(const std::string& src);    // string->wstring型の変換
    static std::string  conv_string(const std::wstring& src);   // wstring->string型の変換
    static double moving_average(double val, PMOVE_AVERAGE_DATA buf, int num);  // 移動平均処理

private:
};
