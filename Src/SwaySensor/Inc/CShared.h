#pragma once

#include <windows.h>
#include <string>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world455d.lib")
#else
#pragma comment(lib, "opencv_world455.lib")
#endif

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
//----------------------------------------------------------------------------
// .ini�t�@�C���֘A�ݒ�
#define FOLDER_OF_INIFILE                               L"Data\\"                       // .ini�t�@�C���t�H���_
#define EXT_OF_INIFILE                                  L"ini"                          // .ini�t�@�C���g���q

#define INI_SCT_COMMON                                  L"COMMON"                       // ���ʐݒ�
#define INI_KEY_COMMON_IMAGE_SOURCE_CAMERA              L"CMN_IMAGE_SOURCE_CAMERA"      // �J�����摜�捞��
#define INI_KEY_COMMON_IMAGE_SOURCE_CAMERA_ITEM_NUM     1                               // 
#define INI_KEY_COMMON_IMAGE_SOURCE_FILENAME            L"CMN_IMAGE_SOURCE_FILENAME"    // �捞�݉摜�t�@�C����
#define INI_KEY_COMMON_IMAGE_SOURCE_FILENAME_ITEM_NUM   1                               // 
#define INI_KEY_COMMON_IMAGE_OUTPUT_FILENAME            L"CMN_IMAGE_OUTPUT_FILENAME"    // �摜�ۑ��t�@�C����
#define INI_KEY_COMMON_IMAGE_OUTPUT_FILENAME_ITEM_NUM   1                               // 
#define INI_KEY_COMMON_IMAGE_SCREEN_LAYOUT              L"CMN_IMAGE_SCREEN_LAYOUT"      // �摜�\�����C�A�E�g
#define INI_KEY_COMMON_IMAGE_SCREEN_LAYOUT_ITEM_NUM     4                               // 

#define INI_SCT_SYSTEM                                  L"SYSTEM"                       // �V�X�e���ݒ�
#define INI_KEY_SYSTEM_CYCLE_TIME                       L"SYS_CYCLE_TIME"               // ���C���X���b�h���s����
#define INI_KEY_SYSTEM_CYCLE_TIME_ITEM_NUM              1                               // 
#define INI_KEY_SYSTEM_DEVICE_ID                        L"SYS_DEVICE_ID"                // �@��ID(���p�p����4����)
#define INI_KEY_SYSTEM_DEVICE_ID_ITEM_NUM               2                               // 

#define INI_SCT_TASK                                    L"TASK"                         // �^�X�N�ݒ�
#define INI_KEY_TASK_ENVIRONMENT                        L"TSK_ENVIRONMENT"              // Environment
#define INI_KEY_TASK_POLICY                             L"TSK_POLICY"                   // Policy
#define INI_KEY_TASK_AGENT                              L"TSK_AGENT"                    // Agent
#define INI_KEY_TASK_SCADA                              L"TSK_SCADA"                    // Scada
#define INI_KEY_TASK_ITEM_NUM                           3                               // 

#define INI_SCT_ETHERNET                                L"ETHERNET"                     // �ʐM�ݒ�
#define INI_KEY_ETHERNET_LOCAL                          L"NET_LOCAL"                    // Local�ݒ�
#define INI_KEY_ETHERNET_REMOTE                         L"NET_REMOTE"                   // Remote�ݒ�
#define INI_KEY_ETHERNET_ITEM_NUM                       2                               // 

#define INI_SCT_CAMERA                                  L"CAMERA"                       // �J�����ݒ�
#define INI_KEY_CAMERA_BASIS_IP_ADDRESS                 L"CAM_BASIS_IP_ADDRESS"         // �J������IP�A�h���X
#define INI_KEY_CAMERA_BASIS_IP_ADDRESS_ITEM_NUM        1                               // 
#define INI_KEY_CAMERA_BASIS_ROI                        L"CAM_BASIS_ROI"                // ROI(�̈�)
#define INI_KEY_CAMERA_BASIS_ROI_ITEM_NUM               4                               // 
#define INI_KEY_CAMERA_BASIS_PACKET_SIZE                L"CAM_BASIS_PACKET_SIZE"        // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y
#define INI_KEY_CAMERA_BASIS_PACKET_SIZE_ITEM_NUM       1                               // 
#define INI_KEY_CAMERA_BASIS_FRAME_RATE                 L"CAM_BASIS_FRAME_RATE"         // �t���[�����[�g
#define INI_KEY_CAMERA_BASIS_FRAME_RATE_ITEM_NUM        1                               // 
#define INI_KEY_CAMERA_BASIS_BLACKLEVEL                 L"CAM_BASIS_BLACKLEVEL"         // �����x���ݒ�
#define INI_KEY_CAMERA_BASIS_BLACKLEVEL_ITEM_NUM        1                               // 
#define INI_KEY_CAMERA_BASIS_GAMMA                      L"CAM_BASIS_GAMMA"              // �K���}�␳
#define INI_KEY_CAMERA_BASIS_GAMMA_ITEM_NUM             1                               // 
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO              L"CAM_BASIS_WBALANCE_AUTO"      // �z���C�g�o�����X�Q�C���ݒ�
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO_ITEM_NUM     3                               // 
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE                 L"CAM_BASIS_VIEW_ANGLE"         // �J��������p
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE_ITEM_NUM        2                               // 
#define INI_KEY_CAMERA_GAIN                             L"CAM_GAIN"                     // �Q�C���ݒ�
#define INI_KEY_CAMERA_GAIN_ITEM_NUM                    3                               // 
#define INI_KEY_CAMERA_GAIN_AUTO                        L"CAM_GAIN_AUTO"                // �����Q�C���ݒ�
#define INI_KEY_CAMERA_GAIN_AUTO_ITEM_NUM               2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO_START                  L"CAM_GAIN_AUTO_START"          // �����Q�C������P�x(�J�n)
#define INI_KEY_CAMERA_GAIN_AUTO_START_ITEM_NUM         2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO_STOP                   L"CAM_GAIN_AUTO_STOP"           // �����Q�C������P�x(��~)
#define INI_KEY_CAMERA_GAIN_AUTO_STOP_ITEM_NUM          2                               // 
#define INI_KEY_CAMERA_EXPSTIME                         L"CAM_EXPSTIME"                 // �I�����Ԑݒ�
#define INI_KEY_CAMERA_EXPSTIME_ITEM_NUM                3                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO                    L"CAM_EXPSTIME_AUTO"            // �����I���ݒ�
#define INI_KEY_CAMERA_EXPSTIME_AUTO_ITEM_NUM           2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO_START              L"CAM_EXPSTIME_AUTO_START"      // �����I������P�x(�J�n)
#define INI_KEY_CAMERA_EXPSTIME_AUTO_START_ITEM_NUM     2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO_STOP               L"CAM_EXPSTIME_AUTO_STOP"       // �����I������P�x(��~)
#define INI_KEY_CAMERA_EXPSTIME_AUTO_STOP_ITEM_NUM      2                               // 
#define INI_KEY_CAMERA_ERROR                            L"CAM_ERROR"                    // �J�����ُ̈픻��ݒ�
#define INI_KEY_CAMERA_ERROR_ITEM_NUM                   2                               // 
#define INI_KEY_CAMERA_BOOT                             L"CAM_BOOT"                     // �J�����̋N���ݒ�
#define INI_KEY_CAMERA_BOOT_ITEM_NUM                    3                               // 

