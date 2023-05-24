#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include "CEnvironment.h"
#include "CAgent.h"

extern std::vector<void*> VectpCTaskObj;    // TaskObj�̃|�C���^
extern TASK_INDEX         g_task_index;     // TaskObj�̃C���f�b�N�X

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
#define MOVE_AVERAGE_COUNT      1   // �ړ����ϐ�
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum��`
#pragma region ENUM_DEFINITION
#pragma endregion ENUM_DEFINITION

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
// �U�ꒆ�S�v���f�[�^
typedef struct TAG_SWAY_ZERO_DATA {
    LARGE_INTEGER time_counter;         // �v�����ԗp�p�t�H�[�}���X�J�E���^�[
    double        sway_min[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // �U��p�ŏ��l
    double        sway_max[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // �U��p�ő�l
    double        sway_zero[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];  // �U��[���_
} SWAY_ZERO_DATA, *PSWAY_ZERO_DATA;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CPolicy

class CPolicy : public CTaskObj
{
public:
    CPolicy();
    ~CPolicy();

    // �����o�[�ϐ�

    // �����o�[�֐�

protected:

private:
    // �����o�[�ϐ�
    CShared*      m_shared;         // CShared�N���X
    CEnvironment* m_environment;    // CEnvironment�N���X

    static CONFIG_COMMON    m_cnfgcmn;      // ���ʐݒ�
    static CONFIG_CAMERA    m_cnfgcam;      // �J�����ݒ�
    static CONFIG_MOUNTING  m_cnfgmnt;      // ��t���@�ݒ�
    static CONFIG_IMGPROC   m_cnfgprc;      // �摜���������ݒ�
    static INFO_TILTMT_DATA m_infotlt_data; // �X�Όv���f�[�^
    static INFO_CLIENT_DATA m_infoclt_data; // �N���C�A���g���f�[�^
    static INFO_ADJUST_DATA m_infoajs_data; // �������f�[�^
    static INFO_IMGPRC_DATA m_infoprc_data; // �摜�������f�[�^

    MOVE_AVERAGE_DATA m_move_avrg_data; // �P�x�ړ����σf�[�^
    SWAY_ZERO_DATA    m_sway_zero_data; // �U�ꒆ�S�v���f�[�^

    LARGE_INTEGER m_cycle_time_counter; // �p�t�H�[�}���X�J�E���^�[���ݒl
    int32_t       m_cycle_time;         // ���s����[ms]

    // �����o�[�֐�
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // ����������
    void close(void);           // �I������
    void proc_main(void);       // ���C������

    BOOL proc_center_gravity(std::vector<std::vector<cv::Point>> contours,
                             double* outPosX,
                             double* outPosY,
                             int* outTgtSize,
                             uint32_t sel); // �d�S���o
    void proc_sway(void);                   // �U�ꌟ�o����

    double get_sway_zero(uint32_t idx); // �U��[���_�ݒ菈��
    void set_expstime(void);            // �V���b�^�R���g���[��

    void set_tab_func_txt(void);    // �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
    void set_tab_tip_txt(void);     // �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // �^�u�p�l���̐ݒ�
};
