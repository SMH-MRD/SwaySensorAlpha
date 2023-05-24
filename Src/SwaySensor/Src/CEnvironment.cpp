#include "CEnvironment.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////
// CEnvironment

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief Constructor
/// @param
/// @return
/// @note
CEnvironment::CEnvironment()
{
    m_shared      = NULL;   // CSharedクラス
    m_telicam_lib = NULL;   // CTeliCamLibクラス
    m_canopen_lib = NULL;   // CCANopenLibクラス

    m_td_camera_start_hndl        = NULL;   // スレッドハンドル
    m_td_camera_start_stat        = FALSE;  // スレッドステータス (FALSE:Exit TRUE:Run)
    m_td_camera_restart_hndl      = NULL;   // スレッドハンドル
    m_td_camera_restart_stat      = FALSE;  // スレッドステータス (FALSE:Exit TRUE:Run)
    m_td_canopen_start_hndl       = NULL;   // スレッドハンドル
    m_td_canopen_start_stat       = FALSE;  // スレッドステータス (FALSE:Exit TRUE:Run)
    m_td_canopen_tilt_preset_hndl = NULL;   // スレッドハンドル
    m_td_canopen_tilt_preset_stat = FALSE;  // スレッドステータス (FALSE:Exit TRUE:Run)
    m_td_canopen_tilt_get_hndl    = NULL;   // スレッドハンドル
    m_td_canopen_tilt_get_stat    = FALSE;  // スレッドステータス (FALSE:Exit TRUE:Run)

	m_img_src.data_bgr = NULL;  // 画像データバッファのポインタ(BGR 24bit)
}

/// @brief Destructor
/// @param
/// @return
/// @note
CEnvironment::~CEnvironment()
{
    //----------------------------------------------------------------------------
    // 終了処理
    close();
}

/// @brief 画像データ取得
/// @param
/// @return 
/// @note
IMAGE_DATA CEnvironment::get_opencv_image(void)
{
    //----------------------------------------------------------------------------
    // 画像データ取得(画像ファイル)
    if (m_cnfgcmn.img_source_camera != static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA)) {
        m_img_src.data_opencv = cv::imread(CHelper::conv_string(m_cnfgcmn.img_source_fname));
        if (m_img_src.data_opencv.data != NULL) {
	        m_img_src.status |= static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED);  // 画像ステータス:画像有効
	        m_img_src.width   = m_img_src.data_opencv.cols ;                        // 画像サイズ(水平画素) [pixel]
	        m_img_src.height  = m_img_src.data_opencv.rows;                         // 画像サイズ(垂直画素) [pixel]
            m_img_src.fps     = m_cnfgcam.basis.framerate;                          // 画像フレームレート[fps]

            return m_img_src;
        }
	    m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // 画像ステータス:画像有効
	    m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // 画像サイズ(水平画素) [pixel]
	    m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // 画像サイズ(垂直画素) [pixel]
        m_img_src.fps     = 0.0;                                                            // 画像フレームレート[fps]
    }
 
    //----------------------------------------------------------------------------
    // 画像データ取得(カメラ)
    if (m_img_src.data_bgr != NULL) {
        if ((m_telicam_lib != NULL) &&
            (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)))) { // カメラ再接続中
            // 画像情報を取得
            if ((m_telicam_lib->get_image(m_img_src.data_bgr) >= 0) &&
                (m_telicam_lib->get_image_size(&m_img_src.width, &m_img_src.height) >= 0) &&
                (m_telicam_lib->get_image_fps(&m_img_src.fps) >= 0)) {
                m_img_src.status |= (static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));    // 画像ステータス:画像有効
            }
            else {
	            m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // 画像ステータス:画像有効
	            m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // 画像サイズ(水平画素) [pixel]
	            m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // 画像サイズ(垂直画素) [pixel]
                m_img_src.fps     = 0.0;                                                            // 画像フレームレート [fps]
	            ZeroMemory(m_img_src.data_bgr, (sizeof(uint8_t) * IMAGE_SIZE * IMAGE_FORMAT_SIZE)); // 画像データバッファのポインタ(BGR 24bit)
            }
        }
        else {
	        m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // 画像ステータス:画像有効
	        m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // 画像サイズ(水平画素) [pixel]
	        m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // 画像サイズ(垂直画素) [pixel]
            m_img_src.fps     = 0.0;                                                            // 画像フレームレート [fps]
	        ZeroMemory(m_img_src.data_bgr, (sizeof(uint8_t) * IMAGE_SIZE * IMAGE_FORMAT_SIZE)); // 画像データバッファのポインタ(BGR 24bit)
        }
        // OpenCV画像への変換
        m_img_src.data_opencv = cv::Mat(m_img_src.height,
                                        m_img_src.width,
                                        CV_8UC3,
                                        m_img_src.data_bgr);    // 画像データ(OpenCV変換画像)    
    }   // if (m_img_src.data_bgr != NULL)
    else {
	    m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // 画像ステータス:画像有効
	    m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // 画像サイズ(水平画素) [pixel]
	    m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // 画像サイズ(垂直画素) [pixel]
        m_img_src.fps     = 0.0;                                                            // 画像フレームレート[fps]
    }   // else

	return m_img_src;
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief
/// @param
/// @return
/// @note
void CEnvironment::init_task(void* param)
{
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(param); // 自クラスインスタンスポインタ
    std::wstring  msg;

    //----------------------------------------------------------------------------
    // 初期化処理
    if (myclass != NULL) {
        std::wstring msg = std::format(L"<Information>Called init_task()");
        myclass->outp_msg_to_listview(msg);
        if (FAILED(myclass->initialize())) {
            return ;
        }
    }

    return ;
}