#define INI_SCT_TILTMETER                               L"TILTMETER"                    // �X�Όv�ݒ�
#define INI_KEY_TILTMETER_CANOPEN                       L"TLT_CANOPEN"                  // CANope�ݒ�
#define INI_KEY_TILTMETER_CANOPEN_ITEM_NUM              3                               // 
#define INI_KEY_TILTMETER_FILTER                        L"TLT_FILTER"                   // �X�Όv�t�B���^�[
#define INI_KEY_TILTMETER_FILTER_ITEM_NUM               2                               // 
#define INI_KEY_TILTMETER_SPEED_TC                      L"TLT_SPEED_TC"                 // �X�Ίp���x�t�B���^�[���萔
#define INI_KEY_TILTMETER_SPEED_TC_ITEM_NUM             1                               // 

#define INI_SCT_MOUNTING                                L"MOUNTING"                     // ��t���@�ݒ�
#define INI_KEY_MOUNTING_OFFSET_L0                      L"MNT_OFFSET_L0"                // �J������]������L0[mm]
#define INI_KEY_MOUNTING_OFFSET_T0                      L"MNT_OFFSET_T0"                // �J������t�p�x��0(*10^6)[rad]
#define INI_KEY_MOUNTING_OFFSET_P0                      L"MNT_OFFSET_P0"                // �J�����ݒu�p�x��0(*10^6)[rad]
#define INI_KEY_MOUNTING_OFFSET_ITEM_NUM                2                               // 

#define INI_SCT_IMGPROC                                 L"IMGPROC"                      // �摜���������ݒ�
#define INI_KEY_IMGPROC_ROI                             L"IMG_ROI"                      // �摜ROI�ݒ�
#define INI_KEY_IMGPROC_ROI_ITEM_NUM                    2                               // 
#define INI_KEY_IMGPROC_MASK                            L"IMG_MASK"                     // �}�X�N�摜�I��
#define INI_KEY_IMGPROC_MASK_ITEM_NUM                   1                               // 
#define INI_KEY_IMGPROC_MASK1_H                         L"IMG_MASK1_H"                  // �摜1�}�X�NH
#define INI_KEY_IMGPROC_MASK1_S                         L"IMG_MASK1_S"                  // �摜1�}�X�NS
#define INI_KEY_IMGPROC_MASK1_V                         L"IMG_MASK1_V"                  // �摜1�}�X�NV
#define INI_KEY_IMGPROC_MASK1_ITEM_NUM                  2                               // 
#define INI_KEY_IMGPROC_MASK2_H                         L"IMG_MASK2_H"                  // �摜2�}�X�NH
#define INI_KEY_IMGPROC_MASK2_S                         L"IMG_MASK2_S"                  // �摜2�}�X�NS
#define INI_KEY_IMGPROC_MASK2_V                         L"IMG_MASK2_V"                  // �摜2�}�X�NV
#define INI_KEY_IMGPROC_MASK2_ITEM_NUM                  2                               // 
#define INI_KEY_IMGPROC_FILTER1                         L"IMG_FILTER1"                  // �t�B���^�[1(�S�}���m�C�Y)
#define INI_KEY_IMGPROC_FILTER1_ITEM_NUM                2                               // 
#define INI_KEY_IMGPROC_FILTER2                         L"IMG_FILTER2"                  // �t�B���^�[2(�����߃m�C�Y)
#define INI_KEY_IMGPROC_FILTER2_ITEM_NUM                2                               // 
#define INI_KEY_IMGPROC_PROC                            L"IMG_PROC"                     // �^�[�Q�b�g���o�A���S���Y��
#define INI_KEY_IMGPROC_PROC_ITEM_NUM                   1                               // 
#define INI_KEY_IMGPROC_SWAY_SPEED_TC                   L"IMG_SWAY_SPEED_TC"            // �U�ꑬ�x�t�B���^�[���萔
#define INI_KEY_IMGPROC_SWAY_SPEED_TC_ITEM_NUM          1                               // 
#define INI_KEY_IMGPROC_SWAY_ZERO_SET                   L"IMG_SWAY_ZERO_SET"            // �U��[���_�ݒ�
#define INI_KEY_IMGPROC_SWAY_ZERO_SET_ITEM_NUM          1                               // 

