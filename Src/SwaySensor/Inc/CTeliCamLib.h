#pragma once

#include <windows.h>
#include <string>
#include <format>

#include "CCsAutoLock.h"
#include "TeliCam\TeliCamApi.h" // For Teli Camera
#include "TeliCam\TeliCamUtl.h" // For Teli Camera

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
//----------------------------------------------------------------------------
#define GEV_CAMERA_REMOVE_EVENT_WAITIME     2000    // �V�O�i����ԂɃZ�b�g�����܂őҋ@����^�C���A�E�g����[ms]
#define CHECK_FPS_INTERVAL_TIME             2000    // fps���`�F�b�N����Ԋu[ms]

#define IMAGE_SIZE_WIDTH                    2048    // �ő�摜�T�C�Y (������f)
#define IMAGE_SIZE_HEIGHT                   1536    // �ő�摜�T�C�Y (������f)
#define IMAGE_SIZE                          (IMAGE_SIZE_WIDTH * IMAGE_SIZE_HEIGHT)  // �ő�摜�T�C�Y
#define IMAGE_FORMAT_SIZE                   3       // �摜�t�H�[�}�b�g�T�C�Y(uint8_t * IMAGE_FORMAT_SIZE)[byte]
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum��`
#pragma region ENUM_DEFINITION
#pragma endregion ENUM_DEFINITION

//============================================================================
// �\����/���p�̒�`
#pragma region STRUCTURE_DEFINITION
// �J�����̐ݒ�
typedef struct TAG_CAMERA_CONFIG {
    BOOL         valid;             // �J�����̗L���܂��͖���[0:���� 1:�L��]
    std::wstring ipaddress;         // �J������IP�A�h���X
    uint32_t     packetsize;        // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y(�ʏ��0���w��)[byte]
    float64_t    framerate_drop;    // �t���[�����[�g�ቺ�̔���l[fps]

    TAG_CAMERA_CONFIG()
        : valid(FALSE)
        , ipaddress(L"0.0.0.0")
        , packetsize(0)
        , framerate_drop(0.0)
    {}
} CAMERA_CONFIG, *PCAMERA_CONFIG;
// �J�����̃X�e�[�^�X
typedef struct TAG_CAMERA_STATUS {
    int32_t               camidx;           // �J�����̃C���f�b�N�X
    Teli::CAM_HANDLE      camhndl;          // �I�[�v�������J�����̃J�����n���h��
    Teli::CAM_STRM_HANDLE strmhndl;         // �I�[�v�������X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
    Teli::CAM_API_STATUS  apistat;          // TeliCamAPI�̃X�e�[�^�X�R�[�h
    Teli::CAM_API_STATUS  errstat;          // �摜�X�g���[����M���̃G���[�X�e�[�^�X�R�[�h
    uint32_t              pyldsize;         // 1�̃X�g���[�����N�G�X�g�Ŏ�M����y�C���[�h�̃T�C�Y(�摜�T�C�Y)
    std::wstring          camname;          // �J�����̖��O
    Teli::CAM_INFO        caminfo;          // �J�����̏��
    uint32_t              camwidth;         // �f���̕�
    uint32_t              camheight;        // �f���̍���
    uint32_t              framecount;       // FPS���v�Z���邽�߂̃t���[���J�E���^�[
    uint32_t              frameidx;         // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
    BOOL                  frameidx_valid;   // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
    uint32_t              fpstimer;         // �t���[���X�V�^�C�}�[
    float64_t             fps;              // ���ۂ�FPS
    BOOL                  framechk_valid;   // �t���[�����[�g�����`�F�b�N
    float64_t             expstime_min;     // �I������(�ŏ�)
    float64_t             expstime_max;     // �I������(�ő�)
    float64_t             expstime;         // �I������(�ő�)
    float64_t             gainmin;          // �Q�C��(�ŏ�)
    float64_t             gainmax;          // �Q�C��(�ő�)
    float64_t             gain;             // �Q�C��

    TAG_CAMERA_STATUS()
        : camidx(0)
        , camhndl(NULL)
        , strmhndl(NULL)
        , apistat(Teli::CAM_API_STS_SUCCESS)
        , errstat(Teli::CAM_API_STS_SUCCESS)
        , pyldsize(0)
        , camname(L"Not found camera")
        , caminfo()
        , camwidth(0)
        , camheight(0)
        , framecount(0)
        , frameidx(0)
        , frameidx_valid(FALSE)
        , fpstimer(0)
        , fps(0.0)
        , framechk_valid(FALSE)
        , expstime_min(0.0)
        , expstime_max(0.0)
        , expstime(0.0)
        , gainmin(0.0)
        , gainmax(0.0)
        , gain(0.0)
    {}
} CAMERA_STATUS, *PCAMERA_STATUS;
// �J�����̏ڍ׏��
typedef struct TAG_CAMERA_DETAILS {
    HANDLE EventHndlCamRemoval; // �J�������O���ʒm�p�̃C�x���g(�V�O�i��)�I�u�W�F�N�g�̃n���h��
    //
    HANDLE td_camera_remove_hndl;       // �X���b�h�n���h��
    BOOL   td_camera_remove_stat;       // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    HANDLE td_gain_control_hndl;        // �X���b�h�n���h��
    BOOL   td_gain_control_stat;        // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    HANDLE td_expstime_control_hndl;    // �X���b�h�n���h��
    BOOL   td_expstime_control_stat;    // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    //
    CAMERA_CONFIG cnfg; // �J�����̐ݒ�
    CAMERA_STATUS stat; // �J�����̃X�e�[�^�X

    TAG_CAMERA_DETAILS()
        : EventHndlCamRemoval(NULL)
        , td_camera_remove_hndl(NULL)
        , td_camera_remove_stat(FALSE)
        , td_gain_control_hndl(NULL)
        , td_gain_control_stat(FALSE)
        , td_expstime_control_hndl(NULL)
        , td_expstime_control_stat(FALSE)
        , cnfg()
        , stat()
    {}
} CAMERA_DETAILS, *PCAMERA_DETAILS;
// �J�����̏��
typedef struct TAG_TELICAM_LIB_INFO {
    uint32_t       camcount;    // ���o�����J�����̐�
    CAMERA_DETAILS details;     // �J�����̏ڍ׏��

    TAG_TELICAM_LIB_INFO()
        : camcount(0)
        , details()
    {}
} TELICAM_LIB_INFO, *PTELICAM_LIB_INFO;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CTeliCamLib

