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
	// スレッドの終了
    if(m_td_camstart_hndl != NULL) {
        if(m_td_camstart_state) {
            m_td_camstart_state = FALSE;
            ResumeThread(m_td_camstart_hndl);
            WaitForSingleObject(m_td_camstart_hndl, 500);   // スレッドが終了するまで待機
        }
        CloseHandle(m_td_camstart_hndl);
	    m_td_camstart_hndl = NULL;																								// Handle is closed.
    }
    m_td_camstart_state = FALSE;
    Sleep(100);

    //----------------------------------------------------------------------------
    // カメラのオブジェクトの削除
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
    CEnvironment* myself = (CEnvironment*)param;  // 自クラスインスタンスポインタ
    CONFIG_COMMON cnfg_common;

    //----------------------------------------------------------------------------
    // Create of camera object
    myself->m_shared.get_appconfig(&cnfg_common);
    if(cnfg_common.img_source == 0) {
        //----------------------------------------------------------------------------
        // Create of TeliCamLib
        myself->m_shared.get_appconfig(&myself->m_cnfgcam); // カメラ設定
        CAMERA_INFO caminfo; // カメラの情報
        caminfo.details.cnfg.valid          = TRUE;                                 // カメラの有効または無効[0:無効 1:有効]
        caminfo.details.cnfg.ipaddress      = myself->m_cnfgcam.basis.ipaddress;    // カメラのIPアドレス
        caminfo.details.cnfg.packetsize     = myself->m_cnfgcam.basis.packetsize;   // ドライバが受け取るパケットの最大サイズ(通常は0を指定)[byte]
        caminfo.details.cnfg.framerate_drop = myself->m_cnfgcam.error.framedrop;    // フレームレート低下の判定値[fps]
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
    CEnvironment* myself = (CEnvironment*)param;  // 自クラスインスタンスポインタ
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
        // PCに接続されているカメラの探索
        if(pTeliCam->update_camera_list() != 0) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[td_camstart]Error update camera list of CTeliCamLib.");
        }

        //----------------------------------------------------------------------------
        // Open device
        // カメラのオープン
        if(pTeliCam->open_camera(Teli::CAM_ACCESS_MODE::CAM_ACCESS_MODE_CONTROL) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[td_camstart]", (LPCTSTR)pTeliCam->get_errormessage());
        }
        // Set of TeliGevCam property
        hr = pParent->set_telicam_property();

        //----------------------------------------------------------------------------
        // Start stream
        // 画像取得用のストリームインターフェースのオープン
        if(pTeliCam->open_stream() != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[td_camstart]", (LPCTSTR)pTeliCam->get_errormessage());
        }
        // 画像ストリームの転送開始
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

    // カメラの基本設定
    {
        PCONFIG_CAMERA_BASIS cam_basis = &m_cnfgcam.basis;  // カメラの基本設定

        //----------------------------------------------------------------------------
        // カメラのビデオストリームのピクセル形式の設定
        if(m_telicam->set_pixelformat(Teli::_CAM_PIXEL_FORMAT::PXL_FMT_BayerBG8) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラのROI(領域)の設定
        if(m_telicam->set_camroi(cam_basis->roi[AXIS_X].offset, cam_basis->roi[AXIS_Y].offset,
	                             cam_basis->roi[AXIS_X].size, cam_basis->roi[AXIS_Y].size) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラのフレームレートの設定
//      if(m_telicam->set_framerate(static_cast<float64_t>(cam_basis->framerate), Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_NO_SPECIFY) != 0) {
        if(m_telicam->set_framerate(static_cast<float64_t>(cam_basis->framerate), Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_MANUAL) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラのトリガー動作モードの設定
        if(m_telicam->set_triggermode(false) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラの黒レベルの設定
        if(m_telicam->set_blacklevel(static_cast<float64_t>(cam_basis->blacklevel)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_blacklevel]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラのガンマ補正値の設定
        if(m_telicam->set_gamma(static_cast<float64_t>(cam_basis->gamma)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_gamma]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラのホワイトバランスゲイン自動調整モードの設定
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
                // カメラのホワイトバランスゲイン(倍率)の設定
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

    // 輝度コントロール設定(露光時間)
    {
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL cam_expstime = &m_cnfgcam.expstime; // 輝度コントロール設定(露光時間)

        //----------------------------------------------------------------------------
        // カメラの露光時間の制御モードの設定
//      if(m_telicam->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_NO_SPECIFY) != 0) {
        if(m_telicam->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_MANUAL)     != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラの露光時間の設定(APIへの設定はスレッドで実行される)
        if(m_telicam->set_expstime(static_cast<float64_t>(cam_expstime->val)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }
    }

    // 輝度コントロール設定(ゲイン)
    {
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL cam_gain = &m_cnfgcam.gain;   // 輝度コントロール設定(ゲイン)

        //----------------------------------------------------------------------------
        // カメラのAGC(Automatic gain control)動作モードの設定
        if(m_telicam->set_gain_auto(Teli::CAM_GAIN_AUTO_TYPE::CAM_GAIN_AUTO_OFF) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }

        //----------------------------------------------------------------------------
        // カメラのゲインの設定(APIへの設定はスレッドで実行される)
        if(m_telicam->set_gain(static_cast<float64_t>(cam_gain->val)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s.\n", L">>>[set_telicam_property]", (LPCTSTR)m_telicam->get_errormessage());
            return (hr = E_FAIL);
        }
    }

    return hr;
}