//----------------------------------------------------------------------------
// 
// H, S, V
#define IMAGE_HSV_H_MIN                 0           // H�ŏ��l
#define IMAGE_HSV_H_MAX                 179         // H�ő�l
#define IMAGE_HSV_S_MIN                 0           // S�ŏ��l
#define IMAGE_HSV_S_MAX                 255         // S�ő�l
#define IMAGE_HSV_V_MIN                 0           // V�ŏ��l
#define IMAGE_HSV_V_MAX                 255         // V�ő�l

// ����
#define EXTN_TARGETDIST_MIN             100.0       // �^�[�Q�b�g����(�ŏ�)
#define EXTN_TARGETDIST_MAX             80000.0     // �^�[�Q�b�g����(�ő�)

// �V���b�^�R���g���[�����[�h
#define	EXPOSURE_CONTROL_HOLD           0           // �V���b�^�R���g���[��:Hold
#define	EXPOSURE_CONTROL_LIGHT          1           // �V���b�^�R���g���[��:���邭
#define	EXPOSURE_CONTROL_DARKEN         -1          // �V���b�^�R���g���[��:�Â�

//----------------------------------------------------------------------------
// 
#define PI              3.14159     // ��
#define CONV_DEG_RAD    (PI/180.0)  // deg��rad
#define CONV_MSEC_SEC   0.001       // msec��sec
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum��`
#pragma region ENUM_DEFINITION
// �X�Όv���
enum class ENUM_TILT_STATUS : uint32_t {
    DEFAULT              = 0x00000000,  // �f�t�H���g
    CANOPEN_STARTED      = 0x00000001,  // CANopen�N��
    ERR_CANOPEN_LIB      = 0x00000010,  // CANopen Lib�G���[
    ERR_CANOPEN_READ     = 0x00000020   // �f�[�^�ǂݍ��݃G���[
} ;

// �摜�������
enum class ENUM_PROCCESS_STATUS : uint32_t {
    DEFAULT            = 0x00000000,    // �f�t�H���g
    IMAGE_ENABLE       = 0x00000001,    // �摜�f�[�^�L��
    TARGET_ENABLE      = 0x00000002,    // �^�[�Q�b�g���o�L��
    ZERO_SET_RUN       = 0x00000010,    // 0�_�ݒ蒆(�J����)
    ZERO_SET_MCC_RUN   = 0x00000020,    // 0�_�ݒ蒆(�J����)(MCC)
    ZERO_SET_COMPLETED = 0x00000100     // 0�_�ݒ芮��(�J����)
} ;

// �V�X�e�����
enum class ENUM_SYSTEM_STATUS : uint32_t {
    DEFAULT                = 0x00000000,    // �f�t�H���g
    CAMERA_RESET_REQ       = 0x00000010,    // �J�����Đڑ��v��
    CAMERA_RESET_REQ_MCC   = 0x00000020,    // �J�����Đڑ��v��(MCC)
    CAMERA_RESET_RUN       = 0x00000040,    // �J�����Đڑ���
    CAMERA_RESET_COMPLETED = 0x00000080,    // �J�����Đڑ�����
    TILT_PRESET_REQ        = 0x00000100,    // �X�Όv�v���Z�b�g�v��
    TILT_PRESET_REQ_MCC    = 0x00000200,    // �X�Όv�v���Z�b�g�v��(MCC)
    TILT_PRESET_RUN        = 0x00000400,    // �X�Όv�v���Z�b�g���s��
    TILT_PRESET_COMPLETED  = 0x00000800     // �X�Όv�v���Z�b�g����
} ;

// �J�����摜�捞��
enum class ENUM_GRAB_IMAGE : uint32_t {
    GRAB_FILE = 0,  // �摜�捞��(�t�@�C��)
    GRAB_CAMERA,    // �摜�捞��(�J����)
    E_MAX
} ;

// �U��Z���T�[
enum class ENUM_SWAY_SENSOR : uint32_t {
    SENSOR_1 = 0,   // �U��Z���T�[1
    SENSOR_2,       // �U��Z���T�[2
    E_MAX
} ;

// �^�[�Q�b�g���o
enum class ENUM_TARGET : uint32_t {
    TARGET_1 = 0,   // �^�[�Q�b�g���o1
    TARGET_2,       // �^�[�Q�b�g���o2
    E_MAX
} ;

// HSV�F���
enum class ENUM_HSV_MODEL : uint32_t {
    H = 0,  // �F��(Hue)
    S,      // �ʓx(Saturation)
    V,      // ���x(Value)
    E_MAX
} ;

// �J�����摜�z�� INDEX
enum class ENUM_IMAGE : uint32_t {
    SOURCE = 0, // ���摜
    MASK_1,     // �}�X�N�摜1
    MASK_2,     // �}�X�N�摜2
    PROCESS,    // ���H�摜
    E_MAX
} ;

// �摜�����z�� INDEX
enum class ENUM_IMAGE_MASK : uint32_t {
    MASK_1 = 0, // �}�X�N�摜1
    MASK_2,     // �}�X�N�摜2
    E_MAX
} ;

// �}�X�N�摜�I��
enum class ENUM_IMAGE_MASK_SEL : uint32_t {
    ALL = 0,    // �}�X�N�摜�I��(����)
    IMAGE_1,    // �}�X�N�摜�I��(�摜1�̂�)
    IMAGE_2,    // �}�X�N�摜�I��(�摜2�̂�)
    E_MAX
} ;

// �m�C�Y�t�B���^�[
enum class ENUM_NOISE_FILTER : uint32_t {
    FILTER_1 = 0,   // �t�B���^�[1
    FILTER_2,       // �t�B���^�[2
    E_MAX
} ;

// �m�C�Y�t�B���^�[1
enum class ENUM_NOISE_FILTER1 : uint32_t {
    NONE = 0,   // �Ȃ�
    MEDIAN,     // �����l�t�B���^�[
    OPENNING,   // �I�[�v�j���O����
    E_MAX
} ;

// �m�C�Y�t�B���^�[2
enum class ENUM_NOISE_FILTER2 : uint32_t {
    NONE = 0,   // �Ȃ�
    CLOSING,    // �N���[�W���O����
    E_MAX
} ;

// �d�S�ʒu�Z�o�A���S���Y��
enum class ENUM_COG : uint32_t {
    MAX_CONTOUR_AREA = 0,   // �ő�֊s�ʐ�
    MAX_CONTOUR_LENGTH,     // �ő�֊s��
    E_MAX
} ;

// ��
enum class ENUM_AXIS : uint32_t {
    X = 0,  // X��
    Y,      // Y��
    E_MAX
} ;
#pragma endregion ENUM_DEFINITION

//============================================================================
// �\����/���p�̒�`
#pragma region STRUCTURE_DEFINITION
//----------------------------------------------------------------------------
// �A�v���P�[�V�����ݒ�̍\����
// ���ʐݒ�
// �摜�\�����C�A�E�g�ݒ�
typedef struct TAG_CONFIG_COMMON_IMAGE_SCREEN_LAYOUT {
    uint32_t x0;        // ���_���WX
    uint32_t y0;        // ���_���WY
    uint32_t width;     // �����T�C�Y
    uint32_t height;    // �����T�C�Y

    TAG_CONFIG_COMMON_IMAGE_SCREEN_LAYOUT()
        : x0(0)
        , y0(0)
        , width(0)
        , height(0)
    {}
} CONFIG_COMMON_IMAGE_SCREEN_LAYOUT, *PCONFIG_COMMON_IMAGE_SCREEN_LAYOUT;
// ���ʐݒ�
typedef struct TAG_CONFIG_COMMON {
    uint32_t     img_source_camera; // �J�����摜�捞��(�J����[1]/�摜�t�@�C��[0])
    std::wstring img_source_fname;  // �捞�݉摜�t�@�C����(CMN_IMAGE_SOURCE_CAMERA=0�̂Ƃ��̉摜)
    std::wstring img_output_fname;  // �摜�ۑ��t�@�C����

    CONFIG_COMMON_IMAGE_SCREEN_LAYOUT img_screen_layout;    // �摜�\�����C�A�E�g�ݒ�

    TAG_CONFIG_COMMON()
        : img_source_camera(0)
        , img_source_fname(L"")
        , img_output_fname(L"")
    {}
} CONFIG_COMMON, *PCONFIG_COMMON;

