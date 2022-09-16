#include "CEnvironment.h"

/// @brief Constructor
/// @param
/// @return
/// @note
CEnvironment::CEnvironment()
{
}

/// @brief Destructor
/// @param
/// @return
/// @note
CEnvironment::~CEnvironment()
{
	//----------------------------------------------------------------------------
	// �X���b�h�̏I��
    if(m_td_camstart_hndl != NULL) {
        if(m_td_camstart_state) {
            m_td_camstart_state = FALSE;
            ResumeThread(m_td_camstart_hndl);
            WaitForSingleObject(m_td_camstart_hndl, 500);   // �X���b�h���I������܂őҋ@
        }
        CloseHandle(m_td_camstart_hndl);
	    m_td_camstart_hndl = NULL;																								// Handle is closed.
    }
    m_td_camstart_state = FALSE;
    Sleep(100);

    //----------------------------------------------------------------------------
    // �J�����̃I�u�W�F�N�g�̍폜
    if(m_telicam != NULL) {
        //----------------------------------------------------------------------------
        // Stop stream
        m_telicam->stop_stream();

        //----------------------------------------------------------------------------
        // Close stream
        m_telicam->close_stream();

        //----------------------------------------------------------------------------
        // Close camera
        m_telicam->close_camera();

        Sleep(1000);
        delete m_telicam;
	    m_telicam = NULL;
	    Sleep(500);
    }
}

//////////////////////////////////////////////////////////////////////////////
// CEnvironment Public method

/// @brief
/// @param
/// @return
/// @note
void CEnvironment::init_task(void* param)
{
    CEnvironment* myself = (CEnvironment*)param;  // ���N���X�C���X�^���X�|�C���^
    CONFIG_COMMON cnfg_common;

    //----------------------------------------------------------------------------
    // Create of camera object
    myself->m_shared.get_appconfig(&cnfg_common);
    if(cnfg_common.img_source == 0) {
        //----------------------------------------------------------------------------
        // Create of TeliCamLib
        myself->m_shared.get_appconfig(&myself->m_cnfgcam); // �J�����ݒ�
        CAMERA_INFO caminfo; // �J�����̏��
        caminfo.details.cnfg.valid          = TRUE;                                 // �J�����̗L���܂��͖���[0:���� 1:�L��]
        caminfo.details.cnfg.ipaddress      = myself->m_cnfgcam.basis.ipaddress;    // �J������IP�A�h���X
        caminfo.details.cnfg.packetsize     = myself->m_cnfgcam.basis.packetsize;   // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y(�ʏ��0���w��)[byte]
        caminfo.details.cnfg.framerate_drop = myself->m_cnfgcam.error.framedrop;    // �t���[�����[�g�ቺ�̔���l[fps]
        myself->m_telicam = new CTeliCamLib(caminfo);
        if(myself->m_telicam == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[init_task]Error creating of CTeliCamLib.");
            return;
        }

        //----------------------------------------------------------------------------
        // Initialize TeliCamAPI system
        if(myself->m_telicam->initialize() != 0) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[init_task]Error initialize of CTeliCamLib.");
            return;
        }

        //----------------------------------------------------------------------------
        // Beginning of thread
        m_td_camstart_state = TRUE;
        m_td_camstart_hndl  = (HANDLE)_beginthreadex(NULL, 0, td_camstart, (LPVOID)this, 0, NULL);
        if(m_td_camstart_hndl == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[init_task]td_camstart create error.");
	        m_td_camstart_state = FALSE;
	        return;
        }
        SetThreadPriority(m_td_camstart_hndl, THREAD_PRIORITY_LOWEST);
    }   // if(cnfg_common.img_source == 0)
}

/// @brief
/// @param
/// @return
/// @note
void CEnvironment::routine_work(void* param)
{
    CEnvironment* myself = (CEnvironment*)param;  // ���N���X�C���X�^���X�|�C���^
}

//////////////////////////////////////////////////////////////////////////////
// CEnvironment Private method

/// @brief
/// @param
/// @return
/// @note
void CEnvironment::set_window(void)
{
    return;
}

/// @brief 
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_camstart(LPVOID lpParam)
{
    HRESULT       hr       = S_OK;
    CEnvironment* pParent  = (CEnvironment*)lpParam;
    DWORD         curtime  = timeGetTime();         //
    CTeliCamLib*  pTeliCam = (pParent->m_telicam);  // CTeliCamLib object

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[td_camstart]Start.");
    while(pParent->m_td_camstart_state) {
        if(pTeliCam == NULL) {
            break;
        }

        //----------------------------------------------------------------------------
        // PC�ɐڑ�����Ă���J�����̒T��
        if(pTeliCam->update_camera_list() != 0) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[td_camstart]Error update camera list of CTeliCamLib.");
        }

        //----------------------------------------------------------------------------
        // Open device
        // �J�����̃I�[�v��
        if(pTeliCam->open_camera(Teli::CAM_ACCESS_MODE::CAM_ACCESS_MODE_CONTROL) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[td_camstart]", (LPCTSTR)pTeliCam->get_errormessage());
        }
        // Set of TeliGevCam property
        hr = pParent->set_telicam_property();

        //----------------------------------------------------------------------------
        // Start stream
        // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃I�[�v��
        if(pTeliCam->open_stream() != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[td_camstart]", (LPCTSTR)pTeliCam->get_errormessage());
        }
        // �摜�X�g���[���̓]���J�n
        if(pTeliCam->start_stream() != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[td_camstart]", (LPCTSTR)pTeliCam->get_errormessage());
        }

        break;  // Exit loop
    }   // while(pParent->m_td_camstart_hndl)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[td_camstart]End.");
    pParent->m_td_camstart_state = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief 
