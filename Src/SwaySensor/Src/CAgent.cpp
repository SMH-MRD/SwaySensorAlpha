#include "CAgent.h"
#include "Resource.h"
#include <time.h>
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////////////
// CAgent

#pragma comment(lib, "SockUDP.lib")

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief
/// @param
/// @return
/// @note
CAgent::CAgent()
{
    m_shared = NULL;    // CSharedクラス

    m_td_comm_send_hndl = NULL;     // スレッドハンドル
    m_td_comm_send_stat = FALSE;    // スレッドステータス(FALSE:Exit TRUE:Run)

    m_sockrecv = NULL;  // Socket(受信)
    m_socksend = NULL;  // Socket(送信)
}

/// @brief
/// @param
/// @return
/// @note
CAgent::~CAgent()
{
    //----------------------------------------------------------------------------
    // 終了処理
    close();
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief
/// @param
/// @return
/// @note
void CAgent::init_task(void* param)
{
    CAgent* myclass = reinterpret_cast<CAgent*>(param); // 自クラスインスタンスポインタ

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
void CAgent::routine_work(void* param)
{
    CAgent*       myclass = reinterpret_cast<CAgent*>(param);   // 自クラスインスタンスポインタ
    LARGE_INTEGER frequency;                // システムの周波数
    LARGE_INTEGER start_count, end_count;   // 現在のカウント数
    LONGLONG      span_usec;                // 時間の間隔[usec]

    if (myclass == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::routine_work]<Error>Object pointer");
        return ;
    }

    if (myclass->m_shared == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::routine_work]<Error>Object pointer");
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
HRESULT CAgent::initialize(void)
{
    HRESULT      ret = S_OK;
    std::wstring msg;

    //----------------------------------------------------------------------------
    // 共有データ初期化
    // CSharedクラスオブジェクトの生成
    if (m_shared == NULL) {
        if ((m_shared = new CShared()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>Creating of CShared");
            msg = std::format(L"<Error>Failed to create CShared");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CShared");
    outp_msg_to_listview(msg);

    // 共有データ読込み
    m_shared->get_app_config(&m_cnfgsys);           // システム設定
    m_shared->get_app_config(&m_cnfgnet);           // 通信設定
    m_shared->get_app_config(&m_cnfgcam);           // カメラ設定
    m_shared->get_app_config(&m_cnfgmnt);           // 取付寸法設定
    m_shared->get_app_info_data(&m_infotlt_data);   // 傾斜計情報データ
    m_shared->get_app_info_data(&m_infoclt_data);   // クライアント情報データ
    m_shared->get_app_info_data(&m_infoprc_data);   // 画像処理情報データ

    // 共有データ初期化
    m_infoclt_data.device_id = L""; // 機器ID
    m_infoclt_data.command   = 0;   // コマンド
    m_infoclt_data.mode      = 0;   // モード
    m_infoclt_data.cycle     = 0;   // 最小受信周期[ms]
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); idx++) {
        m_infoclt_data.tgt_len[idx] = EXTN_TARGETDIST_MIN;  // ターゲット距離[mm]
    }

    // 共有データ書込み
    m_shared->set_app_info_data(m_infoclt_data);    // クライアント情報データ

    //----------------------------------------------------------------------------
    // メンバー変数の初期化
    ZeroMemory(&m_recv_data, sizeof(m_recv_data));  // 受信データ
    ZeroMemory(&m_send_data, sizeof(m_send_data));  // 送信データ

    QueryPerformanceCounter(&m_send_cycle_counter); // 現在のカウント数

    //----------------------------------------------------------------------------
    // スレッド起動
    m_td_comm_send_stat = TRUE;
    if ((m_td_comm_send_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                       0,
                                                                       td_comm_send,
                                                                       reinterpret_cast<LPVOID>(this),
                                                                       0,
                                                                       NULL))) == NULL) {
        m_td_comm_send_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::initialize]<Error>Thread(td_comm_send) handle");
        std::wstring msg = std::format(L"<Error>Failed to start td_comm_send() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_comm_send_hndl, THREAD_PRIORITY_NORMAL);

    //--------------------------------------------------------------------------
    // Winsockの初期化(受信)
    if (m_sockrecv == NULL) {
        if ((m_sockrecv = new CSockUDPLib()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>Creating of CSockUDPLib");
            msg = std::format(L"<Error>Failed to create CSockUDPLib");
            outp_msg_to_listview(msg);

            ret = E_FAIL;
        }
        else {
            if (m_sockrecv->initialize() != SOCK_ERROR_NONE) {
                delete m_sockrecv;
                m_sockrecv = NULL;

                _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>m_sockrecv->initialize()");
                msg = std::format(L"<Error>Failed in CSockUDPLib::initialize()");
                outp_msg_to_listview(msg);

                ret = E_FAIL;
            }
            else {
                //--------------------------------------------------------------------------
                // Socketのオープン
                if (m_sockrecv->get_sock_status() != SOCK_STATUS_OPEN) {
                    std::string str = CHelper::conv_string(m_cnfgnet.local.ipaddress);
                    m_sockrecv->open(SOCK_TYPE_RECV, SOCK_NONBLOCKING,
                                     const_cast<char*>(str.c_str()), m_cnfgnet.local.port, NULL);

                    msg = std::format(L"<Information>Opened recieve socket");
                    outp_msg_to_listview(msg);
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    // Winsockの初期化(送信)
    if (m_socksend == NULL) {
        if ((m_socksend = new CSockUDPLib()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>Creating of CSockUDPLib");
            msg = std::format(L"<Error>Failed to create CSockUDPLib");
            outp_msg_to_listview(msg);

            ret = E_FAIL;
        }
        else {
            if (m_socksend->initialize() != SOCK_ERROR_NONE) {
                delete m_socksend;
                m_socksend = NULL;
                _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>m_socksend->initialize()");
                msg = std::format(L"<Error>Failed in CSockUDPLib::initialize()");
                outp_msg_to_listview(msg);

                ret = E_FAIL;
            }
        }
    }

    return ret;
}

/// @brief 終了処理
/// @param
/// @return 
/// @note
void CAgent::close(void)
{
    //----------------------------------------------------------------------------
    // スレッドの終了
    // Exposure time control thread function
    if (m_td_comm_send_hndl != NULL) {
        if (m_td_comm_send_stat) {
            m_td_comm_send_stat = FALSE;
        	ResumeThread(m_td_comm_send_hndl);
	        WaitForSingleObject(m_td_comm_send_hndl, 500);  // It waits until the thread ends.
        }
        CloseHandle(m_td_comm_send_hndl);   // Handle is closed.
        m_td_comm_send_hndl = NULL;
    }
    m_td_comm_send_stat = FALSE;

    //--------------------------------------------------------------------------
    // Socketのクローズ(送信)
    if (m_socksend != NULL) {
        m_socksend->close();
    }

    //--------------------------------------------------------------------------
    // Socketのクローズ(受信)
    if (m_sockrecv != NULL) {
        m_sockrecv->close();
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
void CAgent::proc_main(void)
{
    int size;
    int err;

    //----------------------------------------------------------------------------
    // 共有データ読込み
    m_shared->get_app_info_data(&m_infotlt_data);   // 傾斜計情報データ
    m_shared->get_app_info_data(&m_infoprc_data);   // 画像処理情報データ

    //----------------------------------------------------------------------------
    if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN)) {
        m_infoclt_data.command &= ~static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_1);
    }

    //--------------------------------------------------------------------------
    // 送信データのセット
    // 機器ID
    std::string str;
    str = CHelper::conv_string(m_cnfgsys.my_id);
    memcpy(&m_send_data.device_id, str.c_str(), sizeof(m_send_data.device_id)); // 機器ID

    // SYSTEMTIME
    GetLocalTime(&m_send_data.system_time);

    // 振れセンサー情報
    for (uint32_t sensor = 0; sensor < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); sensor++) {
        PNW_SEND_DATA_SWAY_SENSOR_INFO sway_sensor_info = &m_send_data.sway_sensor_info[sensor];    // 振れセンサー情報
        // 機器個体情報
        // カメラ情報
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            sway_sensor_info->sensor_spec_info.camera.image_size[axis] = m_cnfgcam.basis.roi[axis].size;    // 画像データ画素数[pixel]
            sway_sensor_info->sensor_spec_info.camera.view_angle[axis] = static_cast<uint32_t>(static_cast<double>(m_cnfgcam.basis.roi[axis].size)
                                                                       / (m_cnfgcam.basis.viewangle[axis] * CONV_DEG_RAD)); // 分解能[pixel/rad]
        }
        // カメラ取付情報
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            sway_sensor_info->sensor_spec_info.mounting.l0[axis] = static_cast<uint32_t>(m_cnfgmnt.offset[axis].l0);                // カメラ回転軸距離L0[mm]
            sway_sensor_info->sensor_spec_info.mounting.t0[axis] = static_cast<uint32_t>(m_cnfgmnt.offset[axis].t0 * pow(10, 6));   // カメラ取付角度θ0(*10^6)[rad]
            sway_sensor_info->sensor_spec_info.mounting.p0[axis] = static_cast<uint32_t>(m_cnfgmnt.offset[axis].p0 * pow(10, 6));   // カメラ設置角度φ0(*10^6)[rad]
        }

        // 機器状態情報
        // 検出モード
        sway_sensor_info->sensor_stat_info.mode = 0;

        // エラー状態
        sway_sensor_info->sensor_stat_info.error = 0;
        if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE))) {
            sway_sensor_info->sensor_stat_info.error |= static_cast<uint32_t>(ENUM_NW_ERR_SENSOR::CAMERA);      // 機器異常(カメラ)
        }
        if (m_infotlt_data.status &= (static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_LIB) | 
                                      static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ))) {
            sway_sensor_info->sensor_stat_info.error |= static_cast<uint32_t>(ENUM_NW_ERR_SENSOR::TILT);        // 機器異常(傾斜計)
        }
        if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE))) {
            sway_sensor_info->sensor_stat_info.error |= static_cast<uint32_t>(ENUM_NW_ERR_SENSOR::TARGET_1);    // 検出異常(ターゲット1)
        }

        // 検出状態
        sway_sensor_info->sensor_stat_info.stat  = 0;
        sway_sensor_info->sensor_stat_info.stat |= static_cast<uint32_t>(ENUM_NW_STAT_SENSOR::TARGET_1);    // 振れ検出中(ターゲット1)
        if ((m_infoprc_data.status &= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_RUN)) ||
            (m_infoprc_data.status &= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN))) {
            sway_sensor_info->sensor_stat_info.stat |= static_cast<uint32_t>(ENUM_NW_STAT_SENSOR::SET_ZERO_CAMERA); // 0点設定中(カメラ)
        }

        // 傾斜計情報
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            sway_sensor_info->tilt_info[axis].angle = static_cast<uint32_t>(static_cast<double>(m_infotlt_data.tilt[axis].rad) * pow(10, 6));       // 傾斜角(*100000)[rad]
            sway_sensor_info->tilt_info[axis].speed = static_cast<uint32_t>(static_cast<double>(m_infotlt_data.tilt[axis].speed_rad) * pow(10, 6)); // 傾斜角速度(*100000)[rad/s]
        }

        // 振れ検出情報
        for (uint32_t target = 0; target < static_cast<uint32_t>(ENUM_TARGET::E_MAX); target++) {
            PNW_SEND_DATA_SWAY_INFO sway_info = &sway_sensor_info->sway_info[target];   // 振れ検出情報
            // 振れ情報
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                sway_info->sway.angle[axis] = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].sway_angle); // 振れ角[pixel]
                sway_info->sway.speed[axis] = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].sway_speed); // 振れ速度[pixel/s]
                sway_info->sway.zero[axis]  = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].sway_zero);  // 振れ中心位置[pixel]
            }
            // ターゲット情報
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                sway_info->target.tilt[axis] = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].target_tilt);   // ターゲット傾き[pixel]
            }
            sway_info->target.size = static_cast<uint32_t>(m_infoprc_data.target_size); // ターゲットサイズ[pixel]
        }

