#pragma once

#include <windows.h>
#include <string>

//============================================================================
// define��`
//----------------------------------------------------------------------------
// .ini�t�@�C���֘A�ݒ�
#define FOLDER_OF_INIFILE                               L"Data\\"                       // .ini�t�@�C���t�H���_
#define EXT_OF_INIFILE                                  L"ini"                          // .ini�t�@�C���g���q
#define PATH_OF_INIFILE                                 m_path                          // �A�v���P�[�V����.ini�t�@�C���p�X

#define INI_SCT_COMMON                                  L"COMMON"                       // ���ʐݒ�
#define INI_KEY_COMMON_SYSTEM_CYCLE_TIME                L"CMN_SYSTEM_CYCLE_TIME"        // ���C���X���b�h���s����
#define INI_KEY_COMMON_IMAGE_SOURCE                     L"CMN_IMAGE_SOURCE"             // �摜�捞�݌�
#define INI_KEY_COMMON_IMAGE_FILENAME_SOURCE            L"CMN_IMAGE_FILENAME_SOURCE"    // �捞�݉摜�t�@�C����
#define INI_KEY_COMMON_IMAGE_FILENAME_OUTPUT            L"CMN_IMAGE_FILENAME_OUTPUT"    // �摜�ۑ��t�@�C����

#define INI_SCT_TASK                                    L"TASK"                         // �^�X�N�ݒ�
#define INI_KEY_TASK_COLUMN_NUM                         3                               // 
#define INI_KEY_TASK_ENVIRONMENT                        L"TSK_ENVIRONMENT"              // Environment
#define INI_KEY_TASK_POLICY                             L"TSK_POLICY"                   // Policy
#define INI_KEY_TASK_AGENT                              L"TSK_AGENT"                    // Agent
#define INI_KEY_TASK_SCADA                              L"TSK_SCADA"                    // Scada

#define INI_SCT_ETHERNET                                L"ETHERNET"                     // �ʐM�ݒ�
#define INI_KEY_ETHERNET_COLUMN_NUM                     2                               // 
#define INI_KEY_ETHERNET_LOCAL                          L"NET_LOCAL"                    // Local�ݒ�
#define INI_KEY_ETHERNET_REMOTE                         L"NET_REMOTE"                   // Remote�ݒ�

#define INI_SCT_CAMERA                                  L"CAMERA"                       // �J�����ݒ�
#define INI_KEY_CAMERA_BASIS_IP_ADDRESS                 L"CAM_BASIS_IP_ADDRESS"         // �J������IP�A�h���X
#define INI_KEY_CAMERA_BASIS_ROI_COLUMN_NUM             4                               // 
#define INI_KEY_CAMERA_BASIS_ROI                        L"CAM_BASIS_ROI"                // ROI(�̈�)
#define INI_KEY_CAMERA_BASIS_PACKET_SIZE                L"CAM_BASIS_PACKET_SIZE"        // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y
#define INI_KEY_CAMERA_BASIS_FRAME_RATE                 L"CAM_BASIS_FRAME_RATE"         // �t���[�����[�g
#define INI_KEY_CAMERA_BASIS_BLACKLEVEL                 L"CAM_BASIS_BLACKLEVEL"         // �����x���ݒ�
#define INI_KEY_CAMERA_BASIS_GAMMA                      L"CAM_BASIS_GAMMA"              // �K���}�␳
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO_COLUMN_NUM   3                               // 
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO              L"CAM_BASIS_WBALANCE_AUTO"      // �z���C�g�o�����X�Q�C���ݒ�
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE_COLUMN_NUM      2                               // 
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE                 L"CAM_BASIS_VIEW_ANGLE"         // �J��������p
#define INI_KEY_CAMERA_GAIN_COLUMN_NUM                  3                               // 
#define INI_KEY_CAMERA_GAIN                             L"CAM_GAIN"                     // �Q�C���ݒ�
#define INI_KEY_CAMERA_GAIN_AUTO_COLUMN_NUM             2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO                        L"CAM_GAIN_AUTO"                // �����Q�C���ݒ�
#define INI_KEY_CAMERA_GAIN_AUTO_LIMIT                  2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO_START                  L"CAM_GAIN_AUTO_START"          // �����Q�C������P�x(�J�n)
#define INI_KEY_CAMERA_GAIN_AUTO_STOP                   L"CAM_GAIN_AUTO_STOP"           // �����Q�C������P�x(��~)
#define INI_KEY_CAMERA_EXPSTIME_COLUMN_NUM              3                               // 
#define INI_KEY_CAMERA_EXPSTIME                         L"CAM_EXPSTIME"                 // �I�����Ԑݒ�
#define INI_KEY_CAMERA_EXPSTIME_AUTO_COLUMN_NUM         2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO                    L"CAM_EXPSTIME_AUTO"            // �����I���ݒ�
#define INI_KEY_CAMERA_EXPSTIME_AUTO_LIMIT_COLUMN_NUM   2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO_START              L"CAM_EXPSTIME_AUTO_START"      // �����I������P�x(�J�n)
#define INI_KEY_CAMERA_EXPSTIME_AUTO_STOP               L"CAM_EXPSTIME_AUTO_STOP"       // �����I������P�x(��~)
#define INI_KEY_CAMERA_ERROR_COLUMN_NUM                 2                               // 
#define INI_KEY_CAMERA_ERROR                            L"CAM_ERROR"                    // �J�����ُ̈픻��ݒ�
#define INI_KEY_CAMERA_BOOT_COLUMN_NUM                  3                               // 
#define INI_KEY_CAMERA_BOOT                             L"CAM_BOOT"                     // �J�����̋N���ݒ�

