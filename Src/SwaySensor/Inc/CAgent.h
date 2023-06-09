#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include "SockUDP.h"

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
#define NW_SEND_IP_AUTO                 L"0.0.0.0"  // ���M���Socket��񂩂猈�肷��ꍇ��IP�ݒ�l

#define NW_SIZE_DEVICE_ID               4           // �@��ID�T�C�Y[byte]
#define NW_SIZE_STRING_INFO             32          // �������T�C�Y[byte]
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum��`
#pragma region ENUM_DEFINITION
// �ʐM�f�[�^(�R�}���h)
enum class ENUM_NW_COMMAND : uint32_t {
    REQ_DATA_CYCLE    = 0x00000001, // �f�[�^���M�v��(�����)
    REQ_DATA_STOP     = 0x00000008, // �f�[�^���M��~�v��
    SET_ZERO_CAMERA_1 = 0x00000010, // 0�_�ݒ�(�J����1)
    SET_ZERO_CAMERA_2 = 0x00000020, // 0�_�ݒ�(�J����2)
    SET_ZERO_TILT_1   = 0x00000040, // 0�_�ݒ�(�X�Όv1)
    SET_ZERO_TILT_2   = 0x00000080, // 0�_�ݒ�(�X�Όv2)
    RESET_CAMERA_1    = 0x00000100, // ���Z�b�g�v��(�J����1)
    RESET_CAMERA_2    = 0x00000200, // ���Z�b�g�v��(�J����2)
    RESET_TILT_1      = 0x00000400, // ���Z�b�g�v��(�X�Όv1)
    RESET_TILT_2      = 0x00000800, // ���Z�b�g�v��(�X�Όv2)
    REQ_SAVE_IMAGE    = 0x00001000, // �摜�ۑ��v��
    REQ_PC_REBOOT     = 0x80000000  // PC�ċN���v��
} ;

// �ʐM�f�[�^(�G���[���)
enum class ENUM_NW_ERR_SENSOR : uint32_t {
    CAMERA   = 0x00000001,  // �@��ُ�(�J����)
    TILT     = 0x00000002,  // �@��ُ�(�X�Όv)
    TARGET_1 = 0x00000010,  // ���o�ُ�(�^�[�Q�b�g1)
    TARGET_2 = 0x00000020   // ���o�ُ�(�^�[�Q�b�g2)
} ;

// �ʐM�f�[�^(���o���)
enum class ENUM_NW_STAT_SENSOR : uint32_t {
    TARGET_1        = 0x00000001,   // �U�ꌟ�o��(�^�[�Q�b�g1)
    TARGET_2        = 0x00000002,   // �U�ꌟ�o��(�^�[�Q�b�g2)
    BOOT_CAMERA     = 0x00000010,   // ������(�J����)
    BOOT_TILT       = 0x00000020,   // ������(�X�Όv)
    SET_ZERO_CAMERA = 0x00000040,   // 0�_�ݒ蒆(�J����)
    SET_ZERO_TILT   = 0x00000080    // 0�_�ݒ蒆(�X�Όv)
};
#pragma endregion ENUM_DEFINITION

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
//----------------------------------------------------------------------------
// �N���C�A���g�ʐM���̍\����
//----------------------------------------------------------------------------
// �N���C�A���g���o��
#pragma pack(push, 1)   // Set 1 byte boundary
// ***��M�f�[�^
// �R�}���h���
typedef struct TAG_NW_RECV_DATA_COMMAND_INFO {
    uint32_t command;   // �R�}���h���
    uint32_t mode;      // ���[�h�ݒ�
    uint32_t cycle;     // �ŏ���M����[ms]
    uint32_t tgt_len[static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX)];   // �^�[�Q�b�g����[mm]
} NW_RECV_DATA_COMMAND_INFO, *PNW_RECV_DATA_COMMAND_INFO;
// ��M�f�[�^
typedef struct TAG_NW_RECV_DATA {
    uint8_t                   device_id[NW_SIZE_DEVICE_ID]; // �@��ID
    SOCKADDR_IN               sock;                         // sockaddrs_in�\����
    NW_RECV_DATA_COMMAND_INFO command_info;                 // �R�}���h���
} NW_RECV_DATA, *PNW_RECV_DATA;

// ***���M�f�[�^
// *�@��̏��
// �J�������
typedef struct TAG_NW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO {
    uint32_t image_size[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // �摜�f�[�^��f��[pixel]
    uint32_t view_angle[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // ��p[pixel/rad]
} NW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO, *PNW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO;
// �J������t���
typedef struct TAG_NW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO {
    uint32_t l0[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // �J������]������L0[mm]
    uint32_t t0[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // �J������t�p�x��0(*10^6)[rad]
    uint32_t p0[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // �J�����ݒu�p�x��0(*10^6)[rad]
} NW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO, *PNW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO;
// �@��̏��
typedef struct TAG_NW_SEND_DATA_SENSOR_SPEC_INFO {
    NW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO          camera;   // �J�������
    NW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO mounting; // ��t���
} NW_SEND_DATA_SENSOR_SPEC_INFO, *PNW_SEND_DATA_SENSOR_SPEC_INFO;
// *�@���ԏ��
typedef struct TAG_NW_SEND_DATA_SENSOR_STAT_INFO {
    uint32_t mode;  // ���o���[�h
    uint32_t error; // �G���[���
    uint32_t stat;  // ���o���
} NW_SEND_DATA_SENSOR_STAT_INFO, *PNW_SEND_DATA_SENSOR_STAT_INFO;
// *�X�Όv���
typedef struct TAG_NW_SEND_DATA_TILT_INFO {
    uint32_t angle; // �X�Ίp(*10^6)[rad]
    uint32_t speed; // �X�Ίp���x(*10^6)[rad/s]
} NW_SEND_DATA_TILT_INFO, *PNW_SEND_DATA_TILT_INFO;
// *�U�ꌟ�o���
// �U����
typedef struct TAG_NW_SEND_DATA_SWAY_SWAY_INFO {
    uint32_t angle[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];    // �U��p[pixel]
    uint32_t speed[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];    // �U�ꑬ�x[pixel/s]
    uint32_t zero[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];     // �U�ꒆ�S�ʒu[pixel]
} NW_SEND_DATA_SWAY_SWAY_INFO, *PNW_SEND_DATA_SWAY_SWAY_INFO;
// �^�[�Q�b�g���
typedef struct TAG_NW_SEND_DATA_SWAY_TARGET_INFO {
    uint32_t tilt[static_cast<uint32_t>(ENUM_AXIS::E_MAX)]; // �^�[�Q�b�g�X��[pixel]
    uint32_t size;                                          // �^�[�Q�b�g�T�C�Y[pixel]
} NW_SEND_DATA_SWAY_TARGET_INFO, *PNW_SEND_DATA_SWAY_TARGET_INFO;
// �U�ꌟ�o���
typedef struct TAG_NW_SEND_DATA_SWAY_INFO {
    NW_SEND_DATA_SWAY_SWAY_INFO   sway;     // �U����
    NW_SEND_DATA_SWAY_TARGET_INFO target;   // �^�[�Q�b�g���
} NW_SEND_DATA_SWAY_INFO, *PNW_SEND_DATA_SWAY_INFO;
// *�U��Z���T�[���
typedef struct TAG_NW_SEND_DATA_SWAY_SENSOR_INFO {
    NW_SEND_DATA_SENSOR_SPEC_INFO sensor_spec_info;                                     // �@��̏��
    NW_SEND_DATA_SENSOR_STAT_INFO sensor_stat_info;                                     // �@���ԏ��
    NW_SEND_DATA_TILT_INFO        tilt_info[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // �X�Όv���
    NW_SEND_DATA_SWAY_INFO        sway_info[static_cast<uint32_t>(ENUM_TARGET::E_MAX)]; // �U�ꌟ�o���
    uint8_t                       message[NW_SIZE_STRING_INFO];                         // �������
} NW_SEND_DATA_SWAY_SENSOR_INFO, *PNW_SEND_DATA_SWAY_SENSOR_INFO;
// ���M�f�[�^
typedef struct TAG_NW_SEND_DATA {
    uint8_t                       device_id[NW_SIZE_DEVICE_ID]; // �@��ID
    SYSTEMTIME                    system_time;                  // SYSTEMTIME���
    NW_SEND_DATA_SWAY_SENSOR_INFO sway_sensor_info[static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX)]; // �U��Z���T�[���
} NW_SEND_DATA, *PNW_SEND_DATA;
#pragma pack(pop)
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CAgent

/// <summary>
/// Agent�^�X�N�ł́A�ȉ��̏�����S�����܂�
/// <para>�Z���T�J�����Ƃ�IF����</para>
/// <para>�X�Όv�Ƃ�IF����</para>
/// <para>����PC�Ƃ�IF����</para>
/// </summary>

class CAgent : public CTaskObj
{
public:
    CAgent();
    ~CAgent();

    // �����o�[�ϐ�

    // �����o�[�֐�

protected:

private:
    // �����o�[�ϐ�
    CShared* m_shared;  // CShared�N���X

    CONFIG_SYSTEM    m_cnfgsys;         // �V�X�e���ݒ�
    CONFIG_ETHERNET  m_cnfgnet;         // �ʐM�ݒ�
    CONFIG_CAMERA    m_cnfgcam;         // �J�����ݒ�
    CONFIG_MOUNTING  m_cnfgmnt;         // ��t���@�ݒ�
    INFO_TILTMT_DATA m_infotlt_data;    // �X�Όv���f�[�^
    INFO_CLIENT_DATA m_infoclt_data;    // �N���C�A���g���f�[�^
    INFO_IMGPRC_DATA m_infoprc_data;    // �摜�������f�[�^

    HANDLE m_td_comm_send_hndl; // �X���b�h�n���h��
    BOOL   m_td_comm_send_stat; // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)

    CSockUDPLib* m_sockrecv;    // Socket(��M)
    CSockUDPLib* m_socksend;    // Socket(���M)

    NW_RECV_DATA m_recv_data;   // ��M�f�[�^
    NW_SEND_DATA m_send_data;   // ���M�f�[�^

    LARGE_INTEGER m_send_cycle_counter; // �p�t�H�[�}���X�J�E���^�[���ݒl

    // �����o�[�֐�
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // ����������
    void close(void);           // �I������
    void proc_main(void);       // ���C������

    void set_tab_func_txt(void);    // �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
    void set_tab_tip_txt(void);     // �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // �^�u�p�l���̐ݒ�

    static unsigned WINAPI td_comm_send(LPVOID lpParam);    // �f�[�^���M�X���b�h�֐�
};