// �V�X�e���ݒ�
typedef struct TAG_CONFIG_SYSTEM {
    uint32_t     cycletime_sys; // ���C���X���b�h���s����[msec]
    std::wstring mcc_id;        // �@��ID(����PC)[���p�p����4����]
    std::wstring my_id;         // �@��ID(�U��Z���T�[PC)[���p�p����4����]

    TAG_CONFIG_SYSTEM()
        : cycletime_sys(0)
        , mcc_id(L"")
        , my_id(L"")
    {}
} CONFIG_SYSTEM, *PCONFIG_SYSTEM;

// �^�X�N�ݒ�
// �^�X�N�ݒ�(�f�[�^)
typedef struct TAG_CONFIG_TASK_DATA {
    uint32_t     cycletime; // ���C���X���b�h���s����[msec]
    std::wstring name;      // �^�X�N��
    std::wstring sname;     // �^�X�N����

    TAG_CONFIG_TASK_DATA()
        : cycletime(0)
        , name(L"")
        , sname(L"")
    {}
} CONFIG_TASK_DATA, *PCONFIG_TASK_DATA;
// �^�X�N�ݒ�
typedef struct TAG_CONFIG_TASK {
    CONFIG_TASK_DATA environment;   // Environment
    CONFIG_TASK_DATA policy;        // Policy
    CONFIG_TASK_DATA agent;         // Agent
    CONFIG_TASK_DATA scada;         // Scada
} CONFIG_TASK, *PCONFIG_TASK;

// �ʐM�ݒ�
// �ʐM�ݒ�(�f�[�^)
typedef struct TAG_CONFIG_ETHERNET_DATA {
    std::wstring ipaddress; // IP�A�h���X
    unsigned int port;      // �|�[�g�ԍ�

    TAG_CONFIG_ETHERNET_DATA()
        : ipaddress(L"")
        , port(0)
    {}
} CONFIG_ETHERNET_DATA, *PCONFIG_ETHERNET_DATA;
// �ʐM�ݒ�
typedef struct TAG_CONFIG_SYS_ETHERNET {
    TAG_CONFIG_ETHERNET_DATA local;     // Local�ݒ�
    TAG_CONFIG_ETHERNET_DATA remote;    // Remote�ݒ�
} CONFIG_ETHERNET, *PCONFIG_ETHERNET;

