#include "CTeliCamLib.h"
#include "CHelper.h"
#include <mmsystem.h>
#include <process.h>

//////////////////////////////////////////////////////////////////////////////
// CTeliCamLib

/// @brief Constructor
/// @param
/// @return
/// @note
CTeliCamLib::CTeliCamLib(void)
{
    m_errmsg = L""; // エラーメッセージ

    m_caminfo.camcount = 0; // 検出したカメラの数
    PCAMERA_DETAILS camdetails = &m_caminfo.details;                // カメラの詳細情報
    // カメラ取外し通知イベント
    camdetails->EventHndlCamRemoval = NULL;                         // カメラ取り外し通知用のイベント(シグナル)オブジェクトのハンドル
    // スレッド情報
    camdetails->ThreadHndlCamRemoval      = NULL;                   // スレッドハンドル
    camdetails->ThreadStatCamRemoval      = FALSE;                  // スレッドステータス(FALSE:Exit TRUE:Run)
    camdetails->ThreadHndlGainControl     = NULL;                   // スレッドハンドル
    camdetails->ThreadStatGainControl     = FALSE;                  // スレッドステータス(FALSE:Exit TRUE:Run)
    camdetails->ThreadHndlExpsTimeControl = NULL;                   // スレッドハンドル
    camdetails->ThreadStatExpsTimeControl = FALSE;                  // スレッドステータス(FALSE:Exit TRUE:Run)
    // カメラの設定
    camdetails->cnfg.valid          = FALSE;                        // カメラの有効または無効[0:無効 1:有効]
    camdetails->cnfg.ipaddress      = L"0.0.0.0";                   // カメラのIPアドレス
    camdetails->cnfg.packetsize     = 0;                            // ドライバが受け取るパケットの最大サイズ(通常は0を指定)[byte]
    camdetails->cnfg.framerate_drop = 10.0;                         // フレームレート低下の判定値[fps]
    // カメラのステータス
    camdetails->stat.camidx         = -1;                           // カメラのインデックス
    camdetails->stat.camhndl        = NULL;                         // オープンしたカメラのカメラハンドル
    camdetails->stat.strmhndl       = NULL;                         // オープンしたストリームインターフェースのストリームハンドル
    camdetails->stat.apistat        = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPIのステータスコード
    camdetails->stat.errstat        = Teli::CAM_API_STS_SUCCESS;    // 画像ストリーム受信時のエラーステータスコード
    camdetails->stat.pyldsize       = 0;                            // 1つのストリームリクエストで受信するペイロードのサイズ(画像サイズ)
    camdetails->stat.camname        = L"Not found camera";          // カメラの名前
    camdetails->stat.camwidth       = 0;                            // 映像の幅
    camdetails->stat.camheight      = 0;                            // 映像の高さ
    camdetails->stat.framecount     = 0;                            // FPSを計算するためのフレームカウンター
    camdetails->stat.frameidx       = 0;                            // カメラから出力されるビデオデータのフレーム番号
    camdetails->stat.frameidx_valid = FALSE;                        // "frameidx"に有効な値があることを示すフラグ
    camdetails->stat.fpstimer       = timeGetTime();                // フレーム更新タイマー
    camdetails->stat.fps            = 0.0;                          // 実際のFPS
    camdetails->stat.framechk_valid = FALSE;                        // フレームレート損失チェック
    camdetails->stat.expstime_min   = 0.0;                          // 露光時間(最小)
    camdetails->stat.expstime_max   = 0.0;                          // 露光時間(最大)
    camdetails->stat.expstime       = 0.0;                          // 露光時間
    camdetails->stat.gainmin        = 0.0;                          // ゲイン(最小)
    camdetails->stat.gainmax        = 0.0;                          // ゲイン(最大)
    camdetails->stat.gain           = 0.0;                          // ゲイン
    ZeroMemory(&camdetails->stat.caminfo, sizeof(Teli::CAM_INFO));  // カメラの情報
}

/// @brief Constructor
/// @param caminfo カメラの情報  
/// @return
/// @note
CTeliCamLib::CTeliCamLib(CAMERA_INFO caminfo)
{
    m_errmsg = L""; // エラーメッセージ

    m_caminfo.camcount = 0;  // 検出したカメラの数
    PCAMERA_DETAILS camdetails = &m_caminfo.details;                // カメラの詳細情報
    // カメラ取外し通知イベント
    camdetails->EventHndlCamRemoval = NULL;                         // カメラ取り外し通知用のイベント(シグナル)オブジェクトのハンドル
    // スレッド情報
    camdetails->ThreadHndlCamRemoval      = NULL;                   // スレッドハンドル
    camdetails->ThreadStatCamRemoval      = FALSE;                  // スレッドステータス(FALSE:Exit TRUE:Run)
    camdetails->ThreadHndlGainControl     = NULL;                   // スレッドハンドル
    camdetails->ThreadStatGainControl     = FALSE;                  // スレッドステータス(FALSE:Exit TRUE:Run)
    camdetails->ThreadHndlExpsTimeControl = NULL;                   // スレッドハンドル
    camdetails->ThreadStatExpsTimeControl = FALSE;                  // スレッドステータス(FALSE:Exit TRUE:Run)
    // カメラの設定
    PCAMERA_CONFIG pCamCnfg = &caminfo.details.cnfg;                // カメラの設定
    camdetails->cnfg.valid          = pCamCnfg->valid;              // カメラの有効または無効[0:無効 1:有効]
    camdetails->cnfg.ipaddress      = pCamCnfg->ipaddress;          // カメラのIPアドレス
    camdetails->cnfg.packetsize     = pCamCnfg->packetsize;         // ドライバが受け取るパケットの最大サイズ(通常は0を指定)[byte]
    camdetails->cnfg.framerate_drop = pCamCnfg->framerate_drop;     // フレームレート低下の判定値[fps]
    // カメラのステータス
    camdetails->stat.camidx           = -1;                         // カメラのインデックス
    camdetails->stat.camhndl        = NULL;                         // オープンしたカメラのカメラハンドル
    camdetails->stat.strmhndl       = NULL;                         // オープンしたストリームインターフェースのストリームハンドル
    camdetails->stat.apistat        = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPIのステータスコード
    camdetails->stat.errstat        = Teli::CAM_API_STS_SUCCESS;    // 画像ストリーム受信時のエラーステータスコード
    camdetails->stat.pyldsize       = 0;                            // 1つのストリームリクエストで受信するペイロードのサイズ(画像サイズ)
    camdetails->stat.camname        = L"Not found camera";          // カメラの名前
    camdetails->stat.camwidth       = 0;                            // 映像の幅
    camdetails->stat.camheight      = 0;                            // 映像の高さ
    camdetails->stat.framecount     = 0;                            // FPSを計算するためのフレームカウンター
    camdetails->stat.frameidx       = 0;                            // カメラから出力されるビデオデータのフレーム番号
    camdetails->stat.frameidx_valid = FALSE;                        // "frameidx"に有効な値があることを示すフラグ
    camdetails->stat.fpstimer       = timeGetTime();                // フレーム更新タイマー
    camdetails->stat.fps            = 0.0;                          // 実際のFPS
    camdetails->stat.framechk_valid = FALSE;                        // フレームレート損失チェック 
    camdetails->stat.expstime_min   = 0.0;                          // 露光時間(最小)
    camdetails->stat.expstime_max   = 0.0;                          // 露光時間(最大)
    camdetails->stat.expstime       = 0.0;                          // 露光時間
    camdetails->stat.gainmin        = 0.0;                          // ゲイン(最小)
    camdetails->stat.gainmax        = 0.0;                          // ゲイン(最大)
    camdetails->stat.gain           = 0.0;                          // ゲイン
    ZeroMemory(&camdetails->stat.caminfo, sizeof(Teli::CAM_INFO));  // カメラの情報
}