/// @brief
/// @param
/// @return
/// @note
void CEnvironment::routine_work(void* param)
{
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(param); // 自クラスインスタンスポインタ
    LARGE_INTEGER frequency;                // システムの周波数
    LARGE_INTEGER start_count, end_count;   // 現在のカウント数
    LONGLONG      span_usec;                // 時間の間隔[usec]

    if (myclass == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::routine_work]<Error>Object pointer");
        return ;
    }

    if (myclass->m_shared == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::routine_work]<Error>Object pointer");
        return ;
    }

    //----------------------------------------------------------------------------
    // 処理時間計測(開始時間取得)
    QueryPerformanceFrequency(&frequency);  // システムの周波数
    QueryPerformanceCounter(&start_count);  // 現在のカウント数

    //----------------------------------------------------------------------------
    // メイン処理
    myclass->proc_main();

    //----------------------------------------------------------------------------
    // 処理時間計測(終了時間取得)
    QueryPerformanceCounter(&end_count);    // 現在のカウント数
    span_usec = ((end_count.QuadPart - start_count.QuadPart) * 1000000L) / frequency.QuadPart;  // 時間の間隔[usec]

    //----------------------------------------------------------------------------
    // メッセージ表示
    std::wstring str;
    str = std::format(L"実行ｶｳﾝﾀｰ:{:4d} 処理時間[ms]:{:4d}", (*(myclass->thread_info.psys_counter) % 1000),
                                                             static_cast<uint32_t>(span_usec / 1000));
    myclass->outp_msg_to_mainwnd(str);

    return ;
}

/// @brief 初期化処理
/// @param
/// @return 
/// @note
HRESULT CEnvironment::initialize(void)
{
    std::wstring msg;

    //----------------------------------------------------------------------------
    // 共有データ初期化
    // CSharedクラスオブジェクトの生成
    if (m_shared == NULL) {
        if ((m_shared = new CShared()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Creating of CShared");
            msg = std::format(L"<Error>Failed to create CShared");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CShared");
    outp_msg_to_listview(msg);

    // 共有データ読込み
    m_shared->get_app_config(&m_cnfgcmn);           // 共通設定
    m_shared->get_app_config(&m_cnfgcam);           // カメラ設定
    m_shared->get_app_config(&m_cnfgtlt);           // 傾斜計設定
    m_shared->get_app_info_data(&m_infotlt_data);   // 傾斜計情報データ
    m_shared->get_app_info_data(&m_infoprc_data);   // 画像処理情報データ
    m_shared->get_app_info_data(&m_infosys_data);   // システム情報データ

    // 共有データ初期化
    m_infotlt_data.status = static_cast<uint32_t>(ENUM_TILT_STATUS::DEFAULT);   // 傾斜計状態:デフォルト
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
        m_infotlt_data.tilt[idx].deg = 0.0; // 傾斜角[deg]
        m_infotlt_data.tilt[idx].rad = 0.0; // 傾斜角[deg]
    }

    m_infosys_data.status = static_cast<uint32_t>(ENUM_SYSTEM_STATUS::DEFAULT); // ステータス

    // 共有データ書込み
    m_shared->set_app_info_data(m_infotlt_data);    // 傾斜計情報データ
    m_shared->set_app_info_data(m_infosys_data);    // システム情報データ

    //----------------------------------------------------------------------------
    // メンバー変数の初期化
    // 
    QueryPerformanceCounter(&m_cycle_time_counter); // パフォーマンスカウンター現在値
    m_cycle_time = thread_info.cycle_ms;            // 実行周期[ms]

    //----------------------------------------------------------------------------
    // カメラ起動処理
	m_img_src.status = static_cast<uint32_t>(ENUM_IMAGE_STATUS::DEFAULT);               // 画像ステータス:デフォルト
	m_img_src.width  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;   // 画像サイズ(水平画素) [pixel]
	m_img_src.height = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;   // 画像サイズ(垂直画素) [pixel]
	if (m_img_src.data_bgr == NULL) {
        m_img_src.data_bgr = new (uint8_t[IMAGE_SIZE * IMAGE_FORMAT_SIZE]); // 画像データバッファのポインタ(BGR 24bit)
    }
	ZeroMemory(m_img_src.data_bgr, (sizeof(uint8_t) * IMAGE_SIZE * IMAGE_FORMAT_SIZE)); // The all clear the data area

    // CTeliCamLibクラスオブジェクトの生成
    if (m_cnfgcmn.img_source_camera == static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA)) {   // カメラ画像取込み
        TELICAM_LIB_INFO caminfo;   // カメラの情報
        caminfo.details.cnfg.valid          = TRUE;                         // カメラの有効または無効[0:無効 1:有効]
        caminfo.details.cnfg.ipaddress      = m_cnfgcam.basis.ipaddress;    // カメラのIPアドレス
        caminfo.details.cnfg.packetsize     = m_cnfgcam.basis.packetsize;   // ドライバが受け取るパケットの最大サイズ(通常は0を指定)[byte]
        caminfo.details.cnfg.framerate_drop = m_cnfgcam.error.framedrop;    // フレームレート低下の判定値[fps]

        if (m_telicam_lib == NULL) {
            if ((m_telicam_lib = new CTeliCamLib(caminfo)) == NULL) {
                _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Creating of CTeliCamLib");
                msg = std::format(L"<Error>Failed to create CTeliCamLib");
                outp_msg_to_listview(msg);

                return E_FAIL;
            }
        }
        msg = std::format(L"<Information>Created CTeliCamLib");
        outp_msg_to_listview(msg);

        // TeliCamAPIの初期化
        if (m_telicam_lib->initialize() != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::init_task]", m_telicam_lib->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::initialize() ({:s})", m_telicam_lib->get_error_message());
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        msg = std::format(L"<Information>Initialized CTeliCamLib");
        outp_msg_to_listview(msg);

        // スレッド起動(カメラ初回起動処理)
        m_td_camera_start_stat = TRUE;
        if ((m_td_camera_start_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                              0,
                                                                              td_camera_start,
                                                                              reinterpret_cast<LPVOID>(this),
                                                                              0,
                                                                              NULL))) == NULL) {
	        m_td_camera_start_stat = FALSE;

            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_camera_start) handle");
            msg = std::format(L"<Error>Failed to start td_camera_start() thread");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        SetThreadPriority(m_td_camera_start_hndl, THREAD_PRIORITY_LOWEST);
        Sleep(10);
        ResumeThread(m_td_camera_start_hndl);

        // スレッド起動(カメラ再起動処理)
        m_td_camera_restart_stat = TRUE;
        if ((m_td_camera_restart_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                                0,
                                                                                td_camera_restart,
                                                                                reinterpret_cast<LPVOID>(this),
                                                                                0,
                                                                                NULL))) == NULL) {
	        m_td_camera_restart_stat = FALSE;

            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_camera_restart) handle");
            msg = std::format(L"<Error>Failed to start td_camera_restart() thread");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        SetThreadPriority(m_td_camera_restart_hndl, THREAD_PRIORITY_LOWEST);
    }   // if (m_cnfgcmn.img_source_camera != static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA))

    //----------------------------------------------------------------------------
    // CANopen起動処理
    // CCANopenLibクラスオブジェクトの生成
    if (m_canopen_lib == NULL) {
        if ((m_canopen_lib = new CCANopenLib()) == NULL) {
            m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_LIB);  // ステータス情報:CANopen Libエラー

            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Creating of CCANopenLib");
            msg = std::format(L"<Error>Failed to create CCANopenLib");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CCANopenLib");
    outp_msg_to_listview(msg);

    // CANopenモジュールの初期化
    int32_t stat;
    if ((stat = m_canopen_lib->initialize(m_cnfgtlt.canopen.port, m_cnfgtlt.canopen.node_id)) != 0) {
        m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_LIB);  // ステータス情報:CANopen Libエラー

        _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::init_task]<Error>Initialize of CCANopenLib", stat);
        msg = std::format(L"<Error>Failed in CCANopenLib::initialize() ({:d})", stat);
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    msg = std::format(L"<Information>Initialized CCANopenLib ({:s})", m_canopen_lib->get_libver());
    outp_msg_to_listview(msg);

    // スレッド起動(CANopen起動処理)
    m_td_canopen_start_stat = TRUE;
    if ((m_td_canopen_start_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                           0,
                                                                           td_canopen_start,
                                                                           reinterpret_cast<LPVOID>(this),
                                                                           0,
                                                                           NULL))) == NULL) {
	    m_td_canopen_start_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_canopen_start) handle");
        msg = std::format(L"<Error>Failed to start td_canopen_start() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_canopen_start_hndl, THREAD_PRIORITY_LOWEST);
    Sleep(10);
    ResumeThread(m_td_canopen_start_hndl);

    // スレッド起動(傾斜計プリセット処理)
    m_td_canopen_tilt_preset_stat = TRUE;
    if ((m_td_canopen_tilt_preset_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                                 0,
                                                                                 td_canopen_tilt_preset,
                                                                                 reinterpret_cast<LPVOID>(this),
                                                                                 0,
                                                                                 NULL))) == NULL) {
	    m_td_canopen_tilt_preset_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_canopen_tilt_preset) handle");
        msg = std::format(L"<Error>Failed to start td_canopen_tilt_preset() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_canopen_tilt_preset_hndl, THREAD_PRIORITY_LOWEST);

    // スレッド起動(傾斜計データ取得処理)
    m_td_canopen_tilt_get_stat = TRUE;
    if ((m_td_canopen_tilt_get_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                              0,
                                                                              td_canopen_tilt_get,
                                                                              reinterpret_cast<LPVOID>(this),
                                                                              0,
                                                                              NULL))) == NULL) {
	    m_td_canopen_tilt_get_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_canopen_tilt_get) handle");
        msg = std::format(L"<Error>Failed to start td_canopen_tilt_get() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_canopen_tilt_get_hndl, THREAD_PRIORITY_LOWEST);

    return S_OK;
}