// �J�����ݒ�
// ROI�ݒ�
typedef struct TAG_CONFIG_CAMERA_ROI {
    uint32_t offset;    // ROI(�̈�)�̊J�n�ʒu(X, Y)
    uint32_t size;      // ROI(�̈�)�̃T�C�Y(X(32�̔{��, 2592�ȉ�), Y(2�̔{��, 2048�ȉ�))

    TAG_CONFIG_CAMERA_ROI()
        : offset(0)
        , size(0)
    {}
} CONFIG_CAMERA_ROI, *PCONFIG_CAMERA_ROI;
// �z���C�g�o�����X�Q�C���ݒ�
typedef struct TAG_CONFIG_CAMERA_BALANCE_WHITE {
    unsigned int wb_auto;       // �z���C�g�o�����X�Q�C�������������[�ݒ�(��������OFF[1]/�A����������[2]/���̂ݎ�������[3])
    double       wb_ratio_red;  // �z���C�g�o�����X�Q�C���ݒ�(Red Gain)
    double       wb_ratio_blue; // �z���C�g�o�����X�Q�C���ݒ�(Blue Gain)

    TAG_CONFIG_CAMERA_BALANCE_WHITE()
        : wb_auto(0)
        , wb_ratio_red(0.0)
        , wb_ratio_blue(0.0)
    {}
} CONFIG_CAMERA_BALANCE_WHITE, *PCONFIG_CAMERA_BALANCE_WHITE;
// �J�����̊�{�ݒ�
typedef struct TAG_CONFIG_CAMERA_BASIS {
    std::wstring                ipaddress;  // IP�A�h���X
    CONFIG_CAMERA_ROI           roi[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // ROI(�̈�)
    uint32_t                    packetsize; // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y[byte]
    double                      framerate;  // �t���[�����[�g[fps]
    double                      blacklevel; // �����x���ݒ�
    double                      gamma;      // �K���}�␳
    CONFIG_CAMERA_BALANCE_WHITE wb;         // �z���C�g�o�����X�Q�C���ݒ�
    double                      viewangle[static_cast<uint32_t>(ENUM_AXIS::E_MAX)]; // ����p[deg]

    TAG_CONFIG_CAMERA_BASIS()
        : ipaddress(L"")
        , packetsize(0)
        , framerate(0.0)
        , blacklevel(0.0)
        , gamma(0.0)
        , viewangle{0.0, 0.0}
    {}
} CONFIG_CAMERA_BASIS, *PCONFIG_CAMERA_BASIS;
// �P�x�R���g���[���ݒ�
typedef struct TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL {
    double val;     // �ݒ�l
    double val_min; // �ŏ��l
    double val_max; // �ő�l

    BOOL   auto_control;    // �����R���g���[��(����[TRUE]/�Œ�[FALSE])
    double auto_rate;       // �����R���g���[���ω���
    double auto_start_h;    // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    double auto_start_l;    // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    double auto_stop_h;     // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    double auto_stop_l;     // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL()
        : val(0.0)
        , val_min(0.0)
        , val_max(0.0)
        , auto_control(FALSE)
        , auto_rate(0.0)
        , auto_start_h(0.0)
        , auto_start_l(0.0)
        , auto_stop_h(0.0)
        , auto_stop_l(0.0)
    {}
} CONFIG_CAMERA_BRIGHTNESS_CONTROL, *PCONFIG_CAMERA_BRIGHTNESS_CONTROL;
// �J�����ُ̈픻��ݒ�
typedef struct TAG_CONFIG_CAMERA_ERROR {
    double   framedrop; // �t���[�����[�g�ቺ�̔���l[fps]
    uint32_t errtime;   // �ُ픻�莞��[msec]

    TAG_CONFIG_CAMERA_ERROR()
        : framedrop(0.0)
        , errtime(0)
    {}
} CONFIG_CAMERA_ERROR, *PCONFIG_CAMERA_ERROR;
// �J�����̋N���ݒ�
typedef struct TAG_CONFIG_CAMERA_BOOT {
    uint32_t bootwaittimer;     // �V�X�e���N����̃J�����N���҂�����[msec]
    uint32_t rebootcount;       // �V�X�e���N����̃J�����N�����s���ɃJ���������ċN�������[count]
    uint32_t rebootingtimer;    // �J�����ċN�����ł��邱�Ƃ�ʒm���鎞��[msec]

    TAG_CONFIG_CAMERA_BOOT()
        : bootwaittimer(0)
        , rebootcount(0)
        , rebootingtimer(0)
    {}
} CONFIG_CAMERA_BOOT, *PCONFIG_CAMERA_BOOT;
// �J�����ݒ�
typedef struct TAG_CONFIG_CAMERA {
    CONFIG_CAMERA_BASIS              basis;     // �J�����̊�{�ݒ�
    CONFIG_CAMERA_BRIGHTNESS_CONTROL gain;      // �P�x�R���g���[���ݒ�(�Q�C��)
    CONFIG_CAMERA_BRIGHTNESS_CONTROL expstime;  // �P�x�R���g���[���ݒ�(�I������)
    CONFIG_CAMERA_ERROR              error;     // �J�����ُ̈픻��ݒ�
    CONFIG_CAMERA_BOOT               boot;      // �J�����̋N���ݒ�
} CONFIG_CAMERA, *PCONFIG_CAMERA;

// �X�Όv�ݒ�
// CANopen�ݒ�
typedef struct TAG_CONFIG_CANOPEN {
    std::wstring setup_fname;   // CANopen set-up .ini file name
    uint8_t      port;          // CANopen port (port1(=0) or port2(=1))
    uint8_t      node_id;       // CANopen node-ID

    TAG_CONFIG_CANOPEN()
        : setup_fname(L"")
        , port(0)
        , node_id(0x00)
    {}
} CONFIG_CANOPEN, *PCONFIG_CANOPEN;
// �X�Όv�ݒ�
typedef struct TAG_CONFIG_TILTMETER {
    CONFIG_CANOPEN canopen;         // CANopen�ݒ�
    uint16_t       dr_filter;       // �X�Όv�t�B���^�[(�ċA�`�f�B�W�^���t�B���^�[)
    uint16_t       ma_filter;       // �X�Όv�t�B���^�[(�ړ����σt�B���^�[)
    double         tiltspdfilter;   // �X�Ίp���x�t�B���^�[���萔

    TAG_CONFIG_TILTMETER()
        : dr_filter(0)
        , ma_filter(0)
        , tiltspdfilter(0.0)
    {}
} CONFIG_TILTMETER, *PCONFIG_TILTMETER;

// �\����`�ݒ�
// ��t�I�t�Z�b�g�ݒ�
typedef struct TAG_CONFIG_MOUNTING_OFFSET {
    double l0;  // �J������]������L0[mm]
    double t0;  // �J������t�p�x��0(*10^6)[rad]
    double p0;  // �J�����ݒu�p�x��0(*10^6)[rad]

    TAG_CONFIG_MOUNTING_OFFSET()
        : l0(0.0)
        , t0(0.0)
        , p0(0.0)
    {}
} CONFIG_MOUNTING_OFFSET, *PCONFIG_MOUNTING_OFFSET;
// �\����`�ݒ�
typedef struct TAG_CONFIG_MOUNTING {
    CONFIG_MOUNTING_OFFSET offset[static_cast<uint32_t>(ENUM_AXIS::E_MAX)]; // ��t�I�t�Z�b�g�ݒ�
} CONFIG_MOUNTING, *PCONFIG_MOUNTING;

// �摜���������ݒ�
// ROI�ݒ�
typedef struct TAG_CONFIG_IMGPROC_ROI {
    BOOL   valid;   // ROI�L���ݒ�(����[FALSE]/�L��[TRUE])
    double scale;   // ROI�X�P�[��(���o�����^�[�Q�b�g�ɑ΂���{��)

    TAG_CONFIG_IMGPROC_ROI()
        : valid(FALSE)
        , scale(0.0)
    {}
} CONFIG_IMGPROC_ROI, *PCONFIG_IMGPROC_ROI;
// �m�C�Y�t�B���^�[
typedef struct TAG_CONFIG_IMGPROC_FILTER {
    uint32_t type;  // �t�B���^�[���
    uint32_t val;   // �t�B���^�[�l

    TAG_CONFIG_IMGPROC_FILTER()
        : type(0)
        , val(0)
    {}
} CONFIG_IMGPROC_FILTER, *PCONFIG_IMGPROC_FILTER;
// �}�X�N�ݒ�
typedef struct TAG_CONFIG_IMGPROC_MASK {
    BOOL     valid;                                                 // �}�X�N�I��(����[FALSE]/�L��[TRUE])
    uint32_t hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX)];   // HSV�}�X�N����l(����)
    uint32_t hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX)];   // HSV�}�X�N����l(���)

    TAG_CONFIG_IMGPROC_MASK()
        : valid(FALSE)
        , hsv_l{0,0,0}
        , hsv_u{0,0,0}
    {}
} CONFIG_IMGPROC_MASK, *PCONFIG_IMGPROC_MASK;
// �摜���������ݒ�
typedef struct TAG_CONFIG_IMGPROC {
    CONFIG_IMGPROC_ROI    roi;                                                      // ROI�ݒ�
    CONFIG_IMGPROC_MASK   mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];      // �}�X�N�ݒ�
    CONFIG_IMGPROC_FILTER filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::E_MAX)];  // �m�C�Y�t�B���^�[(0=�S�}��, 1=������)
    uint32_t              imgprc;                                                   // �^�[�Q�b�g���o�A���S���Y��(0=�ő�֊s�ʐ�, 1=�ő�֊s��)
    double                swayspdfilter;                                            // �U�ꑬ�x�t�B���^�[���萔
    uint32_t              swayzeroset_time;                                         // �U��[���_�ݒ�(�v������[ms])

    TAG_CONFIG_IMGPROC()
        : imgprc(0)
        , swayspdfilter(0.0)
        , swayzeroset_time(0)
    {}
} CONFIG_IMGPROC, *PCONFIG_IMGPROC;