#define INI_SCT_MOUNTING                                L"MOUNTING"                     // ��t���@�ݒ�
#define INI_KEY_MOUNTING_OFFSET_COLUMN_NUM              2                               // 
#define INI_KEY_MOUNTING_OFFSET_LX0                     L"MNT_OFFSET_LX0"               // �݋�ݓ_�`BOX�ݓ_����LX0
#define INI_KEY_MOUNTING_OFFSET_LY0                     L"MNT_OFFSET_LY0"               // �݋�ݓ_�`BOX�ݓ_����LY0
#define INI_KEY_MOUNTING_OFFSET_L0                      L"MNT_OFFSET_L0"                // BOX�ݓ_�`BOX�������S����L0
#define INI_KEY_MOUNTING_OFFSET_LC                      L"MNT_OFFSET_LC"                // BOX�������S�`�J�������S����LC
#define INI_KEY_MOUNTING_OFFSET_A0                      L"MNT_OFFSET_A0"                // BOX���J������t�p�x��0
#define INI_KEY_MOUNTING_OFFSET_AC                      L"MNT_OFFSET_AC"                // BOX�������S�`�J�������S�p�x��c

#define INI_SCT_INCLINOMETER                            L"INCLINOMETER"                 // �X�Όv�ݒ�
#define INI_KEY_INCLINOMETER_DEVICE_COLUMN_NUM          4                               // 
#define INI_KEY_INCLINOMETER_DEVICE                     L"INC_DEVICE"                   // �f�o�C�X�ݒ�
#define INI_KEY_INCLINOMETER_PORT_NUMBER_COLUMN_NUM     2                               // 
#define INI_KEY_INCLINOMETER_PORT_NUMBER                L"INC_PORT_NUMBER"              // �X�Όv�f�[�^�ڑ��|�[�g�ԍ�

#define INI_SCT_IMGPROC                                 L"IMGPROC"                      // �摜���������ݒ�
#define INI_KEY_IMGPROC_ROI_COLUMN_NUM                  2                               // 
#define INI_KEY_IMGPROC_ROI                             L"IMG_ROI"                      // �摜ROI�ݒ�
#define INI_KEY_IMGPROC_MASK                            L"IMG_MASK"                     // �}�X�N�摜�I��
#define INI_KEY_IMGPROC_MASK_LIMIT_COLUMN_NUM           3                               // 
#define INI_KEY_IMGPROC_MASK1_LOW                       L"IMG_MASK1_LOW"                // �摜1�}�X�N����
#define INI_KEY_IMGPROC_MASK1_UP                        L"IMG_MASK1_UP"                 // �摜1�}�X�N���
#define INI_KEY_IMGPROC_MASK2_LOW                       L"IMG_MASK2_LOW"                // �摜2�}�X�N����
#define INI_KEY_IMGPROC_MASK2_UP                        L"IMG_MASK2_UP"                 // �摜2�}�X�N���
#define INI_KEY_IMGPROC_FILTER_COLUMN_NUM               2                               // 
#define INI_KEY_IMGPROC_FILTER1                         L"IMG_FILTER1"                  // �t�B���^1(�S�}���m�C�Y)
#define INI_KEY_IMGPROC_FILTER2                         L"IMG_FILTER2"                  // �t�B���^2(�����߃m�C�Y)
#define INI_KEY_IMGPROC_PROC                            L"IMG_PROC"                     // �^�[�Q�b�g���o�A���S���Y��
#define INI_KEY_IMGPROC_SWAY_SPEED_TC                   L"IMG_SWAY_SPEED_TC"            // �U�ꑬ�x�t�B���^���萔