/// @brief 終了処理
/// @param
/// @return 
/// @note
void CEnvironment::close(void)
{
    //----------------------------------------------------------------------------
    // スレッドの終了
    //傾斜計データ取得処理
    if (m_td_canopen_tilt_get_hndl != NULL) {
        if (m_td_canopen_tilt_get_stat) {
            m_td_canopen_tilt_get_stat = FALSE;
            ResumeThread(m_td_canopen_tilt_get_hndl);
            WaitForSingleObject(m_td_canopen_tilt_get_hndl, 500);   // スレッドが終了するまで待機
        }
        CloseHandle(m_td_canopen_tilt_get_hndl);
	    m_td_canopen_tilt_get_hndl = NULL;																								// Handle is closed.
    }
    m_td_canopen_tilt_get_stat = FALSE;
    Sleep(100);

    //傾斜計プリセット処理
    if (m_td_canopen_tilt_preset_hndl != NULL) {
        if (m_td_canopen_tilt_preset_stat) {
            m_td_canopen_tilt_preset_stat = FALSE;
            ResumeThread(m_td_canopen_tilt_preset_hndl);
            WaitForSingleObject(m_td_canopen_tilt_preset_hndl, 500);    // スレッドが終了するまで待機
        }
        CloseHandle(m_td_canopen_tilt_preset_hndl);
	    m_td_canopen_tilt_preset_hndl = NULL;																								// Handle is closed.
    }
    m_td_canopen_tilt_preset_stat = FALSE;
    Sleep(100);

    // CANopen起動処理
    if (m_td_canopen_start_hndl != NULL) {
        if (m_td_canopen_start_stat) {
            m_td_canopen_start_stat = FALSE;
            ResumeThread(m_td_canopen_start_hndl);
            WaitForSingleObject(m_td_canopen_start_hndl, 500);  // スレッドが終了するまで待機
        }
        CloseHandle(m_td_canopen_start_hndl);
	    m_td_canopen_start_hndl = NULL;																								// Handle is closed.
    }
    m_td_canopen_start_stat = FALSE;
    Sleep(100);

    // カメラ再起動処理
    if (m_td_camera_restart_hndl != NULL) {
        if (m_td_camera_restart_stat) {
            m_td_camera_restart_stat = FALSE;
            ResumeThread(m_td_camera_restart_hndl);
            WaitForSingleObject(m_td_camera_restart_hndl, 500); // スレッドが終了するまで待機
        }
        CloseHandle(m_td_camera_restart_hndl);
	    m_td_camera_restart_hndl = NULL;																								// Handle is closed.
    }
    m_td_camera_restart_stat = FALSE;
    Sleep(100);

    // カメラ初回起動処理
    if (m_td_camera_start_hndl != NULL) {
        if (m_td_camera_start_stat) {
            m_td_camera_start_stat = FALSE;
            ResumeThread(m_td_camera_start_hndl);
            WaitForSingleObject(m_td_camera_start_hndl, 500);   // スレッドが終了するまで待機
        }
        CloseHandle(m_td_camera_start_hndl);
	    m_td_camera_start_hndl = NULL;																								// Handle is closed.
    }
    m_td_camera_start_stat = FALSE;
    Sleep(100);

    //----------------------------------------------------------------------------
    // CCANopenLibクラスオブジェクトの削除
    if (m_canopen_lib != NULL) {
        // CANopenネットワークの無効化
        m_canopen_lib->network_disable();

        Sleep(100);
        delete m_canopen_lib;
	    m_canopen_lib = NULL;
	    Sleep(100);
    }

    //----------------------------------------------------------------------------
    // CTeliCamLibクラスオブジェクトの削除
    if (m_telicam_lib != NULL) {
        // Stop stream
        m_telicam_lib->stop_stream();

        // Close stream
        m_telicam_lib->close_stream();

        // Close camera
        m_telicam_lib->close_camera();

        Sleep(1000);
        delete m_telicam_lib;
	    m_telicam_lib = NULL;
	    Sleep(500);
    }

    //----------------------------------------------------------------------------
    // 画像データバッファの削除
    if (m_img_src.data_bgr != NULL) {
        delete[] m_img_src.data_bgr;
        m_img_src.data_bgr = NULL;
    }

    //----------------------------------------------------------------------------
    // CSharedクラスオブジェクトの削除
    if (m_shared != NULL) {
        delete m_shared;
        m_shared = NULL;
    }

    return ;
}

