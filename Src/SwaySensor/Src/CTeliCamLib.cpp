#include "CTeliCamLib.h"
#include "CHelper.h"
#include <mmsystem.h>
#include <process.h>

//////////////////////////////////////////////////////////////////////////////
// CTeliCamLib

#if defined(_M_X64) || defined(__amd64__)
#pragma comment(lib, "TeliCam\\x64\\TeliCamApi64.lib")
#pragma comment(lib, "TeliCam\\x64\\TeliCamUtl64.lib")
#else
#pragma comment(lib, "TeliCam\\x86\\TeliCamApi.lib")
#pragma comment(lib, "TeliCam\\x86\\TeliCamUtl.lib")
#endif

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief Constructor
/// @param
/// @return
/// @note
CTeliCamLib::CTeliCamLib(void)
{
    m_errmsg = L""; // �G���[���b�Z�[�W

    m_caminfo.camcount = 0; // ���o�����J�����̐�
    PCAMERA_DETAILS camdetails = &m_caminfo.details;                // �J�����̏ڍ׏��
    // �J������O���ʒm�C�x���g
    camdetails->EventHndlCamRemoval = NULL;                         // �J�������O���ʒm�p�̃C�x���g(�V�O�i��)�I�u�W�F�N�g�̃n���h��
    // �X���b�h���
    camdetails->td_camera_remove_hndl    = NULL;                    // �X���b�h�n���h��
    camdetails->td_camera_remove_stat    = FALSE;                   // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    camdetails->td_gain_control_hndl     = NULL;                    // �X���b�h�n���h��
    camdetails->td_gain_control_stat     = FALSE;                   // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    camdetails->td_expstime_control_hndl = NULL;                    // �X���b�h�n���h��
    camdetails->td_expstime_control_stat = FALSE;                   // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    // �J�����̐ݒ�
    camdetails->cnfg.valid          = FALSE;                        // �J�����̗L���܂��͖���[0:���� 1:�L��]
    camdetails->cnfg.ipaddress      = L"0.0.0.0";                   // �J������IP�A�h���X
    camdetails->cnfg.packetsize     = 0;                            // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y(�ʏ��0���w��)[byte]
    camdetails->cnfg.framerate_drop = 10.0;                         // �t���[�����[�g�ቺ�̔���l[fps]
    // �J�����̃X�e�[�^�X
    camdetails->stat.camidx         = -1;                           // �J�����̃C���f�b�N�X
    camdetails->stat.camhndl        = NULL;                         // �I�[�v�������J�����̃J�����n���h��
    camdetails->stat.strmhndl       = NULL;                         // �I�[�v�������X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
    camdetails->stat.apistat        = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPI�̃X�e�[�^�X�R�[�h
    camdetails->stat.errstat        = Teli::CAM_API_STS_SUCCESS;    // �摜�X�g���[����M���̃G���[�X�e�[�^�X�R�[�h
    camdetails->stat.pyldsize       = 0;                            // 1�̃X�g���[�����N�G�X�g�Ŏ�M����y�C���[�h�̃T�C�Y(�摜�T�C�Y)
    camdetails->stat.camname        = L"Not found camera";          // �J�����̖��O
    camdetails->stat.camwidth       = 0;                            // �f���̕�
    camdetails->stat.camheight      = 0;                            // �f���̍���
    camdetails->stat.framecount     = 0;                            // FPS���v�Z���邽�߂̃t���[���J�E���^�[
    camdetails->stat.frameidx       = 0;                            // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
    camdetails->stat.frameidx_valid = FALSE;                        // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
    camdetails->stat.fpstimer       = timeGetTime();                // �t���[���X�V�^�C�}�[
    camdetails->stat.fps            = 0.0;                          // ���ۂ�FPS
    camdetails->stat.framechk_valid = FALSE;                        // �t���[�����[�g�����`�F�b�N
    camdetails->stat.expstime_min   = 0.0;                          // �I������(�ŏ�)
    camdetails->stat.expstime_max   = 0.0;                          // �I������(�ő�)
    camdetails->stat.expstime       = 0.0;                          // �I������
    camdetails->stat.gainmin        = 0.0;                          // �Q�C��(�ŏ�)
    camdetails->stat.gainmax        = 0.0;                          // �Q�C��(�ő�)
    camdetails->stat.gain           = 0.0;                          // �Q�C��
    ZeroMemory(&camdetails->stat.caminfo, sizeof(Teli::CAM_INFO));  // �J�����̏��
}

/// @brief Constructor
/// @param caminfo �J�����̏��  
/// @return
/// @note
CTeliCamLib::CTeliCamLib(TELICAM_LIB_INFO caminfo)
{
    m_errmsg = L""; // �G���[���b�Z�[�W

    m_caminfo.camcount = 0;  // ���o�����J�����̐�
    PCAMERA_DETAILS camdetails = &m_caminfo.details;                // �J�����̏ڍ׏��
    // �J������O���ʒm�C�x���g
    camdetails->EventHndlCamRemoval = NULL;                         // �J�������O���ʒm�p�̃C�x���g(�V�O�i��)�I�u�W�F�N�g�̃n���h��
    // �X���b�h���
    camdetails->td_camera_remove_hndl    = NULL;                    // �X���b�h�n���h��
    camdetails->td_camera_remove_stat    = FALSE;                   // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    camdetails->td_gain_control_hndl     = NULL;                    // �X���b�h�n���h��
    camdetails->td_gain_control_stat     = FALSE;                   // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    camdetails->td_expstime_control_hndl = NULL;                    // �X���b�h�n���h��
    camdetails->td_expstime_control_stat = FALSE;                   // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)
    // �J�����̐ݒ�
    PCAMERA_CONFIG pCamCnfg = &caminfo.details.cnfg;                // �J�����̐ݒ�
    camdetails->cnfg.valid          = pCamCnfg->valid;              // �J�����̗L���܂��͖���[0:���� 1:�L��]
    camdetails->cnfg.ipaddress      = pCamCnfg->ipaddress;          // �J������IP�A�h���X
    camdetails->cnfg.packetsize     = pCamCnfg->packetsize;         // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y(�ʏ��0���w��)[byte]
    camdetails->cnfg.framerate_drop = pCamCnfg->framerate_drop;     // �t���[�����[�g�ቺ�̔���l[fps]
    // �J�����̃X�e�[�^�X
    camdetails->stat.camidx           = -1;                         // �J�����̃C���f�b�N�X
    camdetails->stat.camhndl        = NULL;                         // �I�[�v�������J�����̃J�����n���h��
    camdetails->stat.strmhndl       = NULL;                         // �I�[�v�������X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
    camdetails->stat.apistat        = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPI�̃X�e�[�^�X�R�[�h
    camdetails->stat.errstat        = Teli::CAM_API_STS_SUCCESS;    // �摜�X�g���[����M���̃G���[�X�e�[�^�X�R�[�h
    camdetails->stat.pyldsize       = 0;                            // 1�̃X�g���[�����N�G�X�g�Ŏ�M����y�C���[�h�̃T�C�Y(�摜�T�C�Y)
    camdetails->stat.camname        = L"Not found camera";          // �J�����̖��O
    camdetails->stat.camwidth       = 0;                            // �f���̕�
    camdetails->stat.camheight      = 0;                            // �f���̍���
    camdetails->stat.framecount     = 0;                            // FPS���v�Z���邽�߂̃t���[���J�E���^�[
    camdetails->stat.frameidx       = 0;                            // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
    camdetails->stat.frameidx_valid = FALSE;                        // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
    camdetails->stat.fpstimer       = timeGetTime();                // �t���[���X�V�^�C�}�[
    camdetails->stat.fps            = 0.0;                          // ���ۂ�FPS
    camdetails->stat.framechk_valid = FALSE;                        // �t���[�����[�g�����`�F�b�N 
    camdetails->stat.expstime_min   = 0.0;                          // �I������(�ŏ�)
    camdetails->stat.expstime_max   = 0.0;                          // �I������(�ő�)
    camdetails->stat.expstime       = 0.0;                          // �I������
    camdetails->stat.gainmin        = 0.0;                          // �Q�C��(�ŏ�)
    camdetails->stat.gainmax        = 0.0;                          // �Q�C��(�ő�)
    camdetails->stat.gain           = 0.0;                          // �Q�C��
    ZeroMemory(&camdetails->stat.caminfo, sizeof(Teli::CAM_INFO));  // �J�����̏��
}

/// @brief Destructor
/// @param
/// @return
/// @note
CTeliCamLib::~CTeliCamLib(void)
{
    //----------------------------------------------------------------------------
    // TeliCamAPI�̏I������
    close();
}

/// @brief TeliCamAPI�̏���������
/// @param 
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::initialize(void)
{
    Teli::CAM_API_STATUS apistat = Teli::CAM_API_STS_SUCCESS;   // TeliCamAPI�̃X�e�[�^�X�R�[�h

    m_errmsg = L""; // �G���[���b�Z�[�W
    //----------------------------------------------------------------------------
    // TeliCamAPI�̏���������
   if ((apistat = Teli::Sys_Initialize()) != Teli::CAM_API_STS_SUCCESS) {
        PCAMERA_DETAILS camdetails = &m_caminfo.details;    // �J�����̏ڍ׏��
        if (camdetails->cnfg.valid) {
            camdetails->stat.apistat = apistat;
        }
        m_errmsg = std::format(L"[CTeliCamLib::initialize]<Error>Teli::Sys_Initialize({:#08x})", apistat);
        return -1;
    }   // if ((apistat = Teli::Sys_Initialize()) != CAM_API_STS_SUCCESS)

    PCAMERA_DETAILS camdetails = &m_caminfo.details;    // �J�����̏ڍ׏��
    if (camdetails->cnfg.valid) {
        //----------------------------------------------------------------------------
        // �Q�C���R���g���[���@�X���b�h�N��
        camdetails->td_gain_control_stat = TRUE;
        if ((camdetails->td_gain_control_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                                        0,
                                                                                        td_gain_control,
                                                                                        reinterpret_cast<LPVOID>(camdetails),
                                                                                        0,
                                                                                        NULL))) == NULL) {
            camdetails->td_gain_control_stat = FALSE;
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::initialize]<Error>Thread(td_gain_control) handle");
            return -2;;
        }
        SetThreadPriority(camdetails->td_gain_control_hndl, THREAD_PRIORITY_NORMAL);

        //----------------------------------------------------------------------------
        // �I�����ԃR���g���[���@�X���b�h�N��
        camdetails->td_expstime_control_stat = TRUE;
        if ((camdetails->td_expstime_control_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                                            0,
                                                                                            td_expstime_control,
                                                                                            (LPVOID)camdetails,
                                                                                            0,
                                                                                            NULL))) == NULL) {
            camdetails->td_expstime_control_stat = FALSE;
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::initialize]<Error>Thread(td_expstime_control) handle");
            return -3;
        }
        SetThreadPriority(camdetails->td_expstime_control_hndl, THREAD_PRIORITY_NORMAL);
    }   // if (camdetails->cnfg.valid)

    return 0;
}