//@@@仮
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.angle[static_cast<uint32_t>(ENUM_AXIS::X)] = 10;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.angle[static_cast<uint32_t>(ENUM_AXIS::Y)] = 20;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.speed[static_cast<uint32_t>(ENUM_AXIS::X)] = 30;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.speed[static_cast<uint32_t>(ENUM_AXIS::Y)] = 40;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.zero[static_cast<uint32_t>(ENUM_AXIS::X)]   = 50;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.zero[static_cast<uint32_t>(ENUM_AXIS::Y)]   = 60;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].target.tilt[static_cast<uint32_t>(ENUM_AXIS::X)] = 70;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].target.tilt[static_cast<uint32_t>(ENUM_AXIS::Y)] = 80;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].target.size = 90;
//@@@仮

        // 文字情報
//@@@仮
        if (sensor == static_cast<uint32_t>(ENUM_SWAY_SENSOR::SENSOR_1)) {
            sway_sensor_info->message[0] = 's';
            sway_sensor_info->message[1] = 'e';
            sway_sensor_info->message[2] = 'n';
            sway_sensor_info->message[3] = 's';
            sway_sensor_info->message[4] = 'o';
            sway_sensor_info->message[5] = 'r';
            sway_sensor_info->message[6] = '1';
            sway_sensor_info->message[7] = '\0';
        }
        else {
            sway_sensor_info->message[0] = 's';
            sway_sensor_info->message[1] = 'e';
            sway_sensor_info->message[2] = 'n';
            sway_sensor_info->message[3] = 's';
            sway_sensor_info->message[4] = 'o';
            sway_sensor_info->message[5] = 'r';
            sway_sensor_info->message[6] = '2';
            sway_sensor_info->message[7] = '\0';
        }