/// @brief メイン処理
/// @param
/// @return
/// @note
void CEnvironment::proc_main(void)
{
    std::wstring msg;

    //----------------------------------------------------------------------------
    // 実行周期計測
    LARGE_INTEGER frequency;    // システムの周波数
    LARGE_INTEGER start_count;  // 現在のカウント数
    LONGLONG      span_usec;    // 時間の間隔[usec]

    QueryPerformanceFrequency(&frequency);  // システムの周波数
    QueryPerformanceCounter(&start_count);  // 現在のカウント数
    span_usec = ((start_count.QuadPart - m_cycle_time_counter.QuadPart) * 1000000L) / frequency.QuadPart;   //
    if ((m_cycle_time = static_cast<uint32_t>(span_usec / 1000)) <= 0) { // 実行周期[ms]
        m_cycle_time = thread_info.cycle_ms;
    }
    QueryPerformanceCounter(&m_cycle_time_counter); // パフォーマンスカウンター現在値

    //----------------------------------------------------------------------------
    // 共有データ読込み
    m_shared->get_app_info_data(&m_infoprc_data);   // 画像処理情報データ

    //----------------------------------------------------------------------------
    // カメラ再起動処理
    if ((m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ)) ||        // カメラ再接続要求
        (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ_MCC))) {    // カメラ再接続要求(MCC)
        if (m_td_camera_restart_hndl != NULL) {
            if (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)) &&       // カメラ再接続中
                !(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED))) { // カメラ再接続完了
                ResumeThread(m_td_camera_restart_hndl);
            }
        }
        else {
            m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED); // カメラ再接続完了
            msg = std::format(L"<Error>m_td_camera_restart_hndl NULL");
            outp_msg_to_listview(msg);
        }
        if (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED)) {    // カメラ再接続完了
            m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ)     |    // カメラ再接続要求
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ_MCC) |    // カメラ再接続要求(MCC)
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)     |    // カメラ再接続中
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED));  // カメラ再接続完了
        }
    }

    //----------------------------------------------------------------------------
    // カメラの露光時間の設定(APIへの設定はスレッドで実行される)
    if ((m_telicam_lib != NULL) &&
        (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)))) { // カメラ再接続中
        m_telicam_lib->set_expstime(static_cast<float64_t>(m_infoprc_data.exps_time));
    }

    //----------------------------------------------------------------------------
    // 傾斜計プリセット処理
    if ((m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ)) ||     // 傾斜計プリセット要求
        (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ_MCC))) { // 傾斜計プリセット要求(MCC)
        if (m_td_canopen_tilt_preset_hndl != NULL) {
            if (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN)) &&        // 傾斜計プリセット実行中
                !(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED))) {  // 傾斜計プリセット完了
                ResumeThread(m_td_canopen_tilt_preset_hndl);
            }
        }
        else {
            m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED);  // 傾斜計プリセット完了
            msg = std::format(L"<Error>m_td_canopen_tilt_preset_hndl NULL");
            outp_msg_to_listview(msg);
        }
        if (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED)) {     // 傾斜計プリセット完了
            m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ)     |     // 傾斜計プリセット要求
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ_MCC) |     // 傾斜計プリセット要求(MCC)
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN)     |     // 傾斜計プリセット実行中
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED));   // 傾斜計プリセット完了
        }
    }

    //----------------------------------------------------------------------------
    // 傾斜計データの取得
    if (m_td_canopen_tilt_get_hndl != NULL) {
        ResumeThread(m_td_canopen_tilt_get_hndl);
    }
    double               dt = m_cycle_time * 0.001; // 実行周期[s]
    CANOPEN_TILTIX_SLOPE tiltix_slope = m_canopen_lib->get_tiltix_slope();
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        PTILT_DATA tilt_data       = &m_infotlt_data.tilt[axis];    // 傾斜計データ
        double     last_tilt_angle = tilt_data->deg;                // 傾斜角[deg]

        tilt_data->deg = static_cast<double>(tiltix_slope.data[axis].val)
                       * (static_cast<double>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_001)
                       /  static_cast<double>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_1)); // 傾斜角[deg]
        tilt_data->rad = tilt_data->deg * CONV_DEG_RAD; // 傾斜角[rad]

        double tilt_speed = (tilt_data->deg - last_tilt_angle) / dt;    // 傾斜角速度[deg/s]
        tilt_data->speed_deg = ((dt * tilt_speed) + (m_cnfgtlt.tiltspdfilter * tilt_data->speed_deg))
                             / (m_cnfgtlt.tiltspdfilter + dt);      // 傾斜角速度[deg/s]
        tilt_data->speed_rad = tilt_data->speed_deg * CONV_DEG_RAD; // 傾斜角速度[rad/s]
    }

    //----------------------------------------------------------------------------
    // 共有データ書込み
    m_shared->set_app_info_data(m_infotlt_data);    // 傾斜計情報データ
    m_shared->set_app_info_data(m_infosys_data);    // システム情報データ

    return ;
}

