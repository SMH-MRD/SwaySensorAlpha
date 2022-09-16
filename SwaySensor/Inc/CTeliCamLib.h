#pragma once

#include <windows.h>
#include <string>
#include <format>

#include "CCsAutoLock.h"
#include "TeliCamApi.h"     // For Teli Camera
#include "TeliCamUtl.h"     // For Teli Camera

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
//----------------------------------------------------------------------------
#define GEV_CAMERA_REMOVE_EVENT_WAITIME     2000    // �V�O�i����ԂɃZ�b�g�����܂őҋ@����^�C���A�E�g����[ms]
#define CHECK_FPS_INTERVAL_TIME             2000    // fps���`�F�b�N����Ԋu[ms]
#pragma endregion

//============================================================================
// �\����/���p�̒�`
#pragma region STRUCTURE_DEFINITION
typedef struct TAG_CAMERA_CONFIG {  // �J�����̐ݒ�
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
typedef struct TAG_CAMERA_STATUS {              // �J�����̃X�e�[�^�X
    int32_t               camidx;               // �J�����̃C���f�b�N�X
    Teli::CAM_HANDLE      camhndl;              // �I�[�v�������J�����̃J�����n���h��
    Teli::CAM_STRM_HANDLE strmhndl;             // �I�[�v�������X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
    Teli::CAM_API_STATUS  apistat;              // TeliCamAPI�̃X�e�[�^�X�R�[�h
    Teli::CAM_API_STATUS  errstat;              // �摜�X�g���[����M���̃G���[�X�e�[�^�X�R�[�h
    uint32_t              pyldsize;             // 1�̃X�g���[�����N�G�X�g�Ŏ�M����y�C���[�h�̃T�C�Y(�摜�T�C�Y)
    std::wstring          camname;              // �J�����̖��O
    Teli::CAM_INFO        caminfo;              // �J�����̏��
    uint32_t              camwidth;             // �f���̕�
    uint32_t              camheight;            // �f���̍���
    uint32_t              framecount;           // FPS���v�Z���邽�߂̃t���[���J�E���^�[
    uint32_t              frameidx;             // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
    BOOL                  frameidx_valid;       // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
    uint32_t              fpstimer;             // �t���[���X�V�^�C�}�[
    double                fps;                  // ���ۂ�FPS
    BOOL                  framechk_valid;       // �t���[�����[�g�����`�F�b�N
    float64_t             expstime_min;         // �I������(�ŏ�)
    float64_t             expstime_max;         // �I������(�ő�)
    float64_t             expstime;             // �I������(�ő�)
    float64_t             gainmin;              // �Q�C��(�ŏ�)
    float64_t             gainmax;              // �Q�C��(�ő�)
    float64_t             gain;                 // �Q�C��

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
typedef struct TAG_CAMERA_DETAILS {     // �J�����̏ڍ׏��
    HANDLE EventHndlCamRemoval;         // �J�������O���ʒm�p�̃C�x���g(�V�O�i��)�I�u�W�F�N�g�̃n���h��
    //
    HANDLE ThreadHndlCamRemoval;        // �X���b�h�n���h��
    BOOL   ThreadStatCamRemoval;        // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    HANDLE ThreadHndlGainControl;       // �X���b�h�n���h��
    BOOL   ThreadStatGainControl;       // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    HANDLE ThreadHndlExpsTimeControl;   // �X���b�h�n���h��
    BOOL   ThreadStatExpsTimeControl;   // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    //
    CAMERA_CONFIG cnfg;                 // �J�����̐ݒ�
    CAMERA_STATUS stat;                 // �J�����̃X�e�[�^�X

    TAG_CAMERA_DETAILS()
        : EventHndlCamRemoval(NULL)
        , ThreadHndlCamRemoval(NULL)
        , ThreadStatCamRemoval(FALSE)
        , ThreadHndlGainControl(NULL)
        , ThreadStatGainControl(FALSE)
        , ThreadHndlExpsTimeControl(NULL)
        , ThreadStatExpsTimeControl(FALSE)
        , cnfg()
        , stat()
    {}
} CAMERA_DETAILS, *PCAMERA_DETAILS;
typedef struct TAG_CAMERA_INFO {    // �J�����̏��
    uint32_t       camcount;        // ���o�����J�����̐�
    CAMERA_DETAILS details;         // �J�����̏ڍ׏��

    TAG_CAMERA_INFO()
        : camcount(0)
        , details()
    {}
} CAMERA_INFO, *PCAMERA_INFO;
#pragma endregion

//////////////////////////////////////////////////////////////////////////////
// CTeliCamLib

class CTeliCamLib
{
public:
    CTeliCamLib(void);
    CTeliCamLib(CAMERA_INFO caminfo);
    virtual ~CTeliCamLib(void);

    int32_t initialize(void);                               // TeliCamAPI�̏���������
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

    int32_t get_image(uint32_t bufsize, unsigned char* image);          // �摜�����擾
    int32_t get_camera_imagesize(uint32_t* width, uint32_t* height);    // �摜�T�C�Y�̎擾
    int32_t get_actual_framerate(float64_t* fps);                       // �t���[�����[�g�̎擾

    Teli::CAM_INFO get_stat_caminfo(void);  // �J�������̎擾
    std::wstring   get_cameraname(void);    // �J�������̎擾
    LPCWSTR        get_errormessage(void);  // �G���[���b�Z�[�W�̎擾

protected:

private:
    CMyCriticalSection m_cscam;     // Critical section (exclusion processing)
    CAMERA_INFO        m_caminfo;   // �J�����̏��
    std::wstring       m_errmsg;    // �G���[���b�Z�[�W

    static void CALLBACK callback_image_acquired(Teli::CAM_HANDLE hCam,
                                                 Teli::CAM_STRM_HANDLE hStrm,
                                                 Teli::PCAM_IMAGE_INFO psImageInfo,
                                                 uint32_t uiBufferIndex,
                                                 void* pvContext);  // ��M�����摜���������邽�߂̃R�[���o�b�N�֐�
    static void CALLBACK callback_error_image(Teli::CAM_HANDLE hCam,
                                              Teli::CAM_STRM_HANDLE hStrm,
                                              Teli::CAM_API_STATUS uiErrorStatus,
                                              uint32_t uiBufferIndex,
                                              void* pvContext);     // ��M�G���[���������邽�߂̃R�[���o�b�N�֐�

    static unsigned WINAPI thread_camera_remove(LPVOID lpParam);    // �J������O�����o�X���b�h�֐�(���g�p)
    static unsigned WINAPI thread_gain_control(LPVOID lpParam);     // �Q�C���ݒ�X���b�h�֐�
    static unsigned WINAPI thread_expstime_control(LPVOID lpParam); // �I�����Ԑݒ�X���b�h�֐�

    void    close(void);                            // TeliCamAPI�̏I������
    int32_t get_caminfo_camindex(uint32_t camidx);  // �J�������I�[�v�������Ƃ��̃J�����̃C���f�b�N�X�̎擾
};