//----------------------------------------------------------------------------
// 
// H, S, V
#define IMAGE_HSV_H_MIN                                 0
#define IMAGE_HSV_H_MAX                                 179
#define IMAGE_HSV_S_MIN                                 0
#define IMAGE_HSV_S_MAX                                 255
#define IMAGE_HSV_V_MIN                                 0
#define IMAGE_HSV_V_MAX                                 255

// RIO�G���[�r�b�g
#define RIO_ERR_NONE                                    0x00000000  // �G���[�Ȃ�
#define RIO_ERR_INIT_INCOMPLETE                         0x00000001  // RIO������������
#define RIO_ERR_SET_IOLINKMODE                          0x00000010  // IO LINK MODE�ݒ�G���[
#define RIO_ERR_SET_PARAM_VALID                         0x00000020  // �L�����ݒ�G���[
#define RIO_ERR_SET_PARAM_AI                            0x00000040  // AI�ݒ�G���[
#define RIO_ERR_GET_AI_READ                             0x00000100  // �f�[�^�ǂݍ��݃G���[����

// Process Data Error
#define S7CMPTBL_FORMAT_OVERRANGE                       0x7FFF      // Overrange
#define S7CMPTBL_FORMAT_UNDERRANGE                      0x8000      // Open circuit/short circuit/underrange
#define S7CMPTBL_FORMAT_SIGNBIT                         0x8000      // Sign bit

// �O������
#define EXTN_ROPELEN_MIN                                1000.0      // ���[�v��(�ŏ�)
#define EXTN_ROPELEN_MAX                                32000.0     // ���[�v��(�ő�)

#define EXTN_CAMBOXANGL_MIN                             -4500.0     // �J����BOX�X��(�ŏ�)
#define EXTN_CAMBOXANGL_MAX                             4500.0      // �J����BOX�X��(�ő�)

// �V���b�^�R���g���[�����[�h
#define	EXPOSURE_CONTROL_HOLD                           0           // �V���b�^�R���g���[��:Hold
#define	EXPOSURE_CONTROL_LIGHT                          1           // �V���b�^�R���g���[��:���邭
#define	EXPOSURE_CONTROL_DARKEN                         -1          // �V���b�^�R���g���[��:�Â�

//============================================================================
// enum��`
//----------------------------------------------------------------------------
// �J�����摜�z�� INDEX
enum {
    IMAGE_ID_RAW_A = 0, // ���摜A
    IMAGE_ID_RAW_B,     // ���摜B
    IMAGE_ID_MASK1_A,   // �}�X�N�摜1A
    IMAGE_ID_MASK1_B,   // �}�X�N�摜1B
    IMAGE_ID_MASK2_A,   // �}�X�N�摜2A
    IMAGE_ID_MASK2_B,   // �}�X�N�摜2B
    IMAGE_ID_PROC_A,    // �����摜A
    IMAGE_ID_PROC_B,    // �����摜B
    IMAGE_ID_MAX
};

// �摜�����z�� INDEX
enum {
    IMGPROC_ID_IMG_1 = 0,   // �摜1�����f�[�^
    IMGPROC_ID_IMG_2,       // �摜2�����f�[�^
    IMGPROC_ID_MAX
};

// �X�Όv���̓f�[�^
enum {
    RIO_PORT_1 = 0, // RIO���̓|�[�g1 
    RIO_PORT_2,     // RIO���̓|�[�g2 
    RIO_PORT_MAX
};