/// @brief Destructor
/// @param
/// @return
/// @note
CTeliCamLib::~CTeliCamLib(void)
{
    //----------------------------------------------------------------------------
    // TeliCamAPIの終了処理
    close();
}

//////////////////////////////////////////////////////////////////////////////
// CTeliCamLib Public method

/// @brief TeliCamAPIの初期化処理
/// @param 
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::initialize(void)
{
    int32_t              ret     = 0;
    Teli::CAM_API_STATUS apistat = Teli::CAM_API_STS_SUCCESS;   // TeliCamAPIのステータスコード

    m_errmsg = L""; // エラーメッセージ
    //----------------------------------------------------------------------------
    // TeliCamAPIの初期化処理
    if ((apistat = Teli::Sys_Initialize()) != Teli::CAM_API_STS_SUCCESS) {
        PCAMERA_DETAILS camdetails = &m_caminfo.details;    // カメラの詳細情報
        if (camdetails->cnfg.valid) {
            camdetails->stat.apistat = apistat;
        }
        ret      = -1;
        m_errmsg = std::format(L"[initialize]Sys_Initialize:{:#08x}", apistat);
    }   // if ((apistat = Sys_Initialize()) != CAM_API_STS_SUCCESS)
    else {
        PCAMERA_DETAILS camdetails = &m_caminfo.details;    // カメラの詳細情報
        if (camdetails->cnfg.valid) {
            while (1) {
//////////////////////////////////////////////////////////////////////////////
#if 0   // カメラ取外し検出は別手段で対応する
                //----------------------------------------------------------------------------
                // カメラ取外し検出の設定
                // シグナルオブジェクトの作成
                if ((camdetails->stat.apistat = Teli::Sys_CreateSignal(&camdetails->EventHndlCamRemoval)) != Teli::CAM_API_STS_SUCCESS) {
                    ret      = -2;
                    m_errmsg = std::format(L"[initialize]Sys_CreateSignal:{:#08x}", camdetails->stat.apistat);
                    break;
                }

                // Camera remove thread function
                camdetails->ThreadStatCamRemoval = TRUE;
                if ((camdetails->ThreadHndlCamRemoval = (HANDLE)_beginthreadex(NULL,
                                                                               0,
                                                                               thread_camera_remove,
                                                                               (LPVOID)camdetails,
                                                                               0,
                                                                               NULL)) == NULL) {
                    camdetails->ThreadStatCamRemoval = FALSE;
                    ret = -3;
                    _RPTWN(_CRT_WARN, L"%s\n", L">>>[initialize]thread_camera_remove create error");
                    break;
                }
                else {
                    SetThreadPriority(camdetails->ThreadHndlCamRemoval, THREAD_PRIORITY_NORMAL);
                }
#endif
//////////////////////////////////////////////////////////////////////////////

                //----------------------------------------------------------------------------
                // ゲインコントロール　スレッド起動
                camdetails->ThreadStatGainControl = TRUE;
                if ((camdetails->ThreadHndlGainControl = (HANDLE)_beginthreadex(NULL,
                                                                                0,
                                                                                thread_gain_control,
                                                                                (LPVOID)camdetails,
                                                                                0,
                                                                                NULL)) == NULL) {
                    camdetails->ThreadStatGainControl = FALSE;
                    ret = -4;
                    _RPTWN(_CRT_WARN, L"%s\n", L">>>[initialize]thread_gain_control create error");
                    break;
                }
                else {
                    SetThreadPriority(camdetails->ThreadHndlGainControl, THREAD_PRIORITY_NORMAL);
                }

                //----------------------------------------------------------------------------
                // 露光時間コントロール　スレッド起動
                camdetails->ThreadStatExpsTimeControl = TRUE;
                if ((camdetails->ThreadHndlExpsTimeControl = (HANDLE)_beginthreadex(NULL,
                                                                                    0,
                                                                                    thread_expstime_control,
                                                                                    (LPVOID)camdetails,
                                                                                    0,
                                                                                    NULL)) == NULL) {
                    camdetails->ThreadStatExpsTimeControl = FALSE;
                    ret = -5;
                    _RPTWN(_CRT_WARN, L"%s\n", L">>>[initialize]thread_expstime_control create error");
                    break;
                }
                else {
                    SetThreadPriority(camdetails->ThreadHndlExpsTimeControl, THREAD_PRIORITY_NORMAL);
                }

                break;  // Exit loop
            }   // while (1)
        }   // if (camdetails->cnfg.valid)
    }   // if ((apistat = Teli::Sys_Initialize()) != Teli::CAM_API_STS_SUCCESS) else
    return ret;
}

