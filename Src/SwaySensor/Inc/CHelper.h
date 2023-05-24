#pragma once

#include <string>

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
#define Bitcheck(a, b)              ((a >> b) & 1)
#define Bitset(a, b)                (a |= (1 << b))
#define Bitclear(a, b)              (a &= ~(1 << b))

#define MOVE_AVERAGE_BUFFER_SIZE    20  // �ړ����σo�b�t�@�T�C�Y
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum��`
#pragma region ENUM_DEFINITION
#pragma endregion ENUM_DEFINITION

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
// �ړ����σf�[�^
typedef struct TAG_MOVE_AVERAGE_DATA {
    double  data[MOVE_AVERAGE_BUFFER_SIZE]; // �f�[�^
    int32_t wptr;                           // �����݃|�C���^
    int32_t data_count;                     // �f�[�^��
    double  total_val;                      // �P�x�ώZ
    double  max_val;                        // �ő�P�x(�ړ����ό�)
} MOVE_AVERAGE_DATA, *PMOVE_AVERAGE_DATA;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CHelper

class CHelper
{
public:
    CHelper();
    ~CHelper();

    // �����o�[�ϐ�

    // �����o�[�֐�
    static std::wstring conv_string(const std::string& src);    // string->wstring�^�̕ϊ�
    static std::string  conv_string(const std::wstring& src);   // wstring->string�^�̕ϊ�
    static double moving_average(double val, PMOVE_AVERAGE_DATA buf, int num);  // �ړ����Ϗ���

private:
};