// �J��������p�p�����[�^
enum {
    GRAB_IMG_STOP = 0,      // �摜�捞��(��~)
    GRAB_IMG_GRAB_CAMERA,   // �摜�捞��(�J����)
    GRAB_IMG_GRAB_FILE      // �摜�捞��(�t�@�C��)
};

// �}�X�N�摜�I��
enum {
    MASK_IMG_ALL = 0,   // �}�X�N�摜�I��(����)
    MASK_IMG_IMAGE1 ,   // �}�X�N�摜�I��(�摜1�̂�)
    MASK_IMG_IMAGE2,    // �}�X�N�摜�I��(�摜2�̂�)
};

// �m�C�Y�t�B���^
enum {
    NOISEFILTER1_NONE = 0,  // �m�C�Y�t�B���^:�Ȃ�
    NOISEFILTER1_MEDIAN,    // �m�C�Y�t�B���^:�����l�t�B���^
    NOISEFILTER1_OPENNING,  // �m�C�Y�t�B���^:�I�[�v�j���O����
    NOIZEFILTER1_MAX
};

// �m�C�Y�t�B���^
enum {
    NOISEFILTER2_NONE = 0,  // �m�C�Y�t�B���^:�Ȃ�
    NOISEFILTER2_CLOSING,   // �m�C�Y�t�B���^:�N���[�W���O����
    NOIZEFILTER2_MAX
};

// �d�S�ʒu�Z�o�A���S���Y��
enum {
    COG_ALGORITHM_ALL = 0,  // �d�S�ʒu�Z�o�A���S���Y��(�S�֊s�_)
    COG_ALGORITHM_AREA,     // �d�S�ʒu�Z�o�A���S���Y��(�ő�֊s�ʐ�)
    COG_ALGORITHM_LEN,      // �d�S�ʒu�Z�o�A���S���Y��(�ő�֊s��)
    COG_ALGORITHM_MAX
};

// ��
enum {
    AXIS_X = 0, // X��
    AXIS_Y,     // Y��
    AXIS_MAX
};

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
// .ini�t�@�C�����̍\����
//----------------------------------------------------------------------------
// ���ʐݒ�
typedef struct TAG_CONFIG_COMMON {  // ���ʐݒ�
    unsigned int cycletime_sys;     // ���C���X���b�h���s����[msec]
    unsigned int img_source;        // �摜�捞�݌�(�J����[0]/�摜�t�@�C��[1]) 
    std::wstring imgfname_source;   // �捞�݉摜�t�@�C����(IMAGE_SOURCE=1�̂Ƃ��̉摜)
    std::wstring imgfname_output;   // �摜�ۑ��t�@�C����

    TAG_CONFIG_COMMON()
        : cycletime_sys(0)
        , img_source(0) 
        , imgfname_source(L"")
        , imgfname_output(L"")
    {}
} CONFIG_COMMON, *PCONFIG_COMMON;

// �^�X�N�ݒ�
typedef struct TAG_CONFIG_TASK_DATA {   // �^�X�N�ݒ�(�f�[�^)
    unsigned int cycletime;             // ���C���X���b�h���s����[msec]
    std::wstring name;                  // �^�X�N��
    std::wstring sname;                 // �^�X�N����

    TAG_CONFIG_TASK_DATA()
        : cycletime(0)
        , name(L"")
        , sname(L"")
    {}
} CONFIG_TASK_DATA, *PCONFIG_TASK_DATA;
typedef struct TAG_CONFIG_TASK {    // �^�X�N�ݒ�
    CONFIG_TASK_DATA environment;   // Environment
    CONFIG_TASK_DATA policy;        // Policy
    CONFIG_TASK_DATA agent;         // Agent
    CONFIG_TASK_DATA scada;         // Scada
} CONFIG_TASK, *PCONFIG_TASK;

// �ʐM�ݒ�
typedef struct TAG_CONFIG_ETHERNET_DATA {   // �ʐM�ݒ�(�f�[�^)
    std::wstring ipaddress;                 // IP�A�h���X
    unsigned int port;                      // �|�[�g�ԍ�

    TAG_CONFIG_ETHERNET_DATA()
        : ipaddress(L"")
        , port(0)
    {}
} CONFIG_ETHERNET_DATA, *PCONFIG_ETHERNET_DATA;
typedef struct TAG_CONFIG_SYS_ETHERNET {    // �ʐM�ݒ�
    TAG_CONFIG_ETHERNET_DATA local;         // Local�ݒ�
    TAG_CONFIG_ETHERNET_DATA remote;        // Remote�ݒ�
} CONFIG_ETHERNET, *PCONFIG_ETHERNET;