// ***�A�v���P�[�V�����ݒ�
typedef struct TAG_APP_CONFIG {
    CONFIG_COMMON    common;        // ���ʐݒ�
    CONFIG_SYSTEM    system;        // �V�X�e���ݒ�
    CONFIG_TASK      task;          // �^�X�N�ݒ�
    CONFIG_ETHERNET  ethernet;      // �ʐM�ݒ�
    CONFIG_CAMERA    camera;        // �J�����ݒ�
    CONFIG_TILTMETER tiltmeter;     // �X�Όv�ݒ�
    CONFIG_MOUNTING  mounting;      // ��t���@�ݒ�
    CONFIG_IMGPROC   imageprocess;  // �摜���������ݒ�
} APP_CONFIG, *PAPP_CONFIG;

//----------------------------------------------------------------------------
// �A�v���P�[�V�������̍\����
//----------------------------------------------------------------------------
// �摜���
// �摜���f�[�^
typedef struct TAG_INFO_IMGBUF_DATA {
    cv::Mat cv_mat; // CpenCV
} INFO_IMGBUF_DATA, *PINFO_IMGBUF_DATA;
// �摜���
typedef struct TAG_INFO_IMGBUF {
    CRITICAL_SECTION cs;
    INFO_IMGBUF_DATA data;  // �摜���f�[�^
} INFO_IMGBUF, *PINFO_IMGBUF;