//@@@仮
    }

    //--------------------------------------------------------------------------
    // データ受信処理
    if (m_sockrecv != NULL) {//CSockUDPのインスタンスinitialize()でnew()
        // socketのオープン
        if (m_sockrecv->get_sock_status() != SOCK_STATUS_OPEN) {
            std::string str = CHelper::conv_string(m_cnfgnet.local.ipaddress);
            m_sockrecv->open(SOCK_TYPE_RECV, SOCK_NONBLOCKING,
                             const_cast<char*>(str.c_str()), m_cnfgnet.local.port, NULL);
  
            std::wstring msg = std::format(L"<Information>Opened receive socket");
            outp_msg_to_listview(msg);
        }
        // データ受信
        if (m_sockrecv->get_sock_status() == SOCK_STATUS_OPEN) {
            size = m_sockrecv->recv_data(reinterpret_cast<char*>(&m_recv_data), sizeof(m_recv_data),
                                         NULL, &err);
            if (size > 0) {
                if (size == sizeof(m_recv_data)) {
                    // 受信データ解析
                    str = CHelper::conv_string(m_cnfgsys.mcc_id);
                    if (strncmp(reinterpret_cast<char*>(m_recv_data.device_id), str.c_str(),
                                NW_SIZE_DEVICE_ID) == 0) {
                        // コマンド解析
                        // コマンド：データ送信要求(定周期)
                        if (m_recv_data.command_info.command & static_cast<uint32_t>(ENUM_NW_COMMAND::REQ_DATA_CYCLE)) {
                            m_infoclt_data.cycle = m_recv_data.command_info.cycle;  // 最小受信周期[msec]
                        }
                        // コマンド：データ送信停止要求
                        else if (m_recv_data.command_info.command & static_cast<uint32_t>(ENUM_NW_COMMAND::REQ_DATA_STOP)) {
                            m_infoclt_data.cycle = 0;   // 最小受信周期[msec]
                        }
                        // コマンド：データ送信要求(1ショット)
                        else {
                            if (m_td_comm_send_hndl != NULL) {
                                ResumeThread(m_td_comm_send_hndl);
                            }
                        }
                        // コマンド：0点設定
                        uint32_t command = 0;
                        command = (static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_2)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_TILT_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_TILT_2));
                        if (m_recv_data.command_info.command & command) {
                            m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        }
                        // コマンド：リセット要求
                        command = (static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_CAMERA_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_CAMERA_2)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_TILT_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_TILT_2));
                        m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        if (m_recv_data.command_info.command & command) {
                            m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        }
                        // コマンド：画像送信要求
                        command = static_cast<uint32_t>(ENUM_NW_COMMAND::REQ_DATA_STOP);
                        if (m_recv_data.command_info.command & command) {
                            m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        }

                        // モード設定
                        m_infoclt_data.mode = m_recv_data.command_info.mode;

                        // カメラ～ターゲット間距離[mm]
                        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); idx++) {
                            //@@@カメラごとに分ける
                            m_infoclt_data.tgt_len[idx] = static_cast<double>(m_recv_data.command_info.tgt_len[idx]);
                        }
                    }   // if (strncmp(reinterpret_cast<char*>(m_recv_data.device_id), str.c_str(), NW_SIZE_DEVICE_ID) == 0)
                    else {
                        ;   // 機器ID異常
                    }   // if (strncmp(reinterpret_cast<char*>(m_recv_data.device_id), str.c_str(), NW_SIZE_DEVICE_ID) == 0) else
                }
                else {
                    ;   // 受信データサイズエラー
                }
            }   // if (size > 0)
            else {
                // 異常処理
                if (err != WSAEWOULDBLOCK) {
                    m_sockrecv->close();    // Socketのクローズ
                }
            }   // if (size > 0) else
        }   // if (m_sockrecv->get_sock_status() == SOCK_STATUS_OPEN)
    }   // if (m_sockrecv != NULL)

    //--------------------------------------------------------------------------
    // データ送信(定周期)要求
    LARGE_INTEGER frequency;    // システムの周波数
    LARGE_INTEGER start_count;  // 現在のカウント数
    LONGLONG      span_usec;    // 時間の間隔[usec]

    QueryPerformanceFrequency(&frequency);  // システムの周波数
    QueryPerformanceCounter(&start_count);  // 現在のカウント数
    span_usec = ((start_count.QuadPart - m_send_cycle_counter.QuadPart) * 1000000L) / frequency.QuadPart;   // 時間の間隔[usec]
    if (span_usec >= 0) {
        if (m_infoclt_data.cycle > 0) {
            if (static_cast<uint16_t>(span_usec / 1000) >= m_infoclt_data.cycle) {
                if (m_td_comm_send_hndl != NULL) {
                    ResumeThread(m_td_comm_send_hndl);
                }
                QueryPerformanceCounter(&m_send_cycle_counter); // 現在のカウント数
            }
        }
        else {
            QueryPerformanceCounter(&m_send_cycle_counter); // 現在のカウント数
        }
    }
    else {
        QueryPerformanceCounter(&m_send_cycle_counter); // 現在のカウント数
    }

    //----------------------------------------------------------------------------
    // 共有データ書込み
    m_shared->set_app_info_data(m_infoclt_data);    // クライアント情報データ

    return ;
}