/// @brief カメラの基本設定
/// @param
/// @return 
/// @note
HRESULT CEnvironment::set_telicam_property(void)
{
    std::wstring msg;

    // カメラの基本設定
#pragma region SET_CAMERA_BASIS
    //----------------------------------------------------------------------------
    // カメラのビデオストリームのピクセル形式の設定
    if (m_telicam_lib->set_pixelformat(Teli::_CAM_PIXEL_FORMAT::PXL_FMT_BayerBG8) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_pixelformat() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラのROI(領域)の設定
    if (m_telicam_lib->set_camroi(m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].offset,
                                  m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].offset,
	                              m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size,
                                  m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_camroi() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラのフレームレートの設定
//  if (m_telicam_lib->set_framerate(static_cast<float64_t>(m_cnfgcam.basis.framerate),
//                                   Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_NO_SPECIFY) != 0) {
    if (m_telicam_lib->set_framerate(static_cast<float64_t>(m_cnfgcam.basis.framerate),
                                     Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_MANUAL) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_framerate() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラのトリガー動作モードの設定
    if (m_telicam_lib->set_triggermode(false) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_triggermode() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラの黒レベルの設定
    if (m_telicam_lib->set_blacklevel(static_cast<float64_t>(m_cnfgcam.basis.blacklevel)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_blacklevel]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_blacklevel() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラのガンマ補正値の設定
    if (m_telicam_lib->set_gamma(static_cast<float64_t>(m_cnfgcam.basis.gamma)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_gamma]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_gamma() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラのホワイトバランスゲイン自動調整モードの設定
    if ((static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(m_cnfgcam.basis.wb.wb_auto) == Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_CONTINUOUS) ||
        (static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(m_cnfgcam.basis.wb.wb_auto) == Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_ONCE)) {
        if (m_telicam_lib->set_wbalance_auto(static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(m_cnfgcam.basis.wb.wb_auto)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_auto() ({:s})", m_telicam_lib->get_error_message());
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    else {
        if (m_telicam_lib->set_wbalance_auto(Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_OFF) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_auto() ({:s})", m_telicam_lib->get_error_message());
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        else {
            //----------------------------------------------------------------------------
            // カメラのホワイトバランスゲイン(倍率)の設定
            if (m_telicam_lib->set_wbalance_ratio(static_cast<float64_t>(m_cnfgcam.basis.wb.wb_ratio_red),
                                                  Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE::CAM_BALANCE_RATIO_SELECTOR_RED) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_ratio() ({:s})", m_telicam_lib->get_error_message());
                outp_msg_to_listview(msg);

                return E_FAIL;
            }
            else if (m_telicam_lib->set_wbalance_ratio(static_cast<float64_t>(m_cnfgcam.basis.wb.wb_ratio_blue),
                                                       Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE::CAM_BALANCE_RATIO_SELECTOR_BLUE) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_ratio() ({:s})", m_telicam_lib->get_error_message());
                outp_msg_to_listview(msg);

                return E_FAIL;
            }
            else {
                ;
            }
        }
    }
#pragma endregion SET_CAMERA_BASIS

    // 輝度コントロール設定(露光時間)
#pragma region SET_CAMERA_EXPOSURE_CONTROL
    //----------------------------------------------------------------------------
    // カメラの露光時間の制御モードの設定
//  if (m_telicam_lib->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_NO_SPECIFY) != 0) {
    if (m_telicam_lib->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_MANUAL)     != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_expstime_control() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラの露光時間の設定(APIへの設定はスレッドで実行される)
    if (m_telicam_lib->set_expstime(static_cast<float64_t>(m_cnfgcam.expstime.val)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_expstime() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
#pragma endregion SET_CAMERA_EXPOSURE_CONTROL

    // 輝度コントロール設定(ゲイン)
#pragma region SET_CAMERA_GAIN_CONTROL
    //----------------------------------------------------------------------------
    // カメラのAGC(Automatic gain control)動作モードの設定
    if (m_telicam_lib->set_gain_auto(Teli::CAM_GAIN_AUTO_TYPE::CAM_GAIN_AUTO_OFF) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_gain_auto() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // カメラのゲインの設定(APIへの設定はスレッドで実行される)
    if (m_telicam_lib->set_gain(static_cast<float64_t>(m_cnfgcam.gain.val)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_gain_auto() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
#pragma endregion SET_CAMERA_GAIN_CONTROL

    return S_OK;
}

/// @brief タブパネルのRadio ButtonのStaticテキストを設定
/// @param
/// @return
/// @note
void CEnvironment::set_tab_func_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    wstr = L"ﾒﾆｭｰ1";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), TRUE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), wstr.c_str());

    wstr = L"ﾒﾆｭｰ2";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), wstr.c_str());

    wstr = L"ﾒﾆｭｰ3";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), wstr.c_str());

    wstr = L"ﾒﾆｭｰ4";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), wstr.c_str());

    wstr = L"ﾒﾆｭｰ5";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), wstr.c_str());

    wstr = L"ﾒﾆｭｰ6";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), wstr.c_str());

    //----------------------------------------------------------------------------
    // 
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM3), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM4), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM5), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM6), FALSE);

    //----------------------------------------------------------------------------
    // 
    set_tab_tip_txt();

    return ;
}