// �X�Όv���
// �X�Όv�f�[�^
typedef struct TAG_TILTMT_DATA {
    double deg;         // �X�Ίp[deg]
    double rad;         // �X�Ίp[rad]
    double speed_deg;   // �X�Ίp���x[deg/s]
    double speed_rad;   // �X�Ίp���x[rad/s]

    TAG_TILTMT_DATA()
        : deg(0.0)
        , rad(0.0)
        , speed_deg(0.0)
        , speed_rad(0.0)
    {}
} TILT_DATA, *PTILT_DATA;
// �X�Όv���f�[�^
typedef struct TAG_INFO_TILTMT_DATA {
    int32_t   status;                                           // �X�e�[�^�X���
    TILT_DATA tilt[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];    // �X�Όv�f�[�^

    TAG_INFO_TILTMT_DATA()
        : status(static_cast<uint32_t>(ENUM_TILT_STATUS::DEFAULT))
    {}
} INFO_TILTMT_DATA, *PINFO_TILTMT_DATA;
// �X�Όv���
typedef struct TAG_INFO_TILTMT {
    CRITICAL_SECTION cs;
    INFO_TILTMT_DATA data;  // �X�Όv���f�[�^
} INFO_TILTMT, *PINFO_TILTMT;

// �N���C�A���g���
// �N���C�A���g���f�[�^
typedef struct TAG_INFO_CLIENT_DATA {
    std::wstring device_id;                                                 // �@��ID
    uint32_t     command;                                                   // �R�}���h
    uint32_t     mode;                                                      // ���[�h
    uint32_t     cycle;                                                     // �ŏ���M����[ms]
    double       tgt_len[static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX)];   // �^�[�Q�b�g����[mm]

    TAG_INFO_CLIENT_DATA()
        : device_id(L"")
        , command(0)
        , mode(0)
        , cycle(0)
        , tgt_len{0.0,0.0}
    {}
} INFO_CLIENT_DATA, *PINFO_CLIENT_DATA;
// �N���C�A���g���
typedef struct TAG_INFO_CLIENT {
    CRITICAL_SECTION cs;
    INFO_CLIENT_DATA data;  // �N���C�A���g���f�[�^
} INFO_CLIENT, *PINFO_CLIENT;

// �������
// �������f�[�^
typedef struct TAG_INFO_ADJUST_DATA {
    BOOL   target_distance_fixed;   // �ݓ_�`�^�[�Q�b�g�ԋ����Œ�
    double target_distance;         // �ݓ_�`�^�[�Q�b�g�ԋ���[mm]

    TAG_INFO_ADJUST_DATA()
        : target_distance_fixed(FALSE)
        , target_distance(EXTN_TARGETDIST_MIN)
    {}
} INFO_ADJUST_DATA, *PINFO_ADJUST_DATA;
// �������
typedef struct _stExtnInfo {
    CRITICAL_SECTION cs;
    INFO_ADJUST_DATA data;  // �������f�[�^
} INFO_ADJUST, *PINFO_ADJUST;

// �摜�������
// �^�[�Q�b�g���o�f�[�^
typedef struct TAG_TARGET_DATA {
    BOOL     valid;                                         // ���o���
    double   max_val;                                       // �ő�P�x
    double   pos[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];  // ���o�ʒu[pixel]
    int32_t  size;                                          // ���o�T�C�Y
    cv::Rect roi;                                           // ROI

    TAG_TARGET_DATA()
        : valid(FALSE)
        , max_val(0.0)
        , pos{0.0, 0.0}
        , size(0)
    {}
} TARGET_DATA, *PTARGET_DATA;
// �U�ꌟ�o�f�[�^(�^�[�Q�b�g���o�f�[�^�̒��S)
typedef struct TAG_SWAY_DATA {
    double target_pos;  // �^�[�Q�b�g�ʒu[pixel]
    double target_tilt; // �^�[�Q�b�g�X��[pixel]
    double sway_angle;  // �U��p[pixel]
    double sway_speed;  // �U�ꑬ�x[pixel/s]
    double sway_zero;   // �U�ꒆ�S[pixel]

    TAG_SWAY_DATA()
        : target_pos(0.0)
        , target_tilt(0.0)
        , sway_angle(0.0)
        , sway_speed(0.0)
        , sway_zero(0.0)
    {}
} SWAY_DATA, *PSWAY_DATA;
// �摜�������f�[�^
typedef struct TAG_INFO_IMGPRC_DATA {
    std::vector<std::vector<cv::Point>> contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];    // �֊s���W
    TARGET_DATA target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];                         // �^�[�Q�b�g���o�f�[�^
    SWAY_DATA   sway_data[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];                                 // �U�ꌟ�o�f�[�^(�^�[�Q�b�g���o�f�[�^�̒��S)
    double      target_size;                                                                        // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
    uint32_t    status;                                                                             // ���
    double      img_fps;                                                                            // �t���[�����[�g[fps]
    double      img_val;                                                                            // ���x
    int32_t     exps_mode;                                                                          // �����I���R�}���h(0:��~ 1:Up -1:Down)
    double      exps_time;                                                                          // �I������[us]

    TAG_INFO_IMGPRC_DATA()
        : target_size(0.0)
        , status(static_cast<uint32_t>(ENUM_PROCCESS_STATUS::DEFAULT))
        , img_fps(0.0)
        , img_val(0.0)
        , exps_mode(0)
        , exps_time(0.0)
    {}
} INFO_IMGPRC_DATA, *PINFO_IMGPRC_DATA;
// �摜�������
typedef struct TAG_INFO_IMGPRC {
    CRITICAL_SECTION cs;
    INFO_IMGPRC_DATA data;  // �摜�������f�[�^
} INFO_IMGPRC, *PINFO_IMGPRC;