/// @brief タブパネルのRadio ButtonのStaticテキストを設定
/// @param
/// @return
/// @note
void CAgent::set_tab_func_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    wstr = L"ﾒﾆｭｰ1";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ2";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ3";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ4";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ5";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ6";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), wstr.c_str()); wstr.clear();

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

/// @brief
/// @param
/// @return
/// @note
void CAgent::set_tab_tip_txt(void)
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
void CAgent::set_tab(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
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
            break;
        default:
            break;
        }
        break;
    }

    return ;
}

/// @brief データ送信スレッド関数
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CAgent::td_comm_send(LPVOID lpParam)
{
    CAgent*      myclass = reinterpret_cast<CAgent*>(lpParam);
    std::wstring msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::td_comm_send]<Start>");

    while (myclass->m_td_comm_send_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_comm_send_hndl);    // Suspend thread

        //--------------------------------------------------------------------------
        // データ送信処理
        if (myclass->m_socksend == NULL) {
            continue ;
        }
        if (myclass->m_socksend->get_sock_status() != SOCK_STATUS_OPEN) {
            //--------------------------------------------------------------------------
            // Socketのオープン
            char     addr[INET_ADDRSTRLEN];
            uint16_t port;

            if (myclass->m_cnfgnet.remote.ipaddress == NW_SEND_IP_AUTO) {
	            inet_ntop(AF_INET, &myclass->m_recv_data.sock.sin_addr, addr, sizeof(addr));
                port = ntohs(myclass->m_recv_data.sock.sin_port);
                myclass->m_socksend->open(SOCK_TYPE_SEND, SOCK_NONBLOCKING, addr, port, NULL);

                msg = std::format(L"<Information>Opened send socket");
                myclass->outp_msg_to_listview(msg);
            }
            else {
                std::string str;
                str = CHelper::conv_string(myclass->m_cnfgnet.remote.ipaddress);
                myclass->m_socksend->open(SOCK_TYPE_SEND, SOCK_NONBLOCKING,
                                          const_cast<char*>(str.c_str()), myclass->m_cnfgnet.remote.port, NULL);

                msg = std::format(L"<Information>Opened send socket");
                myclass->outp_msg_to_listview(msg);
            }
            if (myclass->m_socksend->get_sock_status() != SOCK_STATUS_OPEN) {
                continue ;
            }
        }   // if (myclass->m_socksend->get_sock_status() != SOCK_STATUS_OPEN)

        //--------------------------------------------------------------------------
        // データ送信
        int size;
        int err;
        if (myclass->m_socksend->get_sock_status() == SOCK_STATUS_OPEN) {
            size = myclass->m_socksend->send_data(reinterpret_cast<char*>(&myclass->m_send_data),
                                                  sizeof(m_send_data),
                                                  &err);
            if (err != 0) {
                myclass->m_socksend->close();
                msg = std::format(L"<Error>Send socket error ({:d})", err);
                myclass->outp_msg_to_listview(msg);
            }
        }
    }   // while(myclass->m_td_comm_send_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::td_comm_send]<End>");
    msg = std::format(L"<Information>Exited td_comm_send() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_comm_send_stat = FALSE;
    _endthreadex(0);

    return 0;
}