/// @brief TeliCamAPI�̏I������
/// @param 
/// @return Normal(0) or Abnormal(Not 0)
/// @note
void CTeliCamLib::close(void)
{
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPI�̃X�e�[�^�X�R�[�h
    PCAMERA_DETAILS      camdetails = &m_caminfo.details;           // �J�����̏ڍ׏��

    //----------------------------------------------------------------------------
    // End of thread
    // Exposure time control thread function
    if (camdetails->td_expstime_control_hndl != NULL) {
        if (camdetails->td_expstime_control_stat) {
            camdetails->td_expstime_control_stat = FALSE;
        	ResumeThread(camdetails->td_expstime_control_hndl);
	        WaitForSingleObject(camdetails->td_expstime_control_hndl, 500); // It waits until the thread ends.
        }
        CloseHandle(camdetails->td_expstime_control_hndl);  // Handle is closed.
        camdetails->td_expstime_control_hndl = NULL;
    }
    camdetails->td_expstime_control_stat = FALSE;

    // Gain control thread function
    if (camdetails->td_gain_control_hndl != NULL) {
        if (camdetails->td_gain_control_stat) {
            camdetails->td_gain_control_stat = FALSE;
        	ResumeThread(camdetails->td_gain_control_hndl);
	        WaitForSingleObject(camdetails->td_gain_control_hndl, 500); // It waits until the thread ends.
        }
        CloseHandle(camdetails->td_gain_control_hndl);  // Handle is closed.
        camdetails->td_gain_control_hndl = NULL;
    }
    camdetails->td_gain_control_stat = FALSE;

    // Camera remove thread function
    if (camdetails->td_camera_remove_hndl != NULL) {
        if (camdetails->td_camera_remove_stat) {
            camdetails->td_camera_remove_stat = FALSE;
	        ResumeThread(camdetails->td_camera_remove_hndl);
	        WaitForSingleObject(camdetails->td_camera_remove_hndl, GEV_CAMERA_REMOVE_EVENT_WAITIME + 500);   // It waits until the thread ends.
        }
        CloseHandle(camdetails->td_camera_remove_hndl);
        camdetails->td_camera_remove_hndl = NULL;   // Handle is closed.
    }
    camdetails->td_camera_remove_stat = FALSE;

    //----------------------------------------------------------------------------
    // �摜�X�g���[���̓]����~
    stop_stream();
    _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CTeliCamLib::close]", get_error_message());

    //----------------------------------------------------------------------------
    // Stream close
    close_stream();
    _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CTeliCamLib::close]", get_error_message());

    //----------------------------------------------------------------------------
    // Close camera
    close_camera();
    _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CTeliCamLib::close]", get_error_message());

    //----------------------------------------------------------------------------
    // Close a signal object
    if (camdetails->EventHndlCamRemoval != NULL) {
        if ((apistat = Teli::Sys_CloseSignal(camdetails->EventHndlCamRemoval)) != Teli::CAM_API_STS_SUCCESS) {
            _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::close]<Error>Teli::Sys_CloseSignal", apistat);
        }
        camdetails->EventHndlCamRemoval = NULL;
    }

    //----------------------------------------------------------------------------
    // Termination process of TeliCamAPI
    if ((apistat = Teli::Sys_Terminate()) != Teli::CAM_API_STS_SUCCESS) {
        _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::close]<Error>Teli::Sys_Terminate", apistat);
    }

    return ;
}