// �V�X�e�����
// �V�X�e�����f�[�^
typedef struct TAG_INFO_SYSTEM_DATA {
    uint32_t status;    // �X�e�[�^�X

    TAG_INFO_SYSTEM_DATA()
        : status(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::DEFAULT))
    {}
} INFO_SYSTEM_DATA, *PINFO_SYSTEM_DATA;
// �V�X�e�����
typedef struct TAG_INFO_SYSTEM {
    CRITICAL_SECTION cs;
    INFO_SYSTEM_DATA data;  // �V�X�e�����f�[�^
} INFO_SYSTEM, *PINFO_SYSTEM;

// ***�A�v���P�[�V�������
typedef struct TAG_APP_INFO {
    INFO_IMGBUF imgbuf[static_cast<uint32_t>(ENUM_IMAGE::E_MAX)];   // �摜���
    INFO_TILTMT tiltmt;                                             // �X�Όv���
    INFO_CLIENT client;                                             // �N���C�A���g���
    INFO_ADJUST adjust;                                             // �������
    INFO_IMGPRC imgprc;                                             // �摜�������
    INFO_SYSTEM system;                                             // �V�X�e�����
} APP_INFO, *PAPP_INFO;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CShared

/// <summary>
/// �^�X�N�ԋ��L���n���h�����O�N���X
/// ���C���X���b�h�y�ъe�^�X�N�ŃC���X�^���X�쐬
/// </summary>

class CShared
{
public:
    CShared(BOOL init = FALSE);
    ~CShared();

    // �����o�[�ϐ�

    // �����o�[�֐�
    BOOL set_app_config_ini(wchar_t* file_name);    // ini file�ǂݍ��݃p�����[�^�ݒ�

    BOOL set_app_config(CONFIG_COMMON data);        // ���ʐݒ菑����
    BOOL get_app_config(PCONFIG_COMMON data);       // ���ʐݒ�Ǎ���
    BOOL set_app_config(CONFIG_SYSTEM data);        // �V�X�e���ݒ菑����
    BOOL get_app_config(PCONFIG_SYSTEM data);       // �V�X�e���ݒ�Ǎ���
    BOOL set_app_config(CONFIG_TASK data);          // �^�X�N�ݒ菑����
    BOOL get_app_config(PCONFIG_TASK data);         // �^�X�N�ݒ�Ǎ���
    BOOL set_app_config(CONFIG_ETHERNET data);      // �ʐM�ݒ菑����
    BOOL get_app_config(PCONFIG_ETHERNET data);     // �ʐM�ݒ�Ǎ���
    BOOL set_app_config(CONFIG_CAMERA data);        // �J�����ݒ菑����
    BOOL get_app_config(PCONFIG_CAMERA data);       // �J�����ݒ�Ǎ���
    BOOL set_app_config(CONFIG_TILTMETER data);     // �X�Όv�ݒ菑����
    BOOL get_app_config(PCONFIG_TILTMETER data);    // �X�Όv�ݒ�Ǎ���
    BOOL set_app_config(CONFIG_MOUNTING data);      // ��t���@�ݒ菑����
    BOOL get_app_config(PCONFIG_MOUNTING data);     // ��t���@�ݒ�Ǎ���
    BOOL set_app_config(CONFIG_IMGPROC data);       // �摜���������ݒ菑����
    BOOL get_app_config(PCONFIG_IMGPROC data);      // �摜���������ݒ�Ǎ���

    BOOL set_app_info_data(uint8_t id, cv::Mat cv_mat);     // �摜�f�[�^������
    BOOL get_app_info_data(uint8_t id, cv::Mat* cv_mat);    // �摜�f�[�^�Ǎ���
    BOOL set_app_info_data(INFO_TILTMT_DATA data);          // �X�Όv���f�[�^������
    BOOL get_app_info_data(PINFO_TILTMT_DATA data);         // �X�Όv���f�[�^�Ǎ���
    BOOL set_app_info_data(INFO_CLIENT_DATA data);          // �N���C�A���g���f�[�^������
    BOOL get_app_info_data(PINFO_CLIENT_DATA data);         // �N���C�A���g���f�[�^�Ǎ���
    BOOL set_app_info_data(INFO_ADJUST_DATA data);          // �������f�[�^������
    BOOL get_app_info_data(PINFO_ADJUST_DATA data);         // �������f�[�^�Ǎ���
    BOOL set_app_info_data(INFO_IMGPRC_DATA data);          // �摜�������f�[�^������
    BOOL get_app_info_data(PINFO_IMGPRC_DATA data);         // �摜�������f�[�^�Ǎ���
    BOOL set_app_info_data(INFO_SYSTEM_DATA data);          // �V�X�e�����f�[�^������
    BOOL get_app_info_data(PINFO_SYSTEM_DATA data);         // �V�X�e�����f�[�^�Ǎ���

protected:

private:
    // �����o�[�ϐ�
    static APP_CONFIG m_app_cnfg;   // �A�v���P�[�V�����ݒ�
    static APP_INFO   m_app_info;   // �A�v���P�[�V�������

    // �����o�[�֐�
};