class CTeliCamLib
{
public:
    CTeliCamLib(void);
    CTeliCamLib(TELICAM_LIB_INFO caminfo);
    virtual ~CTeliCamLib(void);

    // �����o�[�ϐ�

    // �����o�[�֐�
    int32_t initialize(void);                               // TeliCamAPI�̏���������
    void    close(void);                                    // TeliCamAPI�̏I������
    int32_t update_camera_list(void);                       // PC�ɐڑ�����Ă���J�����̒T��
    int32_t	open_camera(Teli::CAM_ACCESS_MODE accessmode);  // �J�����̃I�[�v��
    int32_t	close_camera(void);                             // �J�����̃N���[�Y
    int32_t open_stream(void);                              // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃I�[�v��
    int32_t close_stream(void);                             // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃N���[�Y
    int32_t start_stream(void);                             // �摜�X�g���[���̓]���J�n
    int32_t stop_stream(void);                              // �摜�X�g���[���̓]����~

    int32_t set_pixelformat(Teli::CAM_PIXEL_FORMAT pixelformat);                                        // �J�����̃r�f�I�X�g���[���̃s�N�Z���`���̐ݒ�
    int32_t set_camroi(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height);          // �J������ROI(�̈�)�̐ݒ�
    int32_t set_framerate(float64_t framerate, Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE ctrltype);            // �J�����̃t���[�����[�g�̐ݒ�
    int32_t set_triggermode(bool8_t triggermode);                                                       // �J�����̃g���K�[���샂�[�h�̐ݒ�
    int32_t set_blacklevel(float64_t blacklevel);                                                       // �J�����̍����x���̐ݒ�
    int32_t set_gamma(float64_t gamma);                                                                 // �J�����̃K���}�␳�l�̐ݒ�
    int32_t set_wbalance_auto(Teli::CAM_BALANCE_WHITE_AUTO_TYPE autotype);                              // �J�����̃z���C�g�o�����X�Q�C�������������[�h�̐ݒ�
    int32_t set_wbalance_ratio(float64_t wbratio, Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE selectortype);  // �J�����̃z���C�g�o�����X�Q�C��(�{��)�̐ݒ�
    int32_t set_gain_auto(Teli::CAM_GAIN_AUTO_TYPE autotype);                                           // �J������AGC(Automatic gain control)���샂�[�h�̐ݒ�
    int32_t set_gain(float64_t gain);                                                                   // �J�����̃Q�C���̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
    int32_t set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE ctrltype);                        // �J�����̘I�����Ԃ̐��䃂�[�h�̐ݒ�
    int32_t set_expstime(float64_t expstime);                                                           // �J�����̘I�����Ԃ̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)

    int32_t get_image(uint32_t bufsize, uint8_t* image);        // �摜�����擾
    int32_t get_image(void* image);                             // �摜�����擾
    int32_t get_image_size(uint32_t* width, uint32_t* height);  // �摜�T�C�Y�̎擾
    int32_t get_image_fps(float64_t* fps);                      // �t���[�����[�g�̎擾

    Teli::CAM_INFO get_stat_caminfo(void);  // �J�������̎擾
    std::wstring   get_camera_name(void);   // �J�������̎擾
    std::wstring   get_error_message(void); // �G���[���b�Z�[�W�̎擾

protected:

private:
    // �����o�[�ϐ�
    CMyCriticalSection m_cscam;     // Critical section (exclusion processing)
    TELICAM_LIB_INFO   m_caminfo;   // �J�����̏��
    std::wstring       m_errmsg;    // �G���[���b�Z�[�W

    // �����o�[�֐�
    int32_t get_caminfo_camindex(uint32_t camidx);  // �J�������I�[�v�������Ƃ��̃J�����̃C���f�b�N�X�̎擾

    static void CALLBACK cb_image_acquired(Teli::CAM_HANDLE hCam,
                                           Teli::CAM_STRM_HANDLE hStrm,
                                           Teli::PCAM_IMAGE_INFO psImageInfo,
                                           uint32_t uiBufferIndex,
                                           void* pvContext);    // ��M�����摜���������邽�߂̃R�[���o�b�N�֐�
    static void CALLBACK cb_error_image(Teli::CAM_HANDLE hCam,
                                        Teli::CAM_STRM_HANDLE hStrm,
                                        Teli::CAM_API_STATUS uiErrorStatus,
                                        uint32_t uiBufferIndex,
                                        void* pvContext);       // ��M�G���[���������邽�߂̃R�[���o�b�N�֐�

    static unsigned WINAPI td_camera_remove(LPVOID lpParam);    // �J������O�����o�X���b�h�֐�(���g�p)
    static unsigned WINAPI td_gain_control(LPVOID lpParam);     // �Q�C���ݒ�X���b�h�֐�
    static unsigned WINAPI td_expstime_control(LPVOID lpParam); // �I�����Ԑݒ�X���b�h�֐�
};