// �J�����ݒ�
// ROI�ݒ�
typedef struct TAG_CONFIG_CAMERA_ROI {  // ROI�ݒ�
    uint32_t offset;                    // ROI(�̈�)�̊J�n�ʒu(X, Y)
    uint32_t size;                      // ROI(�̈�)�̃T�C�Y(X(32�̔{��, 2592�ȉ�), Y(2�̔{��, 2048�ȉ�))

    TAG_CONFIG_CAMERA_ROI()
        : offset(0)
        , size(0)
    {}
} CONFIG_CAMERA_ROI, *PCONFIG_CAMERA_ROI;
// �z���C�g�o�����X�Q�C���ݒ�
typedef struct TAG_CONFIG_CAMERA_BALANCE_WHITE {    // �z���C�g�o�����X�Q�C���ݒ�
    unsigned int wb_auto;                           // �z���C�g�o�����X�Q�C�������������[�ݒ�(��������OFF[1]/�A����������[2]/���̂ݎ�������[3])
    double       wb_ratio_red;                      // �z���C�g�o�����X�Q�C���ݒ�(Red Gain)
    double       wb_ratio_blue;                     // �z���C�g�o�����X�Q�C���ݒ�(Blue Gain)

    TAG_CONFIG_CAMERA_BALANCE_WHITE()
        : wb_auto(0)
        , wb_ratio_red(0.0)
        , wb_ratio_blue(0.0)
    {}
} CONFIG_CAMERA_BALANCE_WHITE, *PCONFIG_CAMERA_BALANCE_WHITE;
// �J�����̊�{�ݒ�
typedef struct TAG_CONFIG_CAMERA_BASIS {                // �J�����̊�{�ݒ�
    std::wstring                ipaddress;              // IP�A�h���X
    CONFIG_CAMERA_ROI           roi[AXIS_MAX];          // ROI(�̈�)
    uint32_t                    packetsize;             // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y[byte]
    double                      framerate;              // �t���[�����[�g[fps]
    double                      blacklevel;             // �����x���ݒ�
    double                      gamma;                  // �K���}�␳
    CONFIG_CAMERA_BALANCE_WHITE wb;                     // �z���C�g�o�����X�Q�C���ݒ�
    double                      viewangle[AXIS_MAX];    // ����p[deg]

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
typedef struct TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL {   // �P�x�R���g���[���ݒ�
    double val;                                         // �ݒ�l
    double minval;                                      // �ŏ��l
    double maxval;                                      // �ő�l

    BOOL   atcontrol;                                   // �����R���g���[��(�Œ�[0]/����[1])
    double atrate;                                      // �����R���g���[���ω���
    double atstart_h;                                   // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    double atstart_l;                                   // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    double atstop_h;                                    // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    double atstop_l;                                    // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL()
        : val(0.0)
        , minval(0.0)
        , maxval(0.0)
        , atcontrol(FALSE)
        , atrate(0.0)
        , atstart_h(0.0)
        , atstart_l(0.0)
        , atstop_h(0.0)
        , atstop_l(0.0)
    {}
} CONFIG_CAMERA_BRIGHTNESS_CONTROL, *PCONFIG_CAMERA_BRIGHTNESS_CONTROL;
// �J�����ُ̈픻��ݒ�
typedef struct TAG_CONFIG_CAMERA_ERROR {    // �J�����ُ̈픻��ݒ�
    double       framedrop;                 // �t���[�����[�g�ቺ�̔���l[fps]
    unsigned int errtime;                   // �ُ픻�莞��[msec]

    TAG_CONFIG_CAMERA_ERROR()
        : framedrop(0.0)
        , errtime(0)
    {}
} CONFIG_CAMERA_ERROR, *PCONFIG_CAMERA_ERROR;
// �J�����̋N���ݒ�
typedef struct TAG_CONFIG_CAMERA_BOOT { // �J�����̋N���ݒ�
    unsigned int bootwaittimer;         // �V�X�e���N����̃J�����N���҂�����[msec]
    unsigned int rebootcount;           // �V�X�e���N����̃J�����N�����s���ɃJ���������ċN�������[count]
    unsigned int rebootingtimer;        // �J�����ċN�����ł��邱�Ƃ�ʒm���鎞��[msec]

    TAG_CONFIG_CAMERA_BOOT()
        : bootwaittimer(0)
        , rebootcount(0)
        , rebootingtimer(0)
    {}
} CONFIG_CAMERA_BOOT, *PCONFIG_CAMERA_BOOT;
typedef struct TAG_CONFIG_CAMERA {              // �J�����ݒ�
    CONFIG_CAMERA_BASIS              basis;     // �J�����̊�{�ݒ�
    CONFIG_CAMERA_BRIGHTNESS_CONTROL gain;      // �P�x�R���g���[���ݒ�(�Q�C��)
    CONFIG_CAMERA_BRIGHTNESS_CONTROL expstime;  // �P�x�R���g���[���ݒ�(�I������)
    CONFIG_CAMERA_ERROR              error;     // �J�����ُ̈픻��ݒ�
    CONFIG_CAMERA_BOOT               boot;      // �J�����̋N���ݒ�
} CONFIG_CAMERA, *PCONFIG_CAMERA;

// �\����`�ݒ�
// ��t�I�t�Z�b�g�ݒ�
typedef struct TAG_CONFIG_MOUNTING_OFFSET { // ��t�I�t�Z�b�g�ݒ�
    double lx0;                             // �݋�ݓ_�`BOX�ݓ_����LX0[mm]
    double ly0;                             // �݋�ݓ_�`BOX�ݓ_����LY0[mm]
    double l0;                              // BOX�ݓ_�`BOX�������S����L0[mm]
    double lc;                              // BOX�������S�`�J�������S����LC[mm]
    double a0;                              // BOX���J������t�p�x��0[deg]
    double ac;                              // BOX�������S�`�J�������S�p�x��c[deg]

    TAG_CONFIG_MOUNTING_OFFSET()
        : lx0(0.0)
        , ly0(0.0)
        , l0(0.0)
        , lc(0.0)
        , a0(0.0)
        , ac(0.0)
    {}
} CONFIG_MOUNTING_OFFSET, *PCONFIG_MOUNTING_OFFSET;
typedef struct TAG_CONFIG_MOUNTING {            // �\����`�ݒ�
    CONFIG_MOUNTING_OFFSET offset[AXIS_MAX];    // ��t�I�t�Z�b�g�ݒ�
} CONFIG_MOUNTING, *PCONFIG_MOUNTING;

// �X�Όv�ݒ�
typedef struct TAG_CONFIG_INCLINOMETER {    // �X�Όv�ݒ�
    std::wstring ipaddress;                 // IP�A�h���X
    unsigned int tcpport;                   // TCP�|�[�g�ԍ�
    unsigned int slaveaddress;              // �X���[�u�A�h���X
    unsigned int timeout;                   // �ʐM�^�C���A�E�g[msec]
    unsigned int port[AXIS_MAX];            // �X�Όv�f�[�^�ڑ��|�[�g�ԍ�[1�`8] (X, Y) 

    TAG_CONFIG_INCLINOMETER()
        : ipaddress(L"")
        , tcpport(0)
        , slaveaddress(0)
        , timeout(0)
        , port{0, 0}
    {}
} CONFIG_INCLINOMETER, *PCONFIG_INCLINOMETER;

// �摜���������ݒ�
// ROI�ݒ�
typedef struct TAG_CONFIG_IMGPROC_ROI { // ROI�ݒ�
    BOOL   valid;                       // ROI�L���ݒ�(����[FALSE]/�L��[TRUE])
    double scale;                       // ROI�X�P�[��(���o�����^�[�Q�b�g�ɑ΂���{��)

    TAG_CONFIG_IMGPROC_ROI()
        : valid(FALSE)
        , scale(0.0)
    {}
} CONFIG_IMGPROC_ROI, *PCONFIG_IMGPROC_ROI;
// HSV�}�X�N����l
typedef struct TAG_CONFIG_IMGPROC_HSV { // HSV�}�X�N����l
    unsigned int h;                     // H
    unsigned int s;                     // S
    unsigned int v;                     // V

    TAG_CONFIG_IMGPROC_HSV()
        : h(0)
        , s(0)
        , v(0)
    {}
} CONFIG_IMGPROC_HSV, *PCONFIG_IMGPROC_HSV;
// �m�C�Y�t�B���^
typedef struct TAG_CONFIG_IMGPROC_FILTER {  // �m�C�Y�t�B���^
    unsigned int type;                      // �t�B���^���
    unsigned int val;                       // �t�B���^�l

    TAG_CONFIG_IMGPROC_FILTER()
        : type(0)
        , val(0)
    {}
} CONFIG_IMGPROC_FILTER, *PCONFIG_IMGPROC_FILTER;
// �}�X�N�ݒ�
typedef struct TAG_CONFIG_IMGPROC_MASK {    // �}�X�N�ݒ�
    BOOL               valid;               // �}�X�N�I��(����[FALSE]/�L��[TRUE])
    CONFIG_IMGPROC_HSV hsv_l;               // HSV�}�X�N����l(����)
    CONFIG_IMGPROC_HSV hsv_h;               // HSV�}�X�N����l(���)

    TAG_CONFIG_IMGPROC_MASK()
        : valid(FALSE)
    {}
} CONFIG_IMGPROC_MASK, *PCONFIG_IMGPROC_MASK;
typedef struct TAG_CONFIG_IMGPROC {             // �摜���������ݒ�
    CONFIG_IMGPROC_ROI    roi;                  // ROI�ݒ�
    CONFIG_IMGPROC_MASK   mask[IMGPROC_ID_MAX]; // �}�X�N�ݒ�
    CONFIG_IMGPROC_FILTER fliter1;              // �S�}���m�C�Y�t�B���^(0=�Ȃ�, 1=�����l�t�B���^, 2=�I�[�v�j���O����), �t�B���^�l(�����l�t�B���^=1,3,5,... �I�[�v�j���O����=1,2,...)
    CONFIG_IMGPROC_FILTER fliter2;              // �����߃m�C�Y�t�B���^(0=�Ȃ�, 1=�N���[�W���O����), �t�B���^�l(1,2,...)
    unsigned int          imgproc;              // �^�[�Q�b�g���o�A���S���Y��(0=�S�֊s�_, 1=�ő�֊s�ʐ�, 2=�ő�֊s��)
    double                swayspdfliter;        // �U�ꑬ�x�t�B���^���萔

    TAG_CONFIG_IMGPROC()
        : imgproc(0)
        , swayspdfliter(0.0)
    {}
} CONFIG_IMGPROC, *PCONFIG_IMGPROC;

// �V�X�e���ݒ�
typedef struct TAG_APP_CONFIG {         // �V�X�e���ݒ�
    CONFIG_COMMON       common;         // ���ʐݒ�
    CONFIG_TASK         task;           // �^�X�N�ݒ�
    CONFIG_ETHERNET     ethernet;       // �ʐM�ݒ�
    CONFIG_CAMERA       camera;         // �ʐM�ݒ�
    CONFIG_MOUNTING     mounting;       // ��t���@�ݒ�
    CONFIG_INCLINOMETER inclinometer;   // �X�Όv�ݒ�
    CONFIG_IMGPROC      imageprocess;   // �摜���������ݒ�
} APP_CONFIG, *PAPP_CONFIG;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@

#if 0
// �O�����͐ݒ�
typedef struct _stExtnParamData                         // �O�����͐ݒ�f�[�^
{
    int    ropelenfixed;                                // ���[�v���Œ�
    double ropelen;                                     // ���[�v��

    int    boxanglefixed;                               // BOX�X���Œ�
    double boxangle[AXIS_MAX];                          // BOX�X��
} stExtnParamData;
typedef struct _stExtnParam                             // �O�����͐ݒ�
{
    CRITICAL_SECTION    cs;
    stExtnParamData     data;                           // �O�����͐ݒ�f�[�^
} stExtnParam;

// �摜���
typedef struct _stImageData
{
    cv::Mat image;
    BOOL    update;                                     // image set��true get��false
} stImageData;
typedef struct _stImageInfo
{
    CRITICAL_SECTION cs;
    stImageData      data;
} stImageInfo;

// �J�������
typedef struct _stCameraInfoData
{
    BOOL    valid;                              // �J�������
    double  cycleTime;                          // �摜�捞�݊Ԋu[ms]
} stCameraInfoData;
typedef struct _stCameraInfo
{
    CRITICAL_SECTION    cs;
    stCameraInfoData    data;
} stCameraInfo;

// RIO���
typedef struct _stInclinometerData              // �X�Όv�f�[�^
{
    int16_t dig;
    double  cur;                                // ���̓f�[�^�ϊ��l(mA)
    double  deg;                                // ���̓f�[�^�ϊ��l(deg.)
} stInclinometerData;
typedef struct _stRIOInfoData                   // RIO�f�[�^
{
    int32_t             error;                  // �G���[���
    stInclinometerData  incldata[AXIS_MAX];     // �X�Όv�f�[�^
} stRIOInfoData;
typedef struct _stRIOInfo
{
    CRITICAL_SECTION    cs;
    stRIOInfoData       data;
} stRIOInfo;

// ���o���
typedef struct _stImageProcData
{
    double      maxv;                           // �ő�P�x
    double      posx;                           // ���o�ʒuX
    double      posy;                           // ���o�ʒuY
    int         tgtsize;                        // ���o�T�C�Y
    cv::Rect    roi;                            // ROI
    BOOL        valid;                          // ���o���
} stImageProcData;
typedef struct _stSwayData                      // �U�ꌟ�o�f�[�^
{
    double  pos;                                // �U��ʒu
    double  deg;                                // �U��p[deg]
    double  rad;                                // �U��p[rad]
    double  spd;                                // �U��p���x
} stSwayData;
typedef struct _stProcInfoData
{
    stImageProcData imgprocdata[IMGPROC_ID_MAX];    // �摜��������
    stSwayData      swaydata[AXIS_MAX];             // �U�ꌟ�o�f�[�^ 
    BOOL            valid;                          // ���o���
    double          imgvalue ;                      // ���x
    int             expmode;                        // �����I���R�}���h(0:��~ 1:Up -1:Down)
    double          exptime;                        // �I������[us]
    double          proctime;                       // ��������[ms]
} stProcInfoData;
typedef struct _stProcInfo
{
    CRITICAL_SECTION    cs;
    stProcInfoData      data;
} stProcInfo;

// �O������
typedef struct _stExtnInfoData                      // �O�����̓f�[�^
{
    double  ropelen;                                // ���[�v��
    double  boxangle[AXIS_MAX];                     // BOX�X��
} stExtnInfoData;
typedef struct _stExtnInfo
{
    CRITICAL_SECTION    cs;
    stExtnInfoData      data;                       // �O�����̓f�[�^
} stExtnInfo;

// �O�����o��
#define ETHER_RECV_WORD_MAX     3                   // Number of Word (receive data)
#define ETHER_SEND_WORD_MAX     5                   // Number of Word (send    data)
#pragma pack(push, 1)                               // Set 1 byte boundary
typedef struct _stEtherRecvData                     // Network reveive data
{
    short word[ETHER_RECV_WORD_MAX];                // Receive data
} stEtherRecvData;
typedef struct _stEtherSendData                     // Network send data
{
    short word[ETHER_SEND_WORD_MAX];                // Send data
} stEtherSendData;
#pragma pack(pop)

// ��M�f�[�^���
typedef struct _stEtherRecvInfo
{
    CRITICAL_SECTION    cs;
    stEtherRecvData     data;
} stEtherRecvInfo;

// ���M�f�[�^���
typedef struct _stEtherSendInfo
{
    CRITICAL_SECTION    cs;
    stEtherSendData     data;
} stEtherSendInfo;

#endif

//////////////////////////////////////////////////////////////////////////////
// CShared

class CShared
{
public:
    CShared();
    ~CShared();

    BOOL set_appconfig(void);
    BOOL get_appconfig(PCONFIG_COMMON data);
    BOOL get_appconfig(PCONFIG_TASK data);
    BOOL get_appconfig(PCONFIG_ETHERNET data);
    BOOL get_appconfig(PCONFIG_CAMERA data);
    BOOL get_appconfig(PCONFIG_MOUNTING data);
    BOOL get_appconfig(PCONFIG_INCLINOMETER data);
    BOOL get_appconfig(PCONFIG_IMGPROC data);

protected:

private:
    WCHAR             m_path[_MAX_PATH];    // �A�v���P�[�V����.ini�t�@�C���̃p�X
    static APP_CONFIG m_appcnfig;
};