/// @param
/// @return 
/// @note
HRESULT CEnvironment::set_telicam_property(void)
{
    HRESULT hr = S_OK;

    // �J�����̊�{�ݒ�
    {
        PCONFIG_CAMERA_BASIS cam_basis = &m_cnfgcam.basis;  // �J�����̊�{�ݒ�

        //----------------------------------------------------------------------------
        // �J�����̃r�f�I�X�g���[���̃s�N�Z���`���̐ݒ�
        if(m_telicam->set_pixelformat(Teli::_CAM_PIXEL_FORMAT::PXL_FMT_BayerBG8) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J������ROI(�̈�)�̐ݒ�
        if(m_telicam->set_camroi(cam_basis->roi[AXIS_X].offset, cam_basis->roi[AXIS_Y].offset,
	                             cam_basis->roi[AXIS_X].size, cam_basis->roi[AXIS_Y].size) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J�����̃t���[�����[�g�̐ݒ�
//      if(m_telicam->set_framerate(static_cast<float64_t>(cam_basis->framerate), Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_NO_SPECIFY) != 0) {
        if(m_telicam->set_framerate(static_cast<float64_t>(cam_basis->framerate), Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_MANUAL) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J�����̃g���K�[���샂�[�h�̐ݒ�
        if(m_telicam->set_triggermode(false) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J�����̍����x���̐ݒ�
        if(m_telicam->set_blacklevel(static_cast<float64_t>(cam_basis->blacklevel)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_blacklevel]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J�����̃K���}�␳�l�̐ݒ�
        if(m_telicam->set_gamma(static_cast<float64_t>(cam_basis->gamma)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_gamma]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J�����̃z���C�g�o�����X�Q�C�������������[�h�̐ݒ�
        if ((static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(cam_basis->wb.wb_auto) == Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_CONTINUOUS) ||
            (static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(cam_basis->wb.wb_auto) == Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_ONCE)) {
           if(m_telicam->set_wbalance_auto(static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(cam_basis->wb.wb_auto)) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
                return (hr = E_FAIL);
            }
        }
        else {
            if(m_telicam->set_wbalance_auto(Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_OFF) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
                return (hr = E_FAIL);
            }
            else {
                //----------------------------------------------------------------------------
                // �J�����̃z���C�g�o�����X�Q�C��(�{��)�̐ݒ�
                if(m_telicam->set_wbalance_ratio(static_cast<float64_t>(cam_basis->wb.wb_ratio_red), Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE::CAM_BALANCE_RATIO_SELECTOR_RED) != 0) {
                    _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
                    return (hr = E_FAIL);
                }
                else if(m_telicam->set_wbalance_ratio(static_cast<float64_t>(cam_basis->wb.wb_ratio_blue), Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE::CAM_BALANCE_RATIO_SELECTOR_BLUE) != 0) {
                    _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
                    return (hr = E_FAIL);
                }
                else {
                    ;
                }
            }
        }
    }

    // �P�x�R���g���[���ݒ�(�I������)
    {
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL cam_expstime = &m_cnfgcam.expstime; // �P�x�R���g���[���ݒ�(�I������)

        //----------------------------------------------------------------------------
        // �J�����̘I�����Ԃ̐��䃂�[�h�̐ݒ�
//      if(m_telicam->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_NO_SPECIFY) != 0) {
        if(m_telicam->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_MANUAL)     != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J�����̘I�����Ԃ̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
        if(m_telicam->set_expstime(static_cast<float64_t>(cam_expstime->val)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }
    }

    // �P�x�R���g���[���ݒ�(�Q�C��)
    {
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL cam_gain = &m_cnfgcam.gain;   // �P�x�R���g���[���ݒ�(�Q�C��)

        //----------------------------------------------------------------------------
        // �J������AGC(Automatic gain control)���샂�[�h�̐ݒ�
        if(m_telicam->set_gain_auto(Teli::CAM_GAIN_AUTO_TYPE::CAM_GAIN_AUTO_OFF) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // �J�����̃Q�C���̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
        if(m_telicam->set_gain(static_cast<float64_t>(cam_gain->val)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }
    }

    return hr;
}