/// @brief タブパネルの説明用Staticテキストを設定
/// @param
/// @return
/// @note
void CEnvironment::set_tab_tip_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM3), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM4), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM5), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM6), FALSE);

    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM3), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM4), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM5), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM6), FALSE);

    //----------------------------------------------------------------------------
    // 
    switch (thread_info.panel_func_id) {
    case IDC_RADIO_TASK_FUNC1:
        wstr = L"ｶﾒﾗ再接続";
        ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), TRUE);
        SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), wstr.c_str());
        if (m_cnfgcmn.img_source_camera == static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA)) {
            EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), TRUE);
        }
        else {
            EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
        }
        wstr = L"傾斜計ﾌﾟﾘｾｯﾄ";
        ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), TRUE);
        SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), wstr.c_str());
        EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), TRUE);
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC2:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC3:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC4:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC5:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC6:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return ;
}

/// @brief タブパネルの設定
/// @param
/// @return 
/// @note
void CEnvironment::set_tab(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    std::wstring str;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDC_RADIO_TASK_FUNC1:
        case IDC_RADIO_TASK_FUNC2:
        case IDC_RADIO_TASK_FUNC3:
        case IDC_RADIO_TASK_FUNC4:
        case IDC_RADIO_TASK_FUNC5:
        case IDC_RADIO_TASK_FUNC6:
            // 機能選択IDセット
            thread_info.panel_func_id = LOWORD(wp);
            set_tab_tip_txt();  // タブパネルの説明用Staticテキストを設定
        case IDC_BUTTON_TASK_ITEM1:
        case IDC_BUTTON_TASK_ITEM2:
        case IDC_BUTTON_TASK_ITEM3:
        case IDC_BUTTON_TASK_ITEM4:
        case IDC_BUTTON_TASK_ITEM5:
        case IDC_BUTTON_TASK_ITEM6:
            // 機能入力IDセット
            thread_info.panel_type_id = LOWORD(wp);
            // 各FUNCTION PB処理
            switch (thread_info.panel_func_id) {
            case IDC_RADIO_TASK_FUNC1:
                switch (thread_info.panel_type_id) {
                case IDC_BUTTON_TASK_ITEM1:
                    // カメラ再接続
                    m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ);   // システム状態:カメラ再接続要求
                    break;
                case IDC_BUTTON_TASK_ITEM2:
                    // 傾斜計プリセット
                    m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ);    // 傾斜計プリセット要求
                    break;
                case IDC_BUTTON_TASK_ITEM3:
                    break;
                case IDC_BUTTON_TASK_ITEM4:
                    break;
                case IDC_BUTTON_TASK_ITEM5:
                    break;
                case IDC_BUTTON_TASK_ITEM6:
                    break;
                default:
                    break;
                }
                break;
            case IDC_RADIO_TASK_FUNC2:
                switch (thread_info.panel_type_id) {
                case IDC_BUTTON_TASK_ITEM1:
                    break;
                case IDC_BUTTON_TASK_ITEM2:
                    break;
                case IDC_BUTTON_TASK_ITEM3:
                    break;
                case IDC_BUTTON_TASK_ITEM4:
                    break;
                case IDC_BUTTON_TASK_ITEM5:
                    break;
                case IDC_BUTTON_TASK_ITEM6:
                    break;
                default:
                    break;
                }
                break;
            case IDC_RADIO_TASK_FUNC3:
                switch (thread_info.panel_type_id) {
                case IDC_BUTTON_TASK_ITEM1:
                    break;
                case IDC_BUTTON_TASK_ITEM2:
                    break;
                case IDC_BUTTON_TASK_ITEM3:
                    break;
                case IDC_BUTTON_TASK_ITEM4:
                    break;
                case IDC_BUTTON_TASK_ITEM5:
                    break;
                case IDC_BUTTON_TASK_ITEM6:
                    break;
                default:
                    break;
                }
                break;
            case IDC_RADIO_TASK_FUNC4:
                switch (thread_info.panel_type_id) {
                case IDC_BUTTON_TASK_ITEM1:
                    break;
                case IDC_BUTTON_TASK_ITEM2:
                    break;
                case IDC_BUTTON_TASK_ITEM3:
                    break;
                case IDC_BUTTON_TASK_ITEM4:
                    break;
                case IDC_BUTTON_TASK_ITEM5:
                    break;
                case IDC_BUTTON_TASK_ITEM6:
                    break;
                default:
                    break;
                }
                break;
            case IDC_RADIO_TASK_FUNC5:
                switch (thread_info.panel_type_id) {
                case IDC_BUTTON_TASK_ITEM1:
                    break;
                case IDC_BUTTON_TASK_ITEM2:
                    break;
                case IDC_BUTTON_TASK_ITEM3:
                    break;
                case IDC_BUTTON_TASK_ITEM4:
                    break;
                case IDC_BUTTON_TASK_ITEM5:
                    break;
                case IDC_BUTTON_TASK_ITEM6:
                    break;
                default:
                    break;
                }
                break;
            case IDC_RADIO_TASK_FUNC6:
                switch (thread_info.panel_type_id) {
                case IDC_BUTTON_TASK_ITEM1:
                    break;
                case IDC_BUTTON_TASK_ITEM2:
                    break;
                case IDC_BUTTON_TASK_ITEM3:
                    break;
                case IDC_BUTTON_TASK_ITEM4:
                    break;
                case IDC_BUTTON_TASK_ITEM5:
                    break;
                case IDC_BUTTON_TASK_ITEM6:
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    }

    return ;
}