/// @brief PCに接続されているカメラの探索
/// @param 
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::update_camera_list(void)
{
    int32_t              ret     = 0;
    Teli::CAM_API_STATUS apistat = Teli::CAM_API_STS_SUCCESS;   // TeliCamAPIのステータスコード

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    //----------------------------------------------------------------------------
    // カメラのステータスのクリア
    m_caminfo.details.stat.camidx         = -1;                             // カメラのインデックス
    m_caminfo.details.stat.camhndl        = NULL;                           // オープンしたカメラのカメラハンドル
    m_caminfo.details.stat.strmhndl       = NULL;                           // オープンしたストリームインターフェースのストリームハンドル
    m_caminfo.details.stat.apistat        = Teli::CAM_API_STS_SUCCESS;      // TeliCamAPIのステータスコード
    m_caminfo.details.stat.errstat        = Teli::CAM_API_STS_SUCCESS;      // 画像ストリーム受信時のエラーステータスコード
    m_caminfo.details.stat.pyldsize       = 0;                              // 1つのストリームリクエストで受信するペイロードのサイズ(画像サイズ)
    m_caminfo.details.stat.camname        = L"Not found camera";            // カメラの名前
    m_caminfo.details.stat.camwidth       = 0;                              // 映像の幅
    m_caminfo.details.stat.camheight      = 0;                              // 映像の高さ
    m_caminfo.details.stat.framecount     = 0;                              // FPSを計算するためのフレームカウンター
    m_caminfo.details.stat.frameidx       = 0;                              // カメラから出力されるビデオデータのフレーム番号
    m_caminfo.details.stat.frameidx_valid = FALSE;                          // "frameidx"に有効な値があることを示すフラグ
    m_caminfo.details.stat.fpstimer       = timeGetTime();                  // フレーム更新タイマー
    m_caminfo.details.stat.fps            = 0.0;                            // 実際のFPS
    m_caminfo.details.stat.expstime_min   = 0.0;                            // 露光時間(最小)
    m_caminfo.details.stat.expstime_max   = 0.0;                            // 露光時間(最大)
    m_caminfo.details.stat.expstime       = 0.0;                            // 露光時間(最大)
    m_caminfo.details.stat.gainmin        = 0.0;                            // ゲイン(最小)
    m_caminfo.details.stat.gainmax        = 0.0;                            // ゲイン(最大)
    m_caminfo.details.stat.gain           = 0.0;                            // ゲイン
    ZeroMemory(&m_caminfo.details.stat.caminfo, sizeof(Teli::CAM_INFO));    // カメラの情報

    //----------------------------------------------------------------------------
    // PCに接続されているカメラの探索
    if ((apistat = Teli::Sys_GetNumOfCameras(&m_caminfo.camcount)) != Teli::CAM_API_STS_SUCCESS) {
        m_caminfo.details.stat.apistat = apistat;
        ret      = -1;
        m_errmsg = std::format(L"[update_camera_list]Sys_GetNumOfCameras:{:#08x}", apistat);
    }
    else
    {
        if (m_caminfo.camcount > 0) {
            Teli::CAM_INFO      teli_caminfo;   // カメラの情報
            Teli::PGEV_CAM_INFO gevcaminfo;     // カメラの情報(GEV)
            std::wstring        ipaddrs;        // Camera IP address
            for (uint32_t camidx = 0; camidx < m_caminfo.camcount; camidx++) {
                //----------------------------------------------------------------------------
                // カメラの情報の取得
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
                gevcaminfo = &teli_caminfo.sGevCamInfo; // カメラの情報(GEV)

                //----------------------------------------------------------------------------
                // IPアドレスをキーに関連付ける
                ipaddrs = std::format(L"{:d}.{:d}.{:d}.{:d}", gevcaminfo->aucIPAddress[0],
                                                              gevcaminfo->aucIPAddress[1],
                                                              gevcaminfo->aucIPAddress[2],
                                                              gevcaminfo->aucIPAddress[3]);
                
                if (ipaddrs.compare(m_caminfo.details.cnfg.ipaddress) == 0) {
                    m_caminfo.details.stat.camidx = camidx;             // カメラのインデックス
                    std::string modelname(teli_caminfo.szModelName);    // カメラのモデル名
                    m_caminfo.details.stat.camname = format(L"{:s}({:s})", CHelper::conv_string(modelname), ipaddrs);   // カメラの名前
                    memcpy(&m_caminfo.details.stat.caminfo, &teli_caminfo, sizeof(Teli::CAM_INFO)); // カメラの情報
                    break;
                }
            }   // for (uint32_t camcount = 0; camcount < m_caminfo.camcount; camcount++)
        }   // if (m_caminfo.camcount > 0)
    }   // if ((apistat = Teli::Sys_GetNumOfCameras(&m_caminfo.camcount)) != Teli::CAM_API_STS_SUCCESS) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのオープン
/// @param [in] accessmode - カメラのアクセスモード
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::open_camera(Teli::CAM_ACCESS_MODE accessmode)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    uint32_t       camwidth;    // 映像の幅
    uint32_t       camheight;   // 映像の高さ
    if ((camstat->camidx < 0) || (camstat->camidx >= (int32_t)m_caminfo.camcount)) {
        ret      = -1;
        m_errmsg = L"[open_camera]camstat->camidx error";
    }
    else
    {
        //----------------------------------------------------------------------------
        // カメラをオープンし、アプリケーションがカメラを使用できるようにします。
//////////////////////////////////////////////////////////////////////////////
#if 0   // カメラ取外し検出は別手段で対応する
        PCAMERA_DETAILS camdetails = &m_caminfo.details;    // カメラの詳細情報
        if ((camstat->apistat = Teli::Cam_Open(camstat->camidx,                             // カメラのインデックス
                                               &camstat->camhndl,                           // オープンしたカメラのカメラハンドル
                                               camdetails->EventHndlCamRemoval,             // カメラ取り外し通知用のイベント(シグナル)オブジェクトのハンドル
                                               true,                                        // GenICamアクセスの有効／無効
                                               NULL,                                        // PC内のカメラ記述情報(XMLデータ)
                                               accessmode)) != Teli::CAM_API_STS_SUCCESS) { // カメラのアクセスモード
//////////////////////////////////////////////////////////////////////////////
#else
        if ((camstat->apistat = Teli::Cam_Open(camstat->camidx,                             // カメラのインデックス
                                               &camstat->camhndl,                           // オープンしたカメラのカメラハンドル
                                               NULL,                                        // カメラ取り外し通知用のイベント(シグナル)オブジェクトのハンドル
                                               true,                                        // GenICamアクセスの有効／無効
                                               NULL,                                        // PC内のカメラ記述情報(XMLデータ)
                                               accessmode)) != Teli::CAM_API_STS_SUCCESS) { // カメラのアクセスモード
#endif
//////////////////////////////////////////////////////////////////////////////
//          camstat->camhndl = NULL;
            ret      = -2;
            m_errmsg = std::format(L"[open_camera]Cam_Open:{:#08x}", camstat->apistat);
        }
        else {
            //----------------------------------------------------------------------------
            // カメラの映像の幅の取得
            if ((camstat->apistat = Teli::GetCamWidth(camstat->camhndl, &camwidth)) !=Teli:: CAM_API_STS_SUCCESS) {
                ret      = -3;
                m_errmsg = std::format(L"[open_camera]GetCamWidth:{:#08x}", camstat->apistat);
            }
            //----------------------------------------------------------------------------
            // カメラの映像の高さの取得
            else if ((camstat->apistat = Teli::GetCamHeight(camstat->camhndl, &camheight)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -4;
                m_errmsg = std::format(L"[open_camera]GetCamHeight:{:#08x}", camstat->apistat);
            }
            else {
                ;
            }
        }
    }   // if ((camstat->camidx < 0) || (camstat->camidx >= m_caminfo.camcount)) else
    if (ret == 0) {
        camstat->camwidth  = camwidth;  // 映像の幅
        camstat->camheight = camheight; // 映像の高さ
    }
    else {
        camstat->camwidth  = 0; // 映像の幅
        camstat->camheight = 0; // 映像の高さ
    }
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのクローズ
/// @param 
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::close_camera(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[close_camera]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのクローズ
        if ((camstat->apistat = Teli::Cam_Close(camstat->camhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[close_camera]Cam_Close:{:#08x}", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else
    camstat->camhndl = NULL;
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief 画像取得用のストリームインターフェースのオープン
/// @param 
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::open_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[open_stream]camstat->camhndl error";
    }
    else {
        PCAMERA_CONFIG camcnfg = &m_caminfo.details.cnfg;   // カメラの設定
        //----------------------------------------------------------------------------
        // 画像取得用のストリームインターフェースのオープン
        // TeliCamAPI内部に画像一時保管用のストリームリクエストリングバッファを作成
        if ((camstat->apistat = Teli::Strm_OpenSimple(camstat->camhndl,                                     // カメラのカメラハンドル
                                                      &camstat->strmhndl,                                   // オープンしたストリームインターフェースのストリームハンドル
                                                      &camstat->pyldsize,                                   // 1つのストリームリクエストで受信するペイロードのサイズ(画像サイズ)
                                                      NULL,                                                 // ストリームを受信し、ストリームリクエストリングバッファが更新されたことを通知するイベント（シグナル）オブジェクトのハンドル
                                                      DEFAULT_API_BUFFER_CNT,                               // TeliCamAPI内部に作成するストリームリクエストリングバッファの数
                                                      camcnfg->packetsize)) != Teli::CAM_API_STS_SUCCESS) { // ドライバが受け取るパケットの最大サイズ
//          camstat->strmhndl = NULL;
            ret      = -2;
            m_errmsg = std::format(L"[open_stream]Strm_OpenSimple:{:#08x}", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // コールバック関数のTeliCamAPIに登録
        // TeliCamAPI内部のストリームリクエストリングバッファの内容を
        // 正常受信した画像データで更新した時にコールバック関数を呼び出す
        else if ((camstat->apistat = Teli::Strm_SetCallbackImageAcquired(camstat->strmhndl,                                                         // ストリームインターフェースのストリームハンドル
                                                                         this,                                                                      // コールバック関数を実行するときに引数として渡すオブジェクトへのポインタ
                                                                         &CTeliCamLib::callback_image_acquired)) != Teli::CAM_API_STS_SUCCESS) {    // コールバック関数
            ret      = -3;
            m_errmsg = std::format(L"[open_stream]Strm_SetCallbackImageAcquired:{:#08x}", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // コールバック関数をTeliCamAPIに登録
        // ストリームを正常に受信できず、TeliCamAPI内部のストリームリクエストリングバッファの内容が
        // エラー更新された時にコールバック関数を呼び出す
        else if ((camstat->apistat = Teli::Strm_SetCallbackImageError(camstat->strmhndl,                                                    // ストリームインターフェースのストリームハンドル
                                                                      this,                                                                 // コールバック関数を実行するときに引数として渡すオブジェクトへのポインタ
                                                                      &CTeliCamLib::callback_error_image)) != Teli::CAM_API_STS_SUCCESS) {  // コールバック関数
            ret      = -4;
            m_errmsg = std::format(L"[open_stream]Strm_SetCallbackImageError:{:#08x}", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief 画像取得用のストリームインターフェースのクローズ
/// @param 
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::close_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->strmhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[close_stream]camstat->strmhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // 画像取得用のストリームインターフェースをクローズ
        if ((camstat->apistat = Teli::Strm_Close(camstat->strmhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[close_stream]Strm_Close:{:#08x}", camstat->apistat);
        }
    }   // if (camstat->strmhndl == NULL) else
    camstat->strmhndl = NULL;
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief 画像ストリームの転送開始
/// @param 
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::start_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->strmhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[start_stream]camstat->strmhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // 画像ストリームの転送開始をカメラに要求
        if ((camstat->apistat = Teli::Strm_Start(camstat->strmhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[start_stream]Strm_Start:{:#08x}", camstat->apistat);
        }
        camstat->framecount     = 0;                // FPSを計算するためのフレームカウンター
        camstat->frameidx       = 0;                // カメラから出力されるビデオデータのフレーム番号
        camstat->frameidx_valid = FALSE;            // "frameidx"に有効な値があることを示すフラグ
        camstat->fpstimer       = timeGetTime();    // フレーム更新タイマー
        camstat->fps            = 0.0;              // 実際のFPS
    }   // if (camstat->StrmHandl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief 画像ストリームの転送停止
/// @param 
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::stop_stream(void)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->strmhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[stop_stream]camstat->strmhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // 画像ストリームの転送停止をカメラに要求
        if ((camstat->apistat = Teli::Strm_Stop(camstat->strmhndl)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[stop_stream]Strm_Stop:{:#08x}", camstat->apistat);
        }
    }   // if (camstat->hstrm == NULL) else
    camstat->framecount     = 0;        // FPSを計算するためのフレームカウンター
    camstat->frameidx       = 0;        // カメラから出力されるビデオデータのフレーム番号
    camstat->frameidx_valid = FALSE;    // "frameidx"に有効な値があることを示すフラグ
    camstat->fps            = 0.0;      // 実際のFPS
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのビデオストリームのピクセル形式の設定
/// @param [in] pixelformat - ピクセルフォーマット(CAM_PIXEL_FORMAT)
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_pixelformat(Teli::CAM_PIXEL_FORMAT pixelformat)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_pixelformat]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラの映像ストリームのピクセルフォーマットを設定
        if ((camstat->apistat = Teli::SetCamPixelFormat(camstat->camhndl,
                                                        pixelformat)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_pixelformat]SetCamPixelFormat:{:#08x}", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのROI(領域)の設定
/// @param [in] offset_x - 映像の水平方向開始位置
/// @param [in] offset_y - 映像の垂直方向開始位置
/// @param [in] width - 映像の幅
/// @param [in] height - 映像の高さ
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_camroi(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat   = &m_caminfo.details.stat; // カメラのステータス
    uint32_t       camwidth  = 0;                       // 映像の幅
    uint32_t       camheight = 0;                       // 映像の高さ
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_camroi]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのROI(領域)を設定
        if ((camstat->apistat = Teli::SetCamRoi(camstat->camhndl,
                                                width, height,
                                                offset_x, offset_y)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_camroi]SetCamRoi:{:#08x}", camstat->apistat);
        }
        else {
            //----------------------------------------------------------------------------
            // カメラの映像の幅を取得
            if ((camstat->apistat = Teli::GetCamWidth(camstat->camhndl,
                                                      &camwidth)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -3;
                m_errmsg = std::format(L"[set_camroi]GetCamWidth:{:#08x}", camstat->apistat);
            }
            //----------------------------------------------------------------------------
            // カメラの映像の高さを取得
            else if ((camstat->apistat = Teli::GetCamHeight(camstat->camhndl,
                                                            &camheight)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -4;
                m_errmsg = std::format(L"[set_camroi]GetCamHeight:{:#08x}", camstat->apistat);
            }
            else {
                ;
            }
        }
    }   // if (camstat->camhndl == NULL) else
    if (ret == 0) {
        camstat->camwidth  = camwidth;  // 映像の幅
        camstat->camheight = camheight; // 映像の高さ
    }
    else {
        camstat->camwidth  = 0; // 映像の幅
        camstat->camheight = 0; // 映像の高さ
    }
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのフレームレートの設定
/// @param [in] framerate - フレームレート
/// @param [in] ctrltype - フレームレート設定(CAM_ACQ_FRAME_RATE_CTRL_TYPE)
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_framerate(float64_t framerate, Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE ctrltype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_framerate]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのフレームレートを設定
        if ((camstat->apistat = Teli::SetCamAcquisitionFrameRateControl(camstat->camhndl,
                                                                        ctrltype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_framerate]SetCamAcquisitionFrameRateControl:{:#08x}", camstat->apistat);
        }
        else if ((camstat->apistat = Teli::SetCamAcquisitionFrameRate(camstat->camhndl,
                                                                      framerate)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -3;
            m_errmsg = std::format(L"[set_framerate]SetCamAcquisitionFrameRate:{:#08x}", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのトリガー動作モードの設定
/// @param [in] triggermode - トリガ動作モード(false:トリガ動作モードOFF true:トリガ動作モードON)
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_triggermode(bool8_t triggermode)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_triggermode]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのトリガ動作モード(TriggerMode)を設定
        if ((camstat->apistat = Teli::SetCamTriggerMode(camstat->camhndl,
                                                        triggermode)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_triggermode]SetCamTriggerMode:{:#08x}", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else	
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラの黒レベルの設定
/// @param [in] blacklevel - 黒レベル
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_blacklevel(float64_t blacklevel)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_blacklevel]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラの黒レベルの最小値と最大値を取得
        float64_t valmin, valmax;
        if ((camstat->apistat = Teli::GetCamBlackLevelMinMax(camstat->camhndl,
                                                             &valmin,
                                                             &valmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_blacklevel]GetCamBlackLevelMinMax:{:#08x}", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // カメラの黒レベルを設定
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
                m_errmsg = std::format(L"[set_blacklevel]SetCamBlackLevel:{:#08x}", camstat->apistat);
            }
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのガンマ補正値の設定
/// @param [in] gamma - ガンマ補正値
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_gamma(float64_t gamma)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_gamma]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのガンマ補正値の最小値と最大値を取得
        float64_t valmin, valmax;
        if ((camstat->apistat = Teli::GetCamGammaMinMax(camstat->camhndl,
                                                        &valmin,
                                                        &valmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_gamma]GetCamGammaMinMax:{:#08x}", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // カメラのガンマ補正値を設定
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
                m_errmsg = std::format(L"[set_gamma]SetCamGamma:{:#08x}", camstat->apistat);
            }
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのホワイトバランスゲイン自動調整モードの設定
/// @param [in] autotype - ホワイトバランスゲイン自動調整モード(CAM_BALANCE_WHITE_AUTO_TYPE)
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_wbalance_auto(Teli::CAM_BALANCE_WHITE_AUTO_TYPE autotype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_wbalance_auto]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのホワイトバランスゲイン自動調整モードを設定
        if ((camstat->apistat = Teli::SetCamBalanceWhiteAuto(camstat->camhndl,
                                                             autotype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_wbalance_auto]SetCamBalanceWhiteAuto:{:#08x}", camstat->apistat);
        }
    }   // if (camstat->camhndl == NULL) else	
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのホワイトバランスゲイン(倍率)の設定
/// @param [in] wbratio - ホワイトバランスゲイン(倍率)
/// @param [in] selectortype - ホワイトバランスゲイン設定の対象となる要素(CAM_BALANCE_RATIO_SELECTOR_TYPE)
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_wbalance_ratio(float64_t wbratio, Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE selectortype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_wbalance_ratio]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのホワイトバランスゲイン自動調整モードがManualに設定されているときの、ゲインの最小値と最大値を取得
        float64_t valmin, valmax;
        if ((camstat->apistat = Teli::GetCamBalanceRatioMinMax(camstat->camhndl,
                                                               selectortype,
                                                               &valmin,
                                                               &valmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_wbalance_ratio]GetCamBalanceRatioMinMax:{:#08x}", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // カメラのホワイトバランスゲイン(倍率)を設定
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
                m_errmsg = std::format(L"[set_wbalance_ratio]SetCamBalanceRatio:{:#08x}", camstat->apistat);
            }
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのAGC(Automatic gain control)動作モードの設定
/// @param [in] autotype - AGC動作モード(CAM_GAIN_AUTO_TYPE)
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_gain_auto(Teli::CAM_GAIN_AUTO_TYPE autotype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_gain_auto]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのAGC(Automatic gain control)動作モードを設定
        if ((camstat->apistat = Teli::SetCamGainAuto(camstat->camhndl,
                                                     autotype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_gain_auto]SetCamGainAuto:{:#08x}", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // カメラのAGC動作モードがManualに設定されているときの、ゲインの最小値と最大値を取得
        else if ((camstat->apistat = Teli::GetCamGainMinMax(camstat->camhndl,
                                                            &camstat->gainmin,
                                                            &camstat->gainmax)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -3;
            m_errmsg = std::format(L"[set_gain_auto]GetCamGainMinMax:{:#08x}", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->CamHNdl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラのゲインの設定(APIへの設定はスレッドで実行される)
/// @param [in] gain - ゲイン
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_gain(float64_t gain)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_gain]camstat->camhndl error";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPIのエラー
        ret      = -2;
        m_errmsg = std::format(L"[set_gain]camstat->apistat error:{:#08x}", camstat->apistat);
    }
    else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // 画像ストリーム受信時のエラー
        ret      = -3;
        m_errmsg = std::format(L"[set_gain]camstat->ErrEtat error:{:#08x}", camstat->errstat);
    }
    else {
        //----------------------------------------------------------------------------
        // カメラのゲインを設定(APIへの設定はスレッドで実行される)
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

        PCAMERA_DETAILS camdtail = &m_caminfo.details;  // カメラの詳細情報
        if (camdtail->ThreadHndlGainControl != NULL) {
            ResumeThread(camdtail->ThreadHndlGainControl);
        }
//////////////////////////////////////////////////////////////////////////////
#if 0   // APIへの設定はスレッドで実行
        if ((camstat->apistat = Teli::SetCamGain(camstat->camhndl,
                                                 camstat->gain)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_gain]SetCamGain:{:#08x}", camstat->apistat);
        }
#endif
//////////////////////////////////////////////////////////////////////////////
    }   // if (camstat->camhndl == NULL)
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラの露光時間の制御モードの設定
/// @param [in] ctrltype - 露光時間の制御モード(CAM_EXPOSURE_TIME_CONTROL_TYPE)
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE ctrltype)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // カメラのステータス
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_expstime_control]camstat->camhndl error";
    }
    else {
        //----------------------------------------------------------------------------
        // カメラの露光時間の制御モードを設定
        if ((camstat->apistat = Teli::SetCamExposureTimeControl(camstat->camhndl,
                                                                ctrltype)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -2;
            m_errmsg = std::format(L"[set_expstime_control]SetCamExposureTimeControl:{:#08x}", camstat->apistat);
        }
        //----------------------------------------------------------------------------
        // カメラの露光時間制御モードがManualに設定されているときの、露光時間の最小値と最大値を取得
        else if ((camstat->apistat = Teli::GetCamExposureTimeMinMax(camstat->camhndl,
                                                                    &camstat->expstime_min,
                                                                    &camstat->expstime_max)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -3;
            m_errmsg = std::format(L"[set_expstime_control]GetCamExposureTimeMinMax:{:#08x}", camstat->apistat);
        }
        else {
            ;
        }
    }   // if (camstat->camhndl == NULL) else
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラの露光時間の設定(APIへの設定はスレッドで実行される)
/// @param [in] expstime - Exposure time
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::set_expstime(float64_t expstime)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if (camstat->camhndl == NULL) {
        ret      = -1;
        m_errmsg = L"[set_expstime]camstat->camhndl error";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPIのエラー
        ret      = -2;
        m_errmsg = std::format(L"[set_expstime]camstat->apistat error:{:#08x}", camstat->apistat);
    }
    else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // 画像ストリーム受信時のエラー
        ret      = -3;
        m_errmsg = std::format(L"[set_expstime]camstat->ErrEtat error:{:#08x}", camstat->errstat);
    }
    else {
        //----------------------------------------------------------------------------
        // カメラの露光時間を設定(APIへの設定はスレッドで実行される)
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
 
        PCAMERA_DETAILS camdtail = &m_caminfo.details;  // カメラの詳細情報
        if (camdtail->ThreadHndlExpsTimeControl != NULL) {
            ResumeThread(camdtail->ThreadHndlExpsTimeControl);
        }
//////////////////////////////////////////////////////////////////////////////
#if 0   // APIへの設定はスレッドで実行
        if ((camstat->apistat = Teli::SetCamExposureTime(camstat->camhndl,
                                                         camstat->expstime)) != Teli::CAM_API_STS_SUCCESS) {
            ret      = -4;
            m_errmsg = std::format(L"[set_expstime]SetCamExposureTime:{:#08x}", camstat->apistat);
        }
#endif
//////////////////////////////////////////////////////////////////////////////
    }
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief 画像情報を取得
/// @param [in] bufsize - Copy destination buffer size 
/// @param [out] image - Image pointer
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::get_image(uint32_t bufsize, unsigned char* image)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if (image == NULL) {
        ret      = -1;
        m_errmsg = L"[get_image]image error";
    }
    else if ((camstat->camhndl  == NULL) || // Handle of camera assigned by TeliCamAPI on opening camera
             (camstat->strmhndl == NULL)) { // Handle of stream channel assigned by TeliCamAPI on opening stream channel
        ret      = -2;
        m_errmsg = L"[get_image]camstat->camhndl/strmhndl error";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPIのエラー
        ret      = -3;
        m_errmsg = std::format(L"[get_image]camstat->apistat error:{:#08x}", camstat->apistat);
    }
    else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // 画像ストリーム受信時のエラー
        ret      = -4;
        m_errmsg = std::format(L"[get_image]camstat->errstat error:{:#08x}", camstat->errstat);
    }
    else {
        if (!camstat->frameidx_valid) {
            ret      = -5;
            m_errmsg = L"[get_image]camstat->frameidx_valid error";
        }
        else {  // Image received
            uint32_t rbufidx;   // Buffer index of the acquired stream request ring buffer
            //----------------------------------------------------------------------------
            // Gets the buffer index of the stream request ring buffer inside TeliCam API that stores the latest stream request (image).
            if ((camstat->apistat = Teli::Strm_GetCurrentBufferIndex(camstat->strmhndl, &rbufidx)) != Teli::CAM_API_STS_SUCCESS) {
                ret      = -6;
                m_errmsg = std::format(L"[get_image]Strm_GetCurrentBufferIndex:{:#08x}", camstat->apistat);
            }
            else {
                Teli::CAM_IMAGE_INFO imginfo;   // Accompanying information of the image stored in the stream request ring buffer to be locked
                //----------------------------------------------------------------------------
                // Get image pointer in Ring Buffer.
                // Image data is locked in Strm_LockApiRingBufferPointer() method.
                if ((camstat->apistat = Teli::Strm_LockBuffer(camstat->strmhndl, rbufidx, &imginfo)) != Teli::CAM_API_STS_SUCCESS) {
                    ret      = -7;
                    m_errmsg = std::format(L"[get_image]Strm_LockBuffer:{:#08x}", camstat->apistat);
                }
                else if (imginfo.uiStatus != Teli::CAM_API_STS_SUCCESS) {
                    camstat->apistat = imginfo.uiStatus;    // TeliCamAPI status
                    ret      = -8;
                    m_errmsg = std::format(L"[get_image]Strm_LockBuffer:{:#08x}", imginfo.uiStatus);
                }
                else if (camstat->pyldsize > bufsize) {
                    ret      = -9;
                    m_errmsg = L"[GetImag]bufsize error";
                }
                else {
                    CopyMemory(image, imginfo.pvBuf, camstat->pyldsize);
                    // フレームレート損失チェック
                    if (camstat->framechk_valid) {
                        PCAMERA_CONFIG camcnfg = &m_caminfo.details.cnfg;   // Camera cofig
                        if (camstat->fps <= camcnfg->framerate_drop) {
                            ret      = -10;
                            m_errmsg = L"[get_image]Frame rate drop";
                        }
                    }
                }

                //----------------------------------------------------------------------------
                // Unlock current image
                Teli::CAM_API_STATUS apistat = Teli::Strm_UnlockBuffer(camstat->strmhndl, rbufidx);
                if (apistat != Teli::CAM_API_STS_SUCCESS) {
                    if (camstat->apistat == Teli::CAM_API_STS_SUCCESS) {
                        camstat->apistat = apistat; // TeliCamAPIのステータスコード
                    }
                    ret      = -11;
                    m_errmsg = std::format(L"[get_image]Strm_UnlockBuffer:{:#08x}", apistat);
                }
            }   // if ((camstat->apistat = Teli::Strm_GetCurrentBufferIndex(camstat->strmhndl, &rbufidx)) != Teli::CAM_API_STS_SUCCESS) else
        }   // if (!camstat->frameidx_valid) else
    }   // if (image == NULL) ... else

    //----------------------------------------------------------------------------
    // Check interval (fps)
    uint32_t       fpstimer = timeGetTime() - camstat->fpstimer;
    const uint32_t interval = CHECK_FPS_INTERVAL_TIME;  // fpsをチェックする間隔[ms]
    if (fpstimer > interval) {
        camstat->fps            = ((float64_t)camstat->framecount / (float64_t)fpstimer) * 1000.0;
        camstat->framecount     = 0;
        camstat->fpstimer       = timeGetTime();
        camstat->framechk_valid = TRUE; // フレームレート損失チェック 
    }
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief 画像サイズの取得
/// @param [out] width - Image width
/// @param [out] height - Image height
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::get_camera_imagesize(uint32_t* width, uint32_t* height)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if ((width == NULL) || (height == NULL)) {
        ret      = -1;
        m_errmsg = L"[get_camera_imagesize]width/height error";
    }
    else if (camstat->camhndl == NULL) {    // Handle of camera assigned by TeliCamAPI on opening camera
        *width   = 0;
        *height  = 0;
        ret      = -2;
        m_errmsg = L"[get_camera_imagesize]camstat->camhndl error";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPIステータスエラー
        *width   = 0;
        *height  = 0;
        ret      = -3;
        m_errmsg = std::format(L"[get_camera_imagesize]camstat->apistat error:{:#08x}", camstat->apistat);
    }
    else {
        *width  = camstat->camwidth;    // 映像の幅
        *height = camstat->camheight;   // 映像の高さ
    }
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief フレームレートの取得
/// @param [out] fps - Actual frame rate
/// @return 結果(0:成功 0以外:失敗)
/// @note
int32_t CTeliCamLib::get_actual_framerate(float64_t* fps)
{
    int32_t ret = 0;

    CCsAutoLock	CsLock(m_cscam);    // Auto Lock
    m_errmsg = L""; // エラーメッセージ

    PCAMERA_STATUS camstat = &m_caminfo.details.stat;   // Camera status
    if (fps == NULL) {
        ret      = -1;
        m_errmsg = L"[get_camera_imagesize]fps error";
    }
    else if ((camstat->camhndl  == NULL) || // Handle of camera assigned by TeliCamAPI on opening camera
             (camstat->strmhndl == NULL)) { // Handle of stream channel assigned by TeliCamAPI on opening stream channel
        *fps     = 0.0;
        ret      = -2;
        m_errmsg = L"[get_camera_imagesize]camstat->camhndl/strmhndl error";
    }
    else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI status
        *fps     = 0.0;
        ret      = -3;
        m_errmsg = std::format(L"[get_camera_imagesize]camstat->apistat error:{:#08x}", camstat->apistat);
    }
    else {
        *fps = camstat->fps;
    }
    CsLock.Release();   // Unlock
    return ret;
}

/// @brief カメラ情報の取得
/// @param 
/// @return カメラの情報
/// @note
inline Teli::CAM_INFO CTeliCamLib::get_stat_caminfo(void)
{
    return m_caminfo.details.stat.caminfo;
}

/// @brief カメラ名の取得
/// @param 
/// @return カメラの名前
/// @note
inline std::wstring CTeliCamLib::get_cameraname(void)
{
    return m_caminfo.details.stat.camname;
}

/// @brief エラーメッセージの取得
/// @param 
/// @return エラーメッセージ
/// @note
inline LPCWSTR CTeliCamLib::get_errormessage(void)
{
    return m_errmsg.c_str();
}

//////////////////////////////////////////////////////////////////////////////
// CTeliCamLib Private method

/// @brief 受信した画像を処理するためのコールバック関数
/// @param [in] hCam - Handler of camera
/// @param [in] hStrm - Handler of stream
/// @param [in] psImageInfo - Additional information of received image
/// @param [in] uiBufferIndex - Index of buffer
/// @param [in] pvContext - Additional caller-specified context
/// @return
/// @note
void CTeliCamLib::callback_image_acquired(Teli::CAM_HANDLE hCam, Teli::CAM_STRM_HANDLE hStrm, Teli::PCAM_IMAGE_INFO psImageInfo, uint32_t uiBufferIndex, void* pvContext)
{
    CTeliCamLib*         telicamlib = reinterpret_cast<CTeliCamLib*>(pvContext);
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPIのステータスコード
    uint32_t             camidx;                                    // Camera index when the camera is opened

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[callback_image_acquired]Call");
    //----------------------------------------------------------------------------
    // Get camera index from camera handle
    if ((apistat = Teli::GetCamIndexFromCamHandle(hCam, &camidx)) != Teli::CAM_API_STS_SUCCESS) {
        if (telicamlib->get_caminfo_camindex(camidx) >= 0) {
            telicamlib->m_caminfo.details.stat.apistat = apistat;   // TeliCamAPIのステータスコード
        }
        _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[callback_image_acquired]GetCamIndexFromCamHandle:", apistat);
        return;
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
            camstat->framecount += ((uint32_t)(psImageInfo->ullBlockId) - camstat->frameidx);   // FPSを計算するためのフレームカウンター
            camstat->frameidx    = static_cast<uint32_t>(psImageInfo->ullBlockId);              // カメラから出力されるビデオデータのフレーム番号
        }
        else {
            camstat->frameidx_valid = TRUE;
            camstat->framecount++;
            camstat->frameidx = static_cast<uint32_t>(psImageInfo->ullBlockId); // カメラから出力されるビデオデータのフレーム番号
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
//////////////////////////////////////////////////////////////////////////////
#if 0   // get_image()で処理する
        //----------------------------------------------------------------------------
        // Check interval (fps)
        uint32_t       fpstimer = timeGetTime() - camstat->fpstimer;
        const uint32_t interval = CHECK_FPS_INTERVAL_TIME;  // Interval to check fps[ms]
        if (fpstimer > interval) {
            camstat->fps        = (float64_t)camstat->framecount * 1000.0 / (float64_t)fpstimer;
            camstat->framecount = 0;
            camstat->fpstimer   = timeGetTime();
        }
#endif
//////////////////////////////////////////////////////////////////////////////
    }
}

/// @brief 受信エラーを処理するためのコールバック関数
/// @param [in] hCam - Handler of camera
/// @param [in] hStrm - Handler of stream
/// @param [in] uiErrorStatus - AError status
/// @param [in] uiBufferIndex - Index of buffer
/// @param [in] pvContext - Additional caller-specified context
/// @return 
/// @note
void CTeliCamLib::callback_error_image(Teli::CAM_HANDLE hCam, Teli::CAM_STRM_HANDLE hStrm, Teli::CAM_API_STATUS uiErrorStatus, uint32_t uiBufferIndex, void* pvContext)
{
    CTeliCamLib*         telicamlib = reinterpret_cast<CTeliCamLib*>(pvContext);
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPIのステータスコード
    uint32_t             camidx;                                    // Camera index when the camera is opened

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[callback_error_image]Call");
    //----------------------------------------------------------------------------
    // Get camera index from camera handle
    if ((apistat = Teli::GetCamIndexFromCamHandle(hCam, &camidx)) != Teli::CAM_API_STS_SUCCESS) {
        if (telicamlib->get_caminfo_camindex(camidx) >= 0) {
            PCAMERA_STATUS camstat = &telicamlib->m_caminfo.details.stat;   // Camera status
            camstat->apistat        = apistat;          // TeliCamAPIのステータスコード
            camstat->framecount     = 0;                // FPSを計算するためのフレームカウンター
            camstat->frameidx       = 0;                // カメラから出力されるビデオデータのフレーム番号
            camstat->frameidx_valid = FALSE;            // "frameidx"に有効な値があることを示すフラグ
            camstat->fpstimer       = timeGetTime();    // Frame update timer
            camstat->fps            = 0.0;              // 実際のFPS
        }
        _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[callback_error_image]GetCamIndexFromCamHandle:", apistat);
    }
    else {
        if (telicamlib->get_caminfo_camindex(camidx) >= 0) {
            PCAMERA_STATUS camstat = &telicamlib->m_caminfo.details.stat;   // Camera status
            switch (uiErrorStatus) {
            case Teli::CAM_API_STS_RESPONSE_TIMEOUT:
            case Teli::CAM_API_STS_BUFFER_FULL:
            case Teli::CAM_API_STS_TOO_MANY_PACKET_MISSING:
//////////////////////////////////////////////////////////////////////////////
////            camstat->errstat        = uiErrorStatus;    // 画像ストリーム受信時のエラーステータスコード
//              camstat->framecount     = 0;                // FPSを計算するためのフレームカウンター
//              camstat->frameidx       = 0;                // カメラから出力されるビデオデータのフレーム番号
//              camstat->frameidx_valid = FALSE;            // "frameidx"に有効な値があることを示すフラグ
//              camstat->fpstimer       = timeGetTime();    // Frame update timer
//              camstat->fps            = 0.0;              // 実際のFPS
//////////////////////////////////////////////////////////////////////////////
                break;
            default:
                camstat->errstat        = uiErrorStatus;    // 画像ストリーム受信時のエラーステータスコード
                camstat->framecount     = 0;                // FPSを計算するためのフレームカウンター
                camstat->frameidx       = 0;                // カメラから出力されるビデオデータのフレーム番号
                camstat->frameidx_valid = FALSE;            // "frameidx"に有効な値があることを示すフラグ
                camstat->fpstimer       = timeGetTime();    // Frame update timer
                camstat->fps            = 0.0;              // 実際のFPS
                break;
            }
        }
        _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[callback_error_image]uiErrorStatus:", uiErrorStatus);
    }

    //----------------------------------------------------------------------------
    // Dummy code for avoiding not refered warning of compliler. 
//  void *pdummy = (void*)&uiBufferIndex;
//  pdummy = &hStrm;
}

/// @brief カメラ取外し検出スレッド関数(未使用)
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CTeliCamLib::thread_camera_remove(LPVOID lpParam)
{
    HRESULT              hr         = S_OK;
    PCAMERA_DETAILS      camdetails = reinterpret_cast<CAMERA_DETAILS*>(lpParam);
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPIのステータスコード

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_camera_remove]Start");
    while (camdetails->ThreadStatCamRemoval) {
        if ((apistat = Teli::Sys_WaitForSignal(camdetails->EventHndlCamRemoval, GEV_CAMERA_REMOVE_EVENT_WAITIME)) != Teli::CAM_API_STS_SUCCESS) {
            if (apistat == Teli::CAM_API_STS_TIMEOUT) {
                _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_camera_remove]Sys_WaitForSignal:CAM_API_STS_TIMEOUT");
            }
            else {
                camdetails->stat.apistat = apistat;
                _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[thread_camera_remove]Sys_WaitForSignal:", apistat);
            }
        }
        else {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_camera_remove]Sys_WaitForSignal:CAM_API_STS_SUCCESS");
        }
    }   // while(camdetails->ThreadStatCamRemoval)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_camera_remove]End");
    camdetails->ThreadStatCamRemoval = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief ゲイン設定スレッド関数
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CTeliCamLib::thread_gain_control(LPVOID lpParam)
{
    HRESULT         hr         = S_OK;
    PCAMERA_DETAILS camdetails = reinterpret_cast<CAMERA_DETAILS*>(lpParam);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_gain_control]Start");
    while (camdetails->ThreadStatGainControl) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(camdetails->ThreadHndlGainControl);   // Suspend thread

        PCAMERA_STATUS camstat = &camdetails->stat; // Camera status
        if (camstat->camhndl == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_gain_control]camstat->camhndl error");
        }
        else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPIのエラー
            _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[thread_gain_control]camstat->apistat error:", camstat->apistat);
        }
        else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // 画像ストリーム受信時のエラー
            _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[thread_gain_control]camstat->errstat error:", camstat->errstat);
        }
        else {
            if ((camstat->apistat = Teli::SetCamGain(camstat->camhndl, camstat->gain)) != Teli::CAM_API_STS_SUCCESS) {
                _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[thread_gain_control]SetCamGain:", camstat->apistat);
            }
        }
    }   // while(camdetails->ThreadStatGainControl)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_gain_control]End");
    camdetails->ThreadStatGainControl = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief 露光時間設定スレッド関数
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CTeliCamLib::thread_expstime_control(LPVOID lpParam)
{
    HRESULT         hr         = S_OK;
    PCAMERA_DETAILS camdetails = reinterpret_cast<CAMERA_DETAILS*>(lpParam);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_expstime_control]Start");
    while (camdetails->ThreadStatExpsTimeControl) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(camdetails->ThreadHndlExpsTimeControl);   // Suspend thread

        PCAMERA_STATUS camstat = &camdetails->stat; // Camera status
        if (camstat->camhndl == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_expstime_control]camstat->camhndl error");
        }
        else if (camstat->apistat != Teli::CAM_API_STS_SUCCESS) {   // TeliCamAPI status
            _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[thread_expstime_control]camstat->apistat error:", camstat->apistat);
        }
        else if (camstat->errstat != Teli::CAM_API_STS_SUCCESS) {   // Error status code when receiving image stream
            _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[thread_expstime_control]camstat->errstat error:", camstat->errstat);
        }
        else {
            if ((camstat->apistat = Teli::SetCamExposureTime(camstat->camhndl, camstat->expstime)) != Teli::CAM_API_STS_SUCCESS) {
                _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[thread_expstime_control]SetCamExposureTime:", camstat->apistat);
            }
        }
    }   // while(camdetails->ThreadStatExpsTimeControl)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[thread_expstime_control]End");
    camdetails->ThreadStatExpsTimeControl = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief TeliCamAPIの終了処理
/// @param 
/// @return Normal(0) or Abnormal(Not 0)
/// @note
void CTeliCamLib::close(void)
{
    Teli::CAM_API_STATUS apistat    = Teli::CAM_API_STS_SUCCESS;    // TeliCamAPIのステータスコード
    PCAMERA_DETAILS      camdetails = &m_caminfo.details;           // カメラの詳細情報

    //----------------------------------------------------------------------------
    // End of thread
    // Exposure time control thread function
    if (camdetails->ThreadHndlExpsTimeControl != NULL) {
        if (camdetails->ThreadStatExpsTimeControl) {
            camdetails->ThreadStatExpsTimeControl = FALSE;
        	ResumeThread(camdetails->ThreadHndlExpsTimeControl);
	        WaitForSingleObject(camdetails->ThreadHndlExpsTimeControl, 500);    // It waits until the thread ends.
        }
        CloseHandle(camdetails->ThreadHndlExpsTimeControl); // Handle is closed.
        camdetails->ThreadHndlExpsTimeControl = NULL;
    }
    camdetails->ThreadStatExpsTimeControl = FALSE;

    // Gain control thread function
    if (camdetails->ThreadHndlGainControl != NULL) {
        if (camdetails->ThreadStatGainControl) {
            camdetails->ThreadStatGainControl = FALSE;
        	ResumeThread(camdetails->ThreadHndlGainControl);
	        WaitForSingleObject(camdetails->ThreadHndlGainControl, 500);    // It waits until the thread ends.
        }
        CloseHandle(camdetails->ThreadHndlGainControl); // Handle is closed.
        camdetails->ThreadHndlGainControl = NULL;
    }
    camdetails->ThreadStatGainControl = FALSE;

    // Camera remove thread function
    if (camdetails->ThreadHndlCamRemoval != NULL) {
        if (camdetails->ThreadStatCamRemoval) {
            camdetails->ThreadStatCamRemoval = FALSE;
	        ResumeThread(camdetails->ThreadHndlCamRemoval);
	        WaitForSingleObject(camdetails->ThreadHndlCamRemoval, GEV_CAMERA_REMOVE_EVENT_WAITIME + 500);   // It waits until the thread ends.
        }
        CloseHandle(camdetails->ThreadHndlCamRemoval);
        camdetails->ThreadHndlCamRemoval = NULL;    // Handle is closed.
    }
    camdetails->ThreadStatCamRemoval = FALSE;

    //----------------------------------------------------------------------------
    // 画像ストリームの転送停止
    stop_stream();
    _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[Close]", (LPCTSTR)get_errormessage());

    //----------------------------------------------------------------------------
    // Stream close
    close_stream();
    _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[Close]", (LPCTSTR)get_errormessage());

    //----------------------------------------------------------------------------
    // Close camera
    close_camera();
    _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[Close]", (LPCTSTR)get_errormessage());

    //----------------------------------------------------------------------------
    // Close a signal object
    if (camdetails->EventHndlCamRemoval != NULL) {
        if ((apistat = Teli::Sys_CloseSignal(camdetails->EventHndlCamRemoval)) != Teli::CAM_API_STS_SUCCESS) {
            _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[Close]Sys_CloseSignal:", apistat);
        }
        camdetails->EventHndlCamRemoval = NULL;
    }

    //----------------------------------------------------------------------------
    // Termination process of TeliCamAPI
    if ((apistat = Teli::Sys_Terminate()) != Teli::CAM_API_STS_SUCCESS) {
        _RPTWN(_CRT_WARN, L"%s%08X\n", L">>>[Close]Sys_Terminate:", apistat);
    }
}

/// @brief カメラをオープンしたときのカメラのインデックスの取得
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