/// @brief PC�ɐڑ�����Ă���J�����̒T��
/// @param 
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::update_camera_list(void)
{
    int32_t              ret     = 0;
    Teli::CAM_API_STATUS apistat = Teli::CAM_API_STS_SUCCESS;   // TeliCamAPI�̃X�e�[�^�X�R�[�h

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    //----------------------------------------------------------------------------
    // �J�����̃X�e�[�^�X�̃N���A
    m_caminfo.details.stat.camidx         = -1;                             // �J�����̃C���f�b�N�X
    m_caminfo.details.stat.camhndl        = NULL;                           // �I�[�v�������J�����̃J�����n���h��
    m_caminfo.details.stat.strmhndl       = NULL;                           // �I�[�v�������X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
    m_caminfo.details.stat.apistat        = Teli::CAM_API_STS_SUCCESS;      // TeliCamAPI�̃X�e�[�^�X�R�[�h
    m_caminfo.details.stat.errstat        = Teli::CAM_API_STS_SUCCESS;      // �摜�X�g���[����M���̃G���[�X�e�[�^�X�R�[�h
    m_caminfo.details.stat.pyldsize       = 0;                              // 1�̃X�g���[�����N�G�X�g�Ŏ�M����y�C���[�h�̃T�C�Y(�摜�T�C�Y)
    m_caminfo.details.stat.camname        = L"Not found camera";            // �J�����̖��O
    m_caminfo.details.stat.camwidth       = 0;                              // �f���̕�
    m_caminfo.details.stat.camheight      = 0;                              // �f���̍���
    m_caminfo.details.stat.framecount     = 0;                              // FPS���v�Z���邽�߂̃t���[���J�E���^�[
    m_caminfo.details.stat.frameidx       = 0;                              // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
    m_caminfo.details.stat.frameidx_valid = FALSE;                          // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
    m_caminfo.details.stat.fpstimer       = timeGetTime();                  // �t���[���X�V�^�C�}�[
    m_caminfo.details.stat.fps            = 0.0;                            // ���ۂ�FPS
    m_caminfo.details.stat.expstime_min   = 0.0;                            // �I������(�ŏ�)
    m_caminfo.details.stat.expstime_max   = 0.0;                            // �I������(�ő�)
    m_caminfo.details.stat.expstime       = 0.0;                            // �I������(�ő�)
    m_caminfo.details.stat.gainmin        = 0.0;                            // �Q�C��(�ŏ�)
    m_caminfo.details.stat.gainmax        = 0.0;                            // �Q�C��(�ő�)
    m_caminfo.details.stat.gain           = 0.0;                            // �Q�C��
    ZeroMemory(&m_caminfo.details.stat.caminfo, sizeof(Teli::CAM_INFO));    // �J�����̏��

    //----------------------------------------------------------------------------
    // PC�ɐڑ�����Ă���J�����̒T��
    if ((apistat = Teli::Sys_GetNumOfCameras(&m_caminfo.camcount)) != Teli::CAM_API_STS_SUCCESS) {
        m_caminfo.details.stat.apistat = apistat;
        ret      = -1;
        m_errmsg = std::format(L"[CTeliCamLib::update_camera_list]<Error>Teli::Sys_GetNumOfCameras({:#08x})", apistat);
    }
    else
    {
        if (m_caminfo.camcount > 0) {
            Teli::CAM_INFO      teli_caminfo;   // �J�����̏��
            Teli::PGEV_CAM_INFO gevcaminfo;     // �J�����̏��(GEV)
            std::wstring        ipaddrs;        // Camera IP address
            for (uint32_t camidx = 0; camidx < m_caminfo.camcount; camidx++) {
                //----------------------------------------------------------------------------
                // �J�����̏��̎擾
                apistat = Teli::Cam_GetInformation(NULL, camidx, &teli_caminfo);
                if (apistat != Teli::CAM_API_STS_SUCCESS) {
                    continue;
                }
                else if (teli_caminfo.eCamType != Teli::CAM_TYPE_GEV) {
                    continue;
                }
                else {
                    ;
                }
                gevcaminfo = &teli_caminfo.sGevCamInfo; // �J�����̏��(GEV)

                //----------------------------------------------------------------------------
                // IP�A�h���X���L�[�Ɋ֘A�t����
                ipaddrs = std::format(L"{:d}.{:d}.{:d}.{:d}", gevcaminfo->aucIPAddress[0],
                                                              gevcaminfo->aucIPAddress[1],
                                                              gevcaminfo->aucIPAddress[2],
                                                              gevcaminfo->aucIPAddress[3]);
                
                if (ipaddrs.compare(m_caminfo.details.cnfg.ipaddress) == 0) {
                    m_caminfo.details.stat.camidx = camidx;             // �J�����̃C���f�b�N�X
                    std::string modelname(teli_caminfo.szModelName);    // �J�����̃��f����
                    m_caminfo.details.stat.camname = std::format(L"{:s}({:s})", CHelper::conv_string(modelname), ipaddrs);   // �J�����̖��O
                    memcpy(&m_caminfo.details.stat.caminfo, &teli_caminfo, sizeof(Teli::CAM_INFO)); // �J�����̏��
                    break;
                }
            }   // for (uint32_t camcount = 0; camcount < m_caminfo.camcount; camcount++)
        }   // if (m_caminfo.camcount > 0)
    }   // if ((apistat = Teli::Sys_GetNumOfCameras(&m_caminfo.camcount)) != Teli::CAM_API_STS_SUCCESS) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃I�[�v��
/// @param [in] accessmode - �J�����̃A�N�Z�X���[�h
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::open_camera(Teli::CAM_ACCESS_MODE accessmode)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    uint32_t       camwidth;    // �f���̕�
    uint32_t       camheight;   // �f���̍���
    if ((camstat->camidx < 0) || (camstat->camidx >= (int32_t)m_caminfo.camcount)) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::open_camera]<Error>camstat->camidx";
    }
    else
    {
        //----------------------------------------------------------------------------
        // �J�������I�[�v�����A�A�v���P�[�V�������J�������g�p�ł���悤�ɂ��܂��B
        if ((camstat->apistat = Teli::Cam_Open(camstat->camidx,                             // �J�����̃C���f�b�N�X
                                               &camstat->camhndl,                           // �I�[�v�������J�����̃J�����n���h��
                                               NULL,                                        // �J�������O���ʒm�p�̃C�x���g(�V�O�i��)�I�u�W�F�N�g�̃n���h��
                                               true,                                        // GenICam�A�N�Z�X�̗L���^����
                                               NULL,                                        // PC���̃J�����L�q���(XML�f�[�^)
                                               accessmode)) != Teli::CAM_API_STS_SUCCESS) { // �J�����̃A�N�Z�X���[�h
//          camstat->camhndl = NULL;
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::open_camera]<Error>Teli::Cam_Open({:#08x})", camstat->apistat);
        }
        else {
            //----------------------------------------------------------------------------
            // �J�����̉f���̕��̎擾
            if ((camstat->apistat = Teli::GetCamWidth(camstat->camhndl, &camwidth)) !=Teli:: CAM_API_STS_SUCCESS) {
                ret      = -3;
                m_errmsg = std::format(L"[CTeliCamLib::open_camera]<Error>Teli::GetCamWidth({:#08x})", camstat->apistat);
            }
            //----------------------------------------------------------------------------
            // �J�����̉f���̍����̎擾
            else if ((camstat->apistat = Teli::GetCamHeight(camstat->camhndl, &camheight)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -4;
                m_errmsg = std::format(L"[CTeliCamLib::open_camera]<Error>Teli::GetCamHeight({:#08x})", camstat->apistat);
            }
            else {
                ;
            }
        }
    }   // if ((camstat->camidx < 0) || (camstat->camidx >= m_caminfo.camcount)) else
    if (ret == 0) {
        camstat->camwidth  = camwidth;  // �f���̕�
        camstat->camheight = camheight; // �f���̍���
    }
    else {
        camstat->camwidth  = 0; // �f���̕�
        camstat->camheight = 0; // �f���̍���
    }
    CsLock.Release();   // Unlock
 
    return ret;
}

/// @brief �J�����̃N���[�Y
/// @param 
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::close_camera(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::close_camera]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̃N���[�Y
        if ((camstat->apistat = Teli::Cam_Close(camstat->camhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::close_camera]<Error>Teli::Cam_Close({:#08x})", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else
    camstat->camhndl = NULL;
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃I�[�v��
/// @param 
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::open_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::open_stream]<Error>camstat->camhndl";
    }
    else {
        PCAMERA_CONFIG camcnfg = &m_caminfo.details.cnfg;   // �J�����̐ݒ�
        //----------------------------------------------------------------------------
        // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃I�[�v��
        // TeliCamAPI�����ɉ摜�ꎞ�ۊǗp�̃X�g���[�����N�G�X�g�����O�o�b�t�@���쐬
        if ((camstat->apistat = Teli::Strm_OpenSimple(camstat->camhndl,                                     // �J�����̃J�����n���h��
                                                      &camstat->strmhndl,                                   // �I�[�v�������X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
                                                      &camstat->pyldsize,                                   // 1�̃X�g���[�����N�G�X�g�Ŏ�M����y�C���[�h�̃T�C�Y(�摜�T�C�Y)
                                                      NULL,                                                 // �X�g���[������M���A�X�g���[�����N�G�X�g�����O�o�b�t�@���X�V���ꂽ���Ƃ�ʒm����C�x���g�i�V�O�i���j�I�u�W�F�N�g�̃n���h��
                                                      DEFAULT_API_BUFFER_CNT,                               // TeliCamAPI�����ɍ쐬����X�g���[�����N�G�X�g�����O�o�b�t�@�̐�
                                                      camcnfg->packetsize)) != Teli::CAM_API_STS_SUCCESS) { // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y
//          camstat->strmhndl = NULL;
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::open_stream]<Error>Teli::Strm_OpenSimple({:#08x})", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // �R�[���o�b�N�֐���TeliCamAPI�ɓo�^
        // TeliCamAPI�����̃X�g���[�����N�G�X�g�����O�o�b�t�@�̓��e��
        // �����M�����摜�f�[�^�ōX�V�������ɃR�[���o�b�N�֐����Ăяo��
        else if ((camstat->apistat = Teli::Strm_SetCallbackImageAcquired(camstat->strmhndl,                                                         // �X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
                                                                         this,                                                                      // �R�[���o�b�N�֐������s����Ƃ��Ɉ����Ƃ��ēn���I�u�W�F�N�g�ւ̃|�C���^
                                                                         &CTeliCamLib::cb_image_acquired)) != Teli::CAM_API_STS_SUCCESS) {    // �R�[���o�b�N�֐�
            ret      = -3;
            m_errmsg = std::format(L"[CTeliCamLib::open_stream]<Error>Teli::Strm_SetCallbackImageAcquired({:#08x})", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // �R�[���o�b�N�֐���TeliCamAPI�ɓo�^
        // �X�g���[���𐳏�Ɏ�M�ł����ATeliCamAPI�����̃X�g���[�����N�G�X�g�����O�o�b�t�@�̓��e��
        // �G���[�X�V���ꂽ���ɃR�[���o�b�N�֐����Ăяo��
        else if ((camstat->apistat = Teli::Strm_SetCallbackImageError(camstat->strmhndl,                                                    // �X�g���[���C���^�[�t�F�[�X�̃X�g���[���n���h��
                                                                      this,                                                                 // �R�[���o�b�N�֐������s����Ƃ��Ɉ����Ƃ��ēn���I�u�W�F�N�g�ւ̃|�C���^
                                                                      &CTeliCamLib::cb_error_image)) != Teli::CAM_API_STS_SUCCESS) {  // �R�[���o�b�N�֐�
            ret      = -4;
            m_errmsg = std::format(L"[CTeliCamLib::open_stream]<Error>Teli::Strm_SetCallbackImageError({:#08x})", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃N���[�Y
/// @param 
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::close_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->strmhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::close_stream]<Error>camstat->strmhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X���N���[�Y
        if ((camstat->apistat = Teli::Strm_Close(camstat->strmhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::close_stream]<Error>Teli::Strm_Close({:#08x})", camstat->apistat);
        }
    }   // if (camstat->strmhndl == NULL) else
    camstat->strmhndl = NULL;
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �摜�X�g���[���̓]���J�n
/// @param 
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::start_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->strmhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::start_stream]<Error>camstat->strmhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �摜�X�g���[���̓]���J�n���J�����ɗv��
        if ((camstat->apistat = Teli::Strm_Start(camstat->strmhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::start_stream]<Error>Teli::Strm_Start({:#08x})", camstat->apistat);
        }
        camstat->framecount     = 0;                // FPS���v�Z���邽�߂̃t���[���J�E���^�[
        camstat->frameidx       = 0;                // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
        camstat->frameidx_valid = FALSE;            // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
        camstat->fpstimer       = timeGetTime();    // �t���[���X�V�^�C�}�[
        camstat->fps            = 0.0;              // ���ۂ�FPS
    }   // if (camstat->StrmHandl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �摜�X�g���[���̓]����~
/// @param 
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::stop_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->strmhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::stop_stream]<Error>camstat->strmhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �摜�X�g���[���̓]����~���J�����ɗv��
        if ((camstat->apistat = Teli::Strm_Stop(camstat->strmhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::stop_stream]<Error>Teli::Strm_Stop({:#08x})", camstat->apistat);
        }
    }   // if (camstat->hstrm == NULL) else
    camstat->framecount     = 0;        // FPS���v�Z���邽�߂̃t���[���J�E���^�[
    camstat->frameidx       = 0;        // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
    camstat->frameidx_valid = FALSE;    // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
    camstat->fps            = 0.0;      // ���ۂ�FPS
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃r�f�I�X�g���[���̃s�N�Z���`���̐ݒ�
/// @param [in] pixelformat - �s�N�Z���t�H�[�}�b�g(CAM_PIXEL_FORMAT)
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_pixelformat(Teli::CAM_PIXEL_FORMAT pixelformat)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_pixelformat]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̉f���X�g���[���̃s�N�Z���t�H�[�}�b�g��ݒ�
        if ((camstat->apistat = Teli::SetCamPixelFormat(camstat->camhndl,
                                                        pixelformat)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_pixelformat]<Error>Teli::SetCamPixelFormat({:#08x})", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J������ROI(�̈�)�̐ݒ�
/// @param [in] offset_x - �f���̐��������J�n�ʒu
/// @param [in] offset_y - �f���̐��������J�n�ʒu
/// @param [in] width - �f���̕�
/// @param [in] height - �f���̍���
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_camroi(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat   = &m_caminfo.details.stat; // �J�����̃X�e�[�^�X
    uint32_t       camwidth  = 0;                       // �f���̕�
    uint32_t       camheight = 0;                       // �f���̍���
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_camroi]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J������ROI(�̈�)��ݒ�
        if ((camstat->apistat = Teli::SetCamRoi(camstat->camhndl,
                                                width, height,
                                                offset_x, offset_y)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_camroi]<Error>Teli::SetCamRoi({:#08x})", camstat->apistat);
        }
        else {
            //----------------------------------------------------------------------------
            // �J�����̉f���̕����擾
            if ((camstat->apistat = Teli::GetCamWidth(camstat->camhndl,
                                                      &camwidth)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -3;
                m_errmsg = std::format(L"[CTeliCamLib::set_camroi]GetCamWidth:{:#08x}", camstat->apistat);
            }
            //----------------------------------------------------------------------------
            // �J�����̉f���̍������擾
            else if ((camstat->apistat = Teli::GetCamHeight(camstat->camhndl,
                                                            &camheight)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -4;
                m_errmsg = std::format(L"[CTeliCamLib::set_camroi]<Error>Teli::GetCamHeight({:#08x})", camstat->apistat);
            }
            else {
                ;
            }
        }
    }   // if (camstat->camhndl == NULL) else
    if (ret == 0) {
        camstat->camwidth  = camwidth;  // �f���̕�
        camstat->camheight = camheight; // �f���̍���
    }
    else {
        camstat->camwidth  = 0; // �f���̕�
        camstat->camheight = 0; // �f���̍���
    }
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃t���[�����[�g�̐ݒ�
/// @param [in] framerate - �t���[�����[�g
/// @param [in] ctrltype - �t���[�����[�g�ݒ�(CAM_ACQ_FRAME_RATE_CTRL_TYPE)
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_framerate(float64_t framerate, Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE ctrltype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_framerate]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̃t���[�����[�g��ݒ�
        if ((camstat->apistat = Teli::SetCamAcquisitionFrameRateControl(camstat->camhndl,
                                                                        ctrltype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_framerate]<Error>Teli::SetCamAcquisitionFrameRateControl({:#08x})", camstat->apistat);
        }
        else if ((camstat->apistat = Teli::SetCamAcquisitionFrameRate(camstat->camhndl,
                                                                      framerate)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -3;
            m_errmsg = std::format(L"[CTeliCamLib::set_framerate]<Error>Teli::SetCamAcquisitionFrameRate({:#08x})", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃g���K�[���샂�[�h�̐ݒ�
/// @param [in] triggermode - �g���K���샂�[�h(false:�g���K���샂�[�hOFF true:�g���K���샂�[�hON)
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_triggermode(bool8_t triggermode)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_triggermode]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̃g���K���샂�[�h(TriggerMode)��ݒ�
        if ((camstat->apistat = Teli::SetCamTriggerMode(camstat->camhndl,
                                                        triggermode)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_triggermode]<Error>Teli::SetCamTriggerMode({:#08x})", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else	
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̍����x���̐ݒ�
/// @param [in] blacklevel - �����x��
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_blacklevel(float64_t blacklevel)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_blacklevel]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̍����x���̍ŏ��l�ƍő�l���擾
        float64_t valmin, valmax;
        if ((camstat->apistat = Teli::GetCamBlackLevelMinMax(camstat->camhndl,
                                                             &valmin,
                                                             &valmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_blacklevel]<Error>Teli::GetCamBlackLevelMinMax({:#08x})", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // �J�����̍����x����ݒ�
        else {
            if (blacklevel < valmin) {
                blacklevel = valmin;
            }
            else if (blacklevel > valmax) {
                blacklevel = valmax;
            }
            else {
                ;
            }
            if ((camstat->apistat = Teli::SetCamBlackLevel(camstat->camhndl,
                                                           blacklevel)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -3;
                m_errmsg = std::format(L"[CTeliCamLib::set_blacklevel]<Error>Teli::SetCamBlackLevel({:#08x})", camstat->apistat);
            }
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃K���}�␳�l�̐ݒ�
/// @param [in] gamma - �K���}�␳�l
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_gamma(float64_t gamma)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_gamma]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̃K���}�␳�l�̍ŏ��l�ƍő�l���擾
        float64_t valmin, valmax;
        if ((camstat->apistat = Teli::GetCamGammaMinMax(camstat->camhndl,
                                                        &valmin,
                                                        &valmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_gamma]<Error>Teli::GetCamGammaMinMax({:#08x})", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // �J�����̃K���}�␳�l��ݒ�
        else {
            if (gamma < valmin) {
                gamma = valmin;
            }
            else if (gamma > valmax) {
                gamma = valmax;
            }
            else {
                ;
            }
            if ((camstat->apistat = Teli::SetCamGamma(camstat->camhndl,
                                                      gamma)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -3;
                m_errmsg = std::format(L"[CTeliCamLib::set_gamma]<Error>Teli::SetCamGamma({:#08x})", camstat->apistat);
            }
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃z���C�g�o�����X�Q�C�������������[�h�̐ݒ�
/// @param [in] autotype - �z���C�g�o�����X�Q�C�������������[�h(CAM_BALANCE_WHITE_AUTO_TYPE)
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_wbalance_auto(Teli::CAM_BALANCE_WHITE_AUTO_TYPE autotype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_wbalance_auto]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̃z���C�g�o�����X�Q�C�������������[�h��ݒ�
        if ((camstat->apistat = Teli::SetCamBalanceWhiteAuto(camstat->camhndl,
                                                             autotype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_wbalance_auto]<Error>Teli::SetCamBalanceWhiteAuto({:#08x})", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else	
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃z���C�g�o�����X�Q�C��(�{��)�̐ݒ�
/// @param [in] wbratio - �z���C�g�o�����X�Q�C��(�{��)
/// @param [in] selectortype - �z���C�g�o�����X�Q�C���ݒ�̑ΏۂƂȂ�v�f(CAM_BALANCE_RATIO_SELECTOR_TYPE)
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_wbalance_ratio(float64_t wbratio, Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE selectortype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_wbalance_ratio]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̃z���C�g�o�����X�Q�C�������������[�h��Manual�ɐݒ肳��Ă���Ƃ��́A�Q�C���̍ŏ��l�ƍő�l���擾
        float64_t valmin, valmax;
        if ((camstat->apistat = Teli::GetCamBalanceRatioMinMax(camstat->camhndl,
                                                               selectortype,
                                                               &valmin,
                                                               &valmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_wbalance_ratio]<Error>Teli::GetCamBalanceRatioMinMax({:#08x})", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // �J�����̃z���C�g�o�����X�Q�C��(�{��)��ݒ�
        else {
            if (wbratio < valmin) {
                wbratio = valmin;
            }
            else if (wbratio > valmax) {
                wbratio = valmax;
            }
            else {
                ;
            }
            if ((camstat->apistat = Teli::SetCamBalanceRatio(camstat->camhndl,
                                                             selectortype,
                                                             wbratio)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -3;
                m_errmsg = std::format(L"[CTeliCamLib::set_wbalance_ratio]<Error>Teli::SetCamBalanceRatio({:#08x})", camstat->apistat);
            }
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J������AGC(Automatic gain control)���샂�[�h�̐ݒ�
/// @param [in] autotype - AGC���샂�[�h(CAM_GAIN_AUTO_TYPE)
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_gain_auto(Teli::CAM_GAIN_AUTO_TYPE autotype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_gain_auto]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J������AGC(Automatic gain control)���샂�[�h��ݒ�
        if ((camstat->apistat = Teli::SetCamGainAuto(camstat->camhndl,
                                                     autotype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_gain_auto]<Error>Teli::SetCamGainAuto({:#08x})", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // �J������AGC���샂�[�h��Manual�ɐݒ肳��Ă���Ƃ��́A�Q�C���̍ŏ��l�ƍő�l���擾
        else if ((camstat->apistat = Teli::GetCamGainMinMax(camstat->camhndl,
                                                            &camstat->gainmin,
                                                            &camstat->gainmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -3;
            m_errmsg = std::format(L"[CTeliCamLib::set_gain_auto]<Error>Teli::GetCamGainMinMax({:#08x})", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->CamHNdl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̃Q�C���̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
/// @param [in] gain - �Q�C��
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_gain(float64_t gain)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_gain]<Error>camstat->camhndl";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI�̃G���[
        ret      = -2;
        m_errmsg = std::format(L"[CTeliCamLib::set_gain]<Error>camstat->apistat({:#08x})", camstat->apistat);
    }
    else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // �摜�X�g���[����M���̃G���[
        ret      = -3;
        m_errmsg = std::format(L"[CTeliCamLib::set_gain]<Error>camstat->errstat({:#08x})", camstat->errstat);
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̃Q�C����ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
        if (gain < camstat->gainmin) {
            gain = camstat->gainmin;
        }
        else if (gain > camstat->gainmax) {
            gain = camstat->gainmax;
        }
        else {
            ;
        }
        camstat->gain = gain;

        PCAMERA_DETAILS camdtail = &m_caminfo.details;  // �J�����̏ڍ׏��
        if (camdtail->td_gain_control_hndl != NULL) {
            ResumeThread(camdtail->td_gain_control_hndl);
        }
    }   // if (camstat->camhndl == NULL)
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̘I�����Ԃ̐��䃂�[�h�̐ݒ�
/// @param [in] ctrltype - �I�����Ԃ̐��䃂�[�h(CAM_EXPOSURE_TIME_CONTROL_TYPE)
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE ctrltype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // �J�����̃X�e�[�^�X
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_expstime_control]<Error>camstat->camhndl";
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̘I�����Ԃ̐��䃂�[�h��ݒ�
        if ((camstat->apistat = Teli::SetCamExposureTimeControl(camstat->camhndl,
                                                                ctrltype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[CTeliCamLib::set_expstime_control]<Error>Teli::SetCamExposureTimeControl({:#08x})", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // �J�����̘I�����Ԑ��䃂�[�h��Manual�ɐݒ肳��Ă���Ƃ��́A�I�����Ԃ̍ŏ��l�ƍő�l���擾
        else if ((camstat->apistat = Teli::GetCamExposureTimeMinMax(camstat->camhndl,
                                                                    &camstat->expstime_min,
                                                                    &camstat->expstime_max)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -3;
            m_errmsg = std::format(L"[CTeliCamLib::set_expstime_control]<Error>Teli::GetCamExposureTimeMinMax({:#08x})", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�����̘I�����Ԃ̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
/// @param [in] expstime - Exposure time
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::set_expstime(float64_t expstime)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::set_expstime]<Error>camstat->camhndl";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI�̃G���[
        ret      = -2;
        m_errmsg = std::format(L"[CTeliCamLib::set_expstime]<Error>camstat->apistat({:#08x})", camstat->apistat);
    }
    else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // �摜�X�g���[����M���̃G���[
        ret      = -3;
        m_errmsg = std::format(L"[CTeliCamLib::set_expstime]<Error>camstat->errstat({:#08x})", camstat->errstat);
    }
    else {
        //----------------------------------------------------------------------------
        // �J�����̘I�����Ԃ�ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
        if (expstime < camstat->expstime_min) {
            expstime = camstat->expstime_min;
        }
        else if (expstime > camstat->expstime_max) {
            expstime = camstat->expstime_max;
        }
        else {
            ;
        }
        camstat->expstime = expstime;
 
        PCAMERA_DETAILS camdtail = &m_caminfo.details;  // �J�����̏ڍ׏��
        if (camdtail->td_expstime_control_hndl != NULL) {
            ResumeThread(camdtail->td_expstime_control_hndl);
        }
    }
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �摜�����擾
/// @param [in] bufsize - Copy destination buffer size 
/// @param [out] image - Image pointer
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::get_image(uint32_t bufsize, uint8_t* image)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if (image == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::get_image]<Error>image";
    }
    else if ((camstat->camhndl  == NULL) || // Handle of camera assigned by TeliCamAPI on opening camera
             (camstat->strmhndl == NULL)) { // Handle of stream channel assigned by TeliCamAPI on opening stream channel
        ret      = -2;
        m_errmsg = L"[CTeliCamLib::get_image]<Error>camstat->camhndl/strmhndl";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI�̃G���[
        ret      = -3;
        m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>camstat->apistat error({:#08x})", camstat->apistat);
    }
    else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // �摜�X�g���[����M���̃G���[
        ret      = -4;
        m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>camstat->errstat({:#08x})", camstat->errstat);
    }
    else {
        if (!camstat->frameidx_valid) {
            ret      = -5;
            m_errmsg = L"[CTeliCamLib::get_image]<Error>camstat->frameidx_valid";
        }
        else {  // Image received
            uint32_t rbufidx;   // Buffer index of the acquired stream request ring buffer
            //----------------------------------------------------------------------------
            // Gets the buffer index of the stream request ring buffer inside TeliCam API that stores the latest stream request (image).
            if ((camstat->apistat = Teli::Strm_GetCurrentBufferIndex(camstat->strmhndl, &rbufidx)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -6;
                m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_GetCurrentBufferIndex({:#08x})", camstat->apistat);
            }
            else {
                Teli::CAM_IMAGE_INFO imginfo;   // Accompanying information of the image stored in the stream request ring buffer to be locked
                //----------------------------------------------------------------------------
                // Get image pointer in Ring Buffer.
                // Image data is locked in Strm_LockApiRingBufferPointer() method.
                if ((camstat->apistat = Teli::Strm_LockBuffer(camstat->strmhndl, rbufidx, &imginfo)) != Teli::CAM_API_STS_SUCCESS) {
                    ret      = -7;
                    m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_LockBuffer({:#08x})", camstat->apistat);
                }
                else if (imginfo.uiStatus != Teli::CAM_API_STS_SUCCESS) {
                    camstat->apistat = imginfo.uiStatus;    // TeliCamAPI status
                    ret      = -8;
                    m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_LockBuffer({:#08x})", imginfo.uiStatus);
                }
                else if (camstat->pyldsize > bufsize) {
                    ret      = -9;
                    m_errmsg = L"[CTeliCamLib::get_image]<Error>bufsize";
                }
                else {
                    CopyMemory(image, imginfo.pvBuf, camstat->pyldsize);
                    // �t���[�����[�g�����`�F�b�N
                    if (camstat->framechk_valid) {
                        PCAMERA_CONFIG camcnfg = &m_caminfo.details.cnfg;   // Camera cofig
                        if (camstat->fps <= camcnfg->framerate_drop) {
                            ret      = -10;
                            m_errmsg = L"[CTeliCamLib::get_image]<Error>Frame rate drop";
                        }
                    }
                }

                //----------------------------------------------------------------------------
                // Unlock current image
                Teli::CAM_API_STATUS apistat = Teli::Strm_UnlockBuffer(camstat->strmhndl, rbufidx);
                if (apistat != Teli::CAM_API_STS_SUCCESS) {
                    if (camstat->apistat == Teli::CAM_API_STS_SUCCESS) {
                        camstat->apistat = apistat; // TeliCamAPI�̃X�e�[�^�X�R�[�h
                    }
                    ret      = -11;
                    m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_UnlockBuffer({:#08x})", apistat);
                }
            }   // if ((camstat->apistat = Teli::Strm_GetCurrentBufferIndex(camstat->strmhndl, &rbufidx)) != Teli::CAM_API_STS_SUCCESS) else
        }   // if (!camstat->frameidx_valid) else
    }   // if (image == NULL) ... else

    //----------------------------------------------------------------------------
    // Check interval (fps)
    uint32_t       fpstimer = timeGetTime() - camstat->fpstimer;
    const uint32_t interval = CHECK_FPS_INTERVAL_TIME;  // fps���`�F�b�N����Ԋu[ms]
    if (fpstimer > interval) {
        camstat->fps            = ((float64_t)camstat->framecount / (float64_t)fpstimer) * 1000.0;
        camstat->framecount     = 0;
        camstat->fpstimer       = timeGetTime();
        camstat->framechk_valid = TRUE; // �t���[�����[�g�����`�F�b�N 
    }
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �摜�����擾
/// @param [out] image - Image pointer
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::get_image(void* image)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if (image == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::get_image]<Error>image";
    }
    else if ((camstat->camhndl  == NULL) || // Handle of camera assigned by TeliCamAPI on opening camera
             (camstat->strmhndl == NULL)) { // Handle of stream channel assigned by TeliCamAPI on opening stream channel
        ret      = -2;
        m_errmsg = L"[CTeliCamLib::get_image]<Error>camstat->camhndl/strmhndl";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI�̃G���[
        ret      = -3;
        m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>camstat->apistat({:#08x})", camstat->apistat);
    }
    else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // �摜�X�g���[����M���̃G���[
        ret      = -4;
        m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>camstat->errstat({:#08x})", camstat->errstat);
    }
    else {
        if (!camstat->frameidx_valid) {
            ret      = -5;
            m_errmsg = L"[CTeliCamLib::get_image]<Error>camstat->frameidx_valid";
        }
        else {  // Image received
            uint32_t rbufidx;   // Buffer index of the acquired stream request ring buffer
            //----------------------------------------------------------------------------
            // Gets the buffer index of the stream request ring buffer inside TeliCam API that stores the latest stream request (image).
            if ((camstat->apistat = Teli::Strm_GetCurrentBufferIndex(camstat->strmhndl, &rbufidx)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -6;
                m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_GetCurrentBufferIndex({:#08x})", camstat->apistat);
            }
            else {
                Teli::CAM_IMAGE_INFO imginfo;   // Accompanying information of the image stored in the stream request ring buffer to be locked
                //----------------------------------------------------------------------------
                // Get image pointer in Ring Buffer.
                // Image data is locked in Strm_LockApiRingBufferPointer() method.
                if ((camstat->apistat = Teli::Strm_LockBuffer(camstat->strmhndl, rbufidx, &imginfo)) != Teli::CAM_API_STS_SUCCESS) {
                    ret      = -7;
                    m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_LockBuffer({:#08x})", camstat->apistat);
                }
                else if (imginfo.uiStatus != Teli::CAM_API_STS_SUCCESS) {
                    camstat->apistat = imginfo.uiStatus;    // TeliCamAPI status
                    ret      = -8;
                    m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_LockBuffer({:#08x})", imginfo.uiStatus);
                }
                else if ((camstat->apistat = Teli::ConvByrBG8ToBGR(image, imginfo.pvBuf, imginfo.uiSizeX, imginfo.uiSizeY)) != Teli::CAM_API_STS_SUCCESS) {
                    ret      = -9;
                    m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::ConvByrBG8ToBGR({:#08x})", camstat->apistat);
                }
                else {
                    // �t���[�����[�g�����`�F�b�N
                    if (camstat->framechk_valid) {
                        PCAMERA_CONFIG camcnfg = &m_caminfo.details.cnfg;   // Camera cofig
                        if (camstat->fps <= camcnfg->framerate_drop) {
                            ret      = -10;
                            m_errmsg = L"[CTeliCamLib::get_image]<Error>Frame rate drop";
                        }
                    }
                }

                //----------------------------------------------------------------------------
                // Unlock current image
                Teli::CAM_API_STATUS apistat = Teli::Strm_UnlockBuffer(camstat->strmhndl, rbufidx);
                if (apistat != Teli::CAM_API_STS_SUCCESS) {
                    if (camstat->apistat == Teli::CAM_API_STS_SUCCESS) {
                        camstat->apistat = apistat; // TeliCamAPI�̃X�e�[�^�X�R�[�h
                    }
                    ret      = -11;
                    m_errmsg = std::format(L"[CTeliCamLib::get_image]<Error>Teli::Strm_UnlockBuffer({:#08x})", apistat);
                }
            }   // if ((camstat->apistat = Teli::Strm_GetCurrentBufferIndex(camstat->strmhndl, &rbufidx)) != Teli::CAM_API_STS_SUCCESS) else
        }   // if (!camstat->frameidx_valid) else
    }   // if (image == NULL) ... else

    //----------------------------------------------------------------------------
    // Check interval (fps)
    uint32_t       fpstimer = timeGetTime() - camstat->fpstimer;
    const uint32_t interval = CHECK_FPS_INTERVAL_TIME;  // fps���`�F�b�N����Ԋu[ms]
    if (fpstimer > interval) {
        camstat->fps            = ((float64_t)camstat->framecount / (float64_t)fpstimer) * 1000.0;
        camstat->framecount     = 0;
        camstat->fpstimer       = timeGetTime();
        camstat->framechk_valid = TRUE; // �t���[�����[�g�����`�F�b�N 
    }
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �摜�T�C�Y�̎擾
/// @param [out] width - Image width
/// @param [out] height - Image height
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::get_image_size(uint32_t* width, uint32_t* height)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if ((width == NULL) || (height == NULL)) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::get_image_size]<Error>width/height";
    }
    else if (camstat->camhndl == NULL) {    // Handle of camera assigned by TeliCamAPI on opening camera
        *width   = 0;
        *height  = 0;
        ret      = -2;
        m_errmsg = L"[CTeliCamLib::get_image_size]<Error>camstat->camhndl";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI�X�e�[�^�X�G���[
        *width   = 0;
        *height  = 0;
        ret      = -3;
        m_errmsg = std::format(L"[CTeliCamLib::get_image_size]<Error>camstat->apistat({:#08x})", camstat->apistat);
    }
    else {
        *width  = camstat->camwidth;    // �f���̕�
        *height = camstat->camheight;   // �f���̍���
    }
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �t���[�����[�g�̎擾
/// @param [out] fps - Actual frame rate
/// @return ����(0:���� 0�ȊO:���s)
/// @note
int32_t CTeliCamLib::get_image_fps(float64_t* fps)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // �G���[���b�Z�[�W

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if (fps == NULL) {
        ret      = -1;
        m_errmsg = L"[CTeliCamLib::get_image_fps]<Error>fps";
    }
    else if ((camstat->camhndl  == NULL) || // Handle of camera assigned by TeliCamAPI on opening camera
             (camstat->strmhndl == NULL)) { // Handle of stream channel assigned by TeliCamAPI on opening stream channel
        *fps     = 0.0;
        ret      = -2;
        m_errmsg = L"[CTeliCamLib::get_image_fps]<Error>camstat->camhndl/strmhndl";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI status
        *fps     = 0.0;
        ret      = -3;
        m_errmsg = std::format(L"[CTeliCamLib::get_image_fps]<Error>camstat->apistat({:#08x})", camstat->apistat);
    }
    else {
        *fps = camstat->fps;
    }
    CsLock.Release();   // Unlock

    return ret;
}

/// @brief �J�������̎擾
/// @param 
/// @return �J�����̏��
/// @note
Teli::CAM_INFO CTeliCamLib::get_stat_caminfo(void)
{
    return m_caminfo.details.stat.caminfo;
}

/// @brief �J�������̎擾
/// @param 
/// @return �J�����̖��O
/// @note
std::wstring CTeliCamLib::get_camera_name(void)
{
    return m_caminfo.details.stat.camname;
}

/// @brief �G���[���b�Z�[�W�̎擾
/// @param 
/// @return �G���[���b�Z�[�W
/// @note
std::wstring CTeliCamLib::get_error_message(void)
{
    return m_errmsg;
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief �J�������I�[�v�������Ƃ��̃J�����̃C���f�b�N�X�̎擾
/// @param [in] camidx - Camera index when the camera is opened
/// @return Camera index
/// @note
int32_t CTeliCamLib::get_caminfo_camindex(uint32_t camidx)
{
    int32_t val = -1;

    PCAMERA_DETAILS camdetails = &m_caminfo.details;   // Camera details
    if (camdetails->stat.camidx == camidx) {
        val = camidx;
    }

    return val;
}

/// @brief ��M�����摜���������邽�߂̃R�[���o�b�N�֐�
/// @param [in] hCam - Handler of camera
/// @param [in] hStrm - Handler of stream
/// @param [in] psImageInfo - Additional information of received image
/// @param [in] uiBufferIndex - Index of buffer
/// @param [in] pvContext - Additional caller-specified context
/// @return
/// @note
void CTeliCamLib::cb_image_acquired(Teli::CAM_HANDLE hCam, Teli::CAM_STRM_HANDLE hStrm, Teli::PCAM_IMAGE_INFO psImageInfo, uint32_t uiBufferIndex, void* pvContext)
{
    CTeliCamLib*         telicamlib = reinterpret_cast<CTeliCamLib*>(pvContext);
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPI�̃X�e�[�^�X�R�[�h
    uint32_t             camidx;                                    // Camera index when the camera is opened

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::cb_image_acquired]<Call>");
    //----------------------------------------------------------------------------
    // Get camera index from camera handle
    if ((apistat = Teli::GetCamIndexFromCamHandle(hCam, &camidx)) != Teli::CAM_API_STS_SUCCESS) {
        if (telicamlib->get_caminfo_camindex(camidx) >= 0) {
            telicamlib->m_caminfo.details.stat.apistat = apistat;   // TeliCamAPI�̃X�e�[�^�X�R�[�h
        }
        _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::cb_image_acquired]<Error>Teli::GetCamIndexFromCamHandle", apistat);
        return ;
    }

    //----------------------------------------------------------------------------
    // Dummy code for avoiding not refered warning of compliler. 
//  void *pdummy = (void*)&uiBufferIndex;
//  pdummy = &hStrm;

    //----------------------------------------------------------------------------
    // Insert image processing code here
    // Image processing which will take a long time should not be executed in this method

    if (telicamlib->get_caminfo_camindex(camidx) >= 0) {
        PCAMERA_STATUS camstat = &telicamlib->m_caminfo.details.stat;   // Camera status
        if (camstat->frameidx_valid) {
            camstat->framecount += ((uint32_t)(psImageInfo->ullBlockId) - camstat->frameidx);   // FPS���v�Z���邽�߂̃t���[���J�E���^�[
            camstat->frameidx    = static_cast<uint32_t>(psImageInfo->ullBlockId);              // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
        }
        else {
            camstat->frameidx_valid = TRUE;
            camstat->framecount++;
            camstat->frameidx = static_cast<uint32_t>(psImageInfo->ullBlockId); // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
        }
        //----------------------------------------------------------------------------
        if ((camstat->apistat == Teli::CAM_API_STS_RESPONSE_TIMEOUT) ||
            (camstat->apistat == Teli::CAM_API_STS_BUFFER_FULL)      ||
            (camstat->apistat == Teli::CAM_API_STS_TOO_MANY_PACKET_MISSING)) {
            camstat->apistat = Teli::CAM_API_STS_SUCCESS;
        }
        if ((camstat->errstat == Teli::CAM_API_STS_RESPONSE_TIMEOUT) ||
            (camstat->errstat == Teli::CAM_API_STS_BUFFER_FULL)      ||
            (camstat->errstat == Teli::CAM_API_STS_TOO_MANY_PACKET_MISSING)) {
            camstat->errstat = Teli::CAM_API_STS_SUCCESS;
        }
    }
}

/// @brief ��M�G���[���������邽�߂̃R�[���o�b�N�֐�
/// @param [in] hCam - Handler of camera
/// @param [in] hStrm - Handler of stream
/// @param [in] uiErrorStatus - AError status
/// @param [in] uiBufferIndex - Index of buffer
/// @param [in] pvContext - Additional caller-specified context
/// @return 
/// @note
void CTeliCamLib::cb_error_image(Teli::CAM_HANDLE hCam, Teli::CAM_STRM_HANDLE hStrm, Teli::CAM_API_STATUS uiErrorStatus, uint32_t uiBufferIndex, void* pvContext)
{
    CTeliCamLib*         telicamlib = reinterpret_cast<CTeliCamLib*>(pvContext);
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPI�̃X�e�[�^�X�R�[�h
    uint32_t             camidx;                                    // Camera index when the camera is opened

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::cb_error_image]<Call>");
    //----------------------------------------------------------------------------
    // Get camera index from camera handle
    if ((apistat = Teli::GetCamIndexFromCamHandle(hCam, &camidx)) != Teli::CAM_API_STS_SUCCESS) {
        if (telicamlib->get_caminfo_camindex(camidx) >= 0) {
            PCAMERA_STATUS camstat = &telicamlib->m_caminfo.details.stat;   // Camera status
            camstat->apistat        = apistat;          // TeliCamAPI�̃X�e�[�^�X�R�[�h
            camstat->framecount     = 0;                // FPS���v�Z���邽�߂̃t���[���J�E���^�[
            camstat->frameidx       = 0;                // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
            camstat->frameidx_valid = FALSE;            // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
            camstat->fpstimer       = timeGetTime();    // Frame update timer
            camstat->fps            = 0.0;              // ���ۂ�FPS
        }
        _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::cb_error_image]<Error>Teli::GetCamIndexFromCamHandle", apistat);
    }
    else {
        if (telicamlib->get_caminfo_camindex(camidx) >= 0) {
            PCAMERA_STATUS camstat = &telicamlib->m_caminfo.details.stat;   // Camera status
            switch (uiErrorStatus) {
            case Teli::CAM_API_STS_RESPONSE_TIMEOUT:
            case Teli::CAM_API_STS_BUFFER_FULL:
            case Teli::CAM_API_STS_TOO_MANY_PACKET_MISSING:
//////////////////////////////////////////////////////////////////////////////
////            camstat->errstat        = uiErrorStatus;    // �摜�X�g���[����M���̃G���[�X�e�[�^�X�R�[�h
//              camstat->framecount     = 0;                // FPS���v�Z���邽�߂̃t���[���J�E���^�[
//              camstat->frameidx       = 0;                // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
//              camstat->frameidx_valid = FALSE;            // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
//              camstat->fpstimer       = timeGetTime();    // Frame update timer
//              camstat->fps            = 0.0;              // ���ۂ�FPS
//////////////////////////////////////////////////////////////////////////////
                break;
            default:
                camstat->errstat        = uiErrorStatus;    // �摜�X�g���[����M���̃G���[�X�e�[�^�X�R�[�h
                camstat->framecount     = 0;                // FPS���v�Z���邽�߂̃t���[���J�E���^�[
                camstat->frameidx       = 0;                // �J��������o�͂����r�f�I�f�[�^�̃t���[���ԍ�
                camstat->frameidx_valid = FALSE;            // "frameidx"�ɗL���Ȓl�����邱�Ƃ������t���O
                camstat->fpstimer       = timeGetTime();    // Frame update timer
                camstat->fps            = 0.0;              // ���ۂ�FPS
                break;
            }
        }
        _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::cb_error_image]uiErrorStatus", uiErrorStatus);
    }

    //----------------------------------------------------------------------------
    // Dummy code for avoiding not refered warning of compliler. 
//  void *pdummy = (void*)&uiBufferIndex;
//  pdummy = &hStrm;
}

/// @brief �J������O�����o�X���b�h�֐�(���g�p)
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CTeliCamLib::td_camera_remove(LPVOID lpParam)
{
    HRESULT              hr         = S_OK;
    PCAMERA_DETAILS      camdetails = reinterpret_cast<CAMERA_DETAILS*>(lpParam);
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPI�̃X�e�[�^�X�R�[�h

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_camera_remove]<Start>");
    while (camdetails->td_camera_remove_stat) {
        if ((apistat = Teli::Sys_WaitForSignal(camdetails->EventHndlCamRemoval, GEV_CAMERA_REMOVE_EVENT_WAITIME)) != Teli::CAM_API_STS_SUCCESS) {
            if (apistat == Teli::CAM_API_STS_TIMEOUT) {
                _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_camera_remove]<Error>Teli::Sys_WaitForSignal(CAM_API_STS_TIMEOUT)");
            }
            else {
                camdetails->stat.apistat = apistat;
                _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::td_camera_remove]<Error>Teli::Sys_WaitForSignal", apistat);
            }
        }
        else {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_camera_remove]<Normal>Teli::Sys_WaitForSignal(CAM_API_STS_SUCCESS)");
        }
    }   // while(camdetails->td_camera_remove_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_camera_remove]<End>");
    camdetails->td_camera_remove_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief �Q�C���ݒ�X���b�h�֐�
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CTeliCamLib::td_gain_control(LPVOID lpParam)
{
    HRESULT         hr         = S_OK;
    PCAMERA_DETAILS camdetails = reinterpret_cast<CAMERA_DETAILS*>(lpParam);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_gain_control]<Start>");
    while (camdetails->td_gain_control_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(camdetails->td_gain_control_hndl);    // Suspend thread

        PCAMERA_STATUS camstat = &camdetails->stat; // Camera status
        if (camstat->camhndl == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_gain_control]<Error>camstat->camhndl");
        }
        else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI�̃G���[
            _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::td_gain_control]<Error>camstat->apistat", camstat->apistat);
        }
        else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // �摜�X�g���[����M���̃G���[
            _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::td_gain_control]<Error>camstat->errstat", camstat->errstat);
        }
        else {
            if ((camstat->apistat = Teli::SetCamGain(camstat->camhndl, camstat->gain)) != Teli::CAM_API_STS_SUCCESS) {
                _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::td_gain_control]<Error>Teli::SetCamGain", camstat->apistat);
            }
        }
    }   // while(camdetails->td_gain_control_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_gain_control]<End>");
    camdetails->td_gain_control_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief �I�����Ԑݒ�X���b�h�֐�
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CTeliCamLib::td_expstime_control(LPVOID lpParam)
{
    HRESULT         hr         = S_OK;
    PCAMERA_DETAILS camdetails = reinterpret_cast<CAMERA_DETAILS*>(lpParam);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_expstime_control]<Start>");
    while (camdetails->td_expstime_control_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(camdetails->td_expstime_control_hndl);    // Suspend thread

        PCAMERA_STATUS camstat = &camdetails->stat; // Camera status
        if (camstat->camhndl == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_expstime_control]<Error>camstat->camhndl");
        }
        else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI status
            _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::td_expstime_control]<Error>camstat->apistat", camstat->apistat);
        }
        else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // Error status code when receiving image stream
            _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[CTeliCamLib::td_expstime_control]<Error>camstat->errstat", camstat->errstat);
        }
        else {
            if ((camstat->apistat = Teli::SetCamExposureTime(camstat->camhndl, camstat->expstime)) != Teli::CAM_API_STS_SUCCESS) {
                _RPTWN(_CRT_WARN, L"%s(%08X)\n", L">>>[td_expstime_control]<Error>Teli::SetCamExposureTime", camstat->apistat);
            }
        }
    }   // while(camdetails->td_expstime_control_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CTeliCamLib::td_expstime_control]<End>");
    camdetails->td_expstime_control_stat = FALSE;
    _endthreadex(0);

    return 0;
}