/// @brief カメラ初回起動処理
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_camera_start(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CTeliCamLib*  telicam = (myclass->m_telicam_lib);   // CTeliCamLib object
    std::wstring  msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_start]<Start>");

    while (myclass->m_td_camera_start_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_camera_start_hndl); // Suspend thread

        if (telicam == NULL) {
            continue;
        }
 
        //----------------------------------------------------------------------------
        // カメラ起動処理
        //----------------------------------------------------------------------------
        // PCに接続されているカメラの探索
        if (telicam->update_camera_list() != 0) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_start]<Error>Update camera list of CTeliCamLib");
            msg = std::format(L"<Error>Failed in CTeliCamLib::update_camera_list() ({:s})", telicam->get_error_message());
            myclass->outp_msg_to_listview(msg);
        }
        else {
            //----------------------------------------------------------------------------
            // カメラのオープン -> 画像ストリームの転送開始
            // カメラのオープン
            if (telicam->open_camera(Teli::CAM_ACCESS_MODE::CAM_ACCESS_MODE_CONTROL) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_start]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_camera() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            // カメラの基本設定
            else if (FAILED(myclass->set_telicam_property())) {
                ;
            }
            // 画像取得用のストリームインターフェースのオープン
            else if (telicam->open_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_start]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            // 画像ストリームの転送開始
            else if (telicam->start_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_start]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::start_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            else {
                msg = std::format(L"<Informasiton>Camera stream started");
                myclass->outp_msg_to_listview(msg);
            }
        }   // if (pTeliCam->update_camera_list() != 0) else
    }   // while (pParent->m_td_camera_start_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_start]<End>");
    msg = std::format(L"<Information>Exited td_camera_start() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_camera_start_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief カメラ再起動処理
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_camera_restart(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CTeliCamLib*  telicam = (myclass->m_telicam_lib);   // CTeliCamLib object
    std::wstring  msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_restart]<Start>");
    while (myclass->m_td_camera_restart_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_camera_restart_hndl);   // Suspend thread

        if (!(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ)) &&      // カメラ再接続要求
            !(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ_MCC))) {  // カメラ再接続要求(MCC)
            continue;
        }

        if (telicam == NULL) {
            myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED);    // カメラ再接続完了
            continue;
        }

        if ((myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)) ||       // カメラ再接続中
            (myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED))) { // カメラ再接続完了
            continue;
        }

        //----------------------------------------------------------------------------
        // カメラ再起動処理
        myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN);  // カメラ再接続中

        //----------------------------------------------------------------------------
        // 画像ストリームの転送停止 -> カメラのクローズ
        // 画像ストリームの転送停止
        telicam->stop_stream();

        // 画像取得用のストリームインターフェースのクローズ
        telicam->close_stream();

        // カメラのクローズ
        telicam->close_camera();

		//----------------------------------------------------------------------------
		// PCに接続されているカメラの探索
        if (telicam->update_camera_list() != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::update_camera_list() ({:s})", telicam->get_error_message());
            myclass->outp_msg_to_listview(msg);
        }
        else {
            //----------------------------------------------------------------------------
            // カメラのオープン -> 画像ストリームの転送開始
            // カメラのオープン
            if (telicam->open_camera(Teli::CAM_ACCESS_MODE::CAM_ACCESS_MODE_CONTROL) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_camera() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            // カメラの基本設定
            else if (FAILED(myclass->set_telicam_property())) {
                ;
            }
            // 画像取得用のストリームインターフェースのオープン
            else if (telicam->open_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
			// 画像ストリームの転送開始
            else if (telicam->start_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::start_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            else {
                msg = std::format(L"<Informasiton>Camera stream started");
                myclass->outp_msg_to_listview(msg);
            }
		}
        myclass->m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN));       // カメラ再接続中
        myclass->m_infosys_data.status |=   static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED);  // カメラ再接続完了
    }   // while (myclass->m_td_camera_restart_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_restart]<End>");
    msg = std::format(L"<Information>Exited td_camera_restart() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_camera_restart_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief CANopen起動処理
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_canopen_start(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CCANopenLib*  canopen = (myclass->m_canopen_lib);   // CCANopenLib object
    int32_t       stat;
    std::wstring  msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_start]<Start>");
    while (myclass->m_td_canopen_start_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_canopen_start_hndl);    // Suspend thread

        if (canopen == NULL) {
            msg = std::format(L"<Error>CCANopenLib object NULL");
            myclass->outp_msg_to_listview(msg);
            continue;
        }

        //----------------------------------------------------------------------------
        // CANopen起動処理
        wchar_t path[_MAX_PATH], szDrive[_MAX_DRIVE], szPath[_MAX_PATH], szFName[_MAX_FNAME], szExt[_MAX_EXT];
        GetModuleFileName(NULL, path, sizeof(path) / sizeof(*path));    // exe failのpathを取得
        _wsplitpath_s(path,
                      szDrive, sizeof(szDrive) / sizeof(*szDrive),
                      szPath,  sizeof(szPath) / sizeof(*szPath),
                      szFName, sizeof(szFName) / sizeof(*szFName),
                      szExt,   sizeof(szExt) / sizeof(*szExt)); // 取得したpathを分割
        swprintf_s(path, sizeof(path) / sizeof(*path),
                   L"%s%s%s%s",
                   szDrive, szPath, FOLDER_OF_INIFILE, myclass->m_cnfgtlt.canopen.setup_fname.c_str()); // フォルダのパスとiniファイルのパスに合成

        //----------------------------------------------------------------------------
        // CANopenネットワークの有効化
        if ((stat = myclass->m_canopen_lib->network_enable(path)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Enable of CCANopenLib", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::network_enable() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>Enable a CANOpen network");
            myclass->outp_msg_to_listview(msg);
        }

        //----------------------------------------------------------------------------
        // CANOpen NMT通信オブジェクトの送信 (Operational)
        PCONFIG_CANOPEN cnfg_canopen = &myclass->m_cnfgtlt.canopen;
        if ((stat = myclass->m_canopen_lib->set_state(CANOPEN_NMT_COMMAND::OPERATIONAL)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Set state of CCANopenLib", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::set_state() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CANOpen NMT communication object ({:x})", static_cast<uint8_t>(CANOPEN_NMT_COMMAND::OPERATIONAL));
            myclass->outp_msg_to_listview(msg);
        }

        //----------------------------------------------------------------------------
        // CANOpen RTR CAN SIDフレームの送信
        if((stat = myclass->m_canopen_lib->request_state()) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Request state of CCANopenLib", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::request_state() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a RTR CAN SID frame");
            myclass->outp_msg_to_listview(msg);
        }

        //----------------------------------------------------------------------------
        // CANopen設定
        int32_t stat;   //
        int32_t cob_id = static_cast<uint32_t>(CANOPEN_TILTIX_CAN_COB_ID::SDO_RX) 
                       + static_cast<uint32_t>(myclass->m_cnfgtlt.canopen.node_id);
        // Digital recursive filter
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::DIGITAL_RECURSIVE_FILTER),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE),
                                        myclass->m_cnfgtlt.dr_filter)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CAN SID frame - Digital recursive filter ({:d})", myclass->m_cnfgtlt.dr_filter);
            myclass->outp_msg_to_listview(msg);
        }

        // Moving Average Filter
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::MOVING_AGERAGE_FILTER),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE),
                                        myclass->m_cnfgtlt.ma_filter)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CAN SID frame - Moving Average Filter ({:d})", myclass->m_cnfgtlt.ma_filter);
            myclass->outp_msg_to_listview(msg);
        }

        // Resolution
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::MOVING_AGERAGE_FILTER),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE),
                                        static_cast<uint16_t>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_001))) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CAN SID frame - Resolution ({:d})", static_cast<uint16_t>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_001));
            myclass->outp_msg_to_listview(msg);
        }

        myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::CANOPEN_STARTED);  // ステータス情報:CANopen起動
    }   // while (myclass->m_td_canopen_start_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_start]<End>");
    msg = std::format(L"<Information>Exited td_canopen_start() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_canopen_start_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief 傾斜計プリセット処理
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_canopen_tilt_preset(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CCANopenLib*  canopen = (myclass->m_canopen_lib);   // CCANopenLib object
    std::wstring  msg;
    int32_t       stat;
    int32_t       cob_id = static_cast<uint32_t>(CANOPEN_TILTIX_CAN_COB_ID::SDO_RX) 
                         + static_cast<uint32_t>(myclass->m_cnfgtlt.canopen.node_id);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_preset]<Start>");
    while (myclass->m_td_canopen_tilt_preset_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_canopen_tilt_preset_hndl);  // Suspend thread

        if (!(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ)) &&       // 傾斜計プリセット要求
            !(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ_MCC))) {   // 傾斜計プリセット要求(MCC)
            continue;
        }

        if (canopen == NULL) {
            myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED); // 傾斜計プリセット完了
            continue;
        }

        if ((myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN)) ||        // 傾斜計プリセット実行中
            (myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED))) {  // 傾斜計プリセット完了
            continue;
        }

        if (!(myclass->m_infotlt_data.status & static_cast<uint32_t>(ENUM_TILT_STATUS::CANOPEN_STARTED)) ||
            !(canopen->get_canopen_stat() & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {
            myclass->m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN));        // 傾斜計プリセット実行中
            myclass->m_infosys_data.status |=   static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED);   // 傾斜計プリセット完了
            continue;
        }

        //----------------------------------------------------------------------------
        // 傾斜計プリセット処理
        myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN);   // 傾斜計プリセット実行中

        // X軸
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::PRESET_X_AXIS),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed to TILTIX preset X-Axis - CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }
        else {
            msg = std::format(L"<Informasiton>TILTIX preset X-Axis");
            myclass->outp_msg_to_listview(msg);
        }

        // Y軸
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::PRESET_Y_AXIS),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed to TILTIX preset Y-Axis - CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }
        else {
            msg = std::format(L"<Informasiton>TILTIX preset Y-Axis");
            myclass->outp_msg_to_listview(msg);
        }

        myclass->m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN));        // 傾斜計プリセット実行中
        myclass->m_infosys_data.status |=   static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED);   // 傾斜計プリセット完了
    }   // while (myclass->m_td_canopen_tilt_preset_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_preset]<End>");
    msg = std::format(L"<Information>Exited td_canopen_tilt_preset() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_canopen_tilt_preset_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief 傾斜計データ取得処理
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_canopen_tilt_get(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CCANopenLib*  canopen = (myclass->m_canopen_lib);   // CCANopenLib object
    std::wstring  msg;
    int32_t       stat;
    int32_t       cob_id = static_cast<uint32_t>(CANOPEN_TILTIX_CAN_COB_ID::SDO_RX) 
                         + static_cast<uint32_t>(myclass->m_cnfgtlt.canopen.node_id);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_get]<Start>");
    while (myclass->m_td_canopen_tilt_get_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_canopen_tilt_get_hndl);  // Suspend thread

        if (canopen == NULL) {
            // 傾斜計データ情報
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // ステータス情報:データ読み込みエラー
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                PTILT_DATA tilt_data = &myclass->m_infotlt_data.tilt[axis]; // 傾斜計データ

                myclass->m_infotlt_data.tilt[axis].deg       = 0.0; // 傾斜角[deg]
                myclass->m_infotlt_data.tilt[axis].rad       = 0.0; // 傾斜角[rad]
                myclass->m_infotlt_data.tilt[axis].speed_deg = 0.0; // 傾斜角速度[deg/s]
                myclass->m_infotlt_data.tilt[axis].speed_rad = 0.0; // 傾斜角速度[rad/s]
            }
            continue;
        }

        if (!(myclass->m_infotlt_data.status & static_cast<uint32_t>(ENUM_TILT_STATUS::CANOPEN_STARTED)) ||
            !(canopen->get_canopen_stat() & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {
            // 傾斜計データ情報
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // ステータス情報:データ読み込みエラー
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                PTILT_DATA tilt_data = &myclass->m_infotlt_data.tilt[axis]; // 傾斜計データ

                myclass->m_infotlt_data.tilt[axis].deg       = 0.0; // 傾斜角[deg]
                myclass->m_infotlt_data.tilt[axis].rad       = 0.0; // 傾斜角[rad]
                myclass->m_infotlt_data.tilt[axis].speed_deg = 0.0; // 傾斜角速度[deg/s]
                myclass->m_infotlt_data.tilt[axis].speed_rad = 0.0; // 傾斜角速度[rad/s]
            }
            continue;
        }

        // X軸
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::UPLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::SLOPE_LONG16),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // ステータス情報:データ読み込みエラー

            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::control_canopen_proc]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }

        // Y軸
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::UPLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::SLOPE_LATERAL16),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // ステータス情報:データ読み込みエラー

            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::control_canopen_proc]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }

        myclass->m_infotlt_data.status &= ~(static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ)); // ステータス情報
    }   // while (myclass->m_td_canopen_tilt_get_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_get]<End>");
    msg = std::format(L"<Information>Exited td_canopen_tilt_get() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_canopen_tilt_get_stat = FALSE;
    _endthreadex(0);

    return 0;
}
