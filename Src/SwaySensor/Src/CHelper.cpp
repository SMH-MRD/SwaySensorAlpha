#include "CHelper.h"
#include <locale.h>

//////////////////////////////////////////////////////////////////////////////
// CHelper

//////////////////////////////////////////////////////////////////////////////
// Public method

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

/// @brief string->wstring�^�̕ϊ�
/// @param
/// @return
/// @note
std::wstring CHelper::conv_string(const std::string& src)
{
	size_t   ptr;
	wchar_t* buffer = new wchar_t[src.size() + 1];

	mbstowcs_s(&ptr, buffer, src.size() + 1, src.c_str(), _TRUNCATE);
	std::wstring str = static_cast<std::wstring>(buffer);
	delete[] buffer;

	return str;
}

/// @brief wstring->string�^�̕ϊ�
/// @param
/// @return
/// @note
std::string CHelper::conv_string(const std::wstring& src)
{
	size_t ptr;
	char*  buffer = new char[src.size() * MB_CUR_MAX + 1];

	wcstombs_s(&ptr, buffer, src.size() * MB_CUR_MAX + 1, src.c_str(), _TRUNCATE);
	std::string str = static_cast<std::string>(buffer);
	delete[] buffer;

	return str;
}

/// @brief �ړ����Ϗ���
/// @param
/// @return
/// @note
double CHelper::moving_average(double val, PMOVE_AVERAGE_DATA buf, int num)
{
    int32_t data_count;
    int32_t old_ptr;
    double  old_val;
    double  ave_val;

    if (num > 1) {
        //----------------------------------------------------------------------------
        // �o�b�t�@�����O
        buf->data[buf->wptr] = val;

        //----------------------------------------------------------------------------
        // �����݃|�C���^�[�̍X�V
        if ((++buf->wptr) >= MOVE_AVERAGE_BUFFER_SIZE) {
            buf->wptr = 0;
        }

        //----------------------------------------------------------------------------
        // �o�b�t�@�[���i�[���̍X�V
        if (buf->data_count < MOVE_AVERAGE_BUFFER_SIZE)	{
            buf->data_count++;
        }

        //----------------------------------------------------------------------------
        // �ł��Â��f�[�^�̎��o��
        if (buf->data_count <= num) {
            data_count = buf->data_count;
            old_val    = 0;
        }
        else {
            data_count = num;
            if((old_ptr = (buf->wptr - 1) - data_count) < 0) {
                old_ptr = MOVE_AVERAGE_BUFFER_SIZE - (data_count - buf->wptr + 1);
            }
            old_val = buf->data[old_ptr];
        }

        //----------------------------------------------------------------------------
        // �ړ����Ϗ���
        buf->total_val = (buf->total_val - old_val) + val;
        if (buf->total_val < 0.0) {
            buf->total_val = 0.0;
        }
        ave_val = buf->total_val / static_cast<double>(data_count);
    }
    else {
        ave_val = val;  // �ړ����ω񐔂�1�ȉ��̏ꍇ�A���̂܂ܕԂ�
    }

    return ave_val;
}

//////////////////////////////////////////////////////////////////////////////
// Private method
